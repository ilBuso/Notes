/*
    Title:  notes.c
    Author: ilBuso
    Date:   12th August 2024

    Simple note organizer for notes
*/

//---

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <time.h>

//---

#define MAX_ARGS_NUMB 3
#define MAX_FLAG_SIZE 32
#define MAX_FILENAME_SIZE 255 // imposed by GNU/Linux
#define MAX_FIELD_SIZE 13
#define MAX_VALUE_SIZE 242

//---

char* color_red = "\033[31m";
char* color_green = "\033[32m";
char* color_reset = "\033[0m";

//---

typedef struct {
    char tags[MAX_VALUE_SIZE];
    char arguments[MAX_VALUE_SIZE];
    int fd;
    int page_offset;
} Metadata;

//---

Metadata get_metadata(char file_name[]) {
    // declare array
    Metadata metadata;
    
    // open stream
    FILE* ptr_file = fopen(file_name, "r");
    if (ptr_file == NULL) {
        fprintf(stderr, "%s[ERROR]%s: Failed while opening the file\n", color_red, color_reset);
        exit(2);
    }

    char field[MAX_FIELD_SIZE];
    char value[MAX_VALUE_SIZE];
    bool twice = false;
    // get every line field and value until the EOF
    while (fgets(field, MAX_FIELD_SIZE, ptr_file) != NULL) {
        // if beginning or end of metadata 
        if (strcmp(field, "---\n") == 0) {
            if (!twice) {
                twice = true;
            } else {
                // get the file descriptor of the file
                metadata.fd = fileno(ptr_file);
                // get the pointer value
                metadata.page_offset = ftell(ptr_file) + 1;
                // exit the loop
                break;
            }
        } else if (fgets(value, MAX_VALUE_SIZE, ptr_file) != NULL) {
            // if "Tag"/"Argument" line then save value and set boolean
            if (strncmp(field, "Tag", 3) == 0) {
                // null terminate string
                value[strcspn(value, "\n")] = '\0';
                // save value
                strcpy(metadata.tags, value);
            } else if (strncmp(field, "Argument", 8) == 0) {
                // null terminate string
                value[strcspn(value, "\n")] = '\0';
                // save value
                strcpy(metadata.arguments, value);
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

    // get line until EOF
    while (fgets(field, MAX_FIELD_SIZE, ptr_template) != NULL) {

        // if the string is the start/end of the metadata section ot the new line at the bottom
        if (strcmp(field, "---\n") != 0 && strcmp(field, "\n") != 0) {
            
            // null terminate string
            field[strcspn(field, "\n")] = '\0';

            // print on stout
            fprintf(stdout, "%s", field);

            // if is date field 
            if (strncmp(field, "Date", 4) == 0) {
                // get time
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                // format value string
                sprintf(value, "%d-%02d-%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
                // print of stdout
                fprintf(stdout, "%s", value);
            } else {
                // get value from user
                fgets(value, sizeof(value), stdin);
                
                //null terminate string
                value[strcspn(value, "\n")] = '\0';
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
    mkdir("./note", 0777);
    mkdir("./note/arguments", 0777);
    mkdir("./note/tags", 0777);

    // create the local path to the right tag folder
    char arguments_file[MAX_VALUE_SIZE];
    strcpy(arguments_file, "./note/arguments/");
    strcat(arguments_file, metadata.arguments);
    strcat(arguments_file, ".md");

    // open stream
    FILE* ptr_file = fopen(file_name, "r");
    if (ptr_file == NULL) {
        fprintf(stderr, "%s[ERROR]%s: Failed while opening the file\n", color_red, color_reset);
        exit(2);
    }

    FILE* ptr_arg = fopen(arguments_file, "w+");
    if (ptr_arg == NULL) {
        fprintf(stderr, "%s[ERROR]%s: Failed while accessing/creating the argument file\n", color_red, color_reset);
        exit(2);
    }

    // ignora metadata
    lseek(metadata.fd, metadata.page_offset, SEEK_CUR);

    // copy all the text in the file to the new file
    int c;
    while ((c = fgetc(ptr_file)) != EOF) {
        fputc(c, ptr_arg);
    }

    // create the local path to the right tag folder
    char tags_folder[MAX_VALUE_SIZE];
    strcpy(tags_folder, "./note/tags/");
    strcat(tags_folder, metadata.tags);

    // create the folder if not already exists
    mkdir(tags_folder, 0777);

    char tag_file[MAX_FILENAME_SIZE];
    strcpy(tag_file, tags_folder);
    strcat(tag_file, "/");
    strcat(tag_file, file_name);

    // move file to the right tag directory
    int move = rename(file_name, tag_file);
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
    if (strcmp(flag, "-n") == 0 || strcmp(flag, "--new") == 0) {
        // get file name
        char file_name[MAX_FILENAME_SIZE];
        strcpy(file_name, argv[2]);

        new(file_name);
    } else if (strcmp(flag, "-s") == 0 || strcmp(flag, "--save") == 0) {
        // get file name
        char file_name[MAX_FILENAME_SIZE];
        strcpy(file_name, argv[2]);

        save(file_name);
    }

    // end
    return 0;
}