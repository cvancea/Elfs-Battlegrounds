// Copyright 2019 - Cosmin Vancea - 313CA

#ifndef GAME_H
#define GAME_H

#include "./elfs.h"
#include "./map.h"

// erori ce pot aparea la citirea si prelucrarea datelor (vezi: game_startup)
enum {
    ERR_OK = 0,
    ERR_INVALID_ARGS,
    ERR_FILE_IN,
    ERR_FILE_OUT,
    ERR_MEM_FILENAME,
    ERR_MEM_MAP,
    ERR_MEM_ELFS,
    ERR_READ_ELFS
};

// structura ce mentine starea jocului
struct game_t {
    FILE *f_in;
    FILE *f_out;
    elfs_t elfs;
    map_t map;
    int players_alive;
};
typedef struct game_t game_t;


int game_startup(const char *exe);
void game_cleanup(game_t *game);

elf_t *get_game_winner(game_t *game);
void game_print_winner(game_t *game);

#endif
