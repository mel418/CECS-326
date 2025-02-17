File Copy Project

to compile: gcc -o filecopy filecopy.c

to run: ./filecopy SOURCE DESTINATION 
ex: ./filecopy input.txt copy.txt

test error for opening source file:
./filecopy non_existing_file.txt copy.txt

test error for incorrect arg:
./filecopy input.txt