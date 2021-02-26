// Copyright 2019 - Cosmin Vancea - 313CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./game.h"
#include "./map.h"


int map_alloc(map_t *map) {
    map->size = 2 * map->radius + 1;
    map->center = map->radius;
    map->times_shrinked = 0;
    map->cells = malloc(map->size * sizeof(cell_t *));
    if (!map->cells) {
        return 0;
    }

    for (int i = 0; i < map->size; ++i) {
        map->cells[i] = malloc(map->size * sizeof(cell_t));
        if (!map->cells[i]) {
            map_free(map);
            return 0;
        }

        for (int j = 0; j < map->size; ++j) {
            map->cells[i][j].x = i;
            map->cells[i][j].y = j;
            map->cells[i][j].elf = NULL;
        }
    }
    return 1;
}

void map_free(map_t *map) {
    if (map->cells != NULL) {
        for (int i = 0; i < map->size; ++i) {
            if (map->cells[i] != NULL) {
                free(map->cells[i]);
                map->cells[i] = NULL;
            }
        }

        free(map->cells);
        map->cells = NULL;
    }
}

void map_read(game_t *game) {
    for (int i = 0; i < game->map.size; ++i) {
        for (int j = 0; j < game->map.size; ++j) {
            fscanf(game->f_in, "%d %d", &game->map.cells[i][j].height,
                                        &game->map.cells[i][j].glove);
        }
    }
}

int map_shrink(map_t *map) {
    map_t shrinked;
    shrinked.radius = map->radius - 1;

    if (!map_alloc(&shrinked)) {
        return 0;
    }
    shrinked.times_shrinked = map->times_shrinked + 1;

    for (int i = 0; i < map->size; ++i) {
        for (int j = 0; j < map->size; ++j) {
            cell_t *orig_cell = &map->cells[i][j];

            if (i == 0 || i == map->size - 1 || j == 0 || j == map->size - 1) {
                // elfii eliminati nu vor mai avea o celula asociata
                if (orig_cell->elf != NULL) {
                    orig_cell->elf->cell = NULL;
                }
            } else {
                cell_t *shrinked_cell = &shrinked.cells[i - 1][j - 1];
                map_move_cell(shrinked_cell, orig_cell);
            }
        }
    }

    map_free(map);
    memcpy(map, &shrinked, sizeof(map_t));
    return 1;
}

void map_move_cell(cell_t *dest, cell_t *src) {
    dest->elf = src->elf;
    dest->glove = src->glove;
    dest->height = src->height;

    if (dest->elf != NULL) {
        dest->elf->cell = dest;
    }
}

int is_point_valid(map_t *map, int x, int y) {
    if (x < 0 || x >= map->size) {
        return 0;
    }

    if (y < 0 || y >= map->size) {
        return 0;
    }

    return 1;
}

int is_point_on_glacier(map_t *map, int x, int y) {
    return is_point_in_range_of_point(x, y, map->center, map->center,
                                      map->radius);
}

int is_point_in_range_of_point(int x, int y, int cx, int cy, int r) {
    return (x - cx) * (x - cx) + (y - cy) * (y - cy) <= r * r;
}
