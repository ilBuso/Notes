SHELL := /bin/bash

FILE_NAME = notes.c

compile:
	@gcc -Wall -o $(basename $(FILE_NAME)) $(FILE_NAME)