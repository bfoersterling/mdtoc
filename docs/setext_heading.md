It seems that three or more `-` below text are header level 2.\
Three or more `=` below text are header level 1.

The `-`s can also be a thematic break, if there is no text on the preceding line.

So you would call `---+` a "dashed_line" and `===+` a "double dashed line".

But the dashed line in a thematic break differs from a setext heading underline.\
Because a thematic break may contain internal spaces.

From spec.txt:
```
The setext heading underline cannot contain internal spaces or tabs:
1459
1460 ```````````````````````````````` example
1461 Foo
1462 = =
1463
1464 Foo
1465 --- -
1466 .
1467 <p>Foo
1468 = =</p>
1469 <p>Foo</p>
1470 <hr />
1471 ````````````````````````````````
```
