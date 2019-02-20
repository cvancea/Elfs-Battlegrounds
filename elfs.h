// Copyright 2019 - Cosmin Vancea - 313CA

#ifndef ELFS_H
#define ELFS_H

enum {
    KILLED,
    MISSED,
    FALLEN,
    SNOWSTORM,
    MELTDOWN
};

typedef struct cell_t cell_t;
typedef struct game_t game_t;
typedef struct elfs_t elfs_t;
typedef struct elf_t elf_t;

struct elf_t {
    char *name;
    cell_t *cell;
    int id;
    int hp;
    int stamina;
    int glove;
    int eliminated; // numarul de jucatori eliminati de acest jucator
};
typedef struct elf_t elf_t;

struct elfs_t {
    elf_t *elf;
    int size;
};
typedef struct elfs_t elfs_t;


int elfs_alloc(elfs_t *elfs);
void elfs_free(elfs_t *elfs, int is_clone);
int elfs_read(game_t *game);

void elf_move_in_cell(game_t *game, elf_t *elf,
                      cell_t* new_cell, cell_t *old_cell);
elf_t *elf_fight(elf_t *attacker, elf_t *defender);
int elf_turnbased_attack(elf_t *damager, elf_t *damaged);
void elf_died(game_t *game, elf_t *elf, elf_t *killer, int reason);

int elf_comp(const void *l, const void *r);

#endif
