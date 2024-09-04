SHELL := /bin/bash

FILE_NAME = notes.c

compile:
	@gcc -Wall -o $(basename $(FILE_NAME)) $(FILE_NAME)

new:
	@./notes --new ciao.md

open:
	@code ciao.md

save:
	@./notes --save ciao.md

clean:
	@rm ciao.md notes