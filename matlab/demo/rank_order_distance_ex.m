%% Rank-order distance calculation demo.
% Rank-order (RO) distances are calculated as defined in [1].
%
% $$d_m(a,b)=\sum_{i=0}^{min(O_a(b),k)} I_b(O_b(f_a(i)),k)$$
%
% $$D(a,b)=\frac{d_m(a,b) + d_m(b,a)}{min(O_a(b),O_b(a))}$$
% where $I_b$ is indicator fuction: 0 if NN is shared; else, 1.
%
%
%%
% 
% This script demonstrates the following:
% # Finding KD-Trees for a set of observations.
% # Building RO distance matrix D based on KD-Trees (i.e., NN lists).
% # Writing matrix D as tuples and compatible with c++ implementation
% 
% 
% @author Joseph P. Robinson
% @date 2016 September 3
%
% [1] Otto, Charles, Dayong Wang, and Anil K. Jain. "Clustering millions
% of faces by identity." arXiv preprint arXiv:1604.00989 (2016).
%%
setup();
%% Load CS3 Encodings
% data = load('~/WORK/janus/sandbox/jrobinson/matlab/clustering/MATs/sub50_100max/subset_encodings.mat');
% X = data.Xsub;
% gt = data.labs;.
addpath ..;
do_kdtree = false;
f_kdtree = '~/WORK/janus/sandbox/jrobinson/matlab/clustering/MATs/KDTrees/kdtree_100.mat';
load_kdtree = true ...
    && exist(f_kdtree,'file');

% f_data = '~/WORK/janus/sandbox/jrobinson/matlab/clustering/MATs/normalized_centralized_cs3_subset.mat';
f_data = '~/WORK/janus/sandbox/jrobinson/matlab/data/CS3/all_sightings_normalized.mat';

% Eps = 2;   
% k=[10 20 50 100 200];        
% k = 300;
k = 100;    % number of NN for each sample (i.e., size of KDTree)

if (do_kdtree || ~exist(f_kdtree,'file'))
    fprintf(1,'\nPreparing KDTree (k = %d)\n',k(end));
    kdtrees = build_kdtrees(f_data, k(end));
    ids_mat = cat(1,kdtrees{:});
    fprintf(1,'\nSaving KDTree: \n\t\t%s\n',f_kdtree);
    save(f_kdtree,'ids_mat');
    load_kdtree = false;
end

if (load_kdtree)
    fprintf(1,'\nLoading KDTree: \n\t\t%s\n',f_kdtree);
    load(f_kdtree);
end
% k = min(k,size(ids_mat,2));

rbin = 'MATs/all_sightings/rankordering/run_k200/';
if ~exist(rbin,'dir')
    mkdir (rbin);
end

%% Calculate RO Distance Matrix.
D = calculate_rank_order_distance(nn_ids);
return;
Eps = 1.6;      % distance threshold to compare rank order distance to

% Eps = [0.05:.05:.45 ];%[.5:.05:.95 1:.05:2 2.1:.1:3 4:10];% [1:.04:2];% 2.1:.1:3 4:10];
% % Eps = [Eps 1.5:.01:1.7];
% Eps = [.5:.05:.95 Eps 1:.04:2 2.1:.1:3 4:10];

% for y = 1:length(k)
%     obin = strcat(rbin, 'results_k%d_Eps%s.mat');
obin = strcat(rbin, 'results_Eps%s.mat');
    for x = 1:length(Eps)
        fout = sprintf(obin, strrep(num2str(Eps(x)),'.','_'));
        if exist(fout, 'file'), continue;   end
        fprintf(1,'\nRank Order Cluster (Eps = %s)\n\n',num2str(Eps(x)));
        display(fout);
        [cluster_ids, D] = rank_order(ids_mat(:,1:k),Eps(x));
        
        save(fout,'cluster_ids','D');
    end
% end

