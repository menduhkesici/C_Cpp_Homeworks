#ifndef _PROCESS_HPP_
#define _PROCESS_HPP_

class Process
{
private:
    int ID, Priority, ArrivalTime, RunTime;
public:
    Process(void);
    Process(int i, int p, int a, int r);
    int getID(void) const;
    int getPriority(void) const;
    int getArrivalTime(void) const;
    int getRunTime(void) const;
    void decRunTime(void);
    void resetID(void);
};

Process::Process(void)
{
}

Process::Process(int i, int p, int a, int r)
{
    ID = i;
    Priority = p;
    ArrivalTime = a;
    RunTime = r;
}

int Process::getID(void) const
{
    return ID;
}

int Process::getPriority(void) const
{
    return Priority;
}

int Process::getArrivalTime(void) const
{
    return ArrivalTime;
}

int Process::getRunTime(void) const
{
    return RunTime;
}

void Process::decRunTime(void)
{
    RunTime--;
}

void Process::resetID(void)
{
    ID = -1;
    Priority = 0;
    ArrivalTime = 0;
    RunTime = 0;
}

#endif //_PROCESS_HPP_
