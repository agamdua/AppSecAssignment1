#include <string.h>

#include "dictionary.h"


int main(int argc, char **argv) {

	char word_list[LENGTH];
	char file_to_check[LENGTH];
	char * misspelled[MAX_MISSPELLED];

	hashmap_t hashtable[HASH_SIZE];

	strcpy(word_list, argv[2]);
	strcpy(file_to_check, argv[1]);

	load_dictionary(word_list, hashtable);

	FILE *fp;
	fp = fopen(file_to_check, "r");

	int misspelled_count = check_words(fp, hashtable, misspelled);
	printf("Misspelled count is [%d]\n", misspelled_count);

	fclose(fp);

	return 0;
}
