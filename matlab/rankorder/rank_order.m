%% Rank-order clustering implementation.
% Transitively merge all pairs with distances below threshold using
% rank-order (RO) distance defined in [1].
%
% $$d_m(a,b)=\sum_{i=0}^{min(O_a(b),k)} I_b(O_b(f_a(i)),k)$$
%
% $$D(a,b)=\frac{d_m(a,b) + d_m(b,a)}{min(O_a(b),O_b(a))}$$
% where $I_b$ is indicator fuction: 0 if NN is shared; else, 1.
%
%
% @param nn_ids - Nxk matrix of the indices of k-NN for N samples
% (OPTIONAL)
% @param Eps    - RO distance threshold (i.e., epsilon) [default 1.6]
% @param D      - RO distance matrix
%
% @return cluster_ids   - Cluster ID assignments
%
% Example Calls:
%   Input KD-Tree, generate cluster assignments (IDs) & distance matrix D.
%   Clustering determined is based on threshold set to default (1.6)
%       [cluster_ids, D] = rank_order_new(nn_ids)
% 
%   Input KD-Tree + Eps, generate cluster assignments (IDs) & distance 
%   matrix D.
%       [cluster_ids, D] = rank_order_new(nn_ids, Eps)
% 
%   Input KD-Tree + Eps + D, generate cluster assignments (IDs). 
%       [cluster_ids, D] = rank_order_new(nn_ids, Eps, D)
% 
%   Input KD-Tree + Eps + D + constraint matrix C, generate cluster 
% assignments (IDs), while constraining 'must-' (1) & 'cannot' (-1) links.
%       [cluster_ids, D] = rank_order_new(nn_ids, Eps, C)
% 
% @author Joseph P. Robinson
% @date 2016 July 25
%
% [1] Otto, Charles, Dayong Wang, and Anil K. Jain. "Clustering millions
% of faces by identity." arXiv preprint arXiv:1604.00989 (2016).
%%
function [cluster_ids, D] = rank_order(nn_ids,Eps,D, C)

if nargin == 0
    ME = MException(['Error: input arg required.\n\t'...
        'nn_ids - Nxk matrix of the indices of k-NN for N samples']);
    throw(ME)
elseif nargin < 3
    % default vals
    if ~exist('Eps','var'), Eps = 1.6; end
    D = calculate_rank_order_distance(nn_ids);
end

if exist('C','var')
    cluster_ids = transitively_merge_clusters(D,Eps,C);
else
    cluster_ids = transitively_merge_clusters(D,Eps);
end
cluster_ids = set_class_ids(cluster_ids);
