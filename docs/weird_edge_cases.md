#### headings inside block quotes

`cmark` will convert this
```
> #### ha this is a quote AND a header!
```
to this:
```
<blockquote>
<h4>ha this is a quote AND a header!</h4>
</blockquote>
```

Should these headers be filtered out by `mdtoc`?\
So that only top level headers will be parsed?

The AST of this:
```
## top level header

> #### ha this is a quote AND a header!

## another top level header
```
looks like this:
```
document [1, 1] - [5, 27]
  heading [1, 1] - [1, 19]
    text [1, 4] - [1, 19]
  block_quote [3, 1] - [3, 39]
    heading [3, 3] - [3, 39]
      text [3, 8] - [3, 39]
  heading [5, 1] - [5, 27]
    text [5, 4] - [5, 27]
```

So maybe only parse headings directly under the `document` node?

When traversing the ast you can probably skip going to the child node \
when the current node is a `block_quote`.\
Should not be too hard, but these edge cases should be treated at a later \
development stage.
