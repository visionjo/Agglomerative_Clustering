%% Generate confusion matrix of clustering (pair-wise)
%
% cluster_ids  - cluster labels (i.e., assignment)
% clabels       - ground truth (i.e., class label)
%
% @author Joseph P. Robinson
% @date 2016 July 25
%% Build
function confusion = class_confusion(cluster_ids, clabels)
% generate confusion matrices

% RGB
cols = [1 0 0; 0 1 0; 0 0 1;1 0 1];

%% sort by class
[clabs,ids_orig]=sort(clabels);
class_list = unique(clabs);   % list of class ids
nclasses = length(class_list);  % No. of classes
% list of cluster IDs (pre- sorted)
cluster_list = unique(cluster_ids);
nsamples = length(clabels);
%% confusion matrix representing sample of pairs (true labels & cluster ID)
% the matrix is constructed as follows: a matrix of size NxN is first
% initialized as all zeros (i.e., black); reference to class (true) labels
% are sorted such that each class is the next 'block' along the diagonal,
% with the size of the block equal to the number of samples in that class;
% for each class, all samples are assumed to be misses (i.e., cluster
% block starts off entirely YELLOW); next, individual class blocks are
% sorted by assigned cluster IDs (of course, for clusters assigned to class
% samples); pairs of each of cluster (sub-sub blocks) are set to be GREEN;
% then incorrect pairs (i.e., samples in cluster not apart of same class
% to different cluster) are set red; pure, incomplete clusters are set BLUE
% (i.e., clusters containing only 1 class, but missing pairs to other
% clusters); pure, complete clusters are set WHITE (i.e., clusters
% containing a single class in its entirety).






%% allocate memory for confusion matrix to return
confusion = zeros(nsamples,nsamples,3);

ids_tracker = 1;    % beginning index of next cluster
for x = 1:nclasses
    %% for each cluster
    
    % xth cluster:
    ids = clabs == class_list(x);   % get samples in cluster
    
    % class samples
    csamps = clabs(ids);
    
    osamps_ids = ids == 0;
    % No. of samples
    nsamps = length(csamps);
    
    % determine cluster IDs for each sample of class
    cluster_assignments = cluster_ids(ids_orig(ids));
    % list cluster ID assignments
    cids = unique(cluster_assignments);
    % number of cluster IDs
    nclusters = length(cids);
    
    
    %% Current Cluster (true and false pairs)
    %% indices for samples in current cluster w.r.t. confusion matrix, i.e., cluster-block along the matrix diagonal
    ids_span = ids_tracker:(nsamps+ids_tracker - 1);
    
    %% determine correct pairs
    % assume all pairs in cluster are misses, i.e., color cluster-block yellow
    subblock = zeros(nsamps,nsamps,3);
    subblock(:,:,1) = cols(1,1);
    subblock(:,:,2) = cols(2,2);
    
    ids_tracker2 = 1;
    for y = 1:nclusters
        %% determine number of true pairs for class samples of the cluster
        % processing clusterID-by-clusterID, thus, sort by cluster within a
        % set of samples from same class
        nmatches = length(find(cids(y) == cluster_assignments));
        
        ntracker = nmatches + ids_tracker2 - 1;
        
        ids_mat = (ids_tracker + ids_tracker2 - 1):(ids_tracker + ntracker - 1);
        %         ids_mat = ones(1,nsamples);
        %         ids_mat(tmp) = 0;
        % color true pairs for this class green (overwrite default yellow)
        subblock(ids_tracker2:ntracker,ids_tracker2:ntracker,1) = cols(2,1);
        
        ids_tracker2 = ntracker + 1;
        
        %% All Other Clusters (i.e., missed pairs)
        %% find samples of different classes in cluster (i.e. incorrect)
        all_samps = (cluster_ids == cids(y));
        
        % samples in same cluster & not in same (this) class
        negatives = all_samps & osamps_ids;%(clabs ~= class_list(x));
        % color red (indexing rows of clustered samples and cols of missed) 
        confusion (ids_mat,negatives,1) = cols(1,1);
    end
    
    if nclusters == 1
       %% if entire class is contained in one cluster
        nnegatives = length(find(negatives));
        if nnegatives == 0
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


