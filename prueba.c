#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    int pipefd[2]; //Crea el array de dos int
    pid_t cpid; // Tipo con el número de proceso
    char buf;

// Hace control de errores
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

// Hace un fork, no tiene sentido usar pipe de otra manera.
    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {    /* Child reads from pipe */
        close(pipefd[1]);          /* Close unused write end */

        while (read(pipefd[0], &buf, 1) > 0) // TODO Va leyendo la info según vaya estando disponible. Aquí hay mucho margen para preguntar
            write(STDOUT_FILENO, &buf, 1);

        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]);
        _exit(EXIT_SUCCESS); // TODO Porque _exit va con una barrabaja delante

    } else {
        close(pipefd[0]);
        write(pipefd[1], argv[1], strlen(argv[1])); // En este caso si lo lee todo de forma eficiente.
        close(pipefd[1]);          /* Reader will see EOF */ //TODO
        wait(NULL);                /* Wait for child */
        exit(EXIT_SUCCESS);
    }
}