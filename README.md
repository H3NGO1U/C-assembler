# C-assembler
## *Final Project* in course 20465, the Open University
> During this project we built an assembler for 32-base **imaginary** "computer language".<br>
> The assembler runs on linux environment.

## The assembler works in the following order:
1. Receives one or more names of files (.as extension) and expands the macros in it (if there are any).
2. Creates new files (type .am) with macros expanded.
3. Goes through the new files, and translates the commands to the imaginary language.
4. During the translation, if any errors occured, prints to the console the information about it.
5. If no errors detected, creates .ob file with the translated code.
6. Also, if were any .entry or .extern instruction, creates .ent file or .ext file respectively.


