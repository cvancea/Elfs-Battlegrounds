// Copyright 2019 - Cosmin Vancea - 313CA

#include <stdio.h>
#include "./game.h"

// pentru testele checker-ului consider ca numele fisierelor de in/out coincid
// cu numele executabilului
// pentru debugging in msvc, consider ca numele este dat ca argument in cli
#ifdef _MSC_VER
    #define FILENAME_ARG 1
#else
    #define FILENAME_ARG 0
#endif

int main(int argc, char *argv[]) {
    if (argc != FILENAME_ARG + 1) {
        fprintf(stderr, "Invalid number of arguments.\n");
        return ERR_INVALID_ARGS;
    }

    return game_startup(argv[FILENAME_ARG]);
}
