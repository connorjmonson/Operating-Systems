#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <sys/resource.h> //include for getting/setting nice value 

/*
You will also have to write a benchmark to validate the lottery scheduler implementation. 
Your benchmark should show that the processes with higher nice values should be scheduled more often. 
What your benchmark should do is:
- Create multiple processes.
- Assign different nice values to the processes.
- Collect information about what processes are being scheduled 
  (using the statistics produced from your lottery scheduler)
- Write the collected information in a file that shows relevant statistics. 
  You can choose the type of statistics that will highlight how processes with higher nice values are scheduled more.
- You should include the results of the benchmark in your design document and benchmark report. 
- Also, you should provide a script that allows the graders to recreate the benchmark.
*/

int main() {

	//initialize 
	pid_t childPIDorZero;
	int status; 
	int someNum = 10;
	int which = PRIO_PROCESS;
	int ret;
	int new_set_priority = 0; //use this value to set the priority? 
	FILE * fPointer;
	fPointer = fopen("outfile.txt", "w");

	//default nice value is 0

	//create multiple processes
	//https://www.geeksforgeeks.org/create-n-child-process-parent-process-using-fork-c/
	for(int process_ID = 0; process_ID< someNum; process_ID++){
		childPIDorZero = fork();
		//A child created by fork(2) inherits its parent's nice value. 
		if (childPIDorZero == 0){
			printf("In current child process with process_ID: %d\n", process_ID);
			childPIDorZero = getpid();
			ret = getpriority(which, childPIDorZero); //current scheduling 
			printf("current scheduling for the process ID returned by the call to getpid(): %d\n", ret);
			fprintf(fPointer, "In current child process with process_ID: %d , current priority value: %d\n", process_ID, ret); // maybe put relevant statistics? 

				//assign nice value to the process? 
				//https://linux.die.net/man/3/setpriority
					/*
					childPIDorZero = getpid();
					ret = setpriority(which, childPIDorZero, new_set_priority);
					*/
		}
	}
	fclose(fPointer);


}