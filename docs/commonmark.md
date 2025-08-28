https://spec.commonmark.org/current/

#### valid markdown

> Any sequence of characters is a valid CommonMark document.

(https://spec.commonmark.org/0.31.2/#preliminaries) \
(beginning of "2.1 Characters and lines")

#### ATX headings

https://spec.commonmark.org/0.31.2/#atx-heading

> An ATX heading consists of a string of characters, parsed as inline content,\
between an opening sequence of 1–6 unescaped # characters and an optional closing sequence of any number of unescaped # characters.

=> 1-6 `#` characters.

> The opening sequence of # characters must be followed by spaces or tabs, or by the end of line.

=> space/tab after the `#`s (end of line is probably for empty headings) \
=> NOT valid: `####my header` -> valid: `#### my header`

> The opening # character may be preceded by up to three spaces of indentation.\
The raw contents of the heading are stripped of leading and trailing space or tabs before being parsed as inline content.

=> 0-3 spaces before the `#` characters are allowed.
