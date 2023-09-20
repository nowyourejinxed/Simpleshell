//Sabrina Jackson
//Date: 9-19-2023
//Assignment to create basic OS shell in C

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>


void mainShellLoop();
char** parseInput(char *input);
void changeDirectory(char **cmds);
void executeCommand(char **cmds);


int main()
{
    mainShellLoop();
    return 0;
}

char** parseInput(char *input)
{
    int i = 0;
    int j = 64;
    const char s[6] = " \t\r\n\a";
    
    char** inputs = (char**)malloc(j * sizeof(char*));
    char* token;
    
    token = strtok(input, s);

    while(token != NULL)
    {
        inputs[i] = token;
        i++;
        if(i >= j)
        {
            j += j;
            inputs = (char**)realloc(inputs, j * sizeof(char*));
            if(!inputs)
            {
                printf("Error: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, s);
    }
    inputs[i] = NULL;
    return inputs;
}

void changeDirectory(char **cmds)
{
    if (chdir(cmds[1]) != 0)
		{
			perror("error changing directory\n");
            printf("Error: %s\n", strerror(errno));
		}
}

void executeCommand(char **cmds)
{
    int i;
    pid_t pid = fork();

    if(pid == 0)
    {
        execvp(cmds[0], cmds);
        
        perror("error in child process");
        printf("Error: %s\n", strerror(errno));
        
            exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("forking error");
        printf("Error: %s\n", strerror(errno));
    }
    else
    {
        do {
            waitpid(pid, &i, WUNTRACED);
        } while (!WIFEXITED(i) && !WIFSIGNALED(i));
        
    }
    
    
}

void mainShellLoop()
{
    char netID[17] = "sabrinajackson: ";
    char end[3] = "$ ";
    char *input = (char*)malloc(sizeof(char) * 50); 
    int buffer = 200;
    char currentDirectory[200];
    char change[3] = "cd";
    char leave[5] = "exit";
    char** inputs = (char**)malloc(sizeof(char*) * 50);
    int status = 1;
    //prompt for command - loop
    //read in command
    //separate commands
    //perform command

    do{
        //get cwd and display
        getcwd(currentDirectory, sizeof(currentDirectory));
            if (getcwd(currentDirectory, sizeof(currentDirectory)) == NULL)
            {
                perror("error in getcwd()");
                printf("Error: %s\n", strerror(errno));
            } 
            else
            {
                printf("%s %s %s", netID, currentDirectory, end);
            
            }
   
            fgets(input, buffer, stdin);

        //parse input and check cmd

        inputs = parseInput(input);
 
        if(inputs[0] == NULL)
        {
            perror("cmd is null");
            printf("Error: %s\n", strerror(errno));
        }
        else if(strcmp(inputs[0], change) == 0)
        {
            changeDirectory(inputs);
        }
        else if(strcmp(inputs[0], leave) == 0)
        {
            exit(0);
        }
        else
        {
            executeCommand(inputs);
        }
    
    
     
     //free(inputs);
     //free(input);

    }
    while(status == 1);
}

