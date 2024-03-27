vfs:	main.o vfs.o
	g++ main.o vfs.o -o vfs
main.o: main.cpp
	g++ -c main.cpp
vfs.o:  vfs.cpp vfs.hpp queue.hpp inode.hpp vector.hpp
	g++ -c vfs.cpp
clean:
	rm vfs.o main.o vfs