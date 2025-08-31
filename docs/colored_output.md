If colored output is included, there need to be ways to disable it.

From `man bat`:
> --color <when>\
Specify when to use colored output.\
The automatic mode only enables colors if an interactive terminal is  detected.\
Possible  values: auto, never, always.

`bat` looks at the environment var `$COLORTERM`.\
If it is set to "truecolor" or "24bit", then `bat` says that the terminal \
supports color.

It also looks at the env var `NO_COLOR`. \
https://no-color.org/

> By adopting this standard, users that prefer to have plain, non-colored \
text output can export NO_COLOR=1 to their shell’s environment and \
automatically disable color by default in all supported software.
