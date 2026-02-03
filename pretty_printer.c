#include <cmark.h>
#include <stdlib.h>
#include <string.h>

#include "pretty_printer.h"

// Prototypes for static functions.
static int determine_item_pos(cmark_node* node);
static bool inside_block_quote(cmark_node* node);
static bool is_block_node(cmark_node* node);
[[maybe_unused]] static bool needs_leading_nl(cmark_node* node);
static int number_of_leading_nl(cmark_node* node);
[[maybe_unused]] static int number_of_trailing_nl(cmark_node* node);
static void pretty_print_cmark_block_quote(cmark_node* node, FILE* stream);
static void pretty_print_cmark_code(cmark_node* node, FILE* stream);
static void pretty_print_cmark_code_block(cmark_node* node, FILE* stream);
static void pretty_print_cmark_heading(cmark_node* node, FILE* stream);
static void pretty_print_cmark_html_block(cmark_node* node, FILE* stream);
static void pretty_print_cmark_html_inline(cmark_node* node, FILE* stream);
static void pretty_print_cmark_image(cmark_node* node, FILE* stream);
static void pretty_print_cmark_item(cmark_node* node, FILE* stream);
static void pretty_print_cmark_linebreak(cmark_node* node, FILE* stream);
static void pretty_print_cmark_link(cmark_node* node, FILE* stream);
static void pretty_print_cmark_list(cmark_node* node, FILE* stream);
static void pretty_print_cmark_paragraph(cmark_node* node, FILE* stream);
static void pretty_print_cmark_softbreak(cmark_node* node, FILE* stream);
static void pretty_print_cmark_strong(cmark_node* node, FILE* stream);
static void pretty_print_cmark_text(cmark_node* node, FILE* stream);
static void pretty_print_cmark_thematic_break(cmark_node* node, FILE* stream);

// Determine the position of an item in a numbered list.
	static int
determine_item_pos(cmark_node* node)
{
	int i = 1;

	for (cmark_node* n = cmark_node_previous(node);
			n != NULL;
			n = cmark_node_previous(n)) {
		i++;
	}

	return i;
}

// If one ancestor is a block quote, the node is inside a block quote.
	static bool
inside_block_quote(cmark_node* node)
{
	for(
			cmark_node* parent = cmark_node_parent(node);
			parent != NULL;
			parent = cmark_node_parent(parent)
	   ) {
		if (cmark_node_get_type(parent) == CMARK_NODE_BLOCK_QUOTE)
			return true;
	}

	return false;
}

	static bool
is_block_node(cmark_node* node)
{
	cmark_node_type type = cmark_node_get_type(node);

	if (type >= 1 && type <= 10)
		return true;

	return false;
}

	static bool
needs_leading_nl(cmark_node* node)
{
	// The first node does not need a leading new line.
	if (cmark_node_previous(node) == NULL)
		return false;

	if (is_block_node(node))
		return true;

	return false;
}

	static int
number_of_leading_nl(cmark_node* node)
{
	if (cmark_node_get_type(cmark_node_parent(node)) == CMARK_NODE_DOCUMENT
			&& cmark_node_previous(node) != NULL)
		return 1;

	return 0;
}

	static int
number_of_trailing_nl(cmark_node* node)
{
	//cmark_node_type node_type = cmark_node_get_type(node);
	//cmark_node_type parent_type = cmark_node_get_type(cmark_node_parent(node));

	return 0;
}

	static void
pretty_print_cmark_block_quote(cmark_node* node, FILE* stream)
{
}

	static void
pretty_print_cmark_code(cmark_node* node, FILE* stream)
{
	fprintf(stream, "\033[36m");
	fprintf(stream, "%s", cmark_node_get_literal(node));
	fprintf(stream, "\033[0m");
}

	static void
pretty_print_cmark_code_block(cmark_node* node, FILE* stream)
{
	const char* code_block = cmark_node_get_literal(node);
	char* code_block_copy = strdup(code_block);

	// When the terminal is at the bottom and needs to scroll the background
	// color will expand until the end of the line.
	// So we need to reset colors and clear the rest of the line before
	// printing the newline.

	for (
			char* line = strtok(code_block_copy, "\r\n");
			line != NULL;
			line = strtok(NULL, "\r\n")
		) {
		fprintf(stream, "\033[33;100m");
		fprintf(stream, "%s", line);
		fprintf(stream, "\033[0m");
		fprintf(stream, "\033[K");
		fprintf(stream, "\n");
	}

	free(code_block_copy);
}

	static void
pretty_print_cmark_heading(cmark_node* node, FILE* stream)
{
	fprintf(stream, "\033[32m");

	for (int i = 0; i < cmark_node_get_heading_level(node); i++)
		fprintf(stream, "#");

	fprintf(stream, " ");
}

	static void
pretty_print_cmark_html_block(cmark_node* node, FILE* stream)
{
	char* html_block_copy = strdup(cmark_node_get_literal(node));

	for (
			char* line = strtok(html_block_copy, "\r\n");
			line != NULL;
			line = strtok(NULL, "\r\n")
		) {
		fprintf(stream, "\033[33m");
		fprintf(stream, "%s", line);
		fprintf(stream, "\033[0m");
		fprintf(stream, "\033[K");
		fprintf(stream, "\n");
	}

	free(html_block_copy);
}

	static void
pretty_print_cmark_html_inline(cmark_node* node, FILE* stream)
{
	fprintf(stream, "\033[33m");
	fprintf(stream, "%s", cmark_node_get_literal(node));
	fprintf(stream, "\033[0m");
}

	static void
pretty_print_cmark_image(cmark_node* node, FILE* stream)
{
	fprintf(stream, "![%s]", cmark_node_get_literal(cmark_node_first_child(node)));
	fprintf(stream, "(%s)", cmark_node_get_url(node));
}

	static void
pretty_print_cmark_item(cmark_node* node, FILE* stream)
{
	cmark_list_type list_type = cmark_node_get_list_type(cmark_node_parent(node));

	if (list_type == CMARK_BULLET_LIST)
		fprintf(stream, "- ");
	else if (list_type == CMARK_ORDERED_LIST)
		fprintf(stream, "%d. ", determine_item_pos(node));
}

	static void
pretty_print_cmark_linebreak(cmark_node* node, FILE* stream)
{
	fputc('\n', stream);
}

	static void
pretty_print_cmark_link(cmark_node* node, FILE* stream)
{
	// A link has no literal and cmark_node_get_title returns an empty string.
	fprintf(stream, "[%s]", cmark_node_get_literal(cmark_node_first_child(node)));
	fprintf(stream, "(\033[4m%s\033[0m)", cmark_node_get_url(node));
}

	static void
pretty_print_cmark_list(cmark_node* node, FILE* stream)
{
	// Reset colors from previous nodes, but do not set any new colors
	// as they would be overwritten by paragraph nodes.
	fprintf(stream, "\033[0m");
}

	void
pretty_print_cmark_node(cmark_node* node, FILE* stream)
{
	for (int i = 0; i < number_of_leading_nl(node); i++)
		fputc('\n', stream);

	switch (cmark_node_get_type(node)) {
		case CMARK_NODE_BLOCK_QUOTE:
			pretty_print_cmark_block_quote(node, stream);
			break;
		case CMARK_NODE_CODE:
			pretty_print_cmark_code(node, stream);
			break;
		case CMARK_NODE_CODE_BLOCK:
			pretty_print_cmark_code_block(node, stream);
			break;
		case CMARK_NODE_DOCUMENT:
			break;
		case CMARK_NODE_HEADING:
			pretty_print_cmark_heading(node, stream);
			break;
		case CMARK_NODE_HTML_BLOCK:
			pretty_print_cmark_html_block(node, stream);
			break;
		case CMARK_NODE_HTML_INLINE:
			pretty_print_cmark_html_inline(node, stream);
			break;
		case CMARK_NODE_IMAGE:
			pretty_print_cmark_image(node, stream);
			break;
		case CMARK_NODE_ITEM:
			pretty_print_cmark_item(node, stream);
			break;
		case CMARK_NODE_LINEBREAK:
			pretty_print_cmark_linebreak(node, stream);
			break;
		case CMARK_NODE_LIST:
			pretty_print_cmark_list(node, stream);
			break;
		case CMARK_NODE_LINK:
			pretty_print_cmark_link(node, stream);
			break;
		case CMARK_NODE_PARAGRAPH:
			pretty_print_cmark_paragraph(node, stream);
			break;
		case CMARK_NODE_SOFTBREAK:
			pretty_print_cmark_softbreak(node, stream);
			break;
		case CMARK_NODE_STRONG:
			pretty_print_cmark_strong(node, stream);
			break;
		case CMARK_NODE_TEXT:
			pretty_print_cmark_text(node, stream);
			break;
		case CMARK_NODE_THEMATIC_BREAK:
			pretty_print_cmark_thematic_break(node, stream);
			break;
		default:
			fprintf(stream, "Node type %s not implemented yet.\n", cmark_node_get_type_string(node));
			break;
	}

	fflush(stream);
}

	static void
pretty_print_cmark_paragraph(cmark_node* node, FILE* stream)
{
	if (cmark_node_get_type(cmark_node_parent(node)) != CMARK_NODE_BLOCK_QUOTE)
		fprintf(stream, "\033[0m");
}

	static void
pretty_print_cmark_softbreak(cmark_node* node, FILE* stream)
{
	fputc(' ', stream);
}

	static void
pretty_print_cmark_strong(cmark_node* node, FILE* stream)
{
}

	static void
pretty_print_cmark_text(cmark_node* node, FILE* stream)
{
	cmark_node_type parent_type = cmark_node_get_type(cmark_node_parent(node));

	if (parent_type == CMARK_NODE_LINK || parent_type == CMARK_NODE_IMAGE)
		return;

	// This is needed because inside a strong node the text nodes may be
	// interrupted by code nodes.
	if (parent_type == CMARK_NODE_STRONG)
		fprintf(stream, "\033[1m");

	if (inside_block_quote(node))
		fprintf(stream, "\033[35m");

	fprintf(stream, "%s", cmark_node_get_literal(node));

	if (parent_type == CMARK_NODE_EMPH || parent_type == CMARK_NODE_STRONG) {
		fprintf(stream, "\033[0m");
		return;
	}
}

	static void
pretty_print_cmark_thematic_break(cmark_node* node, FILE* stream)
{
	fprintf(stream, "\033[34;100m");
	for (int i = 0; i <= 80; i++) {
		fputc('-', stream);
	}
	fprintf(stream, "\033[0m");
	fputc('\n', stream);
}

	void
pretty_print_heading_levels(int levels[6], FILE* stream)
{
	for (int i = 0; i < 6; i++) {
		if (levels[i] == 0)
			continue;

		fprintf(stream, "%d.", levels[i]);
	}
	fflush(stream);
}
