%% Generate confusion matrix of clustering (pair-wise)
%
% cluster_ids  - cluster labels (i.e., assignment)
% clabels       - ground truth (i.e., class label)
%
% @author Joseph P. Robinson
% @date 2016 July 25
%% Build
function confusion = cluster_confusion(cluster_ids, clabels)
% generate confusion matrices

% RGB
cols = [1 0 0; 0 1 0; 0 0 1;1 0 1];

%% class label params
class_list = unique(clabels);   % list of class ids
nclasses = length(class_list);  % No. of classes
nsamples = numel(clabels);   % sample count


%% confusion matrix representing sample of pairs (true labels & cluster ID)
% the matrix is constructed as follows: a matrix of size NxN is first
% initialized as all zeros (i.e., black); reference to cluster assignments
% is sorted such that each cluster is the next 'block' along the diagonal,
% with the size of the block equal to the number of samples assigned to it;
% for each cluster, all samples are assumed to be misses (i.e., cluster
% block starts off entirely YELLOW); next, individual cluster blocks are
% sorted by true class labels (of course, for classes present in cluster);
% pairs of each of these classes (sub-sub blocks) are set to be GREEN; then
% all missing samples (i.e., classes of from same class that were assigned
% to different cluster) are set red; pure, incomplete clusters are set BLUE
% (i.e., clusters containing only 1 class, but missing pairs to other
% clusters); pure, complete clusters are set WHITE (i.e., clusters
% containing a single class in its entirety).

%% sort by clusters
[cids,ids_orig]=sort(cluster_ids);
% list of cluster IDs (pre- sorted)
cluster_list = unique(cids);
nclusters = length(cluster_list);



%% allocate memory for confusion matrix to return
confusion = zeros(nsamples,nsamples,3);

ids_tracker = 1;    % beginning index of next cluster
for x = 1:nclusters
    %% for each cluster
    
    % xth cluster:
    ids = cids == cluster_list(x);   % get samples in cluster
    
    csamps = cids(ids);
    % No. of samples
    nsamps = length(csamps);
    
    % No. class types present
    true_labels = clabels(ids_orig(ids));
    label_list = unique(true_labels);
    nclasses = length(label_list);
    
    
    %% Current Cluster (true and false pairs)
    %% indices for samples in current cluster w.r.t. confusion matrix, i.e., cluster-block along the matrix diagonal
    ids_span = ids_tracker:(nsamps+ids_tracker - 1);
    
    %% determine correct pairs
    % assume all pairs in cluster are misses, i.e., color cluster-block yellow
    subblock = zeros(nsamps,nsamps,3);
    subblock(:,:,1) = cols(1,1);
    subblock(:,:,2) = cols(2,2);
    
    ids_tracker2 = 1;
    for y = 1:nclasses
        %% determine number of true pairs for each class present in cluster
        % processing class-by-class, thus, sort by class within current
        % cluster-block
        nmatches = length(find(label_list(y) == true_labels));
        
        ntracker = nmatches + ids_tracker2 - 1;
        
        ids_mat = (ids_tracker + ids_tracker2 - 1):(ids_tracker + ntracker - 1);
        
        % color true pairs for this class green (overwrite default yellow)
        subblock(ids_tracker2:ntracker,ids_tracker2:ntracker,1) = cols(2,1);
        
        ids_tracker2 = ntracker + 1;
        
        %% All Other Clusters (i.e., missed pairs)
        %% find samples of same class in other clusters (i.e. missed pairs)
        all_instances = (clabels == label_list(y));
        
        % samples in same class & not in same (this) cluster
        misses = all_instances & (cids ~= cluster_list(x));
        % color red (indexing rows of clustered samples and cols of missed)
        confusion (ids_mat, misses,1) = cols(1,1);
    end
    
    if nclasses == 1
        %% if cluster is pure
        nmisses = length(find(misses));
        if nmisses == 0
            % color white if clustering is perfect (complete and correct)
            for y = 1:3
                subblock(:,:,y) = 1;%cols(4,y);
            end
        else
            % color blue if pure cluster (note, unclustered pairs exist)
            for y = 1:3
                subblock(:,:,y) = cols(3,y);
            end
        end
    end
    %% update confusion with cluster matrix (block)
    confusion (ids_span,ids_span,:)=subblock;
    % increment index tracker to first sample in next cluster
    ids_tracker=nsamps+ids_tracker;
    
end



end

