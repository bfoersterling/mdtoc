## TODO

- one file "option.c" and "option.h" with functions like
  "option_print_chapter" for all options that can be either used through
  cli argument or in interactive mode
- print_uncolored_markdown function instead of printing raw markdown?
  => would remove unnecessary whitespace
  => replace `\` with real newlines?
- maybe no preamble for chapters that only contain whitespace and control
characters
- respect FORCE_COLOR environment variable that forces colors even when \
  output is redirected (would improve integration tests)?
