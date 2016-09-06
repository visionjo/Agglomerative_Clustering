# Agglomerative_Clustering
MATLAB and C++ tools for agglomerative clustering.


************************************************************************
Agglomerative Clustering C++ Tools
Usage:
/Users/jrob/Agglomerative_Clustering/Agglomerative_Clustering/build/Debug/Agglomerative_Clustering [OPTION...]

Requirements options:
-i, --ifile     FILE          Input CSV file containing distance matrix
-f, --format     FILE FORMAT  Format of input file (tuple (or T) or dmatrix
(or D)) (default: T)
-o, --ofile     FILE          Output filepath

Optional options:
-l, --ltype     INT     Merge Criteria (0=complete-link; 1=single-link;
2=single-pass) (default: 1)
-k, --nclusters INT     Number of clusters to predict (default: 2)
-e, --eps       DOUBLE  Threshold for single-pass clustering (default: 2.6)
-h, --help              Print help
************************************************************************

Sample command line call:
/Users/jrob/Agglomerative_Clustering/Agglomerative_Clustering/build/Debug/Agglomerative_Clustering -i <DATADIR>/distance_matrix.csv -o <OUTDIR>cluster_ids.txt -l 1 -k 4

The usage exemplified above would generate 4 clusters for 'distance_matrix.csv' using single-linkage criteria and dumping results (i.e., cluster ids) in 'cluster_ids.txt'.

Equivalently:
/Users/jrob/Agglomerative_Clustering/Agglomerative_Clustering/build/Debug/Agglomerative_Clustering -ifile <DATADIR>/distance_matrix.csv -ofile <OUTDIR>cluster_ids.txt -ltype 1 -nclusters 4

Also equivalent:
/Users/jrob/Agglomerative_Clustering/Agglomerative_Clustering/build/Debug/Agglomerative_Clustering -i <DATADIR>/distance_matrix.csv -d <OUTBIN> -o cluster_ids.txt -l 1 -f D -k 4

-i <DATADIR>/tuples.csv -d <OUTBIN> -o cluster_ids.txt -l 1 -f T -k 4

Note, command line arguments can be passed in in any order. However, flags are required; else, the input argument will be ignored

************************************************************************