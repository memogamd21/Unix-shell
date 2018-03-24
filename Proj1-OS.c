#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LINE 80 // maximum length of command

char command[MAX_LINE];
char history[10][MAX_LINE];
int count = 0; //an integer to store the number of commands entered by the user

int containsampersand(char x[]){// we pass to this function the command to check for the existence of &
    int length = sizeof(x);
    for(int i = 0; i<length; i++){
        if(x[i] == '&'){
            return 1;
        }
    }
    return 0;
}

void printhistory(){
    printf("Command History is : \n");
    int historyCounter = count;
    int y,x = 0;
    for (y = 0; y<10;y++)
    {
        //command index
        printf("%d.\t", historyCounter);
        while (history[y][x] != '\n' && history[y][x] != '\0')
        {
		//printing command character by charater
            printf("%c", history[y][x]);
            x++;
        }
        printf("\n");//here we finished printing the command character by character
        x = 0;
        historyCounter--;
        if (historyCounter ==  0)// if no commands are in history
            break;
    }
}
int Splitcommands(char input[], char* args) {
	int length; // command line characters
	int indx = 0; // index of where to place the next parameter into args[]
	//read user input on command line and checking whether the command is !! or !n
	length = read(STDIN_FILENO, input, MAX_LINE);
	if (length == 0)
		exit(0);   //end of command
	if (length < 0)
	{
		printf("Command not read, please enter another one \n");
		exit(-1);  //terminate
    }
	// now we use string tokenizer to split the command into parts
	int count = 0;
	args[0] = strtok(input, ' ');		// This splits the first partition of the command only
	int i = sizeof(args[0])+1;
	while (i != length) {                  // we start from the index right after the command to find its parameters
		if (input[i] != ' ' || input[i] != '\t') { // THIS is to make the parameters of the command into segments
			args[count+1] = strtok(NULL, ' ');		// here we used null because in string tokenizer we split the next parts by passing Null
			count++;
		}
		i++;
	}
}


int main(void)
{
	pid_t pid, tpid;
	char *args[MAX_LINE / 2 + 1];
	char history[10][MAX_LINE];
	int should_run = 1;		//Flag to determine when to exit program
	while(should_run){
	printf("osh> ");
	fflush(stdout);
	char commandd[MAX_LINE];
	scanf("%d", commandd);
	strcpy(history[count],commandd);
	count++;
	Splitcommands(commandd, args);
    if(strcmp(args[0],"history")==0){
        printhistory();
    }
    int flag = containsampersand(commandd);
	if (Splitcommands(commandd, args) != -1){// COMMAND WAS SPLIT CORRECTLY
    pid = fork(); // CASE OF FORK BEING FAILED
	if (pid < 0) {//
		fprintf(stderr, "Fork failed");
		return 1;
	}
	else if (pid == 0) {//child process
        execvp(args[0], args);
        if (execvp(args[0], args) == -1){
            printf("Error Executing the command");
        }
    }else{// WE'RE IN THE PARENT PROCESS
        if (flag == 0){
            wait(NULL);
        }
    }
}
	}
	return 0;
}










