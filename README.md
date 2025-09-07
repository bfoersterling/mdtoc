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

Print TOC:
```
$ ./mdtoc test_files/audio.md
audio.md
|-- 1. ## alsa (1)
|   |-- 1.1. #### aplay (8)
|   |-- 1.2. #### configuration (59)
|   |-- 1.3. #### select default soundcard and/or pcm device (85)
|   `-- 1.4. #### audioservers (109)
|-- 2. ## how to figure out if you are using pulseaudio or pipewire? (114)
|-- 3. ## pipewire (128)
|   |-- 3.1. #### pipewire - record audio (137)
|   `-- 3.2. #### pipe audio to pipewire (144)
|-- 4. ## pulseaudio (151)
|   |-- 4.1. #### installation (153)
|   |-- 4.2. #### pulseaudio commands (161)
|   |-- 4.3. #### testing pulseaudio (171)
|   |-- 4.4. #### ffmpeg (188)
|   |-- 4.5. #### play sounds on the command line (204)
|   `-- 4.6. #### test microphone (219)
|-- 5. ## Volume problem in movies (239)
|   |-- 5.1. #### audacity (312)
|   `-- 5.2. #### location of sound files that applications use (326)
`-- 6. ## troubleshooting (346)
    |-- 6.1. #### no audio input from headset (348)
    `-- 6.2. #### no sound issue on Ubuntu (359)
```

Print the chapter under heading 3.1:
```
mdtoc -c 3.1 test_files/audio.md
```

#### Markdown Implementations

https://en.wikipedia.org/wiki/Markdown

There is no official Markdown specification and there are many different \
implementations.\
This seems intended by the creators of the language.

Commonmark seems to be the most widespread option.\
But it is hard to implement.

Currently this project is using a very simplified subset of Markdown.\
Lines starting with `#` chars are (ATX) headings.\
Lines starting with three backticks start and end code blocks.

#### Markdown Parsers

Parsers like `goldmark`, `gomarkdown`, or `blackfriday` are good \
for converting markdown to html but not good for using them as \
parsers.\
`gomarkdown` can extract headings and heading levels, but doesnt provide \
the position of the tokens.

Maybe build a parser for a simplified Markdown implementation \
that creates an AST with token position, literal string, \
token type information and things like heading levels.\
=> ast node interface with types (heading, code block, citation) as structs that implement it

#### TODO

- use goldmark for parsing?
- maybe add support for "Setext-style headers"
- maybe remove pound signs infront of headers (or introduce debug mode to show them)
- interactive mode with "e" (edit chapter) and "c" (print chapter) options
- color highlight code blocks in print chapter mode (=> new `code` struct that implements `line`)
