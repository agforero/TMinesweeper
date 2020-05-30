# TMinesweeper
Terminal-based Minesweeper program, written in C++.

This program requires the use of the `g++` compiler: 
* On UNIX systems, run `g++ --version` to check for `g++`'s presence.
* On MacOS, run `sudo apt get g++` to install the MacOS developer tools, including `git`, `make` and `g++`. You can check for the presence of these commands by running `<command> --version`.
* On Windows systems, instructions can be found [here](https://www3.cs.stonybrook.edu/~alee/g++/g++.html), or [here](https://www.youtube.com/watch?v=lqzuR2USKRM) if you prefer video format.

To compile: 
* On UNIX and MacOS systems, run `make mine` to build the executable. 
* On Windows, either run `setup.bat`, or from a PowerShell `cd`'d into the directory, run `./setup`.

Then, run `./mine h w p`, where
* `h` is height,
* `w` is width, and 
* `p` is percentage of the board covered in mines.

Instructions on how to play provided in the executable. 

Icon created using asset from [toicon](https://www.toicon.com/)'s open icon repository.
