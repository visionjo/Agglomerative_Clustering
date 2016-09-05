%% Calculate pairwise specificity for clustering
%
%
% ids           - cluster labels (i.e., assignment)
% clabels       - ground truth (i.e., class label)
%
% specificity = TN / (FP + TN)  
% 
% @author Joseph P. Robinson
% @date 2016 July 25
%%
function stats = pairwise_specificity(ids,clabels)

%%
bins = unique(ids);
k = length(bins);
binned_gt = cell(1,k);
nsamples = length(clabels);
for x = 1:k
    %% for each cluster
    % index items assigned to kth cluster
    binned_inds = ids == bins(x);
    % determine true labels for items
    binned_gt{x} = sort(clabels(binned_inds));
end
cluster_info = cell(1,k);
%  = ;
for x =1:k
    %% Current cluster:
    % sample ids assigned 
    cats = unique(binned_gt{x});
    % No. class types present
    ncats = length(cats);
    
    % No. of samples not in cluster
    nsamps = nsamples - length(binned_gt{x});

    s_e = zeros(1,ncats);
    bin_flag = ones(1,k); bin_flag(x) = 0;
    other_bins = [binned_gt{bin_flag == 1}];
    % number of possible pairs
%     fnpairs = nchoosek(nsamps,2);
    
    for y = 1:ncats
        %% for each class present in cluster x
        % get true negative (different cluster, different class)
        tn = length(find(cats(y) ~= other_bins));
        % get false positives (same cluster, different class)
        fp = length(find(cats(y) ~= binned_gt{x}));
        
        if tn < 2, tnpairs = 0; s_e(y) = 0; continue;% only one means no pairs
        else tnpairs = nchoosek(tn,2);
        end
%         
%         if fp < 2, fppairs = 0; % only one means no pairs
%         else fppairs = nchoosek(fp,2);
%         end
        % pair count for yth class over the total number of possible pairs.
        s_e(y) = tnpairs/nchoosek(tn + fp,2);%nchoosek((fp + tn),2);
    end
    cluster_info{x}.s_e = s_e;
%     cluster_info(x).tnpairs = npairs;
    cluster_info{x}.specificity = mean(s_e);
end

stats.cluster_info = cell2mat(cluster_info);
stats.AS = mean([stats.cluster_info.specificity]);
end