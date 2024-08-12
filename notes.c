/*
    Title:  notes.c
    Author: ilBuso
    Date:   12th August 2024

    Spiegare a cosa serve sto script e e sue funzionalità
*/

//---

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//---

#define MAX_ARGS_NUMB 2

//---

char* color_red = "\033[31m";
char* color_green = "\033[32m";
char* color_reset = "\033[0m";

//---

int main(int argc, char* argv[]) {

    // check number of arguments
    if (argc > MAX_ARGS_NUMB) {
        fprintf(stderr, "%s[ERROR]%s: Wrong number of arguments\n", color_red, color_reset);
        exit(1);
    }


    // check keyword
    char* flag;
    strcpy(flag, argv[1]);
    if (strcmp(flag, "-n") || strcmp(flag, "--new")) {
        
    }

    return 0;
}