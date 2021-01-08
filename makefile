#FirstName LastName 1001234567
#makefile for C++ program
SRC1 = spell_checker.c
SRC2 = spell.c
OBJ1 = $(SRC1:.c=.o)
OBJ2 = $(SRC2:.c=.o)
EXE = $(SRC1:.c=.e)

CFLAGS = -g

all : $(EXE)
 
$(EXE): $(OBJ1) $(OBJ2)
	gcc $(CFLAGS) $(OBJ1) $(OBJ2) -o $(EXE) 

$(OBJ1) : $(SRC1)
	gcc -c $(CFLAGS) $(SRC1) -o $(OBJ1) 
	
$(OBJ2) : $(SRC2)
	gcc -c $(CFLAGS) $(SRC2) -o $(OBJ2) 	