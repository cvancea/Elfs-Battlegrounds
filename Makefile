CC = gcc
CFLAGS = -Wall -Wextra -std=c99
EXE = snowfight

AUTHOR = VanceaCosminRazvan
GROUP = 313CA
HOMEWORK = 3

build: cmds.o elfs.o game.o main.o map.o utils.o
	$(CC) $(CFLAGS) -o $(EXE) $^

cmds.o: cmds.c
	$(CC) $(CFLAGS) -c $^	
elfs.o: elfs.c
	$(CC) $(CFLAGS) -c $^	
game.o: game.c
	$(CC) $(CFLAGS) -c $^
main.o: main.c
	$(CC) $(CFLAGS) -c $^
map.o: map.c
	$(CC) $(CFLAGS) -c $^
utils.o: utils.c
	$(CC) $(CFLAGS) -c $^

pack: Makefile README
	zip -q -9 "$(GROUP)_$(AUTHOR)_Tema$(HOMEWORK).zip" *.c *.h $^

clean_objects:
	rm -f *.o
clean: clean_objects
	rm -f $(EXE)