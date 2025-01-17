John Gruber is the creator of Markdown.\
He describes the syntax here:\
https://daringfireball.net/projects/markdown/syntax

#### headers

https://daringfireball.net/projects/markdown/syntax#header

> Atx-style headers use 1-6 hash characters at the start \
of the line, corresponding to header levels 1-6.

=> that means header level 6 is the maximum level (vim and bat also do not highlight \
level 7 headings)

But there are also "Setext-style headers" that can only create \
level 1 and 2 headers.

> Any number of underlining =’s or -’s will work.

`bat` does not detect "setext" at all.\
`vim` interprets a line that is followed by a line with at least one \
`=` or `-` as a header. (that line may not include any other characters than \
`-` or `=` and they may not be mixed)
