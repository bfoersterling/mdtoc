## basic logic

Commonmark allows 6 levels of headings.

Heading levels could be represented as an array of 6 integers.

The first heading might look like this:
```
# My Documentation
```
It is a level 1 header.

You always increment the current header level:
```
[ 1 0 0 0 0 0 ]
```

The next headings might be these:
```
## How To Read This Documentation
```

```
[ 1 1 0 0 0 0 ]
```

```
#### Tutorial
```

```
[ 1 1 0 1 0 0 ]
```

But when you move up levels, you need to clear all child levels.

Suppose the next header is this:
```
## Introduction
```

You still need to increment the current level, but you need to clear the \
level 4 counter:
```
[ 1 2 0 0 0 0 ]
```

## pretty printing

To make a readable heading numbering out of the integer array \
you can omit all zeros.
