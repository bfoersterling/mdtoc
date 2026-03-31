#### search file for regex

You could create a chapter tree out of the heading tree.\
Then search this chapter tree for a regex.

Or just make a function that searches the file for a regex.\
Store the line number, then search in the heading tree which heading \
has the biggest line number that is smaller than the line number the regex \
was found in.\
Then display that heading with its text and line field.
