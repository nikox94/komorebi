SRCDIR = ./src
CFLAGS = -g

OBJ = main.o
INC = -I "$(SRCDIR)"

raytracing: $(OBJ)
	g++ $(OBJ) $(CFLAGS) -o raytracing
	rm -f $(OBJ)

main.o:
	g++ $(CFLAGS) -c $(SRCDIR)/main.cpp $(INC)

clean:
	rm -f $(OBJ) raytracing
