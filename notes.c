/*
    Title:  notes.c
    Author: ilBuso
    Date:   12th August 2024

    Spiegare a cosa serve sto script e e sue funzionalità
*/

//---

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//---

#define MAX_ARGS_NUMB 3
#define MAX_FLAG_SIZE 32
#define MAX_FILENAME_SIZE 255 // imposed by GNU/Linux
#define MAX_FIELD_SIZE 128
#define MAX_VALUE_SIZE 115

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
    char flag[MAX_FLAG_SIZE];
    strcpy(flag, argv[1]);

    // check if creating file
    if (strcmp(flag, "-n") || strcmp(flag, "--new")) {
        // get file name
        char file_name[MAX_FILENAME_SIZE];
        strcpy(file_name, argv[2]);

        // check if a file named that way already exists
        int already_exists = access(file_name, F_OK);
        if (!already_exists) {
            fprintf(stderr, "%s[ERROR]%s: A file with this name already exists\n", color_red, color_reset);
            exit(2);
        }

        // open streams
        FILE* ptr_file = fopen(file_name, "w+");
        if (ptr_file == NULL) {
            fprintf(stderr, "%s[ERROR]%s: Failed while creating the new file\n", color_red, color_reset);
            exit(2);
        }

        FILE* ptr_template = fopen("./note-template.md", "r");
        if (ptr_template == NULL) {
            fprintf(stderr, "%s[ERROR]%s: Failed while opening template file\n", color_red, color_reset);
            exit(2);
        }

        // each line from template
        char field[MAX_FIELD_SIZE];
        char value[MAX_VALUE_SIZE];

        // get line until EOF
        while (fgets(field, MAX_FIELD_SIZE, ptr_template) != NULL) {

            // if the string is the start/end of the metadata section ot the new line at the bottom
            if (strcmp(field, "---\n") && strcmp(field, "\n")) {
                
                // null terminate string
                field[strcspn(field, "\n")] = '\0';
                
                // print on stout
                fprintf(stdout, "%s", field);

                // get input from stdin
                fgets(value, sizeof(value), stdin);

                // append value to field
                strcat(field, value);
            }

            // write line on the new file
            fputs(field, ptr_file);
        }
    }

    return 0;
}