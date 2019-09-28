#include <stdlib.h>
#include <string.h>

#include "dictionary.h"


int main(int argc, char **argv) {

	// Declare array to store file paths.
	char word_list[LENGTH];
	char file_to_check[LENGTH];

	// Declare and initialize arrays to NULL
	char * misspelled[MAX_MISSPELLED] = { NULL };
	hashmap_t hashtable[HASH_SIZE] = { NULL };

	// Copy in the filenames to the arrays declared above.
	// These are hardcoded for now but one can and should get them
	// from argv (index 1 and 2) - this was implemented in a
	// previous iteration.
	strcpy(word_list, "wordlist.txt");
	strcpy(file_to_check, "test1.txt");

	// Call the load dictionary function to get the workds from the word
	// list into the hashtable implementation provided.
	load_dictionary(word_list, hashtable);

	// Declare a file pointer and assign the file pointer of the open file.
	// If we want to make fopen() resilient we can check for NULL input.
	FILE *fp;
	fp = fopen(file_to_check, "r");

	// We now call the check words function, which returns the count
	// of the misspelled words.
	int misspelled_count = check_words(fp, hashtable, misspelled);
	printf("Misspelled count is [%d]\n", misspelled_count);

	/*
	 * Below this point we are freeing our resources before exit.
	 */

	// This section frees memory that was allocated for the nodes in the
	// hashtable.
	for (int i = 0; i < HASH_SIZE; i++) {
		hashmap_t current = hashtable[i];

		if (current != NULL) {
			while(current->next) {
				hashmap_t next = current->next;
				free(current);
				current = next;
			}
		}
		free(current);
	}

	// Here we free the memory that has been allocated for nodes that are
	// stored in the misspelled array.
	for (int i = 0; i < MAX_MISSPELLED; i++) {
		char* current = misspelled[i];
		if (current != NULL) {
			free(current);
		}
	}

	// Flush the stream pointed to by the file pointer
	// this also closes the file descriptor (man fclose(3)).
	fclose(fp);

	// 0 return indicates a successful execution of the function
	return 0;
}
