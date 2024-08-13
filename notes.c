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
    char* flag;
    strcpy(flag, argv[1]);
    // check if creating file
    if (strcmp(flag, "-n") || strcmp(flag, "--new")) {
        // get file name
        char* file_name;
        strcpy(file_name, argv[2]);

        // open streams
        FILE* ptr_file = fopen(file_name, "w+");
        if (ptr_file == NULL) {
            fprintf(stderr, "%s[ERROR]%s: Failed while creating the new file\n", color_red, color_reset);
            exit(2);
        }

        FILE* ptr_template = fopen("./note-template.md", "w+");
        if (ptr_file == NULL) {
            fprintf(stderr, "%s[ERROR]%s: Failed while opening template file\n", color_red, color_reset);
            exit(2);
        }

        // each line from template
        char field[MAX_FIELD_SIZE];
        char value[MAX_VALUE_SIZE];
        // get line until EOF
        while (fgets(field, MAX_FIELD_SIZE, ptr_template) != NULL) {
            
            // null terminate string
            field[strcspn(field, "\n")] = '\0';
            
            // print on stout
            fprintf(stdout, "%s", field);

            // get input from stdin
            sscanf(stdin, "%s", value);
            
/// handle exiding size of the array

            // append value to field
            strcat(field, value);

            // write line on the new file
            fputs(field, ptr_file);
        }
    }

    return 0;
}