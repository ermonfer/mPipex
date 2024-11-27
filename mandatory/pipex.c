#include <stdio.h> //perror
#include <stdlib.h> // exit, malloc, free, atoi
#include <unistd.h> // execve, access, pipe, fork, close, dup, dup2
#include "libft.h" // split strncmp, strcpy, strlen
#include <fcntl.h> // Distintas flags como O_RDONLY
#include <sys/types.h> // Tipos como pid_t
#include <sys/wait.h> // wait y waitpid

void error_exit(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[], char *envp[]) {
    int infile;
    int outfile;
    int pipe_fd[2];
    pid_t pid1;
    pid_t pid2;

    if (argc != 5) {
        write(2, "Error: Wrong number of arguments\n", 31);
        return (1);
    }

    // Abrir archivos
    infile = open(argv[1], O_RDONLY);
    if (infile < 0)
        error_exit("Error opening input file");
    
    outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (outfile < 0)
        error_exit("Error opening output file");

    // Crear pipe
    if (pipe(pipe_fd) < 0)
        error_exit("Pipe error");

    // Acceder a la variable de entorno PATH
    char *path = NULL;
    for (int i = 0; envp[i] != NULL; i++) {
        if (strncmp(envp[i], "PATH=", 5) == 0) {
            path = envp[i] + 5;
            break;
        }
    }

    if (path == NULL) {
        error_exit("Error getting PATH");
    }

    // Dividir PATH en directorios usando ft_split
    char **dirs = ft_split(path, ':');
    if (dirs == NULL) {
        error_exit("Error splitting PATH");
    }

    char command_path[1024];
    int command_found = 0;

    for (int i = 0; dirs[i] != NULL; i++) {
        snprintf(command_path, sizeof(command_path), "%s/%s", dirs[i], "ls");
        if (access(command_path, X_OK) == 0) {
            command_found = 1;
            break;
        }
    }

    // Liberar memoria de ft_split
    for (int i = 0; dirs[i] != NULL; i++) {
        free(dirs[i]);
    }
    free(dirs);

    if (!command_found) {
        error_exit("Command not found or not executable");
    }

    // Preparar argumentos para execve
    char *ls_args[] = {command_path, NULL};

    // Ejecutar ls usando execve
    if (execve(command_path, ls_args, envp) == -1) {
        error_exit("Error executing ls");
    }

    return 0;
}
