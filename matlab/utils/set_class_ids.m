function labels_out = set_class_ids(labels_in)
% %% Function that converts cluster assignments to class IDs.
% Given cluster assignments generated from rank ordering, i.e., k labels 
% with values ranging from 1:nsamples, this function sets to labels that
% span 1:k, where k is the number of unique IDs of input vector.

cluster_ids = unique(labels_in);
labels_out = zeros(size(labels_in));
counter = 1;
for x = cluster_ids
    ids = labels_in == x;
    labels_out(ids) = counter;
    counter = counter + 1; 
end

end