%% script to load rank-order clustering results.
f_data = '~/WORK/janus/sandbox/jrobinson/matlab/clustering/MATs/sub50_100max_normalized/true_ids.mat';
load(f_data, 'gt');
labs = gt;  clear gt;
% results dir
rdir = '~/WORK/janus/sandbox/jrobinson/matlab/clustering/MATs/sub50_100max_normalized/rankordering_new/';

% Eps = [1:.04:1.24 1.26:0.02:2 2.1:.1:3 4:10 20];
% Eps = [1:.04:2 2.1:.1:3 4:10];
k=[10 20 50 100 200];        % number of NN for each sample (i.e., KDTree)
% Eps = [.5:.05:.95
Eps = [0.05:0.05:.45 .5:.05:.95 1:.04:2 1.5:.01:1.7 2.1:.1:3 4:10];% [1:.04:2];% 2.1:.1:3 4:10];
%     Eps = [1.5:.01:1.7];% [1:.04:2];% 2.1:.1:3 4:10];
nruns = length(Eps);
% all_stats = cell(1,nruns);


ibin = strcat(rdir, 'run_k100/results_k100_Eps%s.mat');
% ro_stats2={};
cstats = cell(1,nruns);
% for y = 1:length(k)
%     obin = strcat(rdir, 'results_k%d_Eps%s.mat');
counter = 1;

for x = 1:nruns
    %% for all threshold values
    %         fin = sprintf(obin,k(y), strrep(num2str(Eps(x)),'.','_'));
    fin = sprintf(ibin,strrep(num2str(Eps(x)),'.','_'));
    
    if exist(fin, 'file')
        % if results file exists
        fprintf(1,'\n Loading Rank Order Results (Eps = %s)\n\n',num2str(Eps(x)));
        load(fin,'cluster_ids');
        % determine stats, i.e., pair-wise precision, recall, & f-score
        tmp = eval.ClusterMetrics(labs.rlab, cluster_ids);
        
        %  tmp = eval.cluster_pairwise_stats(cluster_ids,labs.rlab);
        %
        % track reference to source (results) file
        tmp.Eps = Eps(x);
        tmp.fpath = fin;
        tmp.numclusters = length(unique(cluster_ids));
        %
        cstats{counter} = tmp;
        counter = counter + 1;
        
    else
        fprintf(2,'(ERROR) NO FILE: %s\n',fin);
    end
end
%      ro_stats2{length(ro_stats2) +1} = cell2mat(cstats);
% end


return
