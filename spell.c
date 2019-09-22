#include "dictionary.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define DICTIONARY "wordlist.txt"
#define TESTDICT "test_worlist.txt"


bool check_word(const char* word, hashmap_t hashtable[])
{
	char* lower_case_word = (char *) malloc(strlen(word));

	for (int i = 0; i < strlen(word); i++) {
		lower_case_word[i] = tolower(word[i]);
	}
	int hash_value = hash_function(lower_case_word);

	// printf("Hash value: [%d]\n", hash_value);

	hashmap_t hash_value_entry = hashtable[hash_value];

	do {
		char* word_at_location = hash_value_entry->word;

		// printf("Checking: [%s] against [%s] at hash value [%d]\n",
		// 	lower_case_word,
		// 	word_at_location,
		// 	hash_value);

		
		if (strcasecmp(word, word_at_location) == 0) {
			return true;
		}

		hash_value_entry = hash_value_entry->next;

	} while (hash_value_entry->word);

	return false;
}


int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[])
{
	char word[LENGTH];

	int i = 0;
	int num_misspelled = 0;

	while(fscanf(fp, " %45s", word) == 1) {
		int word_length = strlen(word);
		char last_char = word[word_length - 1];

		if (ispunct(last_char)) {
			word[word_length - 1] = '\0';
		}
		if (ispunct(word[0]) ) {
			memmove(word, word + 1, strlen(word) - 1);
		}

		if (!check_word(word, hashtable)) {
			printf("Incorrect word: [%s]\n", word);
			num_misspelled++;
			misspelled[i] = (char * ) malloc(strlen(word));
			strcpy(misspelled[i], word);
			i++;
		}

	}

	printf("Count of misspelled: [%d]\n", num_misspelled);

	return num_misspelled;
}

static bool add_to_hashmap(int hash_value, char* word, hashmap_t hashtable[])
{
	hashmap_t new_entry = malloc(sizeof(*new_entry));
	strcpy(new_entry->word, word);

	hashmap_t hash_value_entry = hashtable[hash_value];
	char* word_at_location = hash_value_entry->word;

	if (word_at_location != NULL) {
		new_entry->next = hash_value_entry;
	} else {
		new_entry->next = NULL;
	}

	hashtable[hash_value] = new_entry;

	return true;
}


bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[])
{
	bool is_added;
	int hash_value;
	char word[LENGTH];

	FILE* fp = fopen(dictionary_file, "r");

	// ensure the sacnned length of the word is 45
	// there may be an edge case where a 45 letter word may be the first
	// part of a string which is longer than that,
	// I think it is safe to ignore that for now
	while(fscanf(fp, " %45s", word) == 1) {
		// printf("1 - Processing word [%s]\n", word);

		char* lower_case_word = (char *) malloc(strlen(word));

		for (int i = 0; i < strlen(word); i++) {
			lower_case_word[i] = tolower(word[i]);
		}
		// printf("2 - Processing word [%s]\n", lower_case_word);

		hash_value = hash_function(lower_case_word);

		// printf("Adding word [%s] to location [%d]\n", lower_case_word, hash_value);

		is_added = add_to_hashmap(hash_value, lower_case_word, hashtable);

		if (!is_added) {
			return false;
		}
	}

	fclose(fp);

	return true;
}

// int main(int argc, char **argv) {
// 	hashmap_t hashtable[HASH_SIZE];
// 	int is_loaded = load_dictionary(TESTDICT, hashtable);
// 	return is_loaded ? 0 : 1;
// }

