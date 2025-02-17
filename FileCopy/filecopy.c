#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[])
{
    // Check for correct number of arguments
    if (argc != 3) {
        printf("Error: wrong number of arguments\nExample: $ ./filecopy sourceFile destinationFile\n");
        return EXIT_FAILURE;
    }

    // Open the source file for reading
    int inFile = open(argv[1], O_RDONLY);
    if (inFile < 0) {
        perror("Error opening source file");
        return EXIT_FAILURE;
    }

    // Open the destination file for writing
    int outFile = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (outFile < 0) {
        perror("Error opening/creating destination file");
        close(inFile);
        return EXIT_FAILURE;
    }

    // Create a pipe for inter-process communication
    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("Pipe creation failed");
        close(inFile);
        close(outFile);
        return EXIT_FAILURE;
    }

    // Fork to create a child process
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        close(inFile);
        close(outFile);
        close(pipefd[READ_END]);
        close(pipefd[WRITE_END]);
        return EXIT_FAILURE;
    }

    // Parent process: write from the source file to the pipe
    if (pid > 0) {
        close(pipefd[READ_END]); // Close the read end of the pipe
        char buffer[1024];
        ssize_t bytesRead;

        // Read from the source file and write to the pipe
        while ((bytesRead = read(inFile, buffer, sizeof(buffer))) > 0) {
            if (write(pipefd[WRITE_END], buffer, bytesRead) != bytesRead) {
                perror("Write to pipe failed");
                close(inFile);
                close(outFile);
                close(pipefd[WRITE_END]);
                return EXIT_FAILURE;
            }
        }

        // Close the write end of the pipe and source file
        close(pipefd[WRITE_END]);
        close(inFile);

        // Wait for the child process to finish
        waitpid(pid, NULL, 0);

        // Print success message only in the parent process after child finishes
        printf("File successfully copied from %s to %s.\n", argv[1], argv[2]);
    }
    // Child process: read from the pipe and write to the destination file
    else {
        close(pipefd[WRITE_END]); // Close the write end of the pipe
        char buffer[1024];
        ssize_t bytesRead;

        // Read from the pipe and write to the destination file
        while ((bytesRead = read(pipefd[READ_END], buffer, sizeof(buffer))) > 0) {
            if (write(outFile, buffer, bytesRead) != bytesRead) {
                perror("Write to destination file failed");
                close(outFile);
                close(pipefd[READ_END]);
                return EXIT_FAILURE;
            }
        }

        // Close the read end of the pipe and destination file
        close(pipefd[READ_END]);
        close(outFile);
    }

    return EXIT_SUCCESS;
}
