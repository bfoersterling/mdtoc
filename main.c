#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chapter.h"
#include "interactive.h"
#include "parser.h"
#include "stream.h"

// Prototypes for static functions.
static void handle_cli_args(int argc, char** argv);
static void print_version(void);
static void usage(void);

	static void
handle_cli_args(int argc, char** argv)
{
	static struct option cli_args[] = {
		{"chapter", required_argument, 0, 'c'},
		{"edit", required_argument, 0, 'e'},
		{"help", no_argument, 0, 'h'},
		{"interactive", no_argument, 0, 'i'},
		{"version", no_argument, 0, 'v'},
		{0, 0, 0, 0},
	};

	int c = 0;
	int option_index = 0;

	while(c != -1) {
		c = getopt_long(argc, argv, "hive:c:", cli_args, &option_index);

		switch(c) {
			case 'c':
				if (argc - optind != 1) {
					fprintf(stderr, "Option \"-c\" requires exactly one file!\n");
					exit(1);
				}
				const char* file_path = argv[optind - option_index];

				FILE* md_file = fopen(file_path, "r");

				if (md_file == NULL) {
					fprintf(stderr, "Error opening file \"%s\": %s\n",
							file_path, strerror(errno));
					exit(1);
				}

				print_chapter(md_file, optarg, stdout);

				fclose(md_file);

				exit(0);
				break;
			case 'e':
				int rc = edit_chapter(argv[optind], optarg);
				exit(rc);
				break;
			case 'h':
				usage();
				exit(1);
				break;
			case 'i':
				if (argc - optind != 1) {
					fprintf(stderr, "Option \"-i\" requires exactly one file.\n");
					exit(1);
				}
				do_interactive(argv[optind]);
				exit(0);
				break;
			case 'v':
				print_version();
				break;
		}
	}
}

	static void
print_version(void)
{
	printf("mdtoc version %s comiled at %s.\n", VERSION, COMPILE_DATE);
	exit(0);
}

	static void
usage(void)
{
	printf("mdtoc [OPTION]... FILE...\n\n");
	printf("-c/--chapter CHAPTER FILE\tprint specific chapter ");
	printf("-e/--edit CHAPTER FILE\tprint specific chapter ");
	printf("(i.e. \"2.1.\")\n");
	printf("-h/--help\t\t\tprint this help\n");
	printf("-i/--interactive FILE\t\tinteractive mode\n");
}

	int
main(int argc, char** argv)
{
	handle_cli_args(argc, argv);

	if (argc == 1) {
		char* input = get_user_input();

		struct heading* hhead = parse_headings(input);
		print_heading_tree(hhead, 0, stdout);

		free_heading_tree(hhead);
		free(input);

		return 0;
	}

	for (int i = 1; i < argc; i++) {
		char* file_content = read_file(argv[i]);

		struct heading* hhead = parse_headings(file_content);
		print_heading_tree(hhead, 0, stdout);

		free_heading_tree(hhead);
		free(file_content);
	}

	return 0;
}
