main.exe: main.o queue.o linked_list.o  
	gcc -g --std=c99 main.o queue.o linked_list.o -lcurl -lpcre2-posix -o main.exe -Wl,-rpath,/usr/local/lib

main.o: main.c
	gcc -g -c --std=c99 main.c -o main.o

linked_list.o: linked_list.c
	gcc -g -c --std=c99 linked_list.c -o linked_list.o

queue.o: queue.c
	gcc -g -c --std=c99 queue.c -o queue.o

clean:
	rm *.o *.exe
