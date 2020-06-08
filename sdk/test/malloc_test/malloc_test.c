
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <system.h>


void main() {
	const char sentence[] = "The quick brown fox jumps over the lazy dog.";
	char* first_buffer = (char*)malloc(sizeof(sentence));
	char* second_buffer = (char*)malloc(sizeof(sentence));
	free(first_buffer);
	char* third_buffer = (char*)malloc(sizeof(sentence));
	printf("%d\n", first_buffer == third_buffer);
	memcpy(second_buffer, sentence, sizeof(sentence));
	printf("sentence: '%s'\n", second_buffer);
}


