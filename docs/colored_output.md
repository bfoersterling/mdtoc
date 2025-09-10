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

#### fatih/color

> the go-isatty package will automatically disable color output for non-tty \
output streams (for example if the output were piped directly to less).

> The color package also disables color output if the NO_COLOR environment variable is set to a non-empty string.

> NoColor defines if the output is colorized or not.\
It's dynamically set to false or true based on the stdout's file descriptor \
referring to a terminal or not.\
It's also set to true if the NO_COLOR environment variable is set \
(regardless of its value).

=> So you set `NoColor` to true or false if you want to force/disable colors.\
If you do not set it, it will look for the env var `NO_COLOR` and check if \
it is in a tty.
