/**
 * filecopy.c
 * 
 * This program copies files using a pipe.
 *
 */

 #include <complex.h>
 #include <unistd.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <fcntl.h>
 #include <sys/types.h>
//  #include <sys/wait.h>
 #include <string.h>
 
 #define READ_END 0
 #define WRITE_END 1
 
 int main(int argc, char *argv[])
 {
     /* 
     Your code is here
     */
    // check for correct amount of command-line arguments
    if(argc != 3) {
        printf("Error: wrong amount of arguments\n Example: $ ./filecopy sourceFile destinationFile\n");
        return EXIT_FAILURE;
    }

    // open the source file to read only
    int inFile = open(argv[1], O_RDONLY);
    if (inFile < 0) {
        printf("Error opening source file");
        return EXIT_FAILURE;
    }

    // open the destination file to write and to create if doesnt exist
    int outFile = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);  // O_TRUNC: Truncate the file to zero length if it already exists 
    if (outFile < 0) {
        printf("Error opening/creating destination file");
        close(inFile);
        return EXIT_FAILURE;
    }
    
    // array of two ints: pipefd[0] → Read end of the pipe | pipefd[1] → Write end of the pipe
    int pipefd[2]; 

    // create pipe: communication channel between two processes (parent and child)
    if (pipe(pipefd) < 0) {
        printf("Pipe creation failed");
        close(inFile);
        close(outFile);
        return EXIT_FAILURE;
    }

    // fork creates a new child process
    pid_t pid = fork();
    if (pid < 0) {
        printf("Fork failed");
        close(inFile);
        close(outFile);
        close(pipefd[READ_END]);
        close(pipefd[WRITE_END]);
        return EXIT_FAILURE;
    }

    // child process: only reads from the pipe
    if (pid == 0) {
        close(pipefd[WRITE_END]); // close the write end of the pipe

        char buffer[1024]; // buffer to hold data
        ssize_t bytes_read; // stores the number of bytes read

        while ((bytes_read = read(pipefd[READ_END], buffer, sizeof(buffer))) > 0) {
            if (write(outFile, buffer, bytes_read) != bytes_read) {
                printf("Error");
            }
        }
    }

 }
 