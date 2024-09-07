# Notes - by _ilBuso_

# TOC
- [TOC](#toc)
- [Description](#description)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
    - [1. **Create a New Note**](#1-create-a-new-note)  
    - [2. **Save a Note**](#2-save-a-note)
- [Folder Structure](#folder-structure)
- [Example](#example)
- [License](#license)

# Description
This repository contains a simple C-based command-line tool to organize notes using metadata such as tags and arguments. The program allows users to create new notes, save them, and organize them into specific folders based on tags and arguments. This tool helps you easily classify and store your notes, making it a lightweight and useful addition to your workflow.

# Features
- **Create Notes**: Create new notes using a predefined template.
- **Save Notes**: Save the notes into specific folders organized by tags and arguments.
- **Metadata Support**: Add metadata (tags, arguments) to your notes for better classification.

# Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/ilBuso/notes.git
   ```

2. Navigate into the project directory:
   ```bash
   cd notes
   ```

3. Compile the C code using `make`:
   ```bash
   make
   ```

4. Run the executable:
   ```bash
   ./notes [flags] [file_name]
   ```

# Usage
The program accepts two main flags:

## 1. **Create a New Note**
   - Command:
     ```bash
     ./notes -n [file_name]
     ```
   - This command creates a new note by using the `note-template.md` file located in the `~/.config/` directory. The note will be initialized with metadata fields such as `Tags`, `Arguments`, and `Date`.

## 2. **Save a Note**
   - Command:
     ```bash
     ./notes -s [file_name]
     ```
   - This command saves the note into appropriate directories based on the provided metadata (tags and arguments). It creates folders under `./note/tags/` and `./note/arguments/` as required and moves the note file to the correct location.

# Folder Structure
- `./note/tags/`: Contains directories for each tag. Each note with the corresponding tag is saved in the related folder.
- `./note/arguments/`: Stores notes categorized by their arguments.
- `./.config/note-template.md`: Template used when creating a new note.

# Example
1. Create a new note:
   ```bash
   ./notes -n my_note.md
   ```

2. Save the note with the appropriate tags and arguments:
   ```bash
   ./notes -s my_note.md
   ```

# License

This project is licensed under the MIT License. See the `LICENSE` file for more details.
