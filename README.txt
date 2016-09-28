
                Software Engineering Issues:

1. I have used Linux machine remotely via, Putty & winscp. For coding, I used g++ compiler (c++ coding) with vim for modification. All the modules are made based on the requirement mentioned in the specification.
Disk Manager - disk.cpp & disk.h
System Module contains (Memory,CPU & Loader) - SYSTEM.cpp
Process Control Block contains the details of all the process - PCB.cpp & PCB.h
Error Handler handles all the errors which may occur in the program - error_handler.cpp & error_handler.h
Naming convention has been followed in the procedure like, Error handled by error handler.
Approach:
 a)Disk Manager: It has been implemented in disk.cpp, where, I have used MFT for disk management.
   The whole flow has been explained in the header of disk.cpp
 b) Memory Manager: I have created page frames in Memory. To implement it in my program, I have used array.
 c) Phase-3: 
     MATRIX file (Separate file) : It has been created at the run time with 3 rows and 4 columns.
        where, 3<=n<= 5 and 35<=q<=50.
     For tb file : MATRIX_tb.txt
 q 		 35 	 40 	 45 	 50
 (n = 3)	3683	3316	3115	2875
 (n = 4)	3274	2981	2733	2530
 (n = 5)	2979	2660	2463	2281

     For tb+err  : MATRIX_tbErr.txt
  q 		 35 	 40 	 45 	 50
 (n = 3)	4808	4785	4762	4739
 (n = 4)	4716	4693	4670	4647
 (n = 5)	4624	4601	4578	4555

     MLFBQ (Separate files): 
         All the multilevel feedback queue has been captured in this file.
         interval : 1200
     Aging: If the job has been kept for long time, then, it will be promoted to sub-Queue1
     For tb file : MLFBQ_tb.txt
     For tb +err : MLFBQ_tbErr.txt
 d) All the progress of the current execution has been captured in Progress_File.txt.

  
  










