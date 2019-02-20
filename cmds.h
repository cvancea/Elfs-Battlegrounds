// Copyright 2019 - Cosmin Vancea - 313CA

#ifndef CMDS_H
#define CMDS_H

typedef struct game_t game_t;
typedef struct elf_t elf_t;


void cmds_read(game_t *game);

void cmd_handle_move(game_t *game, char move, elf_t *elf);
void cmd_handle_snowstorm(game_t *game, int x, int y, int r, int dmg);
void cmd_handle_print_scoreboard(game_t *game);
void cmd_handle_meltdown(game_t *game, int stamina);

#endif
