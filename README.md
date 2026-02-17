# mdtoc

Create markdown table of contents.\
Read individual chapters with syntax highlighting.\
Edit chapters with your `$EDITOR`.\
Makes it easy to work with large Markdown files.

Fully Commonmark compliant.

## installation

#### arch user repository

https://aur.archlinux.org/packages/mdtoc

```
yay -S mdtoc
```

#### download the latest release

```
make install_latest_release
```

Dependencies:
- cmark
- readline

#### build from source

```
make
sudo make install
```

## usage

Without arguments `mdtoc` will print a table of content.

The most useful option is probably the interactive mode - `-i`.

In interactive mode you can type in the chapter numbering to print it.\
Editing chapters is possible with `e CHAPTER` (or without CHAPTER to view the \
most recently printed chapter.\
Or you can print the table of content with `toc`.

## architecture

Written in C23.

Markdown is parsed by the Commonmark reference implementation.\
(https://github.com/commonmark/cmark)

Headings are stored in a tree data structure.

Out of tree headings will always be shown, as in Commonmark any document \
is a valid Markdown document and this enables the user to fix the structure \
of the document.
