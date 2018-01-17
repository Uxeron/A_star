ifeq ($(OS),Windows_NT)
	RM = del
	PROG = prog.exe
else
	RM = rm
	PROG = prog.out
endif

all: a_star.o main.o
	g++ main.o a_star.o -o $(PROG)
a_star.o: a_star.cpp
	g++ -c a_star.cpp -o a_star.o
main.o: main.cpp
	g++ -c main.cpp -o main.o
clean:
	$(RM) *.o
