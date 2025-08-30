## Installation

Download the latest release.
```
wget https://github.com/bfoersterling/mdtoc/releases/latest/download/mdtoc_linux_amd64 -O /tmp/mdtoc
sudo install -v -m 755 /tmp/mdtoc /usr/local/bin
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
|-- 1. # yamlfmt (1)
|   |-- 1.1. ## Goals (5)
|   |-- 1.2. ## Maintainers (11)
|   |-- 1.3. ## Blog (15)
|   |-- 1.4. ## Installation (21)
|   `-- 1.5. ## Basic Usage (35)
`-- 2. # Configuration File (54)
```

#### Markdown Implementations

https://en.wikipedia.org/wiki/Markdown

There is no official Markdown specification and there are many different \
implementations.\
This seems intended by the creators of the language.

Commonmark seems to be the most widespread option.

#### TODO

- use goldmark for parsing?
- maybe add support for "Setext-style headers"
- maybe remove pound signs infront of headers (or introduce debug mode to show them)
