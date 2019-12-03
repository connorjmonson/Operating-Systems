#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <limits.h>


extern char ** get_args();	// get argument from console (as a pointer)
pid_t childPIDorZero;
pid_t childPIDorZero_2;
int in_file, out_file;
int status; 





void pipe_it_up (char ** args, int i){
/*	for (int i = 0; args[i] != NULL; i++) { //loop through args until null
	    printf ("Argument %d: %s\n", i, args[i]); //print
	}
*/

// cited from https://www.cs.rutgers.edu/~pxk/416/notes/c-tutorials/pipe.html
/*
int pid;

	switch (pid = fork()) {

	case 0: // child 
		dup2(pfd[0], 0);
		close(pfd[1]);	//the child does not need this end of the pipe 
		execvp(cmd2[0], cmd2);
		perror(cmd2[0]);

	default: // parent 
		dup2(pfd[1], 1);
		close(pfd[0]);	// the parent does not need this end of the pipe 
		execvp(cmd1[0], cmd1);
		perror(cmd1[0]);

	case -1:
		perror("fork");
		exit(1);
	}
*/

// used references from https://www.cs.rutgers.edu/~pxk/416/notes/c-tutorials/
	// specific file cited: http://www.cs.loyola.edu/~jglenn/702/S2005/Examples/dup2.html
	int pipefd[2];
	char ** in = &args[i]; 
	char ** out = &args[i+1];

	pipe(pipefd);
	childPIDorZero = fork();
    if(childPIDorZero < 0) {
        perror("fork() error");
		exit(-1);
    }
    else if(childPIDorZero != 0) {
    	close(pipefd[1]); //close this end
	    waitpid(0,&status,0); //wait
    } 
    else {
    	dup2(pipefd[1], STDOUT_FILENO) // this outputs pipped to input of another command 
    	close(pipefd[0]); //close both ends
    	close(pipefd[1]);
    	//execvp(args[0], args);
    	execvp(args[0], in); //another command input
    	exit(0);
    }

    //same thing but close other end of the pipe
    childPIDorZero_2 = fork();
    if(childPIDorZero_2 < 0) {
        perror("fork() error");
		exit(-1);
    }
    else if(childPIDorZero_2 != 0) {
    	close(pipefd[0]); //close this end
	    waitpid(0,&status,0); //wait
    } 
    else { // child == 0
    	dup2(pipefd[0], STDIN_FILENO) //changed to standard input
    	close(pipefd[1]); //close both ends
    	close(pipefd[0]);
    	//execvp(args[0], args);
    	execvp(out[0], out); 
    	exit(0);
    }


}

void semicolon_function(char **args, int i){
	//do the fork() twice! I think this will only make the semicolon work for 2 
	
	// *************** README *****************
	//currently not working but below is the code I came up with! 
	// ****************************************


	/*
	char **second_argument = args[i+1]; 

    childPIDorZero = fork();
    if(childPIDorZero < 0) {
        perror("fork() error");
		exit(-1);
    }
    else if(childPIDorZero != 0) {
	    wait(NULL);
    } 
    else { //childPIDorZero == 0 ; do stuff 
    	args[i] = NULL; //The array of pointers must be terminated by a NULL pointer?
    	execvp(args[0], args); // run command cited from https://linux.die.net/man/3/execvp
    	wait(NULL);
    }

    pid_t childPIDorZero_number2 = fork();
    if(childPIDorZero < 0) {
        perror("fork() error");
		exit(-1);
    }
    else if(childPIDorZero != 0) {
	    wait(NULL);
    } 
    else { //childPIDorZero == 0 ; do stuff 
    	args[i] = NULL; //The array of pointers must be terminated by a NULL pointer?
    	execvp(second_argument[0], second_argument); // run command for second argument; cited from https://linux.die.net/man/3/execvp
    	wait(NULL);
    }
    */
}

void error_and_pipe(char **args, int i){ //same thing as error_Redirect
	//not sure if this works for the piping but it redirects standard error
	childPIDorZero = fork();
    if(childPIDorZero < 0) {
        perror("fork() error");
		exit(-1);
    }
    else if(childPIDorZero != 0) {
	    waitpid(0,&status,0); //wait
    } 
    else { //childPIDorZero == 0 ; do stuff 
        args[i] = NULL; //The array of pointers must be terminated by a NULL pointer?
	    out_file = open(args[i+1], O_WRONLY); //open for write only
	    if(out_file < 0) {
	        perror("file NOT valid");
			exit(-1);
	    }
	    dup2(out_file, STDERR_FILENO); //standard error
	    close(out_file);
	    execvp(args[0], args); // run command cited from https://linux.die.net/man/3/execvp
    }
}

//for "&<" and "&<<"
void error_and_input_redirect(char **args, int i){
	childPIDorZero = fork();
    if(childPIDorZero < 0) {
        perror("fork() error");
		exit(-1);
    }
    else if(childPIDorZero != 0) {
	    waitpid(0,&status,0); //wait
    } 
    else { //childPIDorZero == 0 ; do stuff 
        args[i] = NULL; //The array of pointers must be terminated by a NULL pointer?
	    in_file = open(args[i+1], O_RDONLY); //open for read only!!!
	    if(out_file < 0) {
	        perror("file NOT valid");
			exit(-1);
	    }
	    //both standard in and standard error
	    dup2(in_file, STDIN_FILENO); 
	    dup2(in_file, STDERR_FILENO); //standard error
	    close(in_file);
	    execvp(args[0], args); // run command cited from https://linux.die.net/man/3/execvp
    }
}

//for ">>&"
void modified_error_and_output_Redirect(char **args, int i){
	childPIDorZero = fork();
    if(childPIDorZero < 0) {
        perror("fork() error");
		exit(-1);
    }
    else if(childPIDorZero != 0) {
	    waitpid(0,&status,0); //wait
    } 
    else { //childPIDorZero == 0 ; do stuff 
        args[i] = NULL; //The array of pointers must be terminated by a NULL pointer?
	    in_file = open(args[i+1], O_WRONLY| O_APPEND); //open for write and append
	    if(out_file < 0) {
	        perror("file NOT valid");
			exit(-1);
	    }
	    //both standard out and standard error
	    dup2(in_file, STDOUT_FILENO); 
	    dup2(in_file, STDERR_FILENO); //standard error
	    close(in_file);
	    execvp(args[0], args); // run command cited from https://linux.die.net/man/3/execvp
    }
}

//for ">&"
void error_and_output_Redirect(char **args, int i){
	childPIDorZero = fork();
    if(childPIDorZero < 0) {
        perror("fork() error");
		exit(-1);
    }
    else if(childPIDorZero != 0) {
	    waitpid(0,&status,0); //wait
    } 
    else { //childPIDorZero == 0 ; do stuff 
        args[i] = NULL; //The array of pointers must be terminated by a NULL pointer?
	    out_file = open(args[i+1], O_WRONLY| O_APPEND); //open for write and append
	    if(out_file < 0) {
	        perror("file NOT valid");
			exit(-1);
	    }
	    dup2(out_file, STDERR_FILENO); //standard error
	    close(out_file);
	    execvp(args[0], args); // run command cited from https://linux.die.net/man/3/execvp
    }
}

//for "&" 
void error_Redirect(char **args, int i){ // cited from https://askubuntu.com/questions/625224/how-to-redirect-stderr-to-a-file
	childPIDorZero = fork();
    if(childPIDorZero < 0) {
        perror("fork() error");
		exit(-1);
    }
    else if(childPIDorZero != 0) {
	    waitpid(0,&status,0); //wait
    } 
    else { //childPIDorZero == 0 ; do stuff 
        args[i] = NULL; //The array of pointers must be terminated by a NULL pointer?
	    out_file = open(args[i+1], O_WRONLY); //open for write only
	    if(out_file < 0) {
	        perror("file NOT valid");
			exit(-1);
	    }
	    dup2(out_file, STDERR_FILENO); //standard error
	    close(out_file);
	    execvp(args[0], args); // run command cited from https://linux.die.net/man/3/execvp
    }
}

// for ">>"
void modifiedOutputRedirectToFile(char **args, int i){ // cited from https://www.youtube.com/watch?v=EL4hCQc7KXY
	//https://unix.stackexchange.com/questions/89386/what-is-symbol-and-in-unix-linux
	childPIDorZero = fork();
    if(childPIDorZero < 0) {
        perror("fork() error");
		exit(-1);
    }
    else if(childPIDorZero != 0) {
	    waitpid(0,&status,0); //wait
    } 
    else { //childPIDorZero == 0 ; do stuff 
        args[i] = NULL; //The array of pointers must be terminated by a NULL pointer.
	    out_file = open(args[i+1], O_WRONLY| O_APPEND| O_CREAT); //open for write only, append to end of file, create file if file doesnt already exist 
	    if(out_file < 0) {
	        perror("file NOT valid");
			exit(-1);
	    }
	    dup2(out_file, STDOUT_FILENO); //standard output
	    close(out_file);
	     
	    execvp(args[0], args); // run command cited from https://linux.die.net/man/3/execvp
    }
}

void outputRedirectToFile(char **args, int i){ // cited from https://www.youtube.com/watch?v=EL4hCQc7KXY
    childPIDorZero = fork();
    if(childPIDorZero < 0) {
        perror("fork() error");
		exit(-1);
    }
    else if(childPIDorZero != 0) {
	    waitpid(0,&status,0); //wait
    } 
    else { //childPIDorZero == 0 ; do stuff 
        args[i] = NULL; //The array of pointers must be terminated by a NULL pointer.
	    out_file = open(args[i+1], O_WRONLY| O_CREAT); //open for write only, create file if file doesnt already exist 
	    if(out_file < 0) {
	        perror("file NOT valid");
			exit(-1);
	    }
	    dup2(out_file, STDOUT_FILENO); // standard output
	    close(out_file);
	     
	    execvp(args[0], args); // run command cited from https://linux.die.net/man/3/execvp
    }
}

void inputRedirectFromFile(char **args, int i){ // cited from https://www.youtube.com/watch?v=EL4hCQc7KXY
	childPIDorZero = fork();
    if(childPIDorZero <  0){
        perror("fork() error");
		exit(-1);
    }
    else if(childPIDorZero != 0){
		waitpid(0, &status, 0); //wait
	}
	else{ // childPIDorZero == 0 
        args[i] = NULL; //The array of pointers must be terminated by a NULL pointer.
        in_file = open(args[i+1], O_RDONLY); //open for read only
        if(in_file < 0) {
    	    perror("file NOT valid");
	    	exit(-1);
        }
        dup2(in_file, STDIN_FILENO); //standard input
        close(in_file);
        execvp(args[0], args);	// run command cited from https://linux.die.net/man/3/execvp
   	}
}

void doshit(char **args, int i){
	childPIDorZero = fork();
	if(childPIDorZero <  0){
        perror("fork() error");
		exit(-1);
    }
    else if(childPIDorZero != 0){
		waitpid(0, &status, 0); //wait
	}
	else{
		execvp(args[i], args); //cited from https://linux.die.net/man/3/execvp
		args[i] = NULL; //The array of pointers must be terminated by a NULL pointer.
		if(!execvp(args[i], args)) { 
    	    perror("what we have here is a failure to communicate");
	    	exit(-1);
        }
	}
}

int
main()
{
	//initialize
    //int         i; 
    char **     args; //2D chars array args (as a pointer)

    while (1) { //do until user enters "exit"
		printf ("Command ('exit' to quit): ");
		args = get_args(); //get arguments from terminals put into args

		for (int i = 0; args[i] != NULL; i++) {
			printf ("Argument %d: %s\n", i, args[i]);
			if (args[0] == NULL) { //if nothing in args
			    printf ("No arguments on line!\n");
			} 
				else if (args[0] != NULL){
					if ( !strcmp (args[0], "exit")) { //1. if it was exit
			    		printf ("Exiting...\n"); //print
			    		//break; // exit
			    		exit(0);
			    	}
			    	else if (!strcmp (args[i], "<")) {
			    		inputRedirectFromFile(args, i); //4. A command, with or without arguments whose input is redirected from  file. 
			    	}
			    	else if (!strcmp (args[i], "<<")) {
			    		// https://en.wikipedia.org/wiki/Here_document
			    		inputRedirectFromFile(args, i); //4. A command, with or without arguments whose input is redirected from  file. 
			    	}
			    	else if (!strcmp (args[i], ">")) {
			    		outputRedirectToFile(args, i); //5. A command, with or without arguments, whose output is redirected to a file.
			    	}
			    	else if (!strcmp (args[i], ">>")) { // ">>" redirects output to a file appending the redirected output at the end.
			    		modifiedOutputRedirectToFile(args, i); //5. A command, with or without arguments, whose output is redirected to a file.
			    	}
			    	else if (!strcmp (args[i], "&")) { //7. redirection of standard error
			    		error_Redirect(args, i); //put program running in the background
			    	}
			    	//7. Redirection of standard error
			    	else if (!strcmp (args[i], ">&")) {
			    		//something
			    		error_and_output_Redirect(args, i); //self explainatory
			    	}
			    	else if (!strcmp (args[i], ">>&")) {
			    		//something
			    		modified_error_and_output_Redirect(args, i); //self explainatory
			    	}
			    	else if (!strcmp (args[i], "&<")) {
			    		//something
			    		error_and_input_redirect(args, i); //self explainatory
			    	}
			    	else if (!strcmp (args[i], "&<<")) {
			    		//something
			    		error_and_input_redirect(args, i); //self explainatory
			    	}
			    	else if (!strcmp (args[i], "|&")) {
			    		//something
			    		//not sure what to do
			    		error_and_pipe(args, i); //not done  
			    	}
			    	//8. Two or more command sequences, possibly involving I/O redirection, separated by a semicolon.
			    	else if (!strcmp (args[i], ";")) {
			    		//something
			    		semicolon_function(args, i); //not done
			    	}
			    	else if ( !strcmp (args[0], "cd")) { //9. if first arg is cd
			    		if (args[1] != NULL){ //and second arg is something
			    			chdir(args[1]); 
			    		} else if (args[1] == NULL){ //and second arg is nothing
			    			//put something with getcwd();
			    			//getcwd();
			    		}
			    	}
			    	// 6. and 10. A command, with or without arguments, whose output is piped to the input of another command.
			    	else if (!strcmp (args[i], "|")) {
			    		//something 
			    		pipe_it_up(args, i);
			    	}
			    	// for regular commands 
			    	else if (args[1] == NULL){ //only 1 arguments
			    		doshit(args, i);
			    	}
			    	else if (arg[2] == NULL){ // 2 arguments
			    		doshit(args, i);
			    	}
			    	else if (arg[3] == NULL){ // 3 arguments 
			    		doshit(args, i);
			    	}
			    	else if (args[4] == NULL){ // 4 arguments
			    		doshit(args, i);
			    	}
				}
		}	
	}
}