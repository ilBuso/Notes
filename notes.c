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
#include <sys/stat.h>
#include <stdbool.h>

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

typedef struct {
    char tags[MAX_VALUE_SIZE];
    char arguments[MAX_VALUE_SIZE];
    int fd;
    off_t page_offset;
} Metadata;

//---

Metadata get_metadata(char file_name[]) {
    // declare array
    static Metadata metadata;
    
    // open stram
    FILE* ptr_file = fopen(file_name, "r");
    if (ptr_file == NULL) {
        fprintf(stderr, "%s[ERROR]%s: Failed while opening the file\n", color_red, color_reset);
        exit(2);
    }

    char field[MAX_FIELD_SIZE];
    char value[MAX_VALUE_SIZE];
    bool twice = false;
    // get every line field and value until the EOF
    while (fgets(field, MAX_FIELD_SIZE, ptr_file) != NULL && fgets(value, MAX_VALUE_SIZE, ptr_file) != NULL) {
        
        // if "Tag"/"Argument" line then save value and set boolean
        if (strncmp(field, "Tag", 3)) {
            strcpy(metadata.tags, value);
        } else if (strncmp(field, "Argument", 8)) {
            strcpy(metadata.arguments, value);
        } else if (strcmp(field, "---\n")) {
            if (!twice) {
                twice = true;
            } else {
                // get the file descriptor of the file
                metadata.fd = fileno(ptr_file);
                // get the pointer value
                metadata.page_offset = lseek(metadata.fd, 0, SEEK_CUR);
                // exit the loop
                break;
            }
        }
    }

    // close stream
    fclose(ptr_file);

    // end
    return metadata;
}

//---

void new(char file_name[]) {

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

    FILE* ptr_template = fopen("./.config/note-template.md", "r");
    if (ptr_template == NULL) {
        fprintf(stderr, "%s[ERROR]%s: Failed while opening template file\n", color_red, color_reset);
        exit(2);
    }

    // each line from template
    char field[MAX_FIELD_SIZE];
    char value[MAX_VALUE_SIZE];
    char tags[MAX_VALUE_SIZE];
    char arguments[MAX_VALUE_SIZE];

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

            if (strncmp(field, "Tag", 3)) {
                strcpy(tags, value);
            } else if (strncmp(field, "Argument", 8)) {
                strcpy(arguments, value);
            }

            // append value to field
            strcat(field, value);
        }

        // write line on the new file
        fputs(field, ptr_file);
    }

    // close stream
    fclose(ptr_file);
    fclose(ptr_template);
}

void save(char file_name[]) {
    Metadata metadata;
    metadata = get_metadata(file_name);

    // create basic folders if they don't already exist
    mkdir("./notes", 0777);
    mkdir("./notes/arguments", 0777);
    mkdir("./notes/tags", 0777);

    // create the local path to the right tag folder
    char arguments_file[MAX_VALUE_SIZE];
    strcpy(arguments_file, "./notes/arguments/");
    strcat(arguments_file, metadata.arguments);
    strcat(arguments_file, ".md");

    // open stream
    FILE* ptr_file = fopen(file_name, "r");
    if (ptr_file == NULL) {
        fprintf(stderr, "%s[ERROR]%s: Failed while creating the new file\n", color_red, color_reset);
        exit(2);
    }

    FILE* ptr_arg = fopen(arguments_file, "a");
    if (ptr_arg == NULL) {
        fprintf(stderr, "%s[ERROR]%s: Failed while accessing/creating the argument file\n", color_red, color_reset);
        exit(2);
    }

    // ignora metadata
    lseek(metadata.fd, metadata.page_offset, SEEK_SET);

    // copy all the text in the file to the new file
    int c;
    while ((c = fgetc(ptr_file)) != -1) {
        fputc(c, ptr_arg);
    }

    // create the local path to the right tag folder
    char tags_folder[MAX_VALUE_SIZE];
    strcpy(tags_folder, "./notes/tags/");
    strcat(tags_folder, metadata.tags);

    // create the folder if not already exists
    mkdir(tags_folder, 0777);

    // move file to the right tag directory
    int move = rename(file_name, tags_folder);
    if (move == -1) {
        fprintf(stderr, "%s[ERROR]%s: Failed while moving file to the tag directory\n", color_red, color_reset);
        exit(3);
    }

    // close stream
    fclose(ptr_file);
    fclose(ptr_arg);

}

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

        new(file_name);
    } else if (strcmp(flag, "-s") || strcmp(flag, "--save")) {
        // get file name
        char file_name[MAX_FILENAME_SIZE];
        strcpy(file_name, argv[2]);

        save(file_name);
    }

    return 0;
}