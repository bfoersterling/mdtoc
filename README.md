## Installation

Download the latest release.
```
wget https://github.com/bfoersterling/mdtoc/releases/download/v0.0.12/mdtoc_linux_amd64_0.0.12_linux_amd64 -O mdtoc
chmod +x mdtoc
sudo install mdtoc /usr/local/bin
```

Or build it:
```
make
sudo make install
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

- detect Windows line endings
- delph deeper into different Markdown implementations (like CommonMark)
- maybe add support for "Setext-style headers"
- maybe replace pound signs infront of headers by numbers
