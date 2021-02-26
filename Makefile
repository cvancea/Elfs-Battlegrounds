CC = gcc
CFLAGS = -Wall -Wextra -std=c99
EXE = snowfight

AUTHOR = VanceaCosminRazvan
GROUP = 313CA
HOMEWORK = 3

build: cmds.o elfs.o game.o main.o map.o utils.o
	$(CC) $(CFLAGS) -o $(EXE) $^

cmds.o: src/cmds.c
	$(CC) $(CFLAGS) -c $^	
elfs.o: src/elfs.c
	$(CC) $(CFLAGS) -c $^	
game.o: src/game.c
	$(CC) $(CFLAGS) -c $^
main.o: src/main.c
	$(CC) $(CFLAGS) -c $^
map.o: src/map.c
	$(CC) $(CFLAGS) -c $^
utils.o: src/utils.c
	$(CC) $(CFLAGS) -c $^

pack: Makefile README.md
	zip -q -9 "$(GROUP)_$(AUTHOR)_Tema$(HOMEWORK).zip" src/ $^

clean_objects:
	rm -f *.o
clean: clean_objects
	rm -f $(EXE)