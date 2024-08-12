SHELL := /bin/bash

FILE_NAME = notes.c

compile:
	@gcc -o $(basename $(FILE_NAME)) $(FILE_NAME)