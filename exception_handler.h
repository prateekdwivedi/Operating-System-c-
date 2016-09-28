
          /* To declare all the functions and class related to error_handler */

#include<string>
#ifndef ERROR_HANDLER
#define ERROR_HANDLER
using namespace std;
/* 
Comment: This class will perform the property of memory 
*/
class errorHandler
{ 
public:
    string descriptive;
    void EHANDLER(int err);
};
#endif