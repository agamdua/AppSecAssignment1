#include <stdlib.h>
#include <string.h>

#include "dictionary.h"


int main(int argc, char **argv) {

	char word_list[LENGTH];
	char file_to_check[LENGTH];

	char * misspelled[MAX_MISSPELLED] = { NULL };

	hashmap_t hashtable[HASH_SIZE] = { NULL };

	strcpy(word_list, "wordlist.txt");
	strcpy(file_to_check, "test1.txt");

	load_dictionary(word_list, hashtable);

	FILE *fp;
	fp = fopen(file_to_check, "r");

	int misspelled_count = check_words(fp, hashtable, misspelled);
	printf("Misspelled count is [%d]\n", misspelled_count);

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

	for (int i = 0; i < MAX_MISSPELLED; i++) {
		char* current = misspelled[i];
		if (current != NULL) {
			free(current);
		}
	}

	fclose(fp);

	return 0;
}
