
/* 
 * Program Description : This file implements the functionality of Error Handling "exception_handler.h". 
 *                       It has various error messages as mentioned below.
 *                       For termination of an abnormal program, I am redirecting the job to HALT state, where it will lose its control
 *                       & hand over all the quantum assigned to it.

 *     Warning : I have assumed Missing **data, Missing **Fin & Double **data as Warning, therefore, the job will not be terminated. However, it will 
                 write the missing message in the Progress_file with index as Job ID.
 */


#include "exception_handler.h"
#include <iostream>

using namespace std;

/* 
 * Function Description : This function will display/or Pop-Up with Error message
 */

void errorHandler::EHANDLER(int errorNo)
{ 
switch(errorNo){
    case 0 : descriptive = "Error [0]: Divide By Zero";
             break;
    case 1:  descriptive = "Error [1]: Invalid Opcode";
             break;
    case 2:  descriptive = "Error [2]: Missing OR Bad Trace bit";
             break;
    case 3:  descriptive = "Error [3]: Program Too long";
             break; 
    case 4:  descriptive = "Error [4]: Suspected Infinite Job";
             break;
    case 5:  descriptive = "Error [5]: Negative Value";
             break;
    case 6:  descriptive = "Warning: Missing **FIN";
             break;
    case 7:  descriptive = "Error [23]: Missing **JOB";
             break;
    case 8:  descriptive = "Warning: Missing **DATA";
             break;
    case 9:  descriptive = "Warning: Double **DATA";
             break;
    case 10: descriptive = "Error [45]: Missing Program";
             break;
    case 11: descriptive = "Error [66]: Null Job";
             break;
    case 12: descriptive = "Error [77]: Bad Character Encountered by Loader";
             break;
    default : cout<<"\n Wrong Entry";
  }
}

