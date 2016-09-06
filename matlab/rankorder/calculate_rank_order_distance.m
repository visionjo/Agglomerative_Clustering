%% Calculate rank-order distances.
% Construct distance matrix using rank order distance measure [1].
%
% $$d_m(a,b)=\sum_{i=0}^{min(O_a(b),k)} I_b(O_b(f_a(i)),k)$$
%
% $$D(a,b)=\frac{d_m(a,b) + d_m(b,a)}{min(O_a(b),O_b(a))}$$
% where $I_b$ is indicator fuction: 0 if NN is shared; else, 1.
% 
% @param nn_ids - Nxk matrix of the indices of k-NN for N samples
% 
% @return D     - RO distance matrix
% 
% @author Joseph P. Robinson
% @date 2016 August 11
% 
% [1] Otto, Charles, Dayong Wang, and Anil K. Jain. "Clustering millions
% of faces by identity." arXiv preprint arXiv:1604.00989 (2016).
%% 
function D = calculate_rank_order_distance(nn_ids)

[nsamples, k] = size(nn_ids);


% rank order distances between samples and k-NN
D = inf(nsamples,nsamples);

touched = zeros(nsamples,k); % track samples that have been compared
for x = 1:nsamples
    %% for each sample
    
    % current face encoding (i.e., face a)
    a_id = x;                               % id of sample face a
    a_nnlist = nn_ids(a_id,:);             % NN list for face a

    for y = 1:k
        %% for each of its k-NN
        % kth NN of face a (i.e., face b)
        
        b_id = a_nnlist(y);                  % id of sample face a
        
        % if face_b has been as face_a
        % if distance between pairs was already claculated
        if ~isinf(D(b_id,a_id)), continue; end
        
        b_nnlist = nn_ids(b_id,:);          % NN list for face a
        
        
        % if face a is in face b's NN list, then determine its rank;
        [~, rank_a] = find(b_nnlist == a_id);
        
        % else, if face a is not in face b's NN list, or
        % if (isempty(rank_a) || touched(b_id)), continue; end
        
        if isempty(rank_a), rank_a = k;
            % if rank_a < k and b_id has been touched, then distance has
            % already been computed between faces (with a-b flipped)
        elseif (touched(b_id)), keyboard;
        end
        
        
        %         if isempty(rank_a), rank_a = k; end
        % determine rank of face b in face a's NN list (just y)
        rank_b = y;
        
        % get face b's NN list
        %        b_vec = nn_ids(b_id,:);
        
        % assymmetric rank order distance (0 for every shared NN; else, 1);
        d_ba = sum(~ismember(a_nnlist(1:rank_b),b_nnlist));
        d_ab = sum(~ismember(b_nnlist(1:rank_a),a_nnlist));
        
        %% rank-order distance
        D(a_id,b_id) = (d_ab + d_ba)/min(rank_a,rank_b);
        D(b_id,a_id) = D(a_id,b_id);
        
    end

   % mark current face for being compared to its entire NN list
   touched(a_id) = 1;
end
