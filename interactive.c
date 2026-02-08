#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
// Need to include this later so that FILE from stdio is known.
#include <readline/history.h>
#include <readline/readline.h>

#include "chapter.h"
#include "parser.h"
#include "string_util.h"

// Prototypes for static functions.
static void add_history_only_once(const char* command);
static char* complete_commands(const char* text, int state);
static void ia_chapter(const char* file_path, const char* user_input);
static void ia_edit(const char* file_path, const char* user_input);
static void ia_help(const char* user_input);
static void ia_help_topic(const char* topic);
static void ia_toc(const char* file_path, const char* user_input);

// Add command to history if it is not already the last entry.
// Returns true if it was added otherwise false.
	static void
add_history_only_once(const char* command)
{
	HIST_ENTRY* cur_hist = history_get(history_length);

	if (history_length == 0) {
		add_history(command);
		return;
	}

	assert(cur_hist != NULL);

	if (strcmp(cur_hist->line, command) == 0) {
		return;
	} else {
		add_history(command);
		return;
	}
}

	static char*
complete_commands(const char* text, int state)
{
	if (state != 0) {
		return (char*)NULL;
	}

	char* commands[] = {
		"edit",
		"chapter",
		"help",
		"toc",
	};

	// You might need to increase this depending on the
	// number of elements in commands array.
	size_t buffer_size = 256;
	char* buffer = malloc(buffer_size);
	memset(buffer, 0, buffer_size);

	int match_count = 0;

	for (int i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
		if (strncmp(commands[i], text, strlen(text)) == 0) {
			match_count++;
			if (strlen(buffer) > 0)
				strcat(buffer, " ");
			strcat(buffer, commands[i]);
		}
	}

	if (match_count == 1)
		return buffer;

	if (match_count > 1) {
		printf("\n%s\n", buffer);
		rl_redraw_prompt_last_line();
	}

	free(buffer);

	return (char*)NULL;
}

	static void
ia_chapter(const char* file_path, const char* user_input)
{
	char* user_input_copy = strdup(user_input);
	// chapter will point to the numbering token.
	char* chapter = NULL;

	int counter = 0;
	for (char* token = strtok(user_input_copy, " ");
			token != NULL;
			token = strtok(NULL, " "))
	{
		if (counter == 0
				&& isdigit((int)*token)) {
			chapter = token;
			break;
		}
		if (counter == 0
				&& strcmp(token, "c") != 0 && strcmp(token, "chapter")) {
			printf("Unknown command %s.\n", token);
			free(user_input_copy);
			return;
		}

		if (counter > 1) {
			printf("Command \"chapter\" only accepts one argument.\n");
			printf("Received \"%s\".\n", token);
			free(user_input_copy);
			return;
		}

		if (counter == 1) {
			chapter = token;
		}

		counter++;
	}

	if (chapter == NULL) {
		printf("Command \"chapter\" requires one argument.\n");
		free(user_input_copy);
		return;
	}

	FILE* source_file = fopen(file_path, "r");

	if (source_file == NULL) {
		printf("Error opening %s: %s\n", file_path, strerror(errno));
		free(user_input_copy);
		return;
	}

	print_chapter(source_file, chapter, stdout);

	fclose(source_file);

	free(user_input_copy);
}

	static void
ia_edit(const char* file_path, const char* user_input)
{
	char* user_input_copy = strdup(user_input);
	const char* chapter = NULL;

	int token_count = 0;
	for (
			char* token = strtok(user_input_copy, " ");
			token != NULL;
			token = strtok(NULL, " ")
		) {
		if (token_count == 0
				&& strcmp(token, "e") != 0
				&& strcmp(token, "edit") != 0) {
			fprintf(stderr, "Unknown command \"%s\".\n", token);
			free(user_input_copy);
			return;
		} else if (token_count == 1) {
			chapter = token;
		}
		if (token_count > 1) {
			fprintf(stderr, "Command \"edit\" does not accept more than one argument.\n");
			fprintf(stderr, "Received \"%s\".\n", token);
			free(user_input_copy);
			return;
		}
		token_count++;
	}

	// If no chapter was provided, the last printed chapter shall be used.
	if (token_count == 1 && history_get(history_length) != NULL
			&& first_digit_in_str(history_get(history_length)->line) != NULL)
		chapter = first_digit_in_str(history_get(history_length)->line);

	if (chapter != NULL && isdigit((int)*chapter))
		edit_chapter(file_path, chapter);
	else
		fprintf(stderr, "No chapter provided for command \"edit\" and no recently"
				" printed chapter detected.\n");

	free(user_input_copy);
}

	static void
ia_help(const char* user_input)
{
	char* user_input_copy = strdup(user_input);
	// topic will point to the help topic.
	char* topic = NULL;

	int counter = 0;
	for (
			char* token = strtok(user_input_copy, " ");
			token != NULL;
			token = strtok(NULL, " ")
		)
	{
		if (counter == 0 &&
				strcmp(token, "help") != 0) {
			printf("Unknown command \"%s\".\n", token);
			free(user_input_copy);
			return;
		}
		if (counter == 1) {
			topic = token;
		}
		if (counter > 1) {
			printf("Command \"help\" does not accept more than one argument.\n");
			free(user_input_copy);
			return;
		}
		counter++;
	}

	if (topic != NULL) {
		ia_help_topic(topic);
		free(user_input_copy);
		return;
	}

	printf("Possible commands:\n\n");
	printf("CHAPTER\t\t\tshow chapter (i.e. \"1.1\")\n");
	printf("c[hapter] CHAPTER\tshow chapter\n");
	printf("e[dit] [CHAPTER]\tedit chapter (without argument the last shown\n"
			"\t\t\tchapter will be edited)\n");
	printf("help [TOPIC]\t\tshow this help\n");
	printf("toc\t\t\tprint table of contents\n");

	free(user_input_copy);
}

	static void
ia_help_topic(const char* topic)
{
	if (strcmp(topic, "chapter") == 0) {
		printf("chapter CHAPTER\n\n");
		printf("example:\n\"chapter 2.1\"\n");
		return;
	} else {
		printf("No help for topic \"%s\".\n", topic);
	}
}

	static void
ia_toc(const char* file_path, const char* user_input)
{
	if (strlen(user_input) > 3) {
		printf("Command \"toc\" has no additional arguments.\n");
		return;
	}
	print_toc(file_path, stdout);
}

	void
do_interactive(const char* file_path)
{
	if (file_path == NULL || *file_path == '\0') {
		fprintf(stderr, "Please provide exactly one file for interactive mode.\n");
		return;
	}

	const char* prompt = "mdtoc> ";
	rl_completion_entry_function = complete_commands;

	for (char* user_input = readline(prompt);
			user_input != NULL;
			user_input = readline(prompt)) {
		if (*user_input == '\0') {
			free(user_input);
			continue;
		}

		trim_space(user_input);

		if (strncmp(user_input, "c", 1) == 0) {
			ia_chapter(file_path, user_input);
		} else if(strncmp(user_input, "e", 1) == 0) {
			ia_edit(file_path, user_input);
		} else if(strncmp(user_input, "toc", 3) == 0) {
			ia_toc(file_path, user_input);
		} else if (strncmp(user_input, "help", 4) == 0) {
			ia_help(user_input);
		} else if (isdigit((int)*user_input)) {
			ia_chapter(file_path, user_input);
		} else {
			printf("Unknown command: \"%s\"\n", user_input);
		}

		add_history_only_once(user_input);

		free(user_input);
	}
}
