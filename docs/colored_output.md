## Needed Or Not?

Syntax highlighting is nice when printing chapters.\
You can get the important aspects of the documentation faster.

## Possible Strategies

#### pretty print functions

Have a pretty print function for each `cmark_node_type`.

Since there is no way to distinguish between atx and setext headings through \
the cmark API you need to print setext headings as atx headings.

Downsides:
- The output is not identical to the actual markdown, it is rendered.

#### use cmark_node_get_start_line and similar

You can use `cmark_node_get_start_line`, `cmark_node_get_start_column` and \
similar functions to get the actual source code of the node.

In `cmark` lines sometimes start at column 0, sometimes at 1.

If you have folded lines, those return 0 values:
```
linebreak [0, 0] - [0, 0]
```

Setext headings end at column 0 of two lines ahead:
```
heading [15, 1] - [17, 0]
```

Downsides:
- You need to search for the line in your source code buffer everytime.
- Hard to get the line and columns match the cmark values.
