// Copyright 2019 - Cosmin Vancea - 313CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./game.h"
#include "./elfs.h"
#include "./map.h"
#include "./utils.h"


int elfs_alloc(elfs_t *elfs) {
    elfs->elf = malloc(elfs->size * sizeof(elf_t));
    if (!elfs->elf) {
        return 0;
    }

    memset(elfs->elf, 0, elfs->size * sizeof(elf_t));
    return 1;
}

void elfs_free(elfs_t *elfs, int is_clone) {
    if (elfs->elf) {
        if (!is_clone) {
            for (int i = 0; i < elfs->size; ++i) {
                elf_t *elf = &elfs->elf[i];
                if (elf->name != NULL) {
                    fread_free_word(elf->name);
                    elf->name = NULL;
                }
            }
        }

        free(elfs->elf);
        elfs->elf = NULL;
    }
}

int elfs_read(game_t *game) {
    game->players_alive = game->elfs.size;

    for (int i = 0; i < game->elfs.size; ++i) {
        elf_t *elf = &game->elfs.elf[i];
        int x, y;

        elf->id = i;
        elf->name = fread_malloc_word(game->f_in);
        if (elf->name == NULL) {
            return 0;
        }
        fscanf(game->f_in, "%d %d %d %d", &x, &y, &elf->hp, &elf->stamina);

        if (is_point_on_glacier(&game->map, x, y)) {
            cell_t *cell = &game->map.cells[x][y];
            elf_move_in_cell(game, elf, cell, NULL);
        } else {
            elf_died(game, elf, NULL, MISSED);
        }
    }

    return 1;
}

// functia pune un elf intr-o celula specificata
// validarea mutarii trebuie facuta de apelant
void elf_move_in_cell(game_t *game, elf_t *elf,
                      cell_t* new_cell, cell_t *old_cell) {
    elf->cell = new_cell;

    if (old_cell != NULL) {
        old_cell->elf = NULL;
    }

    if (new_cell->glove > elf->glove) {
        swap_int(&new_cell->glove, &elf->glove);
    }

    if (new_cell->elf != NULL && new_cell->elf->hp > 0) {
        elf_t *winner = elf_fight(elf, new_cell->elf);
        elf_t *loser;

        if (winner == elf) {
            loser = new_cell->elf;
        } else {
            loser = elf;
        }

        elf_died(game, loser, winner, KILLED);
        new_cell->elf = winner;
    } else {
        new_cell->elf = elf;
    }
}

elf_t *elf_fight(elf_t *attacker, elf_t *defender) {
    elf_t *first_attacker;
    elf_t *second_attacker;

    if (attacker->stamina >= defender->stamina) {
        first_attacker = attacker;
        second_attacker = defender;
    } else {
        first_attacker = defender;
        second_attacker = attacker;
    }

    while (1) {
        if (elf_turnbased_attack(first_attacker, second_attacker)) {
            return first_attacker;
        }

        if (elf_turnbased_attack(second_attacker, first_attacker)) {
            return second_attacker;
        }
    }
}

int elf_turnbased_attack(elf_t *damager, elf_t *damaged) {
    damaged->hp -= damager->glove;

    if (damaged->hp <= 0) {
        return 1;
    }

    return 0;
}

void elf_died(game_t *game, elf_t *elf, elf_t *killer, int reason) {
    if (game->players_alive == 1) {
        return;
    }

    if (elf->cell) {
        elf->cell->elf = NULL;
    }
    if (killer) {
        killer->eliminated++;
        killer->stamina += elf->stamina;
    }

    elf->hp = 0;
    game->players_alive--;

    switch (reason) {
    case KILLED:
        fprintf(game->f_out, "%s sent %s back home.\n",
                killer->name, elf->name);
        break;
    case MISSED:
        fprintf(game->f_out, "%s has missed the glacier.\n",
                elf->name);
        break;
    case FALLEN:
        fprintf(game->f_out, "%s fell off the glacier.\n",
                elf->name);
        break;
    case SNOWSTORM:
        fprintf(game->f_out, "%s was hit by snowstorm.\n",
                elf->name);
        break;
    case MELTDOWN:
        fprintf(game->f_out, "%s got wet because of global warming.\n",
                elf->name);
        break;
    }
}

int elf_comp(const void *l, const void *r) {
    elf_t *left = (elf_t *)l;
    elf_t *right = (elf_t *)r;

    // cazul: unul dry, altul wet
    if (left->hp <= 0 && right->hp > 0) {
        return 1;
    }
    if (left->hp > 0 && right->hp <= 0) {
        return -1;
    }

    // amandoi dry/wet, verific eliminarile
    if (left->eliminated < right->eliminated) {
        return 1;
    }
    if (left->eliminated > right->eliminated) {
        return -1;
    }

    // amandoi dry/wet + acelasi nr de eliminari, verific numele
    return strcmp(left->name, right->name);
}
