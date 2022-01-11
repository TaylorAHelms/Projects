#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "Shell.h"

using namespace std;

Shell::Shell(string diskname, int numberofblocks, int blocksize):Filesys(diskname, numberofblocks, blocksize)//constructor
{
	
}

int Shell::dir()//lists all files
{ 
	vector<string> filelist = ls();
	for (int i = 0; i<filelist.size(); i++)
	{
        	cout << filelist[i] << endl;
        }

	return 1;
}

int Shell::add(string file)//creates a new file
{
	return newfile(file);
}

int Shell::del(string file)//deletes a file
{
	int blocknum = getfirstblocknumber(file);
	if (blocknum == -1)
	{
		cout << "File not found." << endl;
		return 0;
	}
	while(blocknum != 0)
	{
		blocknum = nextblock(file, blocknum);
		delblock(file, getfirstblocknumber(file));
	}
	return rmfile(file);
}

int Shell::type(string file)//lists the contents of the file
{
	if(getfirstblocknumber(file) == -1)
	{
		cout << "File not found." << endl;
		return 0;
	}
	//in the second condition && nextblock(file, i) != 0
	for(int i = getfirstblocknumber(file); i != 0; i = nextblock(file, i))
	{
		string buffer;
		readblock(file, i, buffer);
		printf("%s", buffer.c_str());
	}
	
	printf("\n");

	return (int)!!getfirstblocknumber(file);
}

int Shell::copy(string file1, string file2)//creates a new file and copies the contents of file1 to the new file
{
	int code = getfirstblocknumber(file1);

	if(code == -1)
	{
		cout << "File not found." << endl;
		return 0;
	}
	
	int code2 = getfirstblocknumber(file2);
	
	if(code2 != -1)
	{
		cout << "target file already exists" << endl;
		return 0;
	}
	
	int code3 = newfile(file2);
	
	if(code3 == 0)
	{
		cout << "No space in root directory" << endl;
		return 0;
	}
	delblock(file2, getfirstblocknumber(file2));
	int iblock = getfirstblocknumber(file1);
	while(iblock != 0)
	{
		string b;
		readblock(file1, iblock, b);
		
		int code4 = addblock(file2, b);
		if(code4 == 0)
		{
			cout << "No space left" << endl;
			del(file2);
			return 0;
		}
		iblock = nextblock(file1, iblock);
	}
	
	return 1;
}

int Shell::append(string file)//appends the file with input from keyboard
{
	for(int i = 0; i < getfilenamesize(); i++)
	{	
		if(file == getfilenameEntry(i))
		{
			cout << "Type information to be entered in to the file." << endl;
			cout << "The character '@' will be the end of input character." << endl;
			string input;
			getline(cin, input);
			string toblock;
			int count = 0;
			char c = input[count];
			while(c != '@')
			{
				toblock.push_back(c);
				++count;
				c = input[count];
			}
			vector<string> blocks = block(toblock, getblocksize());
			for(int j = 0; j < blocks.size(); j++)
			{
				addblock(file, blocks[j]);
			}
			return 1;
		}
	}
	cout << "File not found." << endl;
	return 0;
}

int Shell::clobber(string file)//completely empties the file recursively
{
	int fb = getfirstblocknumber(file);
	if(fb == -1)
	{
		cout << "File not found." << endl;
		return 0;
	}
	else if(fb == 0)
	{
		return 0;
	}
	else
	{
		delblock(file, fb);
		return clobber(file);
	}
}
