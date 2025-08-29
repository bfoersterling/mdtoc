## desired behavior

User input is the pretty printed heading numbering, i.e. `1.2.`.\
(`-s` for "section" or `-b` for "branch" (since it is a tree data structure))

The output is every line of the document from that heading number on \
until heading number 1.3., 2., or EOF.\
Read until the pretty printed heading number does not start with `1.2.` anymore.

## general strategy

First determine until which heading to read.\
(get next sibling)

Then there are three options:
- read the file again with line numbers from the headers
- write the entire file into memory and read through that
- parse everything at once

#### reading the file again

Pros:
- saves memory
Cons:
- slower
- the file might have changed in the meantime (unlikely)

#### write file into memory

Pros:
- faster
Cons:
- wastes memory (my biggest markdown file has 1000 lines and is 24kb big)

Make `parse_headings()` return a string slice as second return value.

#### parse everything at once

Pros:
- cleaner
Cons:
- needs a sophisticated parser that compiles to an abstract syntax tree
- rewriting of existing code

## edge cases

#### multiple headings with the same pretty printed number

As this should not happen for a well formatted document \
it is probably ok to just print the section of the first heading that \
was found.

#### section before first heading

Print the "section" before the first heading?

User input is `0.` and then just print everything until the line of the \
first heading.
