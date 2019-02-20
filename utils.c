// Copyright 2019 - Cosmin Vancea - 313CA

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "./utils.h"


// functia citeste un cuvant dintr-un fisier
// in caz de eroare de alocare, returneaza NULL
// daca buffer-ul e gol, returneaza un string gol (strlen = 0)
char *fread_malloc_word(FILE *f) {
    char *ret = malloc(1);
    int idx = 1, ch;

    if (ret == NULL) {
        return NULL;
    }

    ret[0] = 0;
    fconsume_nonalnum(f);
    while ((ch = fgetc(f))) {
        if (ch == ' ' || ch == '\t' || ch == '\n' || ch == EOF) {
            break;
        }

        char *tmp = realloc(ret, idx + 1);
        if (tmp == NULL) {
            // realloc a picat, returnez ce am putut citi pana acum
            return ret;
        }
        ret = tmp;
        ret[idx - 1] = ch;
        ret[idx] = 0;
        idx++;
    }

    return ret;
}

void fread_free_word(char *word) {
    free(word);
}

// sare peste caracterele non-aflanumerice din buffer
void fconsume_nonalnum(FILE *f) {
    int ch;
    while ((ch = fgetc(f))) {
        if (isalnum(ch) || ch == EOF) {
            ungetc(ch, f);
            return;
        }
    }
}

void swap_int(int *x, int *y) {
    int aux = *x;
    *x = *y;
    *y = aux;
}
