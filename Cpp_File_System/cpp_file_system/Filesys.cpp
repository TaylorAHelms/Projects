#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "Filesys.h"

using namespace std;

Filesys::Filesys(string diskname, int numberofblocks, int blocksize):Sdisk(diskname, numberofblocks, blocksize)//constructor
{
	string buffer;
	getblock(0, buffer);
	rootsize = getblocksize()/13;
	fatsize = getblocksize()/4 +1;
	cout << "buffer[0] value:" << buffer[0] << endl;
	cout << "buffer length:" << buffer.length() << endl;
	if(buffer[0] == '#') //no filesystem
	{
		ostringstream outstream;
		for(int i = 0; i < rootsize; i++)
		{
			filename.push_back("xxxxxxxx");
			firstblock.push_back(0);
			outstream << "xxxxxxxx" << " " << 0 << " ";
		}
		buffer = outstream.str();
		vector<string> blocks = block(buffer, getblocksize());
		for (int i = 0; i < blocks.size(); i++)
		{
			putblock(i, blocks[i]);
		}
		fat.push_back(2+fatsize);
		for(int i = 0; i <= fatsize; i++)
		{
			fat.push_back(-1);
		}
		for(int i = 2 + fatsize; i < getnumberofblocks(); i++)
		{
			fat.push_back(i+1);
		}
		cout << "fat[] size: " << fat.size() << endl;
		fat[fat.size() - 1] = 0;
		fssynch();
	}
	else //filesystem exists
	{
		cout << "filesystem exists" << endl;
		istringstream instream;
		instream.str(buffer);
		for(int i = 0; i < rootsize; i++)
		{
			string f;
			string b;
			instream >> f >> b;
			filename.push_back(f);
			firstblock.push_back(stoi(b));
		}

		cout << "filename size: " << filename.size() << endl;
		cout << "firstblock size: " << firstblock.size() << endl;
		cout << "rootsize: " << rootsize << endl;
		cout << "fatsize: " << fatsize << endl;
		cout << "fat[] size: " << fat.size() << endl;

		istringstream instream2;
		buffer.clear();
		for(int i = 0; i < fatsize; i++)
		{
			string g;
			getblock(2 + i, g);
			buffer += g;
		}
		instream2.str(buffer);
		for(int i = 0; i < getnumberofblocks(); i++)
		{
			string k;
			instream2 >> k;
			fat.push_back(stoi(k));
		}
		cout << "fat[] size: " << fat.size() << endl;
		
	}	
}
	
int Filesys::fsclose()//synchs and closes the filesystem
{
	fssynch();
	return 1;
}

void Filesys::fssynch()//synchs the filesystem
{
	string buffer;
	for(int i = 0; i < rootsize; i++)
	{
		buffer += filename[i] + " " + to_string(firstblock[i]) + " ";
	}
	for(int i = buffer.size(); i < getblocksize(); i++)
	{
		buffer += "#";
	}
	putblock(0, buffer);
	
	buffer.clear();
	for(int i = 0; i < fat.size(); i++)
	{
		buffer += to_string(fat[i]) + " ";
	}
	vector<string> blocks = block(buffer, getblocksize());
	for(int i = 0; i < blocks.size(); i++)
	{
		putblock(2 + i, blocks[i]);
	}
}

int Filesys::newfile(string file)//creates a new file
{
	for(int i = 0; i < rootsize; i++)
	{
		if( filename[i] == file)
		{
			cout << "file exists." << endl;
			return 0;
		}
	}
	for(int i = 0; i < rootsize; i++)
	{
		if(filename[i] == "xxxxxxxx")
		{
			filename[i] = file;
			firstblock[i] = fat[0];
			fat[0] = fat[fat[0]];
			fat[firstblock[i]] = 0;
			delblock(file, getfirstblocknumber(file));//makes the new file empty
			fssynch();
			return 1;
		}
	}
	return 0;	
}

int Filesys::rmfile(string file)//removes a file if it is empty
{
	for(int i = 0; i < rootsize; i++)
	{
		if(filename[i] == file)
		{
			if(firstblock[i] != 0)
			{
				cout << "File is not empty." << endl;
				return 0;
			}
			else
			{
				filename[i] = "xxxxxxxx";
				fssynch();
				return 1;
			}
		}
	}
	cout << "File does not exist." << endl;
	return 0;
}

int Filesys::getfirstblocknumber(string file)//finds the first block that a file holds
{
	for(int i = 0; i < filename.size(); i++)
	{
		if(filename[i] == file)
		return firstblock[i];
	}
	//cout << "File not found." << endl;
	return -1;
}

int Filesys::addblock(string file, string buffer)//adds a block to a file
{
	int first = getfirstblocknumber(file);
	if(first == -1) return -1;
	int allocate = fat[0]; 
	if(allocate == 0)
	{
		cout << "No free blocks." << endl;
		return 0; //no free block
	}
	fat[0] = fat[fat[0]]; 
	fat[allocate] = 0; 
	if(first == 0) //empty
	{	
		for(int i = 0; i < rootsize; i++)
		{
			if(filename[i] == file)
			{
				firstblock[i] = allocate;
				fssynch();
				putblock(allocate, buffer);
				return allocate; 
			}
		}
		return -1;
	}
	else //not empty
	{
		int block = first; 
		while(fat[block] != 0)
		{
			block = fat[block];
		}
		fat[block] = allocate; 
		fssynch();
		putblock(allocate, buffer);
		return allocate;
	}
}

bool Filesys::checkblock(string file, int blocknumber)//checks the contents of a block
{
	int iblock = getfirstblocknumber(file);
	while(iblock != 0)
	{
		if(iblock == blocknumber) return true;
		iblock = fat[iblock];
	}
	return false;
}

int Filesys::delblock(string file, int blocknumber)//deletes a block
{
	if(!checkblock(file, blocknumber))
	{
		return -1;
	}
	if(blocknumber == getfirstblocknumber(file))
	{
		for(int i = 0; i < filename.size(); i++)
		{
			if(file == filename[i])
			{
				firstblock[i] = fat[blocknumber];
				break;
			}
		}
	}
	else
	{
		int iblock = getfirstblocknumber(file);
		while(fat[iblock] != blocknumber)
		{
			iblock = fat[iblock];
		}
		fat[iblock] = fat[blocknumber];
	}
	fat[blocknumber] = fat[0];
	fat[0] = blocknumber;
	fssynch();
	return 1;
}

int Filesys::readblock(string file, int blocknumber, string& buffer)//reads the contents of a block
{
	if(checkblock(file, blocknumber))
	{
		getblock(blocknumber, buffer);
		return 1;
	}
	return -1;
}

int Filesys::writeblock(string file, int blocknumber, string buffer)//writes to a block
{
	if(checkblock(file, blocknumber))
	{
		putblock(blocknumber, buffer);
		return 1;
	}
	return -1;
}

int Filesys::nextblock(string file, int blocknumber)//cycles to the next block
{
	if(checkblock(file, blocknumber))
	{
		return fat[blocknumber];
	}
	return -1;
}

void Filesys::printvectors()
{
	for(int i = 0; i < fat.size(); i++)
	{
		cout << "fat[" << i << "] = " << fat[i] << endl;
	}
	
	cout << endl;

	for (int i = 0; i < filename.size(); i++)
	{
		cout << filename[i] << " " << firstblock[i] << endl;
	}	
}

vector<string> Filesys::ls()//lists the file names
{ 
	vector<string> flist;
	for (int i=0; i<filename.size(); i++)
        {
        	if (filename[i] != "xxxxxxxx")
           	{
             		flist.push_back(filename[i]);
        	}
	}
  	return flist;
}

int Filesys::getfilenamesize()//accessor function
{
	return filename.size();
}

string Filesys::getfilenameEntry(int i)//accessor function
{
	return filename[i];
}
/*
void Filesys::printfirstblock()
{
	for (int i = 0; i < firstblock.size(); i++)
		cout << firstblock[i] << endl;
}*/