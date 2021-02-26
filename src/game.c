// Copyright 2019 - Cosmin Vancea - 313CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./game.h"
#include "./map.h"
#include "./elfs.h"
#include "./cmds.h"


int game_startup(const char *exe) {
    game_t game = { 0 };
    int filename_size = strlen(exe) + strlen(".out") + 1;
    char *filename = malloc(filename_size);
    if (filename == NULL) {
        fprintf(stderr, "Couldn't allocate memory for: file name\n");
        game_cleanup(&game);
        return ERR_MEM_FILENAME;
    }

    snprintf(filename, filename_size, "%s.in", exe);
    game.f_in = fopen(filename, "r");
    if (!game.f_in) {
        fprintf(stderr, "Couldn't open file: %s\n", filename);
        free(filename);
        game_cleanup(&game);
        return ERR_FILE_IN;
    }

    snprintf(filename, filename_size, "%s.out", exe);
    game.f_out = fopen(filename, "w");
    if (!game.f_out) {
        fprintf(stderr, "Couldn't open file: %s\n", filename);
        free(filename);
        game_cleanup(&game);
        return ERR_FILE_OUT;
    }
    free(filename);

    fscanf(game.f_in, "%d %d", &game.map.radius, &game.elfs.size);
    if (!map_alloc(&game.map)) {
        fprintf(stderr, "Couldn't allocate memory for: map.\n");
        game_cleanup(&game);
        return ERR_MEM_MAP;
    }
    if (!elfs_alloc(&game.elfs)) {
        fprintf(stderr, "Couldn't allocate memory for: elfs.\n");
        game_cleanup(&game);
        return ERR_MEM_ELFS;
    }

    map_read(&game);
    if (!elfs_read(&game)) {
        fprintf(stderr, "Couldn't read: elfs.\n");
        game_cleanup(&game);
        return ERR_READ_ELFS;
    }
    cmds_read(&game);

    game_print_winner(&game);
    game_cleanup(&game);
    return ERR_OK;
}

void game_cleanup(game_t *game) {
    map_free(&game->map);
    elfs_free(&game->elfs, 0);

    if (game->f_in) {
        fclose(game->f_in);
        game->f_in = NULL;
    }

    if (game->f_out) {
        fclose(game->f_out);
        game->f_out = NULL;
    }
}

elf_t *get_game_winner(game_t *game) {
    if (game->players_alive != 1) {
        return NULL;
    }

    for (int i = 0; i < game->elfs.size; ++i) {
        elf_t *elf = &game->elfs.elf[i];

        if (elf->hp > 0) {
            return elf;
        }
    }
    return NULL;
}

void game_print_winner(game_t *game) {
    elf_t *winner = get_game_winner(game);
    if (winner != NULL) {
        fprintf(game->f_out, "%s has won.\n", winner->name);
    }
}
