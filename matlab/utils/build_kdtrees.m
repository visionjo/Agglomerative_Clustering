%% Prepares KD-Tree with k-NN for data matrix X.
% Provided a filepointer (i.e., fpath) and t
%
% @author Joseph P. Robinson
% @date 2016 July 25
%% Build
function IdxNN = build_kdtrees(fpath,k)

load(fpath, 'X');
[nsamples, ~] = size(X);
nclusters = nsamples; % each sample starts in own cluster


disp('##### Build Tree #####');

IdxNN = cell(1,nclusters);    % indices of  NN

%% Compute top-k NN for each face encoding
Mdl = createns(X,'NSMethod','kdtree','Distance','euclidean');
%
fprintf(['##### ' num2str(k) ' Closest Points #####']);
parfor_progress(nsamples);
parfor x = 1:nsamples
    % Compute pair-wise Rank-Order distances between for face & top-k NN
    xsamp = X(x,:); 
    tmp = knnsearch(Mdl,xsamp,'K',k + 1); % itself + k other NN
    IdxNN{x} = tmp(2:end); % not count itself as nearest neighbor
    parfor_progress;
end
parfor_progress(0);


end