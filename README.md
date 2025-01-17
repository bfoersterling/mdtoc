## building

```
go build
```

## usage

```
$ ./mdtoc test_files/README.md
README.md
|-- # yamlfmt (1)
|   |-- ## Goals (5)
|   |-- ## Maintainers (11)
|   |-- ## Blog (15)
|   |-- ## Installation (21)
|   `-- ## Basic Usage (35)
`-- # Configuration File (54)
```

#### markdown implementations

https://en.wikipedia.org/wiki/Markdown

There is no official Markdown specification and there are many different \
implementations.\
This seems intended by the creators of the language.

#### TODO

- detect Windows line endings
- delph deeper into different Markdown implementations (like CommonMark)
- add support for "Setext-style headers"
- maybe remove pound signs infront of headers
