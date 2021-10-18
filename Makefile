main.exe: main.o queue.o linked_list.o  
	gcc -Wall -g --std=c99 main.o queue.o linked_list.o -lcurl -lpcre2-posix -o main.exe -Wl,-rpath,/usr/local/lib

main.o: main.c
	gcc -Wall -g -c --std=c99 main.c -o main.o

linked_list.o: linked_list.c
	gcc -Wall -g -c --std=c99 linked_list.c -o linked_list.o

queue.o: queue.c
	gcc -Wall -g -c --std=c99 queue.c -o queue.o

link:
	gcc -g -Wall --std=c99 linked_list.c -o linked_list.exe

q:
	gcc -g -Wall --std=c99 queue.c linked_list.c -o q.exe

play:
	gcc -Wall -g --std=c99 playground.c -lpcre2-posix -o play.exe -Wl,-rpath,/usr/local/lib

clean:
	rm *.o *.exe
