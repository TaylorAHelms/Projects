cpp_file_system: main.o Sdisk.o Filesys.o Shell.o
	g++ main.o Sdisk.o Filesys.o Shell.o -o main

main.o: main.cpp
	g++ -c main.cpp

Sdisk.o: Sdisk.cpp
	g++ -c Sdisk.cpp

Filesys.o: Filesys.cpp
	g++ -c Filesys.cpp

Shell.o: Shell.cpp
	g++ -c Shell.cpp

clean:
	rm *.o
