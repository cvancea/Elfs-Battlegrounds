// Copyright 2019 - Cosmin Vancea - 313CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "./game.h"
#include "./cmds.h"
#include "./elfs.h"
#include "./map.h"
#include "./utils.h"


void cmds_read(game_t *game) {
    char *cmd;
    while (game->players_alive > 1 && (cmd = fread_malloc_word(game->f_in))) {
        if (cmd[0] == 0) {
            // nu mai este nimic de citit
            fread_free_word(cmd);
            break;
        }

        if (!strcmp(cmd, "MOVE")) {
            int elf_id, i = 0;
            fscanf(game->f_in, "%d", &elf_id);
            char *moves = fread_malloc_word(game->f_in);

            if (moves != NULL) {
                while (moves[i] != 0 && game->players_alive > 1) {
                    if (!isupper(moves[i])) {
                        continue;
                    }

                    cmd_handle_move(game, moves[i], &game->elfs.elf[elf_id]);
                    i++;
                }

                fread_free_word(moves);
            }
        } else if (!strcmp(cmd, "SNOWSTORM")) {
            int encoded, x, y, r, dmg;
            fscanf(game->f_in, "%d", &encoded);

            x = (encoded & 0x000000FF);
            y = (encoded & 0x0000FF00) >> 8;
            r = (encoded & 0x00FF0000) >> 16;
            dmg = (encoded & 0xFF000000) >> 24;

            // translatarea coordonatelor la harta realocata (daca e necesar)
            x -= game->map.times_shrinked;
            y -= game->map.times_shrinked;

            cmd_handle_snowstorm(game, x, y, r, dmg);
        } else if (!strcmp(cmd, "PRINT_SCOREBOARD")) {
            cmd_handle_print_scoreboard(game);
        } else if (!strcmp(cmd, "MELTDOWN")) {
            int stamina;
            fscanf(game->f_in, "%d", &stamina);

            cmd_handle_meltdown(game, stamina);
        }

        fread_free_word(cmd);
    }
}

void cmd_handle_move(game_t *game, char move, elf_t *elf) {
    if (elf->hp <= 0) {
        return;
    }

    int new_x = elf->cell->x;
    int new_y = elf->cell->y;
    int stamina_required;
    cell_t *new_cell, *old_cell;

    switch (move) {
    case 'U':
        new_x--;
        break;
    case 'D':
        new_x++;
        break;
    case 'L':
        new_y--;
        break;
    case 'R':
        new_y++;
        break;
    }

    if (!is_point_valid(&game->map, new_x, new_y)) {
        elf_died(game, elf, NULL, FALLEN);
        return;
    }

    new_cell = &game->map.cells[new_x][new_y];
    old_cell = elf->cell;
    stamina_required = abs(new_cell->height - old_cell->height);

    if (stamina_required > elf->stamina) {
        return;
    }

    elf->stamina -= stamina_required;

    if (!is_point_on_glacier(&game->map, new_x, new_y)) {
        elf_died(game, elf, NULL, FALLEN);
        return;
    }

    elf_move_in_cell(game, elf, new_cell, old_cell);
}

void cmd_handle_snowstorm(game_t *game, int x, int y, int r, int dmg) {
    for (int i = 0; i < game->elfs.size; ++i) {
        elf_t *elf = &game->elfs.elf[i];

        if (elf->hp > 0 &&
            is_point_in_range_of_point(elf->cell->x, elf->cell->y, x, y, r)) {
            if ((elf->hp -= dmg) <= 0) {
                elf_died(game, elf, NULL, SNOWSTORM);
            }
        }
    }
}

void cmd_handle_print_scoreboard(game_t *game) {
    elfs_t elfs_sorted;
    elfs_sorted.size = game->elfs.size;

    if (!elfs_alloc(&elfs_sorted)) {
        fprintf(stderr, "Couldn't allocate memory for: sorted elfs.\n");
        return;
    }

    memcpy(elfs_sorted.elf, game->elfs.elf, elfs_sorted.size * sizeof(elf_t));
    qsort(elfs_sorted.elf, elfs_sorted.size, sizeof(elf_t), elf_comp);

    fprintf(game->f_out, "SCOREBOARD:\n");
    for (int i = 0; i < elfs_sorted.size; ++i) {
        elf_t *elf = &elfs_sorted.elf[i];

        fprintf(game->f_out, "%s\t%s\t%d\n", elf->name,
                                             elf->hp > 0 ? "DRY" : "WET",
                                             elf->eliminated);
    }

    // dealocare este facuta partial pt. ca numele eflilor este salvat pe heap,
    // iar atat structura sortata, cat si structura normala pointeaza la
    // acelasi nume

    // dealocarea "totala" va fi facuta la sfarsitul jocului
    elfs_free(&elfs_sorted, 1);
}

void cmd_handle_meltdown(game_t *game, int stamina) {
    if (!map_shrink(&game->map)) {
        fprintf(stderr, "Couldn't allocate memory for: shrink map.\n");
        return;
    }

    for (int i = 0; i < game->elfs.size; ++i) {
        elf_t *elf = &game->elfs.elf[i];

        if (elf->hp > 0) {
            if (elf->cell == NULL ||
                !is_point_on_glacier(&game->map, elf->cell->x, elf->cell->y)) {
                elf_died(game, elf, NULL, MELTDOWN);
            } else {
                elf->stamina += stamina;
            }
        }
    }
}
