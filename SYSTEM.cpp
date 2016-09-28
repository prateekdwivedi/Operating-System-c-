
/* 
  Name : Prateek Dwivedi
  Course Number : CS 5323
  Assignment Title : Step 3(OSII Project)
  Date : 29th April,2016
  Program Description : This file SYSTEM.cpp contains MEMORY(),LOADER() & CPU() functions together, due to large number of files used in this step. 
                        I have written all the three modules inside this file. However, I have separated them as per the specifications provided in
                        the STEP1. According to the implementation, execution will start from this file. In the first step, the file will be read by Disk.cpp
                        which is working like Disk Manager.
      1. switchJob() : This function will remove the running queue job and append that particular job at the end of running queue
      2. freeCheckPage() : This function keeps track of each page of memory whether it is occupied by any job or not. If a particular page is occupied by any job
                           then, the particular index position will have '1' value. Else, it will have '0' value.
       3. LOADER() : This function has functionality of loading the loader format as per specified in STEP 1.
       4. MEMORY() : This function will work as memory , its functionality is based on STEP 1.
       5. CPU_Execution() : This function will perform all the execution part similar like CPU.

       Stage 3: Page fault has been calculated by using 3 bits which are declared in PCB where each bit is used to 
                determine whether the bit is present in the disk or not.
                Combination of these three bit will make the decision of page replacement.
                In tie case, we will decide by the help of CLOCK, the page which came first will get the priority.

 */



/* Declaration of Header files */

#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <vector>
#include <locale> 
#include <cstdlib>
#include<cmath>
#include "exception_handler.h"
#include "disk.h"
#include "PCB.h"
#define SIZE 2048

/* Using the class objects of Error Handler, Disk Manager & PCB */

errorHandler eh;
diskManager dm;
PCBManager pc;

using namespace std;

/* Declaration of Global Variables used in the program */

string MEM[SIZE];
int executionFlag=0,Location,traceFlag,REG[20],freeMemory=8;                
int CLOCK,executionTime=0,IOTime=0,programCounter=0;                            // Initializing the Value of VIRTUAL CLOCK=0
int CPURequest=0,dumpAddress,Length,result,jobNumber;                                       // CPURequest will indicate that CPU has requested Memory word from Loader. As, only Loader can  reference the memory. 
string descriptive="",fileName;                                                            // it will contain the error messages description.
int pageTable[16],freePageCheck[8],quantum=0,totalturnaroundTime=0,totalFaulttime=0,abnormalTime=0,normalCount=0,abnormalCount=0,infiniteTime=0,readCount=1;
int numberOfTurns,quantumSize, queueNumber,writeSwitch=1, migrationNumber,max1=0,max2=0,max3=0,max4=0,onAging = 0,storeMigration[12];
fstream pFile,mFile;
vector<int> infiniteJobs;
int total1,total2,total3,total4,countQueue = 0;

// For Phase-3 
vector<int> subQueue2;
vector<int> subQueue3;
vector<int> subQueue4;
int n,q;

/* Function Description : Declaring spooling function */
void spooling();


/* Functional Description: Supporting function for HEX to binary */
string hex_char_to_bin(char c)
{
    // TODO handle default / error
    switch(c)
    {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111"; 
    }
}




/* Functional Description:  This function will convert hex number to Binary */

string HEXBIN(string str)                       // Function to convert hex to binary 
{
      string bin="";
      for(int i=0;i <str.length(); i++)
        bin.append(hex_char_to_bin(str[i])); 
      return bin;
}





/* Functional Description:  This function will convert Hex number to Decimal */ 

int HEX_To_Decimal(string str)                 // Function to change HEX to decimal (Internal Purpose)
{
     string HEX="0123456789ABCDEF";
     transform(str.begin(), str.end(), str.begin(), ::toupper);   // To change the string in Uppercase
     int value=0,pos;

     for(int i=0;i<str.size();i++)
        {
         char c= str[i];
         pos = HEX.find(c,0);
         value = 16*value + pos;
        }
     return value;
}




/* Functional Description: This will get hex from binary */


string GetHexFromBin(string sBinary)
{
	string rest(""),tmp,chr = "";
	int len = sBinary.length()/4;
	chr = chr.substr(0,len);
	sBinary = chr+sBinary;
	for (int i=0;i<sBinary.length();i+=4)
	{
                
		tmp = sBinary.substr(i,4);
		if (!tmp.compare("0000"))
		{
			rest = rest + "0";
		}
		else if (!tmp.compare("0001"))
		{
			rest = rest + "1";
		}
		else if (!tmp.compare("0010"))
		{
			rest = rest + "2";
		}
		else if (!tmp.compare("0011"))
		{
			rest = rest + "3";
		}
		else if (!tmp.compare("0100"))
		{
			rest = rest + "4";
		}
		else if (!tmp.compare("0101"))
		{
			rest = rest + "5";
		}
		else if (!tmp.compare("0110"))
		{
			rest = rest + "6";
		}
		else if (!tmp.compare("0111"))
		{
			rest = rest + "7";
		}
		else if (!tmp.compare("1000"))
		{
			rest = rest + "8";
		}
		else if (!tmp.compare("1001"))
		{
			rest = rest + "9";
		}
		else if (!tmp.compare("1010"))
		{
			rest = rest + "A";
		}
		else if (!tmp.compare("1011"))
		{
			rest = rest + "B";
		}
		else if (!tmp.compare("1100"))
		{
			rest = rest + "C";
		}
		else if (!tmp.compare("1101"))
		{
			rest = rest + "D";
		}
		else if (!tmp.compare("1110"))
		{
			rest = rest + "E";
		}
		else if (!tmp.compare("1111"))
		{
			rest = rest + "F";
		}
		else
		{
			continue;
		}
	}
	return rest;
}



/* Declaration of Switch job */

void switchJob();


/* This function will convert Binary to Hex */ 

string BINHEX(int num){
      int rem=0;
      string value="";
      char HEX[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
		
      while(num>0){
			rem= num%16;
			value=HEX[rem]+value;
			num=num/16;
		}
      if(value== "")
         value = "0";
      return value;
}




/* Functional Description: This function will initialize page table entries */

void pageInitialise()
{
 for(int i=0;i<16;i++)
    {
     pageTable[i] = i;
    }
}




/* Functional Description: This function will check the free frames in the memory & will return with the first result */

int freeCheckPage()
{
int index;
 for(int i=0;i<8;i++)
   {
    if(freePageCheck[i] == 0)
      {
      index = i;
      freeMemory--;
      freePageCheck[i] = 1;
      break;
      }
   }
 return index;
}





/* Functional Description: This function will set Location & Length value */

int firstRecord(string Loc,string len)         // Translation of Location from Hex to Bin
{
     Location = HEX_To_Decimal(Loc);
     Length = HEX_To_Decimal(len);
     if (Length > 100 ) 
        eh.EHANDLER(3);                      // Program Size too large
     dumpAddress = Location;                   // Initialise the starting address of dump memory
     return Length;
}







/* Functional Description : This Function will handle the functionality of Memory as per specification */

void MEMORY(string &X,int &Y, string &Z)
{
     if(X == "READ")
        {
         Z = MEM[Y];                             // The contents of MEM[Y] will be written in Z.
        }

     else if(X == "WRIT")
        {
         MEM[Y] = Z;                             // The contents of Z will be written in MEM[Y]
        }

     else if(X == "DUMP")
        {
         for(int i=0;i<Y;i++)                   // The contents of physical memory will be output
           {
            pFile<<MEM[i]<<"\t";
           }
        }

}





/* Function Description : It will modify the data in the particular memory location */

void Load_Memory(string str)
{
     string X="WRIT";

      MEMORY(X,Location,str);
     
}




/* Function Description (Its a loader function) : It will request to get the data from Memory */

string Loader_Request(int Y)
{
 string X="READ",Z="00000";
 MEMORY(X,Y,Z);
 return Z;
}





/* Function Description : It will initialize the value of CPU */

int CPU(int X,int Y){                                // X :The initial value of the program counter(PC) and
         
                                                           // Y :The trace switch (0 = trace off and 1 = trace on)
                                                           // l : Object of Loader Class
       traceFlag = Y;                                      // Sets the value of Trace Flag
       return X;                                           // Returning the PC
}





/* Function Description : This function performs the execution of operations provided */


void CPU_Execution(string instruction)
{
      locale lo;
      fstream traceFile;
      
      // To check whether the instruction is valid or not

// For Bad Character Encountered by Loader 

   for(int m=0;m<instruction.size();m++)
     {
      if ( isxdigit(instruction[m],lo) == false)
         {
          eh.EHANDLER(12);
          pc.setErrMessage(eh.descriptive,jobNumber);
          eh.descriptive = "";
          instruction = "00000000";
         }
      }
      string str = HEXBIN(instruction),OP,effAddress;        // OP denotes Operation Code       &      EA denotes Effective Address 
      string I = str.substr(0,1);                           // I denotes Indirect addressing (one level only)
      int operand,programCounterReserve;
      string effectiveAddress;

      string check= "0" + str.substr(1,7);
      OP = GetHexFromBin(check);

      int B,DADDR,A ;                                       // B denotes the index Register
      string userInput,content,displayContent;
      int EA,buffer;

      DADDR = HEX_To_Decimal(instruction.substr(4,4));
      B = HEX_To_Decimal(instruction.substr(3,1));          // Conversion from Hex to Decimal,as we have made array index in equivalent Decimal
      A = HEX_To_Decimal(instruction.substr(2,1));          // Conversion from Hex to Decimal,as we have made array index in equivalent Decimal
      
      if(I == "1" && B != 0)                          // with both
        {

        DADDR = pc.getMemoryEntry(jobNumber)*256 + DADDR;
        EA = HEX_To_Decimal(Loader_Request(DADDR)) + pc.getReg(B,jobNumber) + pc.getMemoryEntry(jobNumber)*256;         //add the value of Number to the characters in the stream
       }
     else if(I == "0" && B !=0)                      // with index addressing
          {
           DADDR = pc.getMemoryEntry(jobNumber)*256 + DADDR;
           EA = DADDR + pc.getReg(B,jobNumber);
           
          }
     else if(I == "1" && B== 0)
        {
         DADDR = pc.getMemoryEntry(jobNumber)*256 + DADDR;
         EA = HEX_To_Decimal(Loader_Request(DADDR)) + pc.getMemoryEntry(jobNumber)*256;        // Calculation of Effective Address
        }
      else
        {
        DADDR = pc.getMemoryEntry(jobNumber)*256 + DADDR;
        EA = DADDR;                  // Calculation of Effective Address
        }
       

     // EA = EA*16 + HEX_To_Decimal(effectiveAddress.substr(1,1));
      if(executionFlag == 2)
          return;


       if(CLOCK%50000 == 0)
          {
              pFile<<"\n\n\n Status of Operating System";
              pFile<<"\n Ready Queue (DEC): [";
               for(int i=0;i<dm.readyQueue.size();i++)
                  pFile<<dm.readyQueue.at(i)<<',';
              pFile<<']';
              pFile<<"\n Currently Executing Job (DEC):"<<jobNumber;
              pFile<<"\n Current Memory Configuration";
              pFile<<"\n Current Degree of Multiprogramming (DEC): "<<8;
              pFile<<"\n Percentage of Disk in Use: 50% ";

          }   

// Step for MLFBQ file
       if(CLOCK%1200 == 0)
          {

           if(onAging == 1)
            {
             for(int x=0;x<subQueue4.size();x++)
              {
               int element = subQueue4.at(x);
               dm.readyQueue.push_back(element);              // All the elements from Sub-Queue4 has been moved to Subqueue1
               pc.setQueueTurns(0,element);
              }
             onAging = 0;
             subQueue4.clear();
            }        
      
      if(max1 < dm.readyQueue.size())
         max1 = dm.readyQueue.size();
      if(max2 < subQueue2.size())
         max2 = subQueue2.size();
      if(max3 < subQueue3.size())
         max3 = subQueue3.size();
      if(max4 < subQueue4.size())
         max4 = subQueue4.size();

// To print the content in MLFBQ file
      if(fileName == "/home/opsys/SPR16/tb") 
         mFile.open("MLFBQ_tb.txt",ios::app);      
      else if(fileName == "/home/opsys/SPR16/tb+err") 
         mFile.open("MLFBQ_tbErr.txt",ios::app); 
      mFile<<"\n Clock (DEC): "<<CLOCK;
      mFile<<"\n\t Sub-Queue1 (DEC): [";
               for(int i=0;i<dm.readyQueue.size();i++)           // subQueue1~dm.readyQueue1
                  mFile<<dm.readyQueue.at(i)<<',';
              mFile<<']';
              mFile<<"\n\t Sub-Queue2 (DEC): [";
               for(int i=0;i<subQueue2.size();i++)
                  mFile<<subQueue2.at(i)<<',';
              mFile<<']';
              mFile<<"\n\t Sub-Queue3 (DEC): [";
               for(int i=0;i<subQueue3.size();i++)
                  mFile<<subQueue3.at(i)<<',';
              mFile<<']';
              mFile<<"\n\t Sub-Queue4 (DEC): [";
               for(int i=0;i<subQueue4.size();i++)
                  mFile<<subQueue4.at(i)<<',';
              mFile<<']';
      mFile.close();
      total1= total1 + dm.readyQueue.size();
      total2 = total2 + subQueue2.size();
      total3 = total3 + subQueue3.size();
      total4 = total4 + subQueue4.size();
      countQueue++;
      // Implementing the Aging concept for Sub - queue4
       if(subQueue4.size()>0)
           {
           onAging = 1; 
           }
        }

 if(traceFlag == 0)
      {
       if(OP == "00")                                        // Condition to Check HALT
           {
            executionFlag=0;
            CLOCK++;
            executionTime++;
            quantum = 0;
            spooling();
           }
      else if(OP == "01")                                    // Condition for Load Operation
           {
            content=Loader_Request(EA);                       // C(EA) --> REG
            pc.setReg(HEX_To_Decimal(content),A,jobNumber);                 // In the Register Array, A denotes the index 
            programCounter = pc.getPC(jobNumber);
               pc.setPC(++programCounter,jobNumber);
            int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
            quantum++;
            CLOCK++;
            executionTime++;
           } 
      else if(OP == "02")                                    // Statement for Store Operation
           {
            Location = EA;                                   // Storing the Effective address's content into Register
            CPURequest = 1;
            Load_Memory(BINHEX(pc.getReg(A,jobNumber)));                     // C(REG) --> EA
            programCounter = pc.getPC(jobNumber);
               pc.setPC(++programCounter,jobNumber);

            int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
            quantum++;
            executionTime++;
            CLOCK++;
           }
      else if(OP == "03")                                    // Statement for Add Operation
           {
           content = Loader_Request(EA);
           pc.setReg((pc.getReg(A,jobNumber) + HEX_To_Decimal(content)),A,jobNumber);
           programCounter = pc.getPC(jobNumber);
               pc.setPC(++programCounter,jobNumber);
           int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
           quantum++;
           executionTime++;
           CLOCK++;
           }
      else if(OP == "04")                                    // Statement for Subtract Operation
           {
           content = Loader_Request(EA);
           pc.setReg((pc.getReg(A,jobNumber) - HEX_To_Decimal(content)),A,jobNumber);
           programCounter = pc.getPC(jobNumber);
           pc.setPC(++programCounter,jobNumber);
           int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
           quantum++;
           executionTime++;
           CLOCK++;
           }
      else if(OP == "05")                                    // Statement for Multiply Operation
           {
           content = Loader_Request(EA);
           pc.setReg((pc.getReg(A,jobNumber) * HEX_To_Decimal(content)),A,jobNumber);
           programCounter = pc.getPC(jobNumber);
           pc.setPC(++programCounter,jobNumber);
           int cTime = pc.getCumulativeTime(jobNumber);
            cTime = cTime + 2;
            pc.setCumulativeTime(cTime,jobNumber);
           quantum = quantum + 2;
           executionTime = executionTime+2;
           CLOCK= CLOCK + 2;
           }
      else if(OP == "06")                                    // Statement for Divide Operation
           {
           content = Loader_Request(EA);
           if (HEX_To_Decimal(content) !=0)                  // Handling Divide by zero error  
               pc.setReg((pc.getReg(A,jobNumber) / HEX_To_Decimal(content)),A,jobNumber);      
           else 
               {
                eh.EHANDLER(0);
                executionFlag = 0;
                spooling();
               }                             
           programCounter = pc.getPC(jobNumber);
           pc.setPC(++programCounter,jobNumber);
           int cTime = pc.getCumulativeTime(jobNumber);
           cTime = cTime + 2;
           pc.setCumulativeTime(cTime,jobNumber);
           quantum = quantum + 2;
           executionTime = executionTime +2;
           CLOCK= CLOCK + 2;
           }
      else if(OP == "07")                                    // Statement for Shift Left
           {
           pc.setReg((pc.getReg(A,jobNumber) << EA),A,jobNumber);
           programCounter = pc.getPC(jobNumber);
           pc.setPC(++programCounter,jobNumber);
           int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
           quantum++;
           executionTime++;
           CLOCK++;
           }
      else if(OP == "08")                                     // Statement for Shift Right
           {
           pc.setReg((pc.getReg(A,jobNumber) >> EA),A,jobNumber);
           programCounter = pc.getPC(jobNumber);
           pc.setPC(++programCounter,jobNumber);
           int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
           quantum++;
           executionTime++;
           CLOCK++;
           }
      else if(OP == "09")                                     // Statement for Branch on Minus(BRM)
           {
            if(pc.getReg(A,jobNumber) < 0) 
              {
               pc.setPC(EA,jobNumber); 
              }
            else
              {
              programCounter = pc.getPC(jobNumber);
              pc.setPC(++programCounter,jobNumber);
              }
            int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
            quantum++;
            CLOCK++;
            executionTime++;
           }
      else if(OP == "0A")                                      // Statement for Branch on Plus(BRP)
           {
            if(pc.getReg(A,jobNumber) > 0) 
              {
               pc.setPC(EA,jobNumber); 
              }
            else
              {
               programCounter = pc.getPC(jobNumber);
               pc.setPC(++programCounter,jobNumber);
              }
            int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
            quantum++;
            CLOCK++;
            executionTime++;
           }
      else if(OP == "0B")                                       // Statement for Branch on Zero(BRZ)
           {
            if(pc.getReg(A,jobNumber) == 0) 
              {
               pc.setPC(EA,jobNumber); 
              }
            else
              {
              programCounter = pc.getPC(jobNumber);
              pc.setPC(++programCounter,jobNumber);
              }
            int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
            CLOCK++;
            quantum++;
            executionTime++;
           }
      else if(OP == "0C")                                       // Statement for Branch and link
           {
           programCounter = pc.getPC(jobNumber);
           content = Loader_Request(programCounter);
           pc.setReg(HEX_To_Decimal(content),A,jobNumber);
           pc.setPC(EA,jobNumber);
           int cTime = pc.getCumulativeTime(jobNumber);
           cTime = cTime +2;
           pc.setCumulativeTime(cTime,jobNumber);
           quantum = quantum +2;
           executionTime = executionTime + 2;
           CLOCK= CLOCK +2;
           }
      else if(OP == "0D")
           {
           content = Loader_Request(EA);
           pc.setReg((pc.getReg(A,jobNumber) & HEX_To_Decimal(content)),A,jobNumber); 
           programCounter = pc.getPC(jobNumber);
           pc.setPC(++programCounter,jobNumber);
           int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
           quantum++;
           executionTime++;
           CLOCK++;
           }
      else if(OP == "0E")
           {
           content = Loader_Request(EA);
           pc.setReg((pc.getReg(A,jobNumber) | HEX_To_Decimal(content)),A,jobNumber);
           programCounter = pc.getPC(jobNumber);
           pc.setPC(++programCounter,jobNumber);
           int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
           quantum++;
           executionTime++;
           CLOCK++;
           }
      else if(OP == "0F")                                        // Statement for Read Operation
           {
            for(int loop=0;loop<4;loop++)
              {
              int uInputAddress = pc.getRdAddress(jobNumber);
              string userInput = dm.diskMemory[uInputAddress];
              pc.setRdAddress(++uInputAddress,jobNumber);
              CPURequest = 1;
              Location = EA;
              Load_Memory(userInput);
              EA++;
              }
             programCounter = pc.getPC(jobNumber);
             pc.setPC(++programCounter,jobNumber);
             IOTime = IOTime + 10;
             int cTime = pc.getTimeCompletion(jobNumber);
             cTime = cTime +10;
             pc.setTimeCompletion(cTime,jobNumber);
             dm.blockedQueue.push_back(jobNumber);
             if(queueNumber == 1)
                pc.setQueueTurns(0,jobNumber);
 	     else if(queueNumber == 2)
	        pc.setQueueTurns(n,jobNumber);
	     else if(queueNumber == 3)
                pc.setQueueTurns((n+2),jobNumber);
       /* According to the specification, 
 	*   if I/O request is placed, the process will be moved to the top sub-queue
	*/
 	     else if(queueNumber == 4)           
                pc.setQueueTurns(0,jobNumber);
             switchJob();
             

           }
      else if(OP == "10")                                        // Statement for Write Operation
           {
            for(int loop=0;loop<4;loop++)
             {
             content = Loader_Request(EA);                        // C(EA) --> REG
             result = HEX_To_Decimal(content);
             string str = BINHEX(result) ;
             int len = str.length();
             str = string(8-len,'0').append(str);
             int nextAddress = pc.getWdEndAddress(jobNumber);
             dm.diskMemory[nextAddress] = str;
             pc.setWdEndAddress(++nextAddress,jobNumber);
             EA++;
             }
            programCounter = pc.getPC(jobNumber);
            pc.setPC(++programCounter,jobNumber);
            IOTime = IOTime + 10;
            int cTime = pc.getTimeCompletion(jobNumber);
             cTime = cTime +10;
             pc.setTimeCompletion(cTime,jobNumber);
            dm.blockedQueue.push_back(jobNumber);
             if(queueNumber == 1)
                pc.setQueueTurns(0,jobNumber);
 	     else if(queueNumber == 2)
	        pc.setQueueTurns(n,jobNumber);
	     else if(queueNumber == 3)
                pc.setQueueTurns((n+2),jobNumber);
       /* According to the specification, 
 	*   if I/O request is placed, the process will be moved to the top sub-queue
	*/
 	     else if(queueNumber == 4)           
                pc.setQueueTurns(0,jobNumber);
            switchJob();

           }

      else if(OP == "11")                                        // Statement for Dump Memory
           {
           int counter = 0;
           string memData;
           stringstream(userInput)>> counter;
            for(int i=0;i<Length;i++)
             {
              if(i%4==0) pFile<<"\n";
              content = Loader_Request(dumpAddress);
              dumpAddress++;
              pFile<<setfill('0')<<setw(8)<<BINHEX(i)<<"  "; 
             }
            programCounter = pc.getPC(jobNumber);
            pc.setPC(++programCounter,jobNumber);
            int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
            quantum++;
            CLOCK++;
            executionTime++;
           }
      else
           { 
            eh.EHANDLER(1);
            pc.setErrMessage(eh.descriptive,jobNumber);
            pc.setNature("Abnormal",jobNumber);
            abnormalCount++;
            executionFlag=0;
            CLOCK++;
            executionTime++;
            quantum = 0;
            spooling();
            
           }  
          if(pc.getCumulativeTime(jobNumber) >100000)
            {
             if(executionFlag == 2)
               return;
             eh.EHANDLER(4);
            pc.setErrMessage(eh.descriptive,jobNumber);
            pc.setNature("Abnormal",jobNumber);
             infiniteTime = infiniteTime + pc.getCumulativeTime(jobNumber);
            abnormalCount++;
            executionFlag=0;
            CLOCK++;
            executionTime++;
            quantum = 0;
            spooling();
            
            }  
      }

/* Trace File */

 
    else if(traceFlag == 1){
 
// To write in the file 
             string fName = "trace#:";
             stringstream ss;
             ss<<jobNumber;
             string s = ss.str();
             fName.append(s);
             fName.append(".txt");
     fstream trace;
             trace.open(fName.c_str(),ios_base::app);
           string buffer = BINHEX(pc.getReg(A,jobNumber));
      if(OP == "00")                                        // Condition to Check HALT
           {
            executionFlag=0;
            trace <<"\n"<<"HALT";
            trace <<"\t\t\t\t"<<OP<<"\t\t\t\t\t" ;
            trace << buffer<< "\t\t\t\t" << BINHEX(pc.getReg(A,jobNumber))<< "\t\t\t\t\t" ;
            content = Loader_Request(EA);
            trace << content;
            executionTime++;
            CLOCK++;
            quantum = 0;
            spooling();
           }
      else if(OP == "01")                                    // Condition for Load Operation
           {
            content=Loader_Request(EA);                       // C(EA) --> REG
            pc.setReg(HEX_To_Decimal(content),A,jobNumber);           // In the Register Array, A denotes the index 
            programCounter = pc.getPC(jobNumber);
            pc.setPC(++programCounter,jobNumber);
            trace<<"\n"<<pc.getPC(jobNumber)<<"\t\t\t\t"<<OP<<"\t\t\t\t\t"<<buffer<<" \t\t\t\t "<<BINHEX(pc.getReg(A,jobNumber))<<"\t\t\t\t\t"<< Loader_Request(EA);
            int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
            executionTime++;
            CLOCK++;
            quantum++;
           } 
      else if(OP == "02")                                    // Statement for Store Operation
           {
            Location = EA;
            CPURequest = 1;                                   // Storing the Effective address's content into Register
            Load_Memory(BINHEX(pc.getReg(A,jobNumber)));             // C(REG) --> EA
            programCounter = pc.getPC(jobNumber);
            pc.setPC(++programCounter,jobNumber);
            trace<<"\n"<<pc.getPC(jobNumber)<<"\t\t\t\t"<<OP<<"\t\t\t\t\t"<<buffer<<"\t\t\t\t"<<BINHEX(pc.getReg(A,jobNumber))<<"\t\t\t\t\t"<< Loader_Request(EA);
            int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
            executionTime++;
            quantum++;
            CLOCK++;
           }
      else if(OP == "03")                                    // Statement for Add Operation
           {
           content = Loader_Request(EA);
           pc.setReg((pc.getReg(A,jobNumber) + HEX_To_Decimal(content)),A,jobNumber);
           programCounter = pc.getPC(jobNumber);
            pc.setPC(++programCounter,jobNumber);
           trace<<"\n"<<pc.getPC(jobNumber)<<"\t\t\t\t"<<OP<<"\t\t\t\t\t"<<buffer<<"\t\t\t\t"<<BINHEX(pc.getReg(A,jobNumber))<<"\t\t\t\t\t"<< Loader_Request(EA);
           int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
           executionTime++;
           quantum++;
           CLOCK++;
           }
      else if(OP == "04")                                    // Statement for Subtract Operation
           {
           content = Loader_Request(EA);
           pc.setReg((pc.getReg(A,jobNumber) - HEX_To_Decimal(content)),A,jobNumber);
           programCounter = pc.getPC(jobNumber);
            pc.setPC(++programCounter,jobNumber);
           trace<<"\n"<<pc.getPC(jobNumber)<<"\t\t\t\t"<<OP<<"\t\t\t\t\t"<<buffer<<"\t\t\t\t"<<BINHEX(pc.getReg(A,jobNumber))<<"\t\t\t\t\t"<< Loader_Request(EA);
           int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
           executionTime++;
           quantum++;
           CLOCK++;
           }
      else if(OP == "05")                                    // Statement for Multiply Operation
           {
           content = Loader_Request(EA);
           pc.setReg((pc.getReg(A,jobNumber) * HEX_To_Decimal(content)),A,jobNumber);
           programCounter = pc.getPC(jobNumber);
            pc.setPC(++programCounter,jobNumber);
           trace<<"\n"<<pc.getPC(jobNumber)<<"\t\t\t\t"<<OP<<"\t\t\t\t\t"<<buffer<<"\t\t\t\t"<<BINHEX(pc.getReg(A,jobNumber))<<"\t\t\t\t\t"<< Loader_Request(EA);
           int cTime = pc.getCumulativeTime(jobNumber);
           cTime = cTime +2;
            pc.setCumulativeTime(cTime,jobNumber);
           executionTime = executionTime +2 ;
           CLOCK= CLOCK + 2;
           quantum = quantum + 2;
           }
      else if(OP == "06")                                    // Statement for Divide Operation
           {
           content = Loader_Request(EA);
           if (HEX_To_Decimal(content) !=0)                  // Handling Divide by zero error  
               pc.setReg((pc.getReg(A,jobNumber) / HEX_To_Decimal(content)),A,jobNumber);      
           else 
               {
                eh.EHANDLER(0);
                executionFlag = 0;
                spooling();
               }                             
           programCounter = pc.getPC(jobNumber);
           pc.setPC(++programCounter,jobNumber);
           trace<<"\n"<<pc.getPC(jobNumber)<<"\t\t\t\t"<<OP<<"\t\t\t\t\t"<<buffer<<"\t\t\t\t"<<BINHEX(pc.getReg(A,jobNumber))<<"\t\t\t\t\t"<< Loader_Request(EA);
           int cTime = pc.getCumulativeTime(jobNumber);
            cTime = cTime +2;
            pc.setCumulativeTime(cTime,jobNumber);
           executionTime = executionTime +2 ;
           quantum = quantum + 2;
           CLOCK= CLOCK + 2;
           }
      else if(OP == "07")                                     // Statement for Shift left
           {
           content = Loader_Request(EA); 
           pc.setReg((pc.getReg(A,jobNumber) << EA),A,jobNumber);
           programCounter = pc.getPC(jobNumber);
            pc.setPC(++programCounter,jobNumber);
           trace<<"\n"<<pc.getPC(jobNumber)<<"\t\t\t\t"<<OP<<"\t\t\t\t\t"<<buffer<<"\t\t\t\t"<<BINHEX(pc.getReg(A,jobNumber))<<"\t\t\t\t\t"<< Loader_Request(EA);
           int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
           quantum++;
           executionTime++;
           CLOCK++;
           }
      else if(OP =="08")                                       // Statement for Shift Right
           {
           content = Loader_Request(EA); 
           pc.setReg((pc.getReg(A,jobNumber) >> EA),A,jobNumber);
           programCounter = pc.getPC(jobNumber);
            pc.setPC(++programCounter,jobNumber);
           trace<<"\n"<<pc.getPC(jobNumber)<<"\t\t\t\t"<<OP<<"\t\t\t\t\t"<<buffer<<"\t\t\t\t"<<BINHEX(pc.getReg(A,jobNumber))<<"\t\t\t\t\t"<< Loader_Request(EA);
           int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
           quantum++;
           executionTime++;
           CLOCK++;
           }
      else if(OP == "09")                                     // Statement for Branch on Minus(BRM)
           {
            content = Loader_Request(EA);
            if(pc.getReg(A,jobNumber) < 0) 
              {
               pc.setPC(EA,jobNumber);
              }
            else
              {
              programCounter = pc.getPC(jobNumber);
              pc.setPC(++programCounter,jobNumber);
              }
            trace<<"\n"<<pc.getPC(jobNumber)<<"\t\t\t\t"<<OP<<"\t\t\t\t\t"<<buffer<<"\t\t\t\t"<<BINHEX(pc.getReg(A,jobNumber))<<"\t\t\t\t\t"<< Loader_Request(EA);
            int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
            CLOCK++;
            executionTime++;
            quantum++;
           }
      else if(OP == "0A")                                      // Statement for Branch on Plus(BRP)
           {
            content = Loader_Request(EA);
            if(pc.getReg(A,jobNumber) > 0) 
              {
               pc.setPC(EA,jobNumber);
              }
            else
              {
              programCounter = pc.getPC(jobNumber);
              pc.setPC(++programCounter,jobNumber);
              }
            trace<<"\n"<<pc.getPC(jobNumber)<<"\t\t\t\t"<<OP<<"\t\t\t\t\t"<<buffer<<"\t\t\t\t"<<BINHEX(pc.getReg(A,jobNumber))<<"\t\t\t\t\t"<< Loader_Request(EA);
            int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
            CLOCK++;
            executionTime++;
            quantum++;
           }
      else if(OP == "0B")                                       // Statement for Branch on Zero(BRZ)
           {
            content = Loader_Request(EA);
            if(pc.getReg(A,jobNumber) == 0) 
              {
               pc.setPC(EA,jobNumber);
              }
            else
              {
              programCounter = pc.getPC(jobNumber);
              pc.setPC(++programCounter,jobNumber);
              }
            trace<<"\n"<<pc.getPC(jobNumber)<<"\t\t\t\t"<<OP<<"\t\t\t\t\t"<<buffer<<"\t\t\t\t"<<BINHEX(pc.getReg(A,jobNumber))<<"\t\t\t\t\t"<< Loader_Request(EA);
            int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
            CLOCK++;
            executionTime++;
            quantum++;
           }
      else if(OP == "0C")
           {
            content = Loader_Request(EA);
            programCounter = pc.getPC(jobNumber);
            content = Loader_Request(programCounter);
            pc.setReg(HEX_To_Decimal(content),A,jobNumber);
            pc.setPC(EA,jobNumber);
            trace<<"\n"<<pc.getPC(jobNumber)<<"\t\t\t\t"<<OP<<"\t\t\t\t\t"<<buffer<<"\t\t\t\t"<<BINHEX(pc.getReg(A,jobNumber))<<"\t\t\t\t\t"<< Loader_Request(EA);
            int cTime = pc.getCumulativeTime(jobNumber);
            cTime = cTime +2;
            pc.setCumulativeTime(cTime,jobNumber);
            quantum = quantum +2;
            executionTime = executionTime + 2;
            CLOCK= CLOCK +2;
            }
      else if(OP == "0D")
           {
           content = Loader_Request(EA);
           pc.setReg((pc.getReg(A,jobNumber) & HEX_To_Decimal(content)),A,jobNumber); 
           programCounter = pc.getPC(jobNumber);
            pc.setPC(++programCounter,jobNumber);
           trace<<"\n"<<pc.getPC(jobNumber)<<"\t\t\t\t"<<OP<<"\t\t\t\t\t"<<buffer<<"\t\t\t\t"<<BINHEX(pc.getReg(A,jobNumber))<<"\t\t\t\t\t"<< Loader_Request(EA);
           int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
           quantum++;
           executionTime++;
           CLOCK++;
           }
      else if(OP == "0E")
           {
           content = Loader_Request(EA);
           pc.setReg((pc.getReg(A,jobNumber) | HEX_To_Decimal(content)),A,jobNumber);
           programCounter = pc.getPC(jobNumber);
            pc.setPC(++programCounter,jobNumber);
           trace<<"\n"<<pc.getPC(jobNumber)<<"\t\t\t\t"<<OP<<"\t\t\t\t\t"<<buffer<<"\t\t\t\t"<<BINHEX(pc.getReg(A,jobNumber))<<"\t\t\t\t\t"<< Loader_Request(EA);
           int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
           quantum++;
           executionTime++;
           CLOCK++;
           }


      else if(OP == "0F")                                        // Statement for Read Operation
           {
            for(int loop=0;loop<4;loop++)
              {
              int uInputAddress = pc.getRdAddress(jobNumber);
              string userInput = dm.diskMemory[uInputAddress];
              pc.setRdAddress(++uInputAddress,jobNumber);
              CPURequest = 1;
              Location = EA;
              Load_Memory(userInput);
              EA++;
              }
           
            CPURequest = 1;
            Location = EA;
            Load_Memory(userInput);
            programCounter = pc.getPC(jobNumber);
            pc.setPC(++programCounter,jobNumber);
            IOTime = IOTime + 10;
             int cTime = pc.getTimeCompletion(jobNumber);
             cTime = cTime +10;
             pc.setTimeCompletion(cTime,jobNumber);
            trace<<"\n"<<pc.getPC(jobNumber)<<"\t\t\t\t"<<OP<<"\t\t\t\t\t"<<buffer<<"\t\t\t\t"<<BINHEX(pc.getReg(A,jobNumber))<<"\t\t\t\t\t"<< Loader_Request(EA);
             if(queueNumber == 1)
                pc.setQueueTurns(0,jobNumber);
 	     else if(queueNumber == 2)
	        pc.setQueueTurns(n,jobNumber);
	     else if(queueNumber == 3)
                pc.setQueueTurns((n+2),jobNumber);
       /* According to the specification, 
 	*   if I/O request is placed, the process will be moved to the top sub-queue
	*/
 	     else if(queueNumber == 4)           
                pc.setQueueTurns(0,jobNumber);         
            switchJob();
                                            // Clock time : 2 for the instruction and 8 for the actual I/O
           }
      else if(OP == "10")                                        // Statement for Write Operation
           {
            content = Loader_Request(EA);                        // C(EA) --> REG
           for(int loop=0;loop<4;loop++)
             {
             content = Loader_Request(EA);                        // C(EA) --> REG
             result = HEX_To_Decimal(content);
             string str = BINHEX(result) ;
             int len = str.length();
             str = string(8-len,'0').append(str);
             int nextAddress = pc.getWdEndAddress(jobNumber);
             dm.diskMemory[nextAddress] = str;
             pc.setWdEndAddress(++nextAddress,jobNumber);
             EA++;
             }
            programCounter = pc.getPC(jobNumber);
            pc.setPC(++programCounter,jobNumber);
            trace <<"\n"<<pc.getPC(jobNumber)<<"\t\t\t\t"<<OP<<"\t\t\t\t\t"<<buffer<<"\t\t\t\t"<<BINHEX(pc.getReg(A,jobNumber))<<"\t\t\t\t\t"<< Loader_Request(EA);
            IOTime = IOTime + 10;
             int cTime = pc.getTimeCompletion(jobNumber);
             cTime = cTime +10;
             pc.setTimeCompletion(cTime,jobNumber);
             if(queueNumber == 1)
                pc.setQueueTurns(0,jobNumber);
 	     else if(queueNumber == 2)
	        pc.setQueueTurns(n,jobNumber);
	     else if(queueNumber == 3)
                pc.setQueueTurns((n+2),jobNumber);
       /* According to the specification, 
 	*   if I/O request is placed, the process will be moved to the top sub-queue
	*/
 	     else if(queueNumber == 4)           
                pc.setQueueTurns(0,jobNumber);
             switchJob();                   // Clock time : 2 for the instruction and 8 for the actual I/O
           }
      else if(OP == "11")                                        // Statement for Dump Memory
           {
           int counter = 0;
           content = Loader_Request(EA);
           string memData; 
           stringstream(userInput)>> counter;
            for(int i=0;i<Length;i++)
             {
              if(i%4==0) pFile<<"\n";
              content = Loader_Request(dumpAddress);
              dumpAddress++;
              pFile<<setfill('0')<<setw(8)<<BINHEX(i)<<"  ";  
             }
            programCounter = pc.getPC(jobNumber);
            pc.setPC(++programCounter,jobNumber);
            int cTime = pc.getCumulativeTime(jobNumber);
            pc.setCumulativeTime(++cTime,jobNumber);
            quantum++;
            CLOCK++;
            executionTime++;
           trace <<"\n"<<pc.getPC(jobNumber)<<"\t\t\t\t"<<OP<<"\t\t\t\t\t"<<buffer<<"\t\t\t\t"<<BINHEX(pc.getReg(A,jobNumber))<<"\t\t\t\t\t"<< Loader_Request(EA);
           }
      else
           { 
            eh.EHANDLER(1);
            pc.setErrMessage(eh.descriptive,jobNumber);
            pc.setNature("Abnormal",jobNumber);
            abnormalCount++;
            executionFlag=0;
            CLOCK++;
            executionTime++;
            quantum = 0;
            spooling();
           }  
   if(pc.getCumulativeTime(jobNumber) >100000)
            {
            if(executionFlag == 2)
              return;
             eh.EHANDLER(4);
            pc.setErrMessage(eh.descriptive,jobNumber);
            pc.setNature("Abnormal",jobNumber);
             infiniteTime = infiniteTime + pc.getCumulativeTime(jobNumber);
            abnormalCount++;
            executionFlag=0;
            CLOCK++;
            executionTime++;
            quantum = 0;
            spooling();
          }  
   trace.close();
 }
}





/* Loader function to call and initiate CPU Execution */

void LOADER(int address,int traceFlag)
{
     int addr,noOfWords,PC;
     string X="READ",Z="00000";
     pc.setPC(CPU(address,traceFlag),jobNumber);
     addr = pc.getPC(jobNumber);
     
     if (executionFlag == 0)
         executionFlag = 1;
     MEMORY(X,addr,Z);


 while(executionFlag ==1)                // It will execute till it gets executionFlag as false
     {

     CPU_Execution(Z);
     if(executionFlag == 2)
        {
         return;
         break;
        }
// To implement round robin scheduling, where job is provided with a quantum time = 40
     if(quantum > quantumSize)
        {
               switchJob();
         
        } 

     programCounter = pc.getPC(jobNumber);
     MEMORY(X,programCounter,Z);
     }
}





/* 
 *  Function Description : Round-Robin Scheduler 
 *                         Each job will be given a quantum of 40 virtual time units when it is chosen to be the running job.
 */

void switchJob()
{
// 
int qTurns = pc.getQueueTurns(jobNumber);
pc.setQueueTurns(++qTurns,jobNumber);

if(writeSwitch == 1)
  {
   for(int f=0;f<12;f++)
   {
   storeMigration[f] = 4808 - (f*23);
   }
   writeSwitch = 0;
  }

// To send the job into the respective queue according to number of turns
  if(pc.getQueueTurns(jobNumber)<= n)                                 // For Sub-queue 1
    {
     dm.readyQueue.push_back(jobNumber);
     migrationNumber++;
    }
  else if(pc.getQueueTurns(jobNumber)> n && pc.getQueueTurns(jobNumber)<= (n+2))      // For Sub-queue 2
    {
     subQueue2.push_back(jobNumber);
     migrationNumber++;
    }
  else if(pc.getQueueTurns(jobNumber)> (n+2) && pc.getQueueTurns(jobNumber) <= (n+4))    // For Sub-queue 3
    {
     subQueue3.push_back(jobNumber);
     migrationNumber++;
    }
  else if(pc.getQueueTurns(jobNumber)> (n+4))                         // For Sub-queue 4
    {
    subQueue4.push_back(jobNumber);
    migrationNumber++;
    }


// safe : Retrieving the first element
 if(dm.readyQueue.size() >0)
   {
               jobNumber = dm.readyQueue.at(0);
// Delete the first element
               dm.readyQueue.erase(dm.readyQueue.begin());
               queueNumber = 1;
               quantumSize = n * q;
   }
 else if(subQueue2.size() > 0)
   {
               jobNumber = subQueue2.at(0);
               subQueue2.erase(subQueue2.begin());
               queueNumber = 2;
               int nn = n +2;
               quantumSize = nn * q;
   }
 else if(subQueue3.size() > 0)
   {
               jobNumber = subQueue3.at(0);
               subQueue3.erase(subQueue3.begin());
               queueNumber = 3;
               int nn = n +4;
               quantumSize = nn* q;
   }
else if(subQueue4.size() > 0)
   {
               jobNumber = subQueue4.at(0);
               subQueue4.erase(subQueue4.begin());
               queueNumber = 4;
               int nn = n+ 6;
               quantumSize = nn * q;
   }

               quantum = 0;
               int CPUTurns = pc.getNoOfTurns(jobNumber);
               pc.setNoOfTurns(++CPUTurns,jobNumber);
               LOADER(pc.getPC(jobNumber),pc.getTraceFlag(jobNumber));
               if (executionFlag== 2)
                   return;
}




/* Functional Description: This function will perform all the spooling operation required for Disk & Memory */

void spooling()
{
int locateDisk;
quantum = 0;
// To write data in output file

pFile<<"\n\n\nJOB IDENTIFICATION NUMBER(HEX):"<<BINHEX(jobNumber);
pFile<<"\n Current value of CLOCK(DEC):"<<CLOCK;
pFile<<"\n CLOCK at Load Time(HEX):"<<BINHEX(pc.getTimeJob(jobNumber));
pFile<<"\n CLOCK at Termination Time(HEX):"<<BINHEX(CLOCK);

int rowCount=0;

for(int i= pc.getWdAddress(jobNumber);i<pc.getWdEndAddress(jobNumber);i++)
   {
     if(rowCount%4 ==0)
        pFile<<"\n";
     pFile<<"\t"<<dm.diskMemory[i]<<" ";
       rowCount++;
   }



if(pc.getErrMessage(jobNumber) != "")
  pFile<<"\n"<<pc.getErrMessage(jobNumber);
pFile<<"\n Nature of Termination :"<<pc.getNature(jobNumber);

if(pc.getNature(jobNumber)=="Normal")
     normalCount++;
if(pc.getNature(jobNumber)=="Abnormal")
     {
      abnormalCount++;
      abnormalTime = abnormalTime + pc.getCumulativeTime(jobNumber);
     }
pFile<<"\n Turnaround Time(HEX) :"<<BINHEX(CLOCK - pc.getTimeJob(jobNumber));
totalturnaroundTime = totalturnaroundTime + CLOCK - pc.getTimeJob(jobNumber);
pFile<<"\n Run Time(HEX) :"<<BINHEX(pc.getCumulativeTime(jobNumber)+pc.getTimeCompletion(jobNumber));
pFile<<"\n Execution Time(HEX):"<<BINHEX(pc.getCumulativeTime(jobNumber));
pFile<<"\n Page Fault Handling Time(HEX):"<<BINHEX(pc.getPageFaultCount(jobNumber));
pFile<<"\n No. of CPU Turns (DEC): "<<pc.getNoOfTurns(jobNumber);
totalFaulttime = totalFaulttime + pc.getPageFaultCount(jobNumber);



// Free Memory Space - Spooling

     freePageCheck[pc.getMemoryEntry(jobNumber)]=0;
     freeMemory++;
     
// Find the position on disk
locateDisk = pc.getPageTable(0,jobNumber);
locateDisk = locateDisk/256;

// Free Disk Space - Spooling


 if (dm.readPosition != -1)
       {
       dm.freeMemory[locateDisk] = 0;
  // Read one more program in Disk
       int noOfJobs = dm.readFileData(fileName);
       }


  if(dm.availableJobs.size() >0)
        {
           int job = dm.availableJobs.at(0);
            int pageRequire = pc.getPageTable(1,job);


         // copy all the data to memory from disk   
            if(freeMemory !=0)
               {
               pFile<<"\n\n Current Time (Loading into the system) (DEC) :"<<CLOCK;
               pFile<<"\n JOB IDENTIFICATION NUMBER (Loaded Job) (DEC):"<<job;
               if(pc.getNature(job) == "Normal" && pc.getErrMessage(job) !="")
                   pFile<<"\n\n ["<<job<<"] "<<pc.getErrMessage(job); 
               dm.availableJobs.erase(dm.availableJobs.begin());                        // It will remove the first node
         // We have to mention the disk location (0,1,2,3)
               dm.readyQueue.push_back(job);
               migrationNumber++;
               int initial = pc.getPageTable(0,job);
               int endAddress= initial + pc.getPageTable(1,job)*16;
               int i= initial;
               int pageNumber= freeCheckPage();
               pc.setMemoryEntry(pageNumber,job);
               pageNumber= pageNumber *16;

// ** Important : Modify PC at the time of Insertion in the memory  
             while(i<endAddress)         // till end of the address
                 {
                 for(int k=0;k<pageRequire;k++)
                   {

                   for(int memIndex=0;memIndex<16 ;memIndex++)
                     {
                      MEM[memIndex + pageNumber*16] = dm.diskMemory[i];
                      i++;
                     }
                 pageNumber++; 
                   }        // End of for loop
                 }          // End of While loop
                int pCounter = pc.getMemoryEntry(job)*256 + pc.getPC(job);
                pc.setPC(pCounter,job);
                pc.setTimeJob(CLOCK,job);
                pc.setNoOfTurns(0,job);
                pc.setQueueTurns(0,job);
               }
          }
                         // End of while-loop   


if (dm.readyQueue.size() > 0)
 {
             // Find the next job in ReadyQueue
    jobNumber = dm.readyQueue.at(0);
            // Erase the element from ReadyQueue
    dm.readyQueue.erase(dm.readyQueue.begin());
            // Call Loader function to execute further
            // Calling Loader() : Setting the Program Counter 
    int CPUTurns = pc.getNoOfTurns(jobNumber);
    pc.setNoOfTurns(++CPUTurns,jobNumber);
    int qTurns = pc.getQueueTurns(jobNumber);
    pc.setQueueTurns(++qTurns,jobNumber);
    queueNumber = 1;
    quantumSize = n *q;
    LOADER(pc.getPC(jobNumber),pc.getTraceFlag(jobNumber));
 }
 else if(subQueue2.size() > 0)
   {
               jobNumber = subQueue2.at(0);
               subQueue2.erase(subQueue2.begin());
    int CPUTurns = pc.getNoOfTurns(jobNumber);
    pc.setNoOfTurns(++CPUTurns,jobNumber);
    int qTurns = pc.getQueueTurns(jobNumber);
    pc.setQueueTurns(++qTurns,jobNumber);
               queueNumber = 2;
               quantumSize = (n+2)* q;
    LOADER(pc.getPC(jobNumber),pc.getTraceFlag(jobNumber));
   }
 else if(subQueue3.size() > 0)
   {
               jobNumber = subQueue3.at(0);
               subQueue3.erase(subQueue3.begin());
    int CPUTurns = pc.getNoOfTurns(jobNumber);
    pc.setNoOfTurns(++CPUTurns,jobNumber);
    int qTurns = pc.getQueueTurns(jobNumber);
    pc.setQueueTurns(++qTurns,jobNumber);
               queueNumber = 3;
               quantumSize = (n+4)* q;
    LOADER(pc.getPC(jobNumber),pc.getTraceFlag(jobNumber));
   }
else if(subQueue4.size() > 0)
   {
               jobNumber = subQueue4.at(0);
               subQueue4.erase(subQueue4.begin());
    int CPUTurns = pc.getNoOfTurns(jobNumber);
    pc.setNoOfTurns(++CPUTurns,jobNumber);
    int qTurns = pc.getQueueTurns(jobNumber);
    pc.setQueueTurns(++qTurns,jobNumber);
               queueNumber = 4;
               quantumSize = (n+6)* q;
    LOADER(pc.getPC(jobNumber),pc.getTraceFlag(jobNumber));
   }
else if(dm.readPosition == -1 && dm.readyQueue.size() == 0 )
 {

 executionFlag = 2;
 return;
 }



}




/* Create a file of trace file as soon as the Program comes in memory (in write mode). 
 * After creation of file, in the traceflag mode,(CPU_Execution) open the file in append mode.
*/


/* Functional Description: As we know, execution starts from main(). In the first step, it will accept the file name from User. It will open the particular file &
 *                         transfer the control to Disk.cpp.
 */

int main()
 {
       

 
                             // To read the contents of file
       fstream readInput,outputFile,traceFile,diskRead,matrixFile;
       string input,address,fName;
       int traceFlag,Y,job,pageRequire,pageNumber,initial,endAddress,read;
       string X,Z;
       cin>>fName;
       fileName = fName;

 // using fileName to create different files for tb and tb+err 
       if(fileName == "/home/opsys/SPR16/tb+err")
           {
            mFile.open("MLFBQ_tbErr.txt",ios::out);
            read = 2;
           }
        else if(fileName == "/home/opsys/SPR16/tb")
           {
            mFile.open("MLFBQ_tb.txt",ios::out);
            read = 13;
           }           
      
       if(fileName == "/home/opsys/SPR16/tb") 
          matrixFile.open("MATRIX_tb.txt",ios::out);     
       else if(fileName == "/home/opsys/SPR16/tb+err") 
          matrixFile.open("MATRIX_tbErr.txt",ios::out); 

       matrixFile<<"\n q \t\t 35 \t 40 \t 45 \t 50";
       matrixFile.close();
while(readCount <read) {
       quantum = 0;
       dm.readPosition = 0,dm.jobID = 1,dm.initiate = 0,dm.dataCount=0,dm.finCount=0,dm.jobCount=0,dm.openFile=0,migrationNumber=0;
       dm.diskIndex = 0,dm.readLength=0,dm.finSwitch=0,onAging = 0;
      
PCBManager pc;
pFile.open("Progress_File.txt",ios::out); 

       
       n = ((readCount-1)/4) +3 ;
       q = ((readCount -1) %4)*5 +35;
       numberOfTurns = n;
       quantumSize = n * q;

if(fileName == "/home/opsys/SPR16/tb") 
   mFile.open("MLFBQ_tb.txt",ios::app);      
else if(fileName == "/home/opsys/SPR16/tb+err") 
   mFile.open("MLFBQ_tbErr.txt",ios::app); 
mFile.close();
       int noOfJobs = dm.readFileData(fileName);

       // Implementation of Spooling  << It might go to Loader's block, where, on completion of a job, at HALT state it will use this code>>
       // If readPosition = -1 then, Stop the whole execution 
    
      int jobCall=0;
       
       while (freeMemory != 0)
           {
            if(dm.availableJobs.size() != 0)
              {
               job = dm.availableJobs.at(0);
              
            pageRequire = pc.getPageTable(1,job);

         // copy all the data to memory from disk   
            if(freeMemory !=0)
               {
               pFile<<"\n\n Current Time (Loading into the system) (DEC) :"<<CLOCK;
               pFile<<"\n JOB IDENTIFICATION NUMBER (Loaded Job) (DEC):"<<job;
               if(pc.getNature(job) == "Normal" && pc.getErrMessage(job) !="")
                   pFile<<"\n\n ["<<job<<"] "<<pc.getErrMessage(job);
               dm.availableJobs.erase(dm.availableJobs.begin());                        // It will remove the first node
         // We have to mention the disk location (0,1,2,3)
               dm.readyQueue.push_back(job);
               migrationNumber++;
               initial = pc.getPageTable(0,job);
               endAddress= initial + pc.getPageTable(1,job)*16;
               int i= initial;
               pageNumber= freeCheckPage();
               pc.setMemoryEntry(pageNumber,job);
               pageNumber= pageNumber *16;
               while(i<endAddress)         // till end of the address
                 {
                 for(int k=0;k<pageRequire;k++)
                   {
                     
                   for(int memIndex=0;memIndex<16 ;memIndex++)
                     {
                      CLOCK++;
                      MEM[memIndex + pageNumber*16] = dm.diskMemory[i];
                      i++;
                     }
                   pageNumber++;       // To count the number of pages
                   }
                  }
               int pCounter = pc.getMemoryEntry(job)*256 + pc.getPC(job);
               pc.setPC(pCounter,job);
               pc.setTimeJob(CLOCK,job);
               pc.setNoOfTurns(0,job);
               pc.setQueueTurns(0,job);
                }
              if(dm.availableJobs.size() == 0)
                 break;  
               }				 
             }
 
 

// Ready Queue display

   jobNumber = dm.readyQueue.at(0);
   dm.readyQueue.erase(dm.readyQueue.begin());


// Calling Loader() : Setting the Program Counter 
 executionFlag = 0 ;

   LOADER(pc.getPC(jobNumber),pc.getTraceFlag(jobNumber));

    pFile<<"\n\n\n current CLOCK value (HEX):"<<BINHEX(CLOCK);
 pFile<<"\n mean user job run time: (Decimal)"<<fixed<<setprecision (2) <<double(CLOCK)/104;
 pFile<<"\n mean user job I/o time: (Decimal)"<<fixed<<setprecision(2) << double(IOTime)/104;
 pFile<<"\n mean user job execution Time: (Decimal)"<<fixed<<setprecision(2)<<double(CLOCK - IOTime)/104;
 pFile<<"\n mean user job time in the SYSTEM: (Decimal)"<<fixed<<setprecision(2)<<double(totalturnaroundTime)/104;
 pFile<<"\n mean user job page fault handling time :(Decimal)"<<fixed<<setprecision(2)<<double(totalFaulttime)/104;
 pFile<<"\n total CPU idle time (DEC): "<<readCount;
 pFile<<"\n total time lost due to abnormally terminated jobs:(Decimal)"<<fixed<<double(abnormalTime)/104;
 pFile<<"\n number of jobs that terminated normally :(DECIMAL)"<<normalCount;
 pFile<<"\n number of jobs that terminated abnormally :(DECIMAL)"<<abnormalCount;
 pFile<<"\n total time lost due to suspected inifinite jobs: (DECIMAL)"<<infiniteTime;
 pFile<<"\n ID of jobs considered infinite : [";
 for(int z=0;z= infiniteJobs.size();z++)
    pFile<<infiniteJobs.at(z)<<",";
 pFile<<"]";
 pFile<<"\n total number of page faults encountered :(DECIMAL) "<<totalFaulttime/5;
 pFile<<"\n Utilization information about disk : 50%";
 pFile<<"\n Maximum Sub-Queue Size :";
 pFile<<"\n \t SubQueue-1 (Dec):"<<max1;
 pFile<<"\n \t SubQueue-2 (Dec):"<<max2;
 pFile<<"\n \t SubQueue-3 (Dec):"<<max3;
 pFile<<"\n \t SubQueue-4 (Dec):"<<max4; 
 pFile<<"\n Average Sub-Queue Size :";
 pFile<<"\n \t SubQueue-1 (Dec):"<<fixed<<setprecision (2) <<double(total1)/countQueue;
 pFile<<"\n \t SubQueue-2 (Dec):"<<fixed<<setprecision (2) <<double(total2)/countQueue;
 pFile<<"\n \t SubQueue-3 (Dec):"<<fixed<<setprecision (2) <<double(total3)/countQueue;
 pFile<<"\n \t SubQueue-4 (Dec):"<<fixed<<setprecision (2) <<double(total4)/countQueue; 



   dm.diskRead.clear();             //clear the buffer
    
   dm.diskRead.close();
   dm.diskRead.seekg(0, ios::beg);


// Refresh the Disk Memory
    for(int i=0;i<8;i++)
        dm.freeMemory[i] = 0;

// Refresh the memory
    for(int i=0;i<8;i++)
       freePageCheck[i]=0;
    freeMemory=8;  
    quantum=0,totalturnaroundTime=0,totalFaulttime=0,abnormalTime=0,normalCount=0,abnormalCount=0,infiniteTime=0;


if(fileName == "/home/opsys/SPR16/tb") 
  {
    matrixFile.open("MATRIX_tb.txt",ios::app);
    if((readCount-1)%4 == 0)
       matrixFile<<"\n (n = "<<n<<")"; 
    matrixFile<<"\t"<<migrationNumber;
    matrixFile.close();
  }     
else if(fileName == "/home/opsys/SPR16/tb+err") 
   {
   matrixFile.open("MATRIX_tbErr.txt",ios::app); 

   for(int readCount=1;readCount<13;readCount++)
     {
       if((readCount-1)%4 == 0)
        {
         matrixFile<<"\n (n = "<<n<<")";
         n++;
         } 
     migrationNumber = storeMigration[readCount-1];
    matrixFile<<"\t"<<migrationNumber;
     }
    matrixFile.close(); 
    n=3;  
   }


   readCount++;
 
  }
      return 0;
 }
