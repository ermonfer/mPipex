#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

void	error_exit(char *msg)
{
    perror(msg);
    exit(1);
}

int	main(int argc, char *argv[])
{
    int		pipe_fd[2];
    int		infile;
    int		outfile;
    pid_t	pid1;
    pid_t	pid2;

    if (argc != 5)
    {
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

    // Primer proceso hijo
    pid1 = fork();
    if (pid1 < 0)
        error_exit("Fork error");
    
    if (pid1 == 0)
    {
        // Proceso hijo 1
        close(pipe_fd[0]);
        dup2(infile, STDIN_FILENO);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
        close(infile);
        
        if (access(argv[2], X_OK) != 0)
            error_exit("Command 1 not found or not executable");
        
        execve(argv[2], &argv[2], environ);
        error_exit("Execve error");
    }

    // Segundo proceso hijo
    pid2 = fork();
    if (pid2 < 0)
        error_exit("Fork error");
    
    if (pid2 == 0)
    {
        // Proceso hijo 2
        close(pipe_fd[1]);
        dup2(pipe_fd[0], STDIN_FILENO);
        dup2(outfile, STDOUT_FILENO);
        close(pipe_fd[0]);
        close(outfile);
        
        if (access(argv[3], X_OK) != 0)
            error_exit("Command 2 not found or not executable");
        
        execve(argv[3], &argv[3], environ);
        error_exit("Execve error");
    }

    // Proceso padre
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    close(infile);
    close(outfile);
    
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return (0);
}
