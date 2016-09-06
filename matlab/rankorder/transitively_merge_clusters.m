%% Threshold rank-order distance matrix.
% Transitively step through matrix, merging each pairs w distances below
% threshold Eps into same cluster.
% 
% Provided constraint matrix C, 'must-' and
% 
% @param D   - Rank order distance matrix
% @param Eps - Rank order distance threshold (i.e., epsilon) [default 1.6]
% @param C   - Constraint matrix [default zeros(size(D))]
% 
% @return cluster_tags   - Cluster ID assignments
% 
% @author Joseph P. Robinson
% @date 2016 August 11
%
% SEE RANK_ORDER.M CALCULATE_RANK_ORDER_DISTANCE.M
%% 

function cluster_tags = transitively_merge_clusters(D,Eps,C)


if nargin < 3
   C = zeros(size(D)); 
end
nsamples = size(D,1);


% cluster ids -- initialize all samples in own cluster
cluster_tags = 1:nsamples;      
%% Determine distances that follow below threshold AND are not constrained 
% according to matrix C as 'cannot link' (i.e., c(i,j) ~= -1) OR matrix C
% forces a merge via a reference to 'must link' (i.e., c(i,j) == 1)
to_merge = (D<Eps & C ~= -1) | C == 1;
for x = 1:(nsamples-1)
    %% for each element of upper triangle of symmetric distance matrix
%     distances = D(x,(x+1):end);
%     to_merge = distances < Eps;
    c_to_merge = to_merge(x,(x+1):end);
    c_tag = min([cluster_tags(x) cluster_tags(x+1:end & c_to_merge)]); 
    cluster_tags (x+1:end & c_to_merge) = c_tag;
end

%% set cluster ids to span 1:k

% profile viewer
end

