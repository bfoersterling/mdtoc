## problem

If there is markdown in front of the first heading, `mdtoc` can not print \
this section.

Markdown files without headings will not show any chapters.

## possible solutions

Insert artificial preamble heading after the root heading.

Print a warning that text has been detected before the first heading.
