Program:    STR JANUS Project: Face Clustering Toolbox
Author:     Joseph P. Robinson
Email:      joseph.robinson@stresearch.com
Created:    07/23/2016

TO DO: 
    - Finish README.txt (this)
    - Visualization tools
            - Per cluster and sample confusion matrices
            - Histogram of class IDs for each cluster?
            - Pie charts?
            - How to visualize history (i.e., critical merges, whether for the better of the worse)

OVERVIEW: 
    MATLAB tools designed to run, evaluate, and visualize clustering methods. This effort is a part of the JANUS Project (i.e., face data), however, these tools can be used on feauture vectors of any kind.
         
 MOTIVATION:
    Investigate clustering methods on templates (i.e., subjects) encoded using JANUS-TNE codebase. 


RELATED WORKS:
       
CONTENTS: 

    setup_med.m
        Configures workspace for MED system runs

Importent directories:
    +eval/
        Cluster evaluation tools (i.e., get cluster stats)
    +visualize/
        Cluster visualization tools.
   


REVISION HISTORY:

	7/23/2016:  Started Project
    7/26/2016:  Rank-Order Clustering implemented and added to tools 
    7/26/2016:  Evaluation tools (+eval/) created
                            - Pairwise precision                                        (eval.pairwise_precision())
                            - Pairwise recall                                           (eval.pairwise_recall())
                            - Wrapper function to compile all stats for a given run     (eval.cluster_pairwise_stats())

    7/27/2016:  Visualization tools (+visualization/) created                        
                            - Confusion matrix (sorted by cluster)                      (visualize.cluster_confusion())

    7/28/2016:  Visualization tool:
                            - Confusion matrix (sorted by class)                        (visualize.class_confusion())
                Evaluation tool:
                            - Pairwise specificity                                      (eval.pairwise_specificity())

                Updated eval.cluster_pairwise_stats():
                            - (optionally) add confusion matrices to (returned) structure 'stats'
                            - call specificity
                            - detailed comments specifying math used to generate stats
                Added function to setup MATLAB environment                              (setup.m)
    


3rd Party Dependencies
In order to compile this software the following libraries are needed:
* VL_FEAT library (https://github.com/vlfeat/vlfeat, tested with v9.20)
    - USAGE: vl_gmm