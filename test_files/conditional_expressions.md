## general info

#### documentation

```
man bash
```
(Sektion "CONDITIONAL EXPRESSIONS") (`/^CONDITIONAL`)

https://www.gnu.org/software/bash/manual/html_node/Bash-Conditional-Expressions.html

https://phoenixnap.com/kb/bash-if-statement


|`Syntax                ` |	`What it is` | `When to use`
|---|---|---|
|`if ( <commands> )`	| Subshell executed in a subprocess.	| When the commands affect the current shell or environment. The changes do not remain when the subshell completes.
|`if (( <commands> ))`	| Bash extension.	| Use for arithmetic operations and C-style variable manipulation.
|`if [ <commands> ]`	| POSIX builtin, alias for test <commands>.	| Comparing numbers and testing whether a file exists.
|`if [[ <commands> ]]`	| Bash extension, an advanced version of single square brackets. |	String matching a wildcard pattern.

## usage

#### check for flags

example of a flag used in an ifstatement:
```
#!/bin/bash

myflag=1

if (($myflag)); then
  printf "flag is set.\n"
else
  printf "flag is NOT set.\n"
fi
```

It will only evaluate to false if what is inside the (( )) expression is 0.
```
$ (($myflag)) && echo 'true'
$ myflag=1
$ (($myflag)) && echo 'true'
true
$ myflag=0
$ (($myflag)) && echo 'true'
$
```

#### check if command line argument is empty
```
#!/bin/bash

argument=$1

if [[ -z $argument ]]; then
  printf "You didnt provide an argument.\n"
  exit 1
fi

printf "argument is %s\n" "$argument"
```

#### two if-conditions in one statement

```
#!/bin/bash

mynum=1
read yournum

if [[ $mynum -eq 1 ]] && [[ $yournum -eq 2 ]]; then
  printf "You typed 2.\n"
else
  printf "You did not type 2.\n"
fi
```


#### if contains

```
if [[ $(head -1 /etc/os-release) == *"Debian GNU/Linux"* ]]; then
  source /usr/share/doc/fzf/examples/key-bindings.bash
fi
```

#### compare integers

Integers can be compared with these operators:

```
-eq # Equal
-ne # Not equal
-lt # Less than
-le # Less than or equal
-gt # Greater than
-ge # Greater than or equal
```

#### checking for function arguments

The tricky part is if you supply a cli argument that is also used in a function argument inside the script.\
And you call function also as cli arguments, so the argument for the function is $2 and the function is $1.\
But for the function the argument is $1.

In a case statement have to call the function like that:
```
case $1 in
  delete)
    delete_section $2
    ;;
  read)
    read_section $2
    ;;
esac

```
(because it is the $2 argument for the script)

But in the function itself you have to work with $1:
```
function read_section() {
  local section=$1
  ...
```

#### check if folder exists

```
#!/bin/bash

DIRECTORY=/tmp/new_folder

if [ -d "$DIRECTORY" ]; then
  echo "$DIRECTORY does exist."
else
  echo "$DIRECTORY does not exist."
fi

exit 0
```

#### check if file exists

```
FILE=/tmp/new_file.txt

if [ -f "$FILE" ]; then
    echo "$FILE exists."
fi

if [ ! -f "$FILE" ]; then
  echo "$FILE does not exist."
fi
```

`if [ -r mytext.txt ]; then` seems to do the same thing...
***

#### check for permissions

```
if [[ $(stat -c %U "$LOGDIR") != "myuser" ]]; then
	echo "LOGDIR is not owned by myuser."
fi
```

#### shorter if statements

To check for flags in a short way you could do this:
```
#!/bin/bash

my_flag=1

[ ${my_flag} -eq 1 ] && echo "It's true!"

exit 0
```

or maybe this is more readable:
```
[ ${my_flag} -eq 1 ] \
        && echo "It's true!"
```

#### check if variable is set

```
[[ -v myvar ]] && echo "variable has been assigned a value"
```
When you do
```
unset myvar
```
it will not evaluate to true anymore

### strings

#### check length

From `man bash` Sektion `CONDITIONAL EXPRESSIONS`:
```
-z string
True if the length of string is zero.

-n string
True if the length of string is non-zero.
```

#### compare strings
```
if [[ "$hostname" != "examplemachine" ]]; then
  printf "You are on the wrong machine.\n"
  exit 1
fi
```
