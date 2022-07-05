#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Shell.h"

//This main program builds the filesystem and allows the user to input commands to the shell.
//Commands are input as follows: "command op1 op2"
//Possible commands are as follows:
//"dir": lists all the files
//"add op1": creates a file with the name op1
//"del op1": deletes the file with the name op1
//"type op1": lists the contents of the file
//"copy op1 op2": creates a new file named op2 and copies the contents of op1 in to op2
//"append op1": appends the file op1 with input from the keyboard, ending with "@"
//"clobber op1": clobbers the contents of the file op1 recursively

int main()
{
 Shell crab("disk1.txt", 256, 128);
 crab.newfile("file1");
 crab.newfile("file2");
 //crab.printfirstblock(); test function

 /*
		This portion is used for testing.
 string bfile1;
 string bfile2;

 for (int i=1; i <= 1024; i++)
    {
      bfile1+="1";
    }

  vector<string> blocks = Sdisk::block(bfile1,crab.getblocksize()); 

  //int blocknumber=0;

  for (int i=0; i< blocks.size(); i++)
     {
       crab.addblock("file1",blocks[i]);
     }


  crab.delblock("file1",crab.getfirstblocknumber("file1"));

  for (int i=1; i<=2048; i++)
     {
       bfile2+="2";
     }

  blocks = Sdisk::block(bfile2,crab.getblocksize()); 

  for (int i=0; i< blocks.size(); i++)
     {
       crab.addblock("file2",blocks[i]);
     }
  crab.delblock("file2",crab.getfirstblocknumber("file2"));
  crab.printvectors();*/

 string s;
 string command="go";
 string op1,op2;

 cout << "File system is running." << endl;

 while (command != "quit")
     {
       command.clear();
       op1.clear();
       op2.clear();
       cout << "$";
       getline(cin,s);
       int firstblank=s.find(' ');
       if (firstblank < s.length()) s[firstblank]='#';
       int secondblank=s.find(' ');
       command=s.substr(0,firstblank);
       if (firstblank < s.length())
         op1=s.substr(firstblank+1,secondblank-firstblank-1);
       if (secondblank < s.length())
         op2=s.substr(secondblank+1);
       if (command=="dir")
          {
            // use the ls function.
		crab.dir();
           }
       if (command=="add")
          {
            // The variable op1 is the new file.
		cout << crab.add(op1);
           }
       if (command=="del")
          {
            // The variable op1 is the file.
		crab.del(op1);
           }
       if (command=="type")
          {
            // The variable op1 is the file.
		crab.type(op1);
           }
       if (command=="copy")
          {
            // The variable op1 is the source file and the variable op2 is the destination file.
		crab.copy(op1, op2);
           }
       if (command == "print")
	  {
		crab.printvectors();
	  }
       if (command == "append")//The variable op1 is the file.
	  {
		crab.append(op1);
	  }
       if (command == "clobber")//The variable op1 is the file.
	  {
		crab.clobber(op1);
	  }
       
      }

 return 0;
}
