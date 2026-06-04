#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chapter.h"
#include "interactive.h"
#include "heading.h"
#include "stream.h"

// Prototypes for static functions.
static void handle_cli_args(int argc, char** argv);
static void print_version(void);
static void usage(void);

enum mode {
	EDIT_CHAPTER,
	HELP,
	INTERACTIVE,
	PRINT_CHAPTER,
	PRINT_VERSION,
	SEARCH_STRING,
	TOC,
};

	static void
handle_cli_args(int argc, char** argv)
{
	if (argc == 1)
		return;

	static struct option cli_args[] = {
		{"chapter", required_argument, 0, 'c'},
		{"edit", required_argument, 0, 'e'},
		{"help", no_argument, 0, 'h'},
		{"interactive", no_argument, 0, 'i'},
		{"search", required_argument, 0, 's'},
		{"version", no_argument, 0, 'v'},
		{0, 0, 0, 0},
	};

	int c = 0;
	int option_index = 0;
	const char* file_path = NULL;
	char* source_code = NULL;
	char* chosen_chapter = NULL;
	char* search_string = NULL;
	enum mode chosen_mode = -1;

	while(c != -1) {
		c = getopt_long(argc, argv, "hive:c:s:", cli_args, &option_index);

		switch(c) {
			case 'c':
				if (argc - optind != 1) {
					fprintf(stderr, "Option \"-c\" requires exactly one file!\n");
					exit(1);
				}
				chosen_mode = PRINT_CHAPTER;
				chosen_chapter = optarg;
				break;
			case 'e':
				if (argc - optind != 1) {
					fprintf(stderr, "Option \"-e\" requires exactly one file.\n");
					exit(1);
				}
				chosen_mode = EDIT_CHAPTER;
				chosen_chapter = optarg;
				break;
			case 'h':
				chosen_mode = HELP;
				break;
			case 'i':
				if (argc - optind != 1) {
					fprintf(stderr, "Option \"-i\" requires exactly one file.\n");
					exit(1);
				}
				chosen_mode = INTERACTIVE;
				break;
			case 's':
				if (argc - optind != 1) {
					fprintf(stderr, "Option \"-s\" requires exactly one file.\n");
					exit(1);
				}
				chosen_mode = SEARCH_STRING;
				search_string = optarg;
				break;
			case 'v':
				chosen_mode = PRINT_VERSION;
				break;
		}
	}

	if (file_path != NULL) {
		file_path = argv[optind];
		source_code = read_file(file_path, true);
	}

	switch(chosen_mode) {
		case EDIT_CHAPTER:
			edit_chapter(file_path, chosen_chapter);
			break;
		case HELP:
			usage();
			break;
		case INTERACTIVE:
			do_interactive(file_path);
			break;
		case PRINT_CHAPTER:
			print_chapter(source_code, chosen_chapter, stdout);
			break;
		case PRINT_VERSION:
			print_version();
			break;
		case SEARCH_STRING:
			search_chapters_for_str(source_code, search_string, stdout);
			break;
		default:
			usage();
			break;
	}

	free(source_code);
	exit(0);
}

	static void
print_version(void)
{
	printf("mdtoc version %s compiled at %s.\n", VERSION, COMPILE_DATE);
	exit(0);
}

	static void
usage(void)
{
	printf("mdtoc [OPTION]... FILE...\n\n");
	printf("-c/--chapter CHAPTER FILE\tprint specific chapter ");
	printf("(i.e. \"2.1\")\n");
	printf("-e/--edit CHAPTER FILE\t\tedit specific chapter ");
	printf("(i.e. \"2.1\")\n");
	printf("-h/--help\t\t\tprint this help\n");
	printf("-i/--interactive FILE\t\tinteractive mode\n");
	printf("-s/--search STRING\t\tsearch chapters for string\n");
}

	int
main(int argc, char** argv)
{
	handle_cli_args(argc, argv);

	if (argc == 1) {
		char* input = read_stream(stdin, true);

		struct heading* hhead = parse_headings(input);
		print_heading_tree(hhead, 0, stdout);

		free_heading_tree(hhead);
		free(input);

		return 0;
	}

	for (int i = 1; i < argc; i++) {
		char* file_content = read_file(argv[i], true);

		struct heading* hhead = parse_headings(file_content);
		print_heading_tree(hhead, 0, stdout);

		free_heading_tree(hhead);
		free(file_content);
	}

	return 0;
}
