## headings as tree or flat list?

Creating the headings directly as a tree data structure with a `next` \
and a `first_child` field might be convenient for printing the toc \
as well as printing the section/chapter.

But will that work for out of tree headings?

The table of content should be printed in the order of the headings line \
numbers. And out of tree headers should be included.

The headings are not a real tree because they do not always have one single root.\
Markdown documents may have multiple top level headings.

  ^^^
 ^^^^^
^^^^^^^

So it might be difficult to represent them as a tree data structure.

In practice especially out of tree headers made this very difficult.

Printing the section should be easy comparing the `levels` fields.\
Just implement a function for comparing `levels`.\
Maybe when printing the toc the `levels` field can also be used.\
Get the direct children by comparing `levels`.

## multiple top headings?

It is recommended to have a single root heading \
with heading level 1.\
But it is still valid markdown if you have multiple headings \
at the top, for example three level 2 headings.

## add artificial root

A tree can only have ONE root.

But you could add an artificial root node.

It could look like this:
```
.text = "",
.level = 0,
.levels = int[6]{0,0,0,0,0,0},
.next = (pointer to first heading),
```

The root heading could then be identified through its level.\
It is the only heading with level 0.

Or maybe add the name of the file as .text and print it with the tree?\
But then the root heading should only be printed if reading from a file and \
not stdin.\
=> This could be done at a later development stage!

## head field "id"?

Add an "id" field for struct heading that acts like an index.\
Then you could easily know if it is the first heading.\
But this makes more sense for a list structure and less for a tree structure.
