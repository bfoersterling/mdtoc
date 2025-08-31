If colored output is included, there need to be ways to disable it.

From `man bat`:
> --color <when>\
Specify when to use colored output.\
The automatic mode only enables colors if an interactive terminal is  detected.\
Possible  values: auto, never, always.

`bat` looks at the environment var `$COLORTERM`.\
If it is set to "truecolor" or "24bit", then `bat` says that the terminal \
supports color.

It also looks at the env var `NO_COLOR`.
