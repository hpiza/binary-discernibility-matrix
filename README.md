## An Incremental DFA-Based Algorithm for Simplifying the Binary Discernibility Matrix

This repository provides an ANSI C implementation of an incremental algorithm that can be used to create a boolean matrix known as Binary Discernibility Matrix (BDM) from a Training Matrix (also referred as learning matrix).

The input matrix is given in the form of a comma-separated values (csv) file, such that the first row contains the name of the columns. The output matrix is saved to a text file which name matches the input file, but the extension is mb. This file starts with an integer value R denoting the number of rows of the BDM, followed by a second integer value C denoting the number of rows. The next R rows contains each a space-separated binary string of length C.

### Installing

1. Clone the repository to your local (or download all files). The source code includes the following files:<br> `main.c, prints.c, prints.h, types.h`.

2. Make sure you have a development environment that provides a port of the GNU compiler collection (gcc), like MinGW, Cygwin, clang, MSVC, among others.

3. Compile the source files:<br> `gcc -o createBDM.exe prints.c main.c`<br>

_Note_: replace `createBDM.exe` with whatever name and extension you wish for the executable file.

### Running

1. If running on Windows, just type the name of the file (without extension) and then enter or double-click on the icon under Windows explorer. If running on Linux or Mac, just type `./` before the name of the executable file.

2. This is the main menu of the program:
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
3. First of all, you need to load one or more datasets, i.e. csv files. Select option 1 and then press enter. The following menu will appear:
```
Load dataset file(s)
------------------------------------
[1] File
[2] Directory
[3] Return
Select source: _
```
4. You can either load a single file or all the csv files inside a given subfolder (inside the directory where the program is running).
```
Select source: 1
Enter file name: medium/mushroom.csv
File 'medium/mushroom.csv' loaded successfully
```
```
Select source: 2
Enter directory name: large
File 'large/adult.csv' loaded successfully
File 'large/bank-marketing.csv' loaded successfully
File 'large/product-classification-and-clustering.csv' loaded successfully
File 'large/secondary-mushroom.csv' loaded successfully
```
_Note:_ Everytime you load a file or a directory, the previously loaded dataset(s) will be replaced by the recently loaded one(s).

5. Once you have loaded a dataset, you can proceed to run any of the 4 available algorithms:
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
Select option: 2
====================================
Incremental algorithm for Reducts
Processing assesing-mathematics-learning-in-higher-education - ended
Processing mushroom - ended
Processing room-occupancy-estimation - ended
Processing taiwanese-bankruptcy-prediction - ended
------------------------------------
```
6. After the executions have ended, you will see that folder *bol* now contains a _mb_ file for each _csv_ file that was processed by the algorithm. The *mb* file is the corresponding __Binary Discernibility Matrix__. Make sure this folder exists before running any algorithm.

7. In addition, you will see that a file called __log.csv__ was created (if it didn't previously exist). This file contains the logs of execution, which include the time in seconds that took the algorithm to process each dataset.
