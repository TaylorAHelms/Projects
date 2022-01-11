#ifndef SHELL_H
#define SHELL_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Filesys.h"

using namespace std;

class Shell: public Filesys
{
public :
Shell(string diskname, int blocksize, int numberofblocks);//constructor function
int dir();// lists all files
int add(string file);// add a new file using input from the keyboard
int del(string file);// deletes the file
int type(string file);//lists the contents of file
int copy(string file1, string file2);//copies file1 to file2
int append(string file);//appends an existing file using input from the keyboard
int clobber(string file);//clobbers the file recursively
};

#endif
