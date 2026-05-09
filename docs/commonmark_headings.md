# commonmark headings

According to https://spec.commonmark.org/0.31.2/ headings are "leaf blocks".

> Some blocks (like block quotes and list items) contain other blocks;\
others (like headings and paragraphs) contain inline content—text, links,\
emphasized text, images, code spans, and so on.

=> So headings can only contain `inline` content.

## infinite children

A heading in commonmark can have infinite children.

Example:
```
## [a link with *italic and **bold with *italic* in it** content* in it](https://example.com)
```
The ast will look like this:
```
heading [1, 1] - [1, 90]
    link [1, 4] - [1, 90]
      text [1, 5] - [1, 16]
      emph [1, 17] - [1, 62]
        text [1, 18] - [1, 28]
        strong [1, 29] - [1, 56]
          text [1, 31] - [1, 40]
          emph [1, 41] - [1, 48]
            text [1, 42] - [1, 47]
          text [1, 49] - [1, 54]
        text [1, 57] - [1, 61]
      text [1, 63] - [1, 68]
```

## maximum number of characters

A heading cannot span over multiple lines with `\`.

Sane people will not use more than 80 characters for a heading.

It is probably fair to use `LINE_MAX` from `limits.h` for a buffer size.
