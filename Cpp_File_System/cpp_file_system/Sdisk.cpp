#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include "Sdisk.h"

using namespace std;

Sdisk::Sdisk(string diskname, int numberofblocks, int blocksize)//constructor
{	
	this->diskname = diskname;//initializing variables
	this->numberofblocks = numberofblocks;//initializing variables
	this->blocksize = blocksize;//initializing variables
	fstream iofile;
	iofile.open(diskname.c_str());
	
	if(!iofile.good())//if opening the file is not successful
	{
		cout << "iofile bad" << endl;
		system(string("type nul > " + diskname).c_str());
		iofile.open(diskname.c_str(), ios::out);
		iofile.close();
		iofile.open(diskname.c_str(), ios::in | ios::app);
		for(int i=0; i<blocksize*numberofblocks; i++)//fills the Sdisk with "#"
		{
			iofile << "#";
		}
		iofile.close();
	}
	iofile.close();
}

int Sdisk::getblock(int blocknumber, string& buffer)//reads a block from iofile to buffer
{
	fstream iofile;
	iofile.open(diskname.c_str());//opens diskname as iofile
	if(iofile.good())//if iofile successfully opens
	{
		iofile.seekg(blocknumber*blocksize);//moves get pointer to the start of the desired block
		for(int i = 0; i < blocksize; i++)//loops for the entirety of the block
		{
			buffer.push_back(iofile.get());//reads one character to buffer from the iofile
		}
		iofile.close();
		return 1;
	}
	else//if iofile fails to open
	{
		return 0;
	}
}

int Sdisk::putblock(int blocknumber, string buffer)//writes buffer to a block in iofile
{
	fstream iofile;
	iofile.open(diskname.c_str());//opens diskname as iofile
	if(iofile.good())//if iofile successfully opens
	{
		iofile.seekp(blocknumber*blocksize);//moves put pointer to the start of the desired block
		for(int i = 0; i < blocksize; i++)//loops for the entirety of the block
		{
			iofile << buffer[i];//writes one character at a time from buffer to iofile
		}
		iofile.close();
		return 1;
	}
	else//if iofile fails to open
	{
		return 0;
	}
}

int Sdisk::getnumberofblocks()//returns the number of blocks in the disk
{
	return numberofblocks;
}

int Sdisk::getblocksize()//returns the size of each block in the disk
{
	return blocksize;
}

vector<string> Sdisk::block(string buffer, int b)//builds the block
{
	vector<string> blocks;
	int numberofblocks2 = 0;

	if(buffer.length()%b == 0)
	{
		numberofblocks2 = buffer.length()/b;
	}
	else
	{
		numberofblocks2 = buffer.length()/b + 1;
	}

	string tempblock;

	for(int i = 0; i < numberofblocks2; i++)
	{
		tempblock = buffer.substr(b*i, b);
		blocks.push_back(tempblock);
	}

	int lastblock = blocks.size() - 1;

	for(int i = blocks[lastblock].length(); i < b; i++)
	{
		blocks[lastblock] += "#";
	}

	return blocks;
}

