#ifndef FILESYS_H
#define FILESYS_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Sdisk.h"

using namespace std;

class Filesys: public Sdisk
{
public:
	Filesys(string diskname, int numberofblocks, int blocksize);//constructor function
	int fsclose();//closes the filesystem
	void fssynch();//synchs the filesystem and the buffer
	int newfile(string file);//creates a new file
	int rmfile(string file);//removes a file
	int getfirstblocknumber(string file);//accessor function
	int addblock(string file, string block);//adds a block to a file
	int delblock(string file, int blocknumber);//deletes a block from a file
	int readblock(string file, int blocknumber, string& buffer);//reads the contents of a block
	int writeblock(string file, int blocknumber, string buffer);//writes to a block
	int nextblock(string file, int blocknumber);//moves to the next block in a file
	void printvectors();//prints the contents of the fat
	vector<string> ls();//lists the files
	int getfilenamesize();//accessor function
	string getfilenameEntry(int i);//accessor function
	//void printfirstblock(); test function
private:
	int rootsize;           // maximum number of entries in ROOT
	int fatsize;            // number of blocks occupied by FAT
	vector<string> filename;   // filenames in ROOT
	vector<int> firstblock; // firstblocks in ROOT
	vector<int> fat;             // FAT
	bool checkblock(string file, int blocknumber);
};

#endif
