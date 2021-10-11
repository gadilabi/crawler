main.exe: main.o myArray.o myString.o
	gcc -g --std=c99 main.o myArray.o myString.o -lcurl -lpcre2-posix -o main.exe -Wl,-rpath,/usr/local/lib

main.o: main.c
	gcc -g -c --std=c99 main.c -o main.o

myArray.o: myArray.c
	gcc -g -c --std=c99 myArray.c -o myArray.o

myString.o: myString.c
	gcc -g -c --std=c99 myString.c -o myString.o

clean:
	rm *.o *.exe
