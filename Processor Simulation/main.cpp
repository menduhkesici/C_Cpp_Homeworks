#include <iostream>
#include <fstream> // file streams
#include "Simulator.hpp"

using namespace std;

// It is assumed that the processes in the input file
// are arranged in the order of their arrival time.

int main()
{
    int  pID, priority, arrTime, runTime;

    ifstream infile; // input file stream

    infile.open("input.txt"); // input file is input.txt

    Simulator Processor; // Processor is an object from the Simulator class

    while(!infile.eof())  // continue while input file has unread text
    {
        infile >> pID >> priority >> arrTime >> runTime; // read input like cin

        Processor.addProcess(pID, priority, arrTime, runTime); // transfer the processes to the Simulator queue
    }

    Processor.startSimulation(); // start the simulation and write the results to output files

    infile.close(); // close the file

    return 0;
}
