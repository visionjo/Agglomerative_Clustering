%%
% Determines pair-wise precision of the ith cluster w.r.t. clabels, i.e.,
% ground-truth is referenced to determine the observations from the same
% class (identity) and, hence, should be clustered together.
%
% stats.precision = TP / (TP + FP)                  for each class label
%
function stats = pairwise_precision(ids,clabels)

%%
bins = unique(ids);
k = length(bins);

cluster_info = cell(1,k);
for x =1:k
    %% for each cluster
    % index items assigned to kth cluster
    binned_inds = ids == bins(x);
    % determine true labels for items
    binned_gt = sort(clabels(binned_inds));
    
    % sample ids assigned
    cats = unique(binned_gt);
    % No. class types present
    ncats = length(cats);
    % No. of samples 
    nsamps = length(binned_gt);

    if nsamps == 1
        cluster_info{x}.p_e = 1;
%         cluster_info(x).tnpairs = 0;
        cluster_info{x}.precision = 1;
        continue;
    end
    
    p_e = zeros(1,ncats);
    % number of possible pairs
    tnpairs = nchoosek(nsamps,2);
    
    for y = 1:ncats
        %% for each class present in cluster x
        nmatches = length(find(cats(y) == binned_gt));
        
        if nmatches < 2, npairs = 1; % only one pair
        else npairs = nchoosek(nmatches,2);
        end
        % pair count for yth class over the total number of possible pairs.
        p_e(y) = npairs/tnpairs;
    end
    cluster_info{x}.p_e = p_e;
%     cluster_info(x).tnpairs = npairs;
    cluster_info{x}.precision = mean(p_e);
end

stats.cluster_info = cell2mat(cluster_info);
stats.AP = mean([stats.cluster_info.precision]);
end