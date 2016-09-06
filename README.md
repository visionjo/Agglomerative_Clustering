STR Hierarchical Agglomerative Clustering (STR-HAC) library
============================================================
MATLAB and C++ tools for agglomerative clustering.

ROJECT INFO
============

Systems & Technology Research
Janus Program <janus@stresearch.com>

Version: 1.0
Date:    6 September 2016
Author: Joseph Robinson
Email: robinson.jo@husky.neu.edu

OVERVIEW
============

STR-HAC is a set of tools developed to run large scale agglomerative clustering.
Software package is in MATLAB and C++. Upon reading this README, the organization of package
should be clear; else, please contact me with any questions (in which case this will likely 
trigger reason to update this README to clarify any items that were found to be unclear).

REQUIREMENTS
============
Development was done on Mac OS X El Capitan (Version 10.11.6), but should work on any Unix or Linux based system with the following requirements satisfied:

- MATLAB 2015b (or newer)
- CMake 2.8 (or newer)
- OpenCV 2.4.11 (or newer)

INSTALL
============
COMPILING STR-HAC C++ PROGRAM
Go into the project folder and type:
>  cd cpp; mkdir build; cd build; cmake .. ; cmake --build .

NOTE: It is required that project is make is done in build directory, as MATLAB demos assume this is the location of C++ executables.

UNIT TEST
=============


PACKAGE CONTENTS
================
The organization of this package is as follows:

~~~
.
├── cpp
└── matlab
    ├── +eval
    ├── demo
    ├── rankorder
    ├── utils
    └── +visualize
~~~

cpp/    contains all C++ source code, along with CMakeLists.txt. See above for installation instructions

matlab/ contains several functions for agglomerative clustering. Furthermore, the demos show how to use the C++ tool in conjunction with that of the MATLAB

C++ Tools provide functionality for agglomerative clustering. In other words, input for the C++ program must be a filepointer to data either as a distance matrix or a set of tuples. In either case, the file must be CSV format.

MATLAB tools span the whole pipeline. In other words, it provides functions to build KD-Trees, determine Rank-Order distances, and do agglomerative clustering. The feature implemented in C++ but not in MATLAB is merging by a single-link criteria, as MATLAB only provides functionality to do single-pass clustering (and C++ provides functionality for both single-pass and greedy single-link).

See MATLAB demos for examples of how to interface with package. 



PROGRAM OPTIONS
================
As --help would display:

> ************************************************************************
> Agglomerative Clustering C++ Tools
> 
>
> Usage:
>
> /Users/jrob/Agglomerative_Clustering/Agglomerative_Clustering/build/Debug/Agglomerative_Clustering [OPTION...]
> 
> Requirements options:
>
> -i, --ifile     FILE          Input CSV file containing distance matrix
>
> -o, --ofile     FILE          Output filepath
> 
>
> Optional options:
>
> -l, --ltype     INT     Merge Criteria (0=complete-link; 1=single-link; 2=single-pass) (default: 1)
>
> -f, --format    FILE FORMAT  Format of input file (tuple (or T) or dmatrix (or D)) (default: T)
>
> -k, --nclusters INT     Number of clusters to predict (default: 2)
>
> -e, --eps       DOUBLE  Threshold for single-pass clustering (default: 2.6)
>
> -h, --help              Print help
>
> ************************************************************************
> 
>
> Sample command line call:
>
> ./Agglomerative_Clustering -i <DATADIR>/distance_matrix.csv -o <OUTDIR>cluster_ids.txt -l 1 -k 4
> 
>
> The usage exemplified above would generate 4 clusters for 'distance_matrix.csv' using single-linkage criteria and dumping results (i.e., cluster ids) in 'cluster_ids.txt'.
> 
>
> Equivalently:
>
> ./Agglomerative_Clustering -ifile <DATADIR>/distance_matrix.csv -ofile <OUTDIR>cluster_ids.txt -ltype 1 -nclusters 4
> 
>
> Or:
>
> ./Agglomerative_Clustering -i <DATADIR>/distance_matrix.csv -d <OUTBIN> -o cluster_ids.txt -l 1 -f D -k 4
>
> 
>
> If the data file (i.e., input file) is formatted as a set of tuples, then executable is called as follows:
>
> ./Agglomerative_Clustering -i <DATADIR>/tuples.csv -d <OUTBIN> -o cluster_ids.txt -l 1 -f T -k 4
>
>
>
> Note, command line arguments can be passed in in any order. However, -i and -o (i/o) args are required; else, the program will print error and terminate.
> 
> ************************************************************************
