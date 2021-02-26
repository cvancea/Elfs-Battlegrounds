// Copyright 2019 - Cosmin Vancea - 313CA

#ifndef UTILS_H
#define UTILS_H

char *fread_malloc_word(FILE *f);
void fread_free_word(char *word);
void fconsume_nonalnum(FILE *f);

void swap_int(int *x, int *y);

#endif
