// Copyright 2019 - Cosmin Vancea - 313CA

#ifndef MAP_H
#define MAP_H

typedef struct elf_t elf_t;
typedef struct game_t game_t;

struct cell_t {
    int x;
    int y;
    int glove;
    elf_t *elf;
    int height;
};
typedef struct cell_t cell_t;

struct map_t {
    cell_t **cells;
    int radius;
    int center;
    int size;
    int times_shrinked;
};
typedef struct map_t map_t;


int map_alloc(map_t *map);
void map_free(map_t *map);
void map_read(game_t *game);
int map_shrink(map_t *map);
void map_move_cell(cell_t *dest, cell_t *src);

int is_point_valid(map_t *map, int x, int y);
int is_point_on_glacier(map_t *map, int x, int y);
int is_point_in_range_of_point(int x, int y, int cx, int cy, int r);

#endif
