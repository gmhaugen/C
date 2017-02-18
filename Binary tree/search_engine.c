/*
	Author: Geir Haugen

	NOTES:
		If the user wants to use files other than the test file,
		the files has to be in the same folder as the program.

		If user wants to switch between case sensitive and case insensitive
		search, uncomment/uncomment in method inputHandler.
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

// The file to read from.
FILE *inputFile;

// Number of files to use.
int numOfFiles = 0;

// The symbols/characters to exclude when reading from file.
// Add other characters to this array to have them excluded when reading words.
char exclude[] = {'.', ',', ' ', '-', '(', ')', ';', ':', '!', '?'};

// Node structure
struct node {
	char *word;
	struct node *left;
	struct node *right;

	char *inFile;
};

// This is the start of the tree. The root node.
struct node *tree;

// Switch for wether to clear the display in the inputHandler method.
bool clear_display = true;

// Method declarations.
void insert();
void initialize();
void buildTree();
struct node *search_tree_case_sensitive();
struct node *search_tree_case_insensitive();
void inputHandler();
void enterFilename();
void useTestFile();
void getOther();

int main() {

	initialize();

	return 0;
}

// Initialize method for the program.
void initialize() {

	char *inputBuf = malloc(sizeof(char));	// Buffer for the user input.

	printf("Would you like to use the test-book or to use another book? (t/a).\n>");

	scanf("\n%c", inputBuf);

	if (*inputBuf == 't') {
		free(inputBuf);
		useTestFile();

	} else if (*inputBuf == 'a') {
		free(inputBuf);
		getOther();
	} else {
		free(inputBuf);
		printf("Invalid input.\n");
		initialize();
	}
}

// Using the testfile (pg4300.txt)
void useTestFile() {

	bool filePresent = false;
	char *downloadAnswer = malloc(sizeof(char));	// Buffer for answer on wheter user wats to download "test file" or not.

	inputFile = fopen("pg4300.txt", "r");

	if (inputFile) {
		filePresent = true;
	} else {
		filePresent = false;
	}

	if (filePresent) {	// If the file is present, the file is loaded to the tree.
		buildTree(inputFile, "pg4300.txt");
		//free(inputFile);
   		inputHandler();

	} else {	// Ask the user to download the "test file".
		system("clear");
		printf("Book is not present.\n");
		printf(" \n");

		printf("Would you like to download the book? (y/n)\n>");

		scanf(" %c", downloadAnswer);

		if (*downloadAnswer == 'y') {
			free(downloadAnswer);
			system("sudo wget 'http://home.nith.no/~esphav/pg4300.txt'");
			
			inputFile = fopen("pg4300.txt", "r");
			printf("File downloaded!\nPress [enter] to proceed.");
			
			// Need two getchar(); because scanf left "enter" in the buffer.
			getchar();
			getchar();

			useTestFile();

		} else {
			fputs("ERROR: Text file not present!\nAborting program...\n", stderr);
			exit(1);
		}
	}
	free(downloadAnswer);
}

// Starting point for adding custom files to the tree.
void getOther() {
	printf("Files in current folder:\n");
	system("ls");
	printf("\nHow many files would you like to read from?\n>");

	scanf(" %d", &numOfFiles);

	enterFilename();
}

// Method for entering filenames and adding the content to the tree.
void enterFilename() {

	char *filenameBuf = malloc(sizeof(char) * 50);	// Buffer for the filename entered.
	bool filePresent = false;
	
	for (int i = 0; i < numOfFiles; ++i) {
		printf("Enter filename.(file %d of %d)\n", i+1, numOfFiles);
		printf(">");
		scanf("%s", filenameBuf);

		inputFile = fopen(filenameBuf, "r");


		if (inputFile) {
			filePresent = true;
		} else {
			filePresent = false;
		}

		if (filePresent) {
			printf("\nFile found!\n");
			buildTree(inputFile, filenameBuf);

		} else {
			printf("No file found with filename '%s'", filenameBuf);
			enterFilename();	// Redirects recursively if file not found.
		}
	}
	free(filenameBuf);

	inputHandler();
}

// Method for adding words to the tree.
void buildTree(FILE *file, char *filename) {

	double diff = 0.0;
    time_t start;
    time_t stop;

	char wordFromFile[100];
	char c = 0;

	printf("Adding to tree..");

    time(&start);

    while (c != EOF) {

		c = fscanf(file,"%s",wordFromFile);

   		for (int i = 0; i < strlen(wordFromFile); ++i) {
   			for (int j = 0; j < sizeof(exclude); ++j) {
   				if (wordFromFile[i] == exclude[j]) {	// Ends the string if any of the excluding characters are found.
   					wordFromFile[i] = '\0';
   				}
   			}
   		}
    	insert(&tree, &wordFromFile, filename);
    }

    time(&stop);

    diff = difftime(stop, start);	// Calculate the time used.

    printf("This took %g seconds.\n\n", diff);

	fclose(file);
}

// Method for inserting new nodes/words into the tree.
void insert(struct node **tree, char *word, char *filename) {

	if (!(*tree)) {
		*tree = (struct node*)malloc(sizeof(struct node));
		(*tree)->word = malloc(strlen(word) + 1);
		(*tree)->inFile = malloc(sizeof(char)*30);

		strcpy((*tree)->inFile, filename);

		strcpy((*tree)->word, word);

		(*tree)->left = NULL;
		(*tree)->right = NULL;

	} else if (strcmp(word, (*tree)->word) < 0) {
		insert(&(*tree)->left, word, filename);

	} else if (strcmp(word, (*tree)->word) > 0) {
		insert(&(*tree)->right, word, filename);

	}
}

// Method for searching the tree case sensitive.
// Search recursively until end of tree.
struct node *search_tree_case_sensitive(struct node **NODE, char *word) {

	printf(".");	// Showing how many levels the search goes through.

	if (*NODE != NULL) {
		if (strcmp(word, (*NODE)->word) == 0) {
			printf("'%s' was found!", word);
			printf(" - in file '%s'\n", (*NODE)->inFile);
			return *NODE;

		} else if (strcmp(word, (*NODE)->word) < 0) {
			return search_tree_case_sensitive(&(*NODE)->left, word);
		
		} else if (strcmp(word, (*NODE)->word) > 0) {
			return search_tree_case_sensitive(&(*NODE)->right, word);
		
		}
	} else {
		printf("'%s' was not found!\n", word);
		return 0;
	}
	return 0;
}

//Method for searching the tree case insensitive.
	// This method converts all words (input and from the file) to lowercase.

struct node *search_tree_case_insensitive(struct node **NODE, char *word) {
	printf(".");	// Showing how many levels the search goes through.

	if ((*NODE) != NULL) {

		for (int i = 0; i < strlen((*NODE)->word); ++i) {
			(*NODE)->word[i] = tolower((*NODE)->word[i]);
		}


		for (int i = 0; i < strlen(word); ++i) {
			word[i] = tolower(word[i]);
		}

		if (strcmp(word, (*NODE)->word) == 0) {
			printf("Word found!");
			printf(" - in file '%s'\n", (*NODE)->inFile);	// Prints the file where the word is from.
			return (*NODE);

		} else if (strcmp(word, (*NODE)->word) < 0) {
			return search_tree_case_insensitive(&(*NODE)->left, word);
		} else if (strcmp(word, (*NODE)->word) > 0) {
			return search_tree_case_insensitive(&(*NODE)->right, word);
		}
	} else {
		printf("'%s' was not found!\n", word);
		return 0;
	}
	return 0;
}

// Method for handling the input from the user.
void inputHandler() {

	char inputWordBuffer[20];

	if (clear_display) {
		system("clear");
		printf("\nEnter the word to search for.\n");
	}

	clear_display = false;

	printf("\nsearch-file>");

	scanf("%s", inputWordBuffer);

    search_tree_case_insensitive(&tree, inputWordBuffer);
    //search_tree_case_insensitive(&tree, inputWordBuffer);
		
    inputHandler();		// Calls the method recursively so the user can query several times.
}