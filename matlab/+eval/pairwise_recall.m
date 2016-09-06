%%
% Determines pair-wise recall of the ith cluster w.r.t. clabels, i.e.,
% ground-truth is referenced to determine the observations from the same
% class (identity) and, hence, should be clustered together.
%
%
function stats = pairwise_recall(ids,clabels)

% get list of cluster IDs
bins = unique(ids);
k = length(bins);   % number of clusters

% determine count of number of samples for each class, i.e., how many
% should be assigned to cluster w reference to its class labels.
nsamps_per_cat=hist(clabels,1:length(unique(clabels)));
tsample_count = sum(nsamps_per_cat);   % total number of observations

if k == 1
    %% etreme case where all samples are assigned to a single cluster
    stats.cluster_info.r_e = 1;
    stats.cluster_info.tnpairs = tsample_count;
    stats.cluster_info.recall = 1;
    stats.AR = 1;
    return;
end


binned_gt = cell(1,k);

for x = 1:k
    %% for each cluster
    % index items assigned to kth cluster
    binned_inds = ids == bins(x);   
    % determine true labels for items
    binned_gt{x} = sort(clabels(binned_inds));
end
cluster_info = cell(1,k);
for x =1:k
    %% for k clusters
    cbin = binned_gt{x};
    
    % current cluster
    cats = unique(cbin);
    ncats = length(cats);
   
    
    r_e = zeros(1,ncats);    
    for y = 1:ncats
        %% for each class assigned to current cluster
        % determine true positive
        nmatches = length(find(cats(y) == cbin));
        % false negatives (i.e., instances of class in other clusters)
        %         misses = length(find(cats(y) == other_bins));
        
        % number of pairs of class considering all clusters
        %         npairs = nchoosek((matches + misses),2);
        
        ntotal = length(find(cats(y) == [binned_gt{:}]));
        if nmatches > 1
            npairs = nchoosek(nmatches,2);
        elseif ntotal == 1
            r_e(y) = ntotal;
            continue;
        else
            npairs = 1;
        end

        %         others = cats([1:nsamps]>y);
        %         r_e(y) = npairs/tnpairs;
        r_e(y) = npairs/nchoosek(ntotal,2);
        %
    end
    cluster_info{x}.r_e = r_e;
    cluster_info{x}.recall = mean(r_e);
end

% recall
stats.cluster_info = cell2mat(cluster_info);
stats.AR = mean([stats.cluster_info.recall]);
end