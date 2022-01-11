#ifndef SDISK_H
#define SDISK_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Sdisk
{
 public:
	Sdisk(string diskname, int numberofblocks, int blocksize);//constructor function
	int getblock(int blocknumber, string& buffer);//reads block from file to buffer
	int putblock(int blocknumber, string buffer);//writes buffer to block in file
	int getnumberofblocks(); //accessor function
	int getblocksize(); //accessor function
	static vector<string> block(string buffer, int b);
 private:
	string diskname;	//file name of the software-disk
	int numberofblocks;	//number of blocks on disk
	int blocksize;		//block size in bytes
};

#endif
