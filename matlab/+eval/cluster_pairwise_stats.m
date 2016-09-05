function stats = cluster_pairwise_stats(ids, clabels, gen_confusionmats)
%% Calculate pair-wise stats for clustering assignments.
% INPUT
% ids = cluster ID (i.e., cluster assignment)
% clabels = true class labels
%
% OUTPUT
% stats is a structure array
% stats.confusionMat
% 
% Note, all metrics are pairwise (i.e., counting pairs)
% 
%               Predicted Classes
%                    p'    n'
%              ___|_____|_____| 
%       Actual  p |     |     |
%      Classes  n |     |     |
%
% 
% stats.precision = TP / (TP + FP)                  for each class label
% stats.recall = TP / (TP + FN)                     for each class label
% stats.specificity = TN / (FP + TN)                for each class label
% stats.fscore = 2*TP /(2*TP + FP + FN)             for each class label
% 
%
% TP: true positive, TN: true negative, 
% FP: false positive, FN: false negative
% 
% Confusion Matrices:
% stats.cluster_confusion  -   confusion matrix sorted by cluster
% stats.class_confusion   -   confusion matrix sorted by classes
% 
% Settings/ Params:
% stats.nclasses                                    No. of true classes
% stats.clabels                                     true class labels
% stats.k                                           No. of clusters
% stats.ids                                         cluster assignments
% 
% @todo 
% - add additional pairwise metric
%   e.g., stats.random_index = (TP + TN)/(TP + FP + FN + TN) avg. accuracy 
%% 
% @author Joseph P. Robinson
% @date 2016 July 25
%%


if nargin < 3
   gen_confusionmats = false; 
end


%% pair-wise precision
stats.precision = eval.pairwise_precision(ids,clabels);

%% pair-wise recall
stats.recall = eval.pairwise_recall(ids,clabels);

%% pair-wise recall
stats.specificity = eval.pairwise_specificity(ids,clabels);

%% Fscore 
ar = stats.recall.AR;    ap = stats.precision.AP;
stats.fscore= (2*ar*ap)/(ar+ap);

%% number of true classes
stats.nclasses = length(unique(clabels));
%% k
stats.k = length(unique(ids));
stats.ids = ids;
stats.clabels = clabels;

if gen_confusionmats
    stats.cluster_confusion = ...
        visualize.cluster_confusion(cluster_ids,clabels);
    stats.class_confusion = ...
        visualize.class_confusion(cluster_ids,clabels);
else
   stats.cluster_confusion = [];
   stats.class_confusion = [];
end
end