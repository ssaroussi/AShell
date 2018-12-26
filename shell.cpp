#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_LEN (1024)

char **recive_cmd(char **cmd, char ***args, size_t *noft);
char **strsplit(const char* str, const char* delim, size_t *noft);

int main(int argc, char *argv[])
{
	size_t noft = 0;
    bool is_exit = false;
    pid_t pid = 0;
    int status = 0,
        c_ret = 0;
    char *cmd = NULL,
         **args = NULL,
         **words = NULL;

    while (!is_exit)
    {
    	words = recive_cmd(&cmd, &args, &noft);
	    if (strcmp(cmd, "exit"))
	    {
	        /* The New Process Creation */
	        pid = fork();

	        if (pid == 0)
            {
                /* Creates a new Adress-Space and loads the image */  
	            execvp(cmd, args); 
	        }

	        if ((c_ret = waitpid(pid, &status, 0)) == -1)
	            fprintf(stderr, "Can not run the command '%s' with the given arguments.\n", cmd);
	        
	        if (c_ret == pid)
	            printf("'%s' returned with value: %d (%s)\n", cmd, status, !status ? "succeed" : "failed");
	    }
	    else
	        is_exit = true;

	    /* Free the dynamic allocations */
	    for (size_t i = 0; i <= noft; i++)
        {
	    	free(words[i]);
            words[i] = NULL;
        }
	    free(words);
        words = NULL;
	}

   return (0);
}

/**
 * recive_cmd() - Recives a command (and arguments) from the user
 *
 * @cmd: A pointer to the command varible (the command will be stored there)
 * @args A pointer to the args variable (the arguments will be stored there)
 * @nofa: A pointer to the number of arguments (the number will be stored there)
 *
 * Return: A list of arguments, represented as an array of literals
 */
char **recive_cmd(char **cmd, char ***args, size_t *nofa)
{
    char input[MAX_INPUT_LEN] = { 0 };
    size_t num_of_args = 0;

    printf("ashell:>> ");
    fgets(input, MAX_INPUT_LEN, stdin);
    input[strlen(input) - 1] = '\0';
    char **words = strsplit(input, " ", &num_of_args);

    num_of_args -= 1;
    *cmd = words[0];
    *args = words;
    
    *nofa = num_of_args;
    return words;
}

/**
 * strsplit() - Creates an array of substrings by splitting the input string based on a delimiter
 *
 * @str: The string which should be splitted
 * @delim: The delimiter
 * @nofa: A pointer to the number of arguments (the number will be stored there)
 *
 * Return: An array of substrings (based on @delim)
 */
char **strsplit(const char* str, const char* delim, size_t *noft) 
{
    char *s = strdup(str);
    size_t tokens_alloc = 1;
    size_t tokens_used = 0;
    char **tokens = (char **) calloc(tokens_alloc, sizeof(char*));
    char *token, *rest = s;

    while ((token = strsep(&rest, delim)) != NULL)
    {
        if (tokens_used == tokens_alloc) {
            tokens_alloc *= 2;
            tokens = (char **) realloc(tokens, tokens_alloc * sizeof(char*));
        }
        tokens[tokens_used++] = strdup(token);
    }

    if (tokens_used == 0) 
    {
        free(tokens);
        tokens = NULL;
    } 
    else 
        tokens = (char **) realloc(tokens, tokens_used * sizeof(char*));

    *noft = tokens_used;
    free(s);

    return tokens;
}
