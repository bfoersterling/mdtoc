## newlines

#### paragraph

A paragraph can have more than one child node.\
The child nodes can be `text`, `linebreak`, `code`, `strong`, `emph` \
and maybe more.\
=> for printing trailing newlines you would need to determine that the \
current node is the last child of a paragraph.

A paragraph can be a child of a list item.\
=> in this case you need exactly one trailing newline char.

A paragraph can be the child of a block quote.

A paragraph that is the direct child of a document node needs two \
trailing newlines.

A paragraph should have two leading newlines, but not when the parent \
is an `item`.\
And when the parent is a `block_quote` then the question is whether \
the `block_quote` prints the two newlines or the paragraph.

How to know if a paragraph is over?\
=> Proposition: in the function `print_colored_cmark_tree`, that traverses \
the tree between
```
if (cmark_node_first_child(node))
```
and
```
if (cmark_node_next(node))
```

#### child of document

Proposition: Every child of a document should have two trailing newlines \
except for the first child.

=> The statement is false because a `heading` should not have any trailing \
newlines, only its last child should.

Proposition: Every child of a document should have two leading newlines \
except for the first child.

=> I could not disprove this proposition so far.

#### heading

A heading usually has at least one `text` child node.\
But it can have multiple `text` and `code` children.

We need two new lines after the last child.

#### text

Proposition: A `text` node that has no `next` sibling needs a trailing newline.

=> False: When a `text` node is the child of `strong` there should be no \
trailing nl because there may be a `text` node after the `strong` node.

#### global var that tracks the number of newlines?

Have a `static int` var in `pretty_printing.c` that tracks the number of \
consecutive newlines printed.

And then you can limit the maximum number of newlines to 2.

The downside is that you have a variable with a file wide scope.

Or drag a variable through all functions?

## colors

#### WARNING

When the terminal is at the bottom and needs to scroll the background \
color glitches at the end of the line!

You will need to print `\033[K` to the stream instantly after resetting \
the colors and before the newline.

#### general

Nodes that have everything in its `literal` can switch colors on and off \
inside their own functions.\
=> This is the case for `code`, `code_block`, and `thematic_break`.

Nodes that do not have a `literal`, but (nested) child nodes with `literal` \
can switch on colors in their own function and the reset needs to happen \
in other functions.\
=> This is the case for `block_quote`, `heading`, `strong`.

## hyperlinks

https://gist.github.com/egmontkob/eb114294efbcd5adb1944c9f3cb5feda

> Currently there's no way of detecting whether the terminal emulator \
supports hyperlinks.

https://en.wikipedia.org/wiki/ANSI_escape_code

> A non-xterm extension is the hyperlink, `ESC ]8;;link ST` from 2017,\
used by VTE, iTerm2, and mintty, among others.

For now do not use it.\
Maybe test multiple terminals and put it in if it is supported by most \
terminals.

## using a state?

A `block_quote` for example is not the direct parent of the underlying inline \
nodes like `text` or `code`.\
There is a `paragraph` in between.

Proposition: Inline nodes are mutually exclusive.\
=> False, `strong` is the parent of `text`.
