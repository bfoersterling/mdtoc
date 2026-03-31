#### first draft

1. parse headings
1. build chapter struct out of headings tree

```
struct chapter {
    struct heading* title;
    int start_line;
    int end_line;
    const char* body;
    struct chapter* next;
    struct chapter* first_child;
    struct chapter* parent;
};
```

You will need a tree structure similar to the `heading` struct.\
This tree needs exactly one root.\
This root is the parent of all chapters.\
Its "title" should be the root of the `heading` struct.

OR

Without a new data structure.\
Search for regex in the AST.\
On a match search backwards for the last heading.

To keep `mdtoc` extensible in the future you should go for the data structure \
approach.
