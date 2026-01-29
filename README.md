## An Incremental DFA-Based Algorithm for Simplifying the Binary Discernibility Matrix

This repository provides an ANSI C implementation of an incremental algorithm that can be used to create a boolean matrix known as Binary Discernibility Matrix (BDM) from a Training Matrix (also referred as learning matrix).

The input matrix is given in the form of a comma-separated values (csv) file, such that the first row contains the name of the columns. The output matrix is saved to a text file which name matches the input file, but the extension is mb. This file starts with an integer value R denoting the number of rows of the BDM, followed by a second integer value C denoting the number of rows. The next R rows contains each a space-separated binary string of length C.

### Installing

1. Clone the repository to your local (or download all files). The source code includes the following files:<br> `main.c, prints.c, prints.h, types.h`.

2. Make sure you have a development environment that provides a port of the GNU compiler collection (gcc), like MinGW, Cygwin, clang, MSVC, among others.

3. Compile the source files:<br>

`gcc -o createBDM.exe prints.c main.c`<br>

_Note_: replace `createBDM.exe` with whatever name and extension you wish for the executable file.

### Running

1. If running on Windows, just type the name of the file (without extension) and then enter or double-click on the icon under Windows explorer. If running on Linux or Mac, just type `./` before the name of the executable file.

```
====================================
MAIN MENU
------------------------------------
[1] Load dataset file(s)
[2] Incremental algorithm for reducts
[3] Original algorithm for reducts
[4] Incremental algorithm for constructs
[5] Original algorithm for constructs
[6] Exit
Select option: 1
====================================
```

\## Authors

Contributors names and contact info

ex. Dominique Pizzie

ex. \[@DomPizzie](https://twitter.com/dompizzie)



\## Version History



\* 0.2

    \* Various bug fixes and optimizations

    \* See \[commit change]() or See \[release history]()

\* 0.1

    \* Initial Release



\## License



This project is licensed under the \[NAME HERE] License - see the LICENSE.md file for details



\## Acknowledgments



Inspiration, code snippets, etc.

\* \[awesome-readme](https://github.com/matiassingers/awesome-readme)

\* \[PurpleBooth](https://gist.github.com/PurpleBooth/109311bb0361f32d87a2)

\* \[dbader](https://github.com/dbader/readme-template)

\* \[zenorocha](https://gist.github.com/zenorocha/4526327)

\* \[fvcproductions](https://gist.github.com/fvcproductions/1bfc2d4aecb01a834b46)

