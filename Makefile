CC=gcc
INCLUDES=site.h AVL_SITE.h AVL_KEYWORDS.h AVREL_SITE.h inout.h heap.h newFun.h 
SOURCE=site.c AVL_SITE.c AVL_KEYWORDS.c AVREL_SITE.c inout.c heap.c newFun.c 
OBJECTS=site.o AVL_SITE.o AVL_KEYWORDS.o AVREL_SITE.o inout.o heap.o newFun.o
PROG=main.c
FLAGS= -Wall -Werror 
TARGET= main

all: $(OBJECTS)
	@$(CC) $(FLAGS) -o $(TARGET) $(OBJECTS) $(PROG) -lm -g

$(OBJECTS):
	@$(CC) -c $(SOURCE) -lm -g

run:
	@./$(TARGET)

valgrind: all
	@valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(TARGET)

clean: 
	@rm *.o
     
zip:
	zip arq-googlebot2 $(SOURCE) $(INCLUDES) $(PROG) Makefile README.txt googlebot.txt

