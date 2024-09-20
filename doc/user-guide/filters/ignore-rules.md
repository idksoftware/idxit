# Ignore Rules

Here are the general rules for matching patterns in `ignore` files: 

| Pattern | Explanation/Matches | Examples |
| --- | --- | --- |
|     | Blank lines are ignored |     |
| # *text comment* | Lines starting with # are ignored |     |
| \*:/windows/ | Absolute path. Ending with / specifies the pattern is for a folder. | \*:/windows/ |
| *name* | All *name* files, *name* folders, and files and folders in any *name* folder | /name.log  <br>/name/file.txt  <br>/lib/name.log |
| *name*/ | Ending with / specifies the pattern is for a folder. Matches all files and folders in any *name* folder | /name/file.txt  <br>/name/log/name.log<br><br>**no match:**  <br>/name.log |
| *name*.*file* | All files with the *name.file* | /name.file  <br>/lib/name.file |
| */name*.*file* | Starting with / specifies the pattern matches only files in the root folder | /name.file<br><br>**no match:**  <br>/lib/name.file |
| *lib/name*.*file* | Patterns specifiing files in specific folders are always realative to root (even if you do not start with / ) | /lib/name.file<br><br>**no match:**  <br>name.file  <br>/test/lib/name.file |
| \*\**/lib/name.file* | Starting with \*\* before / specifies that it matches any folder in the repository. Not just on root. | /lib/name.file  <br>/test/lib/name.file |
| \*\**/name* | All *name* folders, and files and folders in any *name* folder | /name/log.file  <br>/lib/name/log.file  <br>/name/lib/log.file |
| /lib/\*\**/name* | All *name* folders, and files and folders in any *name* folder within the lib folder. | /lib/name/log.file  <br>/lib/test/name/log.file  <br>/lib/test/ver1/name/log.file<br><br>**no match:**  <br>/name/log.file |
| \*.*file* | All files withe *.file* extention | /name.file  <br>/lib/name.file |
| \**name*/ | All folders ending with *name* | /lastname/log.file  <br>/firstname/log.file |
| *name*?.*file* | ? matches a **single** non-specific character | /names.file  <br>/name1.file<br><br>**no match:**  <br>/names1.file |
| *name*\[a-z\].*file* | \[*range*\] matches a **single** character in the specified range (in this case a character in the range of a-z, and also be numberic.) | /names.file  <br>/nameb.file<br><br>**no match:**  <br>/name1.file |
| *name*\[abc\].*file* | \[*set*\] matches a **single** character in the specified set of characters (in this case either a, b, or c) | /namea.file  <br>/nameb.file<br><br>**no match:**  <br>/names.file |
| *name*\[!abc\].*file* | \[!*set*\] matches a **single** character, **except** the ones spesified in the set of characters (in this case a, b, or c) | /names.file  <br>/namex.file<br><br>**no match:**  <br>/namesb.file |
| *name*/  <br>!*name*/secret.log | ! specifies a negation or exception. Matches all files and folders in any *name* folder, except name/secret.log | /name/file.txt  <br>/name/log/name.log<br><br>**no match:**  <br>/name/secret.log |

* * *

## Local and Personal Git Ignore Rules

It is also possible to ignore files or folders but not show it in the distubuted `.gitignore` file.

These kinds of ignores are specified in the `.git/info/exclude` file. It works the same way as `.gitignore` but are not shown to anyone else.