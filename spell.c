#include "dictionary.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define DICTIONARY "wordlist.txt"
#define TESTDICT "test_worlist.txt"


bool check_word(const char* word, hashmap_t hashtable[])
{
	// Get the lower case representation of the word to compare against.
	char* lower_case_word = (char *)malloc((strlen(word) + 1) * sizeof(char));
	for (int i = 0; i < strlen(word) + 1; i++) {
		lower_case_word[i] = tolower(word[i]);
	}

	// Get the hash_value to identify the location in the hashmap.
	int hash_value = hash_function(lower_case_word);
	hashmap_t hash_value_entry = hashtable[hash_value];

	// Check the values in the hashmap at the index location.
	do {
		char* word_at_location = hash_value_entry->word;

		// We could potentially just use strcmp() here, but to
		// be on the safe side we use strcasecmp to leave case
		// out of the equation>
		if (strcasecmp(word, word_at_location) == 0) {
			free(lower_case_word);
			return true;
		}

		// check the next node
		hash_value_entry = hash_value_entry->next;

	// Repeat the loop until we no longer have nodes in the linked list
	} while (hash_value_entry->word);

	// allocated memory is let go of
	free(lower_case_word);

	// If we don't specifically check out as a correctly spelled
	// word, we return false
	return false;
}


int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[])
{
	char word[LENGTH];

	int i = 0;
	int num_misspelled = 0;

	bool is_long_word = false;

	// load 45 chars of the string passed into our buffer (word)
	// 45 is the max length a word can be, so we don't need to
	// read more than this at once.
	while(fscanf(fp, "%45s", word) == 1) {
		int word_length = strlen(word);

		// Detect beginning or middle of a word.
		// If it is a whitespace, then this could be a
		// word with length 45, so we will treat it like
		// any other valid word that needs to be checked.
		if (word_length == 45) {
			// here we don't know if this is the end of the
			// word, so we look ahead to see if it is a
			// whitespace or not
			// if it is not a whitespace, we don't need to
			// process further, we just mark the is_long_word
			// bool true in case it is the first part of this
			// string. If it is in the middle, this is
			// essentially a no-op, the assumption being
			// re-assigning this value would obviate the need
			// for an if check, presumably more expensive.
			char next_char = fgetc(fp);
			ungetc(next_char, fp);
			if (!isspace(next_char)) {
				is_long_word = true;
				continue;
			}
		}

		// End of long word.
		// Words can only be upto 45 chars according to the
		// definition of LENGTH in dictionary.h
		if (is_long_word && word_length < LENGTH) {
			is_long_word = false;
			num_misspelled++;
			misspelled[i] = (char *)malloc(strlen(word));
			strcpy(misspelled[i], word);
			i++;
			continue;
		}

		// the following few lines removes punctuation at the start
		// and end of a word
		char last_char = word[word_length - 1];
		if (ispunct(last_char)) {
			// '\0' is how C terminates strings
			word[word_length - 1] = '\0';
		}
		if (ispunct(word[0]) ) {
			// get rid of the first char by copying the
			// rest of the word back into the word
			// we already know where word is, so we don't
			// really need the return value from the call
			memmove(word, word + 1, strlen(word));
		}

		// The moment we have all been waiting for!
		// This checks whether the single word we have read
		// from the the file is actually a word that is spelled
		// correctly.
		if (!check_word(word, hashtable)) {
			num_misspelled++;
			misspelled[i] = malloc(strlen(word) + 1);
			strcpy(misspelled[i], word);
			i++;
		}

	}
	return num_misspelled;
}

/**
 * Adds a node to the hashmap at the desired location. Returns true
 * if the operation is successful.
 **/
/*
 * Inputs:
 *  hash_value: Value of the index in the hashmap array the word is
 *  		supposed to be inserted to.
 *  word: 	String which we want to insert.
 *  hashtable: 	The hashmap we want to mutate.
 *
 *  Returns:
 *   bool: 	Representing a successful addtion to the hashmap.
 *
 *  Modifies:
 *   hashtable: If successful, a new node will be added to the hashmap.
 *
 *  Example:
 *   bool is_added = add_to_hashmap(hash_value, word, hashtable);
 *
 *  Note:
 *   Should be noted  that memory is allocated to add to an array that
 *   be freed from the context of the caller of any function which uses
 *   this. How one keeps track of this in production C code using best
 *   practices is beyond me, and I can see one way of how memory leaks
 *   are almost inevitable. Rust handles this well with their concept of
 *   ownership.
 */
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

	// Get the file pointer for the dictionary file.
	FILE* fp = fopen(dictionary_file, "r");

	// ensure the sacnned length of the word is 45
	// there may be an edge case where a 45 letter word may be the first
	// part of a string which is longer than that,
	// I think it is safe to ignore that for now
	while(fscanf(fp, " %45s", word) == 1) {
		// Allocate memory for the lower case representation of the word
		// that we will store in the hashmap.
		char* lower_case_word = (char *) malloc((strlen(word) + 1)*sizeof(char));
		if (lower_case_word == NULL) {
			return false;
		}

		// Actually convert to lower case and add to hashmap.
		for (int i = 0; i < (strlen(word) + 1); i++) {
			lower_case_word[i] = tolower(word[i]);
		}
		hash_value = hash_function(lower_case_word);
		is_added = add_to_hashmap(hash_value, lower_case_word, hashtable);

		// Error handling in case the operation fails.
		// Although as a caller we are trying to be safe, it
		// should be noted that there isn't really an explicit
		// return false in the actual function.
		if (!is_added) {
			return false;
		}

		// We don't need no memory leaks
		// We don't need no OOM control
		// Really trying hard there with the pink floyd
		// reference.
		free(lower_case_word);
	}


	fclose(fp);
	return true;
}
