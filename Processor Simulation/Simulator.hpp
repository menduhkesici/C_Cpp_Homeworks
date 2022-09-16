#ifndef _SIMULATOR_HPP_
#define _SIMULATOR_HPP_

#include <iostream>
#include <fstream> // file streams
#include "Stack.hpp"
#include "Queue.hpp"
#include "Process.hpp"

using namespace std;

class Simulator
{
private:
    Stack<Process, 2> stack1; // the stack holds 2 items max.
    Queue<Process, 1000> queue1, queue2, queue3; // the queues hold 1000 items max.
    Queue<Process, 10000> process_queue; // process_queue holds all the processes in the order of their arrival time.
                                         // max. number of items in the process_queue is 10.000
public:
    Simulator(void);
    void addProcess(int i, int p, int a, int r);
    void startSimulation();
};

Simulator::Simulator(void)
{
}

void Simulator::addProcess(int i, int p, int a, int r)
{
    Process temp(i, p, a, r);
    process_queue.Insert(temp);
}

void Simulator::startSimulation()
{
    ofstream outputf; // output file stream
    ofstream latencyf; // output file stream

    outputf.open("output.txt"); // output file is output.txt
    latencyf.open("latency.txt"); // output file is latency.txt

    Process Running_Process(-1, 0, 0, 0);

    int cycle = 0;
    int flag = 0;
    // flag is set to 1 when cycle number is output to file.
    // it prevents double output of the cycle number.
    int flag2 = 0;
    // flag2 prevents the blank line at the beginning.

    while ((process_queue.QEmpty() == 0)
            || (queue1.QEmpty() == 0)
            || (queue2.QEmpty() == 0)
            || (queue3.QEmpty() == 0)
            || (stack1.StackEmpty() == 0)           // The simulation continues until all of the queues and
            || (Running_Process.getID() != -1))     // the stack are empty, and there is no running process.
    {
        flag = 0;

        // if the process is finished, reset the Running_Process, also write to output and latency files.
        if ((Running_Process.getID() != -1) && (Running_Process.getRunTime() == 0))
        {
            if (flag == 0)
            {
                if (flag2 == 1)
                    outputf << endl;
                else
                    flag2 = 1;
                outputf << "t=" << cycle << " ";
                flag = 1;
            }
            outputf << "P" << Running_Process.getID() << " finished" << endl;
            latencyf << "latency for P" << Running_Process.getID() << " is " << (cycle - Running_Process.getArrivalTime()) << endl;
            Running_Process.resetID();
        }

        // if a new process arrives in the corresponding cycle
        if ((process_queue.QEmpty() == 0) && (cycle == process_queue.QFront().getArrivalTime()))
        {
            Process temp = process_queue.Delete();
            if (flag == 0)
            {
                if (flag2 == 1)
                    outputf << endl;
                else
                    flag2 = 1;
                outputf << "t=" << cycle << " ";
                flag = 1;
            }
            outputf << "P" << temp.getID() << " arrives" << endl;

            // if the processor is idle, and the queues and the stack are empty, start processing the newly arrived one
            if ((Running_Process.getID() == -1)
                && (queue1.QEmpty() == 1)
                && (queue2.QEmpty() == 1)
                && (queue3.QEmpty() == 1)
                && (stack1.StackEmpty() == 1))
            {
                Running_Process = temp;
            }
            // if the running process has a lower priority (higher value), push the running process to stack, and start processing the newly arrived one
            else if (Running_Process.getPriority() > temp.getPriority())
            {
                outputf << "P" << Running_Process.getID() << " pushed on S with runTime " << Running_Process.getRunTime() << endl;
                outputf << "P" << temp.getID() << " preempted the CPU" << endl;

                stack1.Push(Running_Process); // push the Running_Process to the stack
                Running_Process = temp; // start processing the newly arriving process
            }
            // if running process has a higher or equal priority, insert the newly arrived one to the corresponding queue.
            else
            {
                outputf << "P" << temp.getID() << " pushed on Q";
                switch(temp.getPriority())
                {
                case 1:
                    queue1.Insert(temp);
                    outputf << "1" << endl;
                    break;
                case 2:
                    queue2.Insert(temp);
                    outputf << "2" << endl;
                    break;
                case 3:
                    queue3.Insert(temp);
                    outputf << "3" << endl;
                    break;
                default:
                    break;
                }
            }
        }

        // if there is no process running on processor, choose one from the stack or one of the queues and start processing it.
        if (Running_Process.getID() == -1)
        {
            // if stack is empty, check for the queues
            if (stack1.StackEmpty() == 1)
            {
                if (queue1.QEmpty() == 0) // if queue 1 has a process, start it
                {
                    Running_Process = queue1.Delete();
                    outputf << "P" << Running_Process.getID() << " popped from Q1" << endl;
                }
                else if (queue2.QEmpty() == 0) // if queue 2 has a process, start it
                {
                    Running_Process = queue2.Delete();
                    outputf << "P" << Running_Process.getID() << " popped from Q2" << endl;
                }
                else if (queue3.QEmpty() == 0) // if queue 3 has a process, start it
                {
                    Running_Process = queue3.Delete();
                    outputf << "P" << Running_Process.getID() << " popped from Q3" << endl;
                }
            }
            // if top of the stack has a priority 1, start it
            else if (stack1.Peek().getPriority() == 1)
            {
                Running_Process = stack1.Pop();
                outputf << "P" << Running_Process.getID() << " popped from S" << endl;
            }
            // if top of the stack has a priority 2, check for queue 1, start the minimum priority one
            else if (stack1.Peek().getPriority() == 2)
            {
                if (queue1.QEmpty() == 0) // if queue 1 has a process, start it
                {
                    Running_Process = queue1.Delete();
                    outputf << "P" << Running_Process.getID() << " popped from Q1" << endl;
                }
                else // if queue 1 doesn't have a process, start the top of the stack.
                {
                    Running_Process = stack1.Pop();
                    outputf << "P" << Running_Process.getID() << " popped from S" << endl;
                }
            }
            // if top of the stack has a priority 3, check for queues 1 and 2, start the minimum priority one
            else if (stack1.Peek().getPriority() == 3)
            {
                if (queue1.QEmpty() == 0) // if queue 1 has a process, start it
                {
                    Running_Process = queue1.Delete();
                    outputf << "P" << Running_Process.getID() << " popped from Q1" << endl;
                }
                else if (queue2.QEmpty() == 0) // if queue 2 has a process, start it
                {
                    Running_Process = queue2.Delete();
                    outputf << "P" << Running_Process.getID() << " popped from Q2" << endl;
                }
                else // if queue 1 and 2 don't have a process, start the top of the stack.
                {
                    Running_Process = stack1.Pop();
                    outputf << "P" << Running_Process.getID() << " popped from S" << endl;
                }
            }
        }

        // RunTime of the running process is decremented by one and cycle is incremented by one before going to the next cycle.
        Running_Process.decRunTime();
        cycle++;
    }

    outputf.close(); // close the files
    latencyf.close();
}

#endif // _SIMULATOR_HPP_
