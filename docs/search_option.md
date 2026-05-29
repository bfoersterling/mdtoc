#### Search File For String/Pattern

You can search the file case insensitive for a string with `-s/--search` \
and `s[earch]` in interactive mode.

The output will show all chapters that have this string in its `title` \
or in its `body`.\
Currently all ancesters of the chapter will be shown as well, as they \
have all their descendants in their body.\
Since this is visually obvious through the indentation it looks like a \
clean solution.

###### Regex?

Do you really need/want to search for a regex?

Or is the default case insensitive search for a string in chapters and \
headings enough?

What would be the use case for a regex search?\
Something like `^foobar$` does not seem that useful except if you want \
to match an exact heading.\
Maybe if you want to search for two strings you could to something like this:\
`foo.*bar`.
