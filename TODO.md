## TODO

- print_uncolored_markdown function instead of printing raw markdown?
  => fix missing newlines after preamble headings
  => would remove unnecessary whitespace
  => replace `\` with real newlines?
- one file "option.c" and "option.h" with functions like
  "option_print_chapter" for all options that can be either used through
  cli argument or in interactive mode
- maybe no preamble for chapters that only contain whitespace and control
characters
- respect FORCE_COLOR environment variable that forces colors even when \
  output is redirected (would improve integration tests)?
- support multiple files for all options?
  (this would be a breaking api change -> 1.0.0)
