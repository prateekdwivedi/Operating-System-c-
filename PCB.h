
          /* To declare all the functions and class related to PCB */
#include<string>
#ifndef PCB_H
#define PCB_H
#include <vector>
using namespace std;

/* 
Comment: This class will perform the property of Process Control Block (or context) 
*/

class PCBManager
{ 
public:
    int PC;                           // Program Counter
    int REG[16];                      // Registers
    int pageTable[2];                    // page table reference
    int timeJob;                      // Time job entered the system
    int cumulativeTime;               // Cumulative time used by job
    int jobID;                        // Job ID
    int rdAddress;                    // Disk Address of current RD data record 
    int wrAddress;                    // Disk address of current WR record
    int wrEndAddress;                
    int timeCompletion;               // Time of completion of current I/O operation
    int loaderLength;
    int traceFlag;                    // Trace flag bit
    int memoryEntry;                    
    string errMessage;                // Error Message 
    vector<int> memoryTrack;          // It will track the memory 
    string nature;                    // Nature of Job termination    
    int pageFaultCount;
    int noOfTurns;
    int queueTurns;

// Assigning each page with bit0, bit1 & bit2
    vector<int> bit0;
    vector<int> bit1;
    vector<int> bit2;
          
// Function declarations i.e., Getters and Setters of all the variables
    void setPC(int content,int index);  // To set the programCounter 
    int getPC(int index);
    void setReg(int content,int index,int jobNumber);
    int getReg(int index,int jobNumber);   
    void setPageTable(int content,int index,int jobNumber);
    int getPageTable(int index,int jobNumber);
    void setTimeJob(int content,int index);
    int getTimeJob(int index);
    void setCumulativeTime(int content,int index);
    int getCumulativeTime(int index);
    void setJobID(int content,int index);
    int getJobID(int index);

    void setNoOfTurns(int content,int index);
    int getNoOfTurns(int index);
    void setQueueTurns(int content,int index);
    int getQueueTurns(int index);
    void setRdAddress(int content,int index);
    int getRdAddress(int index); 
    void setWdAddress(int content,int index);
    int getWdAddress(int index); 
    void setWdEndAddress(int content,int index);
    int getWdEndAddress(int index);
    void setTimeCompletion(int content,int index);
    int getTimeCompletion(int index);
    void setMemoryTrack(int content,int index);
    int getMemoryTrack(int index,int i);  
    void setTraceFlag(int content,int index);
    int getTraceFlag(int index);  
    void setMemoryEntry(int content,int index);
    int getMemoryEntry(int index); 
    void setErrMessage(string content,int index);
    string getErrMessage(int index);
    void setNature(string content,int index);
    string getNature(int index);
    void setPageFaultCount(int content,int index);
    int getPageFaultCount(int index);
    int findPage(int content,int index);
};


#endif
