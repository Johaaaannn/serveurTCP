#Makefile

# all
all:serveur main.o serveurtcp.o 

# serveur
serveur:main.o serveurtcp.o Makefile
	g++ -o serveur main.o serveurtcp.o -lpthread -lrt -Wall -lmysqlcppconn

# main.o
main.o: main.cpp Makefile
	g++ -c -o main.o main.cpp -lpthread -lrt -Wall

# serveurtcp.o
serveurtcp.o: serveurtcp.cpp serveurtcp.h Makefile
	g++ -c -o serveurtcp.o serveurtcp.cpp

# clean
clean:
	rm -f serveur *.o
