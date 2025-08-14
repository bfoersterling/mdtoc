## Installation

Download the latest release.
```
wget https://github.com/bfoersterling/mdtoc/releases/latest/download/mdtoc_linux_amd64 -O mdtoc
sudo install -v -m 755 mdtoc /usr/local/bin
```

Or build it:
```
git clone https://github.com/bfoersterling/mdtoc.git
sudo just install
```

## Usage

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

#### Markdown Implementations

https://en.wikipedia.org/wiki/Markdown

There is no official Markdown specification and there are many different \
implementations.\
This seems intended by the creators of the language.

#### TODO

- use goldmark for parsing?
- detect Windows line endings
- maybe add support for "Setext-style headers"
- maybe replace pound signs infront of headers by numbers
- real heading numbering (for example 2.3.1) \
=> would require validating the markdown file first \
(or print `0.0.0` for invalid headers outside of the tree)
- option to print content under header x.x.x
