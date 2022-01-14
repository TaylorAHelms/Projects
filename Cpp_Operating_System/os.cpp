/**************************************************************************
* Eric Blasko
* Taylor Helms
*
* os.cpp
*
* 10/31/2018
*
* This program imitates a functoning Operating System. OS loads '.s'
* files and sends them to Assembler to compile to '.o' files. OS creates
* a list of PCB's for each process and loads its object code into vm's
* memory. All process are loaded into the readyQ initially. OS then loads
* vm with the first process in the readyQ. When the VM is interupted, OS
* saves the state of the running process, reschedules all process, moves
* the last running process to its proper queue based on return status or
* terminates if necessary, then loads the next process from the readyQ into
* the VM. If readyQ is empty and waitQ is full, OS idle's until another
* process is available
*************************************************************************/

#include "os.h"
#include <iostream>

using namespace std;

//default constructor. loads programs on start
OS::OS()
{
    idle_time = 0;
    context_time = 0;
    sys_time = 0;
    pageFaults = 0;
    pageTable = PageTable(FRAMES);
    fifo = true;
    invertedPT.resize(FRAMES);
    for (int i = 0; i < FRAMES; i++)
        emptyFrames.push_back(false);
}

//gathers all '.s' files into a program file and compiles them to '.o' files. 
//Then creates a PCB for each process and loads it into the readyQ. Program
//is deleted after loading is complete
void OS::LoadProcess(string process, string process1, string process2, string process3, string process4, string process5, string process6
    , string process7, string process8, string process9)
{
	/*
    system("ls *.s > program");
    fstream filesIn;
    filesIn.open("program", ios::in);

    if (!filesIn)
    {
      cerr << "Error" << endl;
      exit(1);
    }

    string prog;

    while (filesIn >> prog)
    {
       newQ.push(prog);
    }

    system("rm program");
	*/
    newQ.push(process);
    newQ.push(process1);
    newQ.push(process2);
    newQ.push(process3);
    newQ.push(process4);
    newQ.push(process5);
    newQ.push(process6);
    newQ.push(process7);
    newQ.push(process8);
    newQ.push(process9);
    LongTermScheduler();
}

void OS::LongTermScheduler()
{
    int limit = 0;
    while (readyQ.size() + waitQ.size() < 5 && !newQ.empty())
    {
        string prog = newQ.front();
        newQ.pop();

        cout << "Loading " << prog << "..." << endl;
        fstream assemblyCode, objectCode;
        assemblyCode.open(prog.c_str(), ios::in);
        if (!assemblyCode)
        {
            cerr << "Error loading" << prog << endl;
            continue;
        }

        int pos = prog.find(".");
        string name = prog.substr(0, pos);
        string outputFile = name + ".o";
        string inFile = name + ".in";
        string outFile = name + ".out";
        string st = name + ".st";

        objectCode.open(outputFile.c_str(), ios::out);
        if (!objectCode)
        {
            cerr << "Error loading" << prog << endl;
            continue;
        }

        as.Assemble(assemblyCode, objectCode);

        assemblyCode.close();
        objectCode.close();

        objectCode.open(outputFile.c_str(), ios::in);
        if (!objectCode)
        {
            cerr << "Error loading" << prog << endl;
            continue;
        }

        int base = limit = 0;
        string temp;
        while (!objectCode.eof())
        {
            objectCode >> temp;
            limit++;
        }

        objectCode.close();

        PCB * p = new PCB(name, base, limit - 1);

        p->obj.open(outputFile.c_str(), ios::in);
        if (!p->obj)
        {
            cerr << "Error loading" << prog << endl;
            continue;
        }

        p->in.open(inFile.c_str(), ios::in);
      //  if (!p->in)
  //      {
   //         cerr << "Error loading " << prog << endl;
   //         continue;
   //     }
        p->out.open(outFile.c_str(), ios::out);

        if (!p->out)
        {
            cerr << "Error loading" << prog << endl;
            continue;
        }

        p->stack.open(st.c_str(), ios::in | ios::out);

        readyQ.push(p);
        jobs.push_back(p);

        objectCode.close();
    }
}

//Deconstructor. Prints system information to each processes output file. After, each process is done
//its '.st' file is removed, all files are closed and PCB * is deleted
OS::~OS()
{
    cout << endl;
    double userClocks = 0;
    double processes = 0;
    for (auto const& i : jobs)
    {
        userClocks += i->cpu_time;
        processes++;
    }

    for (auto const& i : jobs) {

        double systemTime = ((double)(context_time)+(double)(idle_time)) / 1000.0;
        double cpuTime = (double)(vm.clock) / 1000.0;
        double cpuUtil = (double)(vm.clock - idle_time) / (double)(vm.clock) * 100;
        double userCpu = userClocks / (double)(vm.clock + context_time) * 100;
        double throughput = (double)(processes) / ((double)(sys_time) / 1000.0);


        i->out << "\n\tSystem Information\n";
        i->out << "System Time: \t\t" << fixed << setprecision(3) << systemTime << " Seconds\n";
        i->out << "CPU Time: \t\t" << cpuTime << " Seconds\n";
        i->out << "CPU Utilization: \t" << cpuUtil << "%\n";
        i->out << "User CPU Utilization: \t" << userCpu << "%\n";
        i->out << "Throughput: \t\t" << throughput << " Per Seconds\n";
        i->out.close();
        i->in.close();
        i->stack.close();
        remove(string(i->prog + ".st").c_str());
        cout << "Closing " << i->prog << "..." << endl;
        delete i;
    }

}

///Runs as long as a process is in ready or wait queue's. Loads PCB to VM, runs VM, then
//performs context switch on return. if all process are in waitQ, system will idle
void OS::Run()
{
    while (!readyQ.empty() || !waitQ.empty() || running != NULL)
    {
        if (!readyQ.empty() || running != NULL)
        {
            if (running == NULL)
            {
                running = readyQ.front();
                readyQ.pop();
            }
            LoadState();
            vm.run(TIME_SLICE, running->in, running->out);
            ContextSwitch();
        }
        else
        {
            sys_time++;
            idle_time++;
            CheckWaitQ();
        }
    }
}


//loads current running process data into vm. If it has a stack, load
//it into memeory from stack file
void OS::LoadState()
{
    for (int i = 0; i < 4; i++)
        vm.r[i] = running->r[i];

    vm.pc = running->pc;
    vm.ir = running->ir;
    vm.sr = running->sr;
    vm.sp = running->sp;
    vm.base = running->base;
    vm.limit = running->limit;
    vm.tlb = running->pageTable;
    running->wait_time += (sys_time - running->wait_time_begin);

    if (running->sp < 256)
    {
        running->stack.open(string(running->prog + ".st").c_str(), ios::in);
        int temp = running->sp;
        while (temp < 256)
            running->stack >> vm.mem[temp++];

        running->stack.close();
    }
}

//saves vm data into the returned process. If it had a stack, it is 
//saved to stack file
void OS::SaveState()
{
    for (int i = 0; i < 4; i++)
        running->r[i] = vm.r[i];

    running->pc = vm.pc;
    running->ir = vm.ir;
    running->sr = vm.sr;
    running->sp = vm.sp;
    running->base = vm.base;
    running->limit = vm.limit;
    running->cpu_time += (vm.clock - vm.session);
    running->pageTable = vm.tlb;
    sys_time += (vm.clock - vm.session);

    if (running->sp < 256)
    {
        running->stack.open(string(running->prog + ".st").c_str(), ios::out);
        int stackSize = 0;
        int temp = running->sp;
        while (temp < 256)
        {
            running->stack << vm.mem[temp++] << endl;
            stackSize++;
        }

        if (running->largestStack < stackSize)
            running->largestStack = stackSize;

        running->stack.close();
    }
}

//Saves state of returned process. Then checks if any process finished
//in the waitQ. Then looks at VM return status to tell if a read, write,
//timeslice, halt or error occured. Will ether end the process or put
//it into the proper queue 
void OS::ContextSwitch()
{
    context_time += CONTEXT_SWITCH_TIME;
    sys_time += CONTEXT_SWITCH_TIME;
    SaveState();

    CheckWaitQ();

    switch ((running->sr >> 5) & 0x27)
    {
    case 0: //Timeslice
        running->wait_time_begin = sys_time;
        readyQ.push(running);
        break;
    case 1: //halt
        message = "Successfully Completed";
        EndProcess(message);
        break;
    case 2: //out of bounds
        message = "Out of Bounds";
        EndProcess(message);
        break;
    case 3: //stack overflow
        message = "Stack Overflow Detected";
        EndProcess(message);
        break;
    case 4: //stack underflow
        message = "Stack Underflow Detected";
        EndProcess(message);
        break;
    case 5: //invalid opcode
        message = "Invalid Opcodes";
        EndProcess(message);
        break;
    case 6: //read
        waitQ.push(running);
        running->io_time_begin = sys_time;
        break;
    case 7: //write
        waitQ.push(running);
        running->io_time_begin = sys_time;
        break;
    case 32: //page fault
        pageFaults++;
        PageReplacement();
        waitQ.push(running);
        running->io_time_begin = sys_time;
        break;
    }

    if (!readyQ.empty())                                     //Process in readyQ
    {
        running = readyQ.front();
        readyQ.pop();
    }
    else                                                    //readyQ empty
        running = NULL;
}

//checks if I/O operation has completed. if it has, increase its io time,
//push to readyQ and start its wait time
void OS::CheckWaitQ()
{
    if (!waitQ.empty())
    {
        while (!waitQ.empty())
        {
            PCB * waiting = waitQ.front();
            if ((sys_time - waiting->io_time_begin) >= 27)
            {
                waiting->io_time += 27;
                waitQ.pop();
                readyQ.push(waiting);
                waiting->wait_time_begin = sys_time;
            }
            else
                return;
        }
    }
}

void OS::PageReplacement()
{
    int freeFrame = FindFreeFrame();
    if (freeFrame != -1)
    {
        LoadFrame(freeFrame);
    }

    if (freeFrame == -1 && fifo == true)
    {
        cout << "here" << endl;
        freeFrame = FindVictimFrame();
        LoadFrame(freeFrame);
    }
    else
    {

    }
    cout << fifo << endl;
    cout << "emptyframe: " << freeFrame << endl;
    for (auto i : invertedPT)
    {
        cout << "pid: \t" << i.pid << "\tPage:" << i.page << endl;
    }
}

int OS::FindFreeFrame()
{
    for (int i = 0; i < emptyFrames.size() - 15; i++)
    {
        if (emptyFrames[i] == false)
            return i;
    }
    return -1;
}

void OS::LoadFrame(int frameNumber)
{
    emptyFrames[frameNumber] = true;
    int page = running->sr >> 11;
    invertedPT[frameNumber].pid = running->prog;
    invertedPT[frameNumber].page = page;
    invertedPT[frameNumber].timeStamp = vm.clock;
    running->obj.seekp(page * 6 * 8, ios::beg);
    running->pageTable.page_table[page].frame = frameNumber;
    running->pageTable.page_table[page].valid = true;
    for (int j = 0; j < PAGESIZE; j++)
    {
        running->obj >> vm.mem[frameNumber * 8 + j];
        cout << running->prog << endl;
        cout << "FRAME: " << frameNumber << endl;
        cout << vm.mem[frameNumber * 8 + j] << endl;
    }
}

int OS::FindVictimFrame()
{
    int victimFrame = invertedPT[0].timeStamp;
    int frameNumber = 0;
    for (int i = 0; i < invertedPT.size() - 15; i++)
    {
        if (victimFrame > invertedPT[i].timeStamp)
        {
            victimFrame = invertedPT[i].timeStamp;
            frameNumber = i;
        }
    }

    string prog = invertedPT[frameNumber].pid;
    for (auto x : jobs)
    {
        if (x->prog == prog)
        {
            if (x->pageTable.page_table[frameNumber].modified == true)
            {
                x->obj.seekg(frameNumber * 6 * 8, ios::beg);
                for (int j = 0; j < PAGESIZE; j++)
                {
                    running->obj << vm.mem[frameNumber * 8 + j];
                }
            }
            x->pageTable.page_table[frameNumber].valid = false;
            invertedPT[frameNumber].pid = "";
            invertedPT[frameNumber].page = 0;
        }
    }

    emptyFrames[frameNumber] = false;
    return frameNumber;
}

//if a process has ended, print its status to its output file
void OS::EndProcess(string message)
{
    for (int i = 0; i < invertedPT.size(); i++)
    {
        if (invertedPT[i].pid == running->prog)
        {
            emptyFrames[i] = false;
            invertedPT[i].pid = "";
            invertedPT[i].page = 0;
        }
    }

    double time = running->cpu_time + running->wait_time + running->io_time;

    running->out << "\n\tProcess Information\n";
    running->out << message << endl;
    running->out << "CPU Time: \t\t" << running->cpu_time << " Ticks\n";
    running->out << "Waiting Time: \t\t" << running->wait_time << " Ticks\n";
    running->out << "I/O Time: \t\t" << running->io_time << " Ticks\n";
    running->out << "Turnaround Time: \t" << fixed << setprecision(2) << (time / 1000) << " Seconds\n";
    running->out << "Largest Stack: \t\t" << running->largestStack << endl;

    cout << running->prog << " Completed\n";
    running = NULL;
    LongTermScheduler();
}

//create instance of os and run it
int main(int argc, char *argv[])
{
    OS os;

    os.LoadProcess(argv[1],argv[2],argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9], argv[10]);
    cout << endl << "Running...\n" << endl;
    os.Run();
}


