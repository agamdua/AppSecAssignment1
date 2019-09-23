#include "dictionary.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define DICTIONARY "wordlist.txt"
#define TESTDICT "test_worlist.txt"


bool check_word(const char* word, hashmap_t hashtable[])
{
	char* lower_case_word = (char *)malloc((strlen(word) + 1) * sizeof(char));

	for (int i = 0; i < strlen(word) + 1; i++) {
		lower_case_word[i] = tolower(word[i]);
	}
	int hash_value = hash_function(lower_case_word);

	hashmap_t hash_value_entry = hashtable[hash_value];

	do {
		char* word_at_location = hash_value_entry->word;
		
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

	bool is_long_word = false;

	while(fscanf(fp, "%45s", word) == 1) {
		int word_length = strlen(word);

		// Detect beginning or middle of a word.
		// If it is a whitespace, then this could be a
		// word with length 45, so we will treat it like
		// any other valid word that needs to be checked.
		if (word_length == 45) {
			char next_char = fgetc(fp);
			ungetc(next_char, fp);

			if (!isspace(next_char)) {
				is_long_word = true;
				continue;
			}
		}

		// end of long word
		if (is_long_word && word_length < 45) {
			is_long_word = false;
			num_misspelled++;
			misspelled[i] = (char * ) malloc(strlen(word));
			strcpy(misspelled[i], word);
			i++;
			continue;
		}

		char last_char = word[word_length - 1];

		if (ispunct(last_char)) {
			word[word_length - 1] = '\0';
		}
		if (ispunct(word[0]) ) {
			memmove(word, word + 1, strlen(word) - 1);
		}

		if (!check_word(word, hashtable)) {
			num_misspelled++;
			misspelled[i] = (char * ) malloc(strlen(word));
			strcpy(misspelled[i], word);
			i++;
		}

	}

	// printf("Number of misspelled words: [%d]\n", num_misspelled);

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
		char* lower_case_word = (char *) malloc((strlen(word) + 1)*sizeof(char));

		if (lower_case_word == NULL) {
			return false;
		}

		for (int i = 0; i < (strlen(word) + 1); i++) {
			lower_case_word[i] = tolower(word[i]);
		}

		hash_value = hash_function(lower_case_word);

		is_added = add_to_hashmap(hash_value, lower_case_word, hashtable);

		if (!is_added) {
			return false;
		}
		free(lower_case_word);
	}


	fclose(fp);

	return true;
}
