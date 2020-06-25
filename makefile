snek_AI: main.o snek_api.o
	gcc -o snek_AI main.o snek_api.o

main.o: main.c snek_api.h
	gcc -O -c main.c

snek_api.o: snek_api.c snek_api.h
	gcc -O -c snek_api.c

libsnek_py.so: snek_api.c snek_api.h 
	gcc -fPIC -shared -o ./py/libsnek_py.so snek_api.c
	
clean:
	rm *.o
