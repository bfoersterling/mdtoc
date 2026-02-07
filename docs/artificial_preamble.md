## problem

If there is markdown in front of the first heading, `mdtoc` can not print \
this section.

Markdown files without headings will not show any chapters.

## implemented solution

Insert artificial preamble heading after the root heading.

It has level 1 or the level of the first real heading.\
Its levels member is all zeros like the artificial root heading.

The only way to discriminate between the root and the preamble heading \
is to look at their levels.\
The root heading is the only heading with level 0.

## alternate ways

Print a warning that text has been detected before the first heading.

This goes somewhat against the Commonmark spec which says:\
> Any sequence of characters is a valid CommonMark document.

So even whitespace is valid markdown.

Linting with warnings or errors goes against this \
and this is not the goal of this tool.
