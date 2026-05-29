#pragma once

#include <cmark.h>

void pretty_print_cmark_node(cmark_node* node, FILE* stream);
void pretty_print_heading_levels(int levels[6], FILE* stream);
void print_colored_markdown(const char* source_code, FILE* stream);
