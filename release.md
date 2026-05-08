#### how to publish a new release?

Build the new binary:
```
make
```

Create a new tag and increment the semantic version accordingly:
```
git tag vX.X.X
```

Create new release:
```
gh release create vX.X.X
```

Upload new binary:
```
gh release upload vX.X.X my_binary
```
