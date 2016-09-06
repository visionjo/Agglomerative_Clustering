%% Load clustering stats.
%
% din - directories containing results; multiple directories are assumed to
% be different runs of the same experiment, i.e., results are averaged out
%
% @author Joseph P. Robinson
% @date 2016 July 25
%%
%% Build
function stats = eval_all(dirs_in, clabels, kspan)

nruns = length(dirs_in);



counter = 1;
stats = cell(1,length(kspan));
for k = kspan
    cstats = [];
    for x = 1:nruns
        rbin = strcat(dirs_in{x},'/results_k%d.mat');
        fpath = sprintf(rbin,k);
        
        if exist(fpath, 'file')
            % if results file exists
            fprintf(1,'(run %d) Loading k = %d\n',x,k);
                        load(fpath,'idx');
%             load(fpath,'posteriors');
%             [~,idx] = max(posteriors);
%             if length(idx) == 1, idx = posteriors; end
            cstats = [cstats; eval.ClusterMetrics(clabels, idx)];
        else
            fprintf(2,'(ERROR) NO FILE: %s\n',fpath)
        end
        
    end
    stats{counter}.precision = mean([cstats.precision]);
    stats{counter}.wprecision = mean([cstats.wprecision]);
    stats{counter}.gpurity = mean([cstats.gpurity]);
    stats{counter}.recall = mean([cstats.recall]);
     
    stats{counter}.wrecall = mean([cstats.wrecall]);
    stats{counter}.cpurity = mean([cstats.cpurity]);
    stats{counter}.numsamples = cstats.numsamples;
    stats{counter}.numclusters = cstats.numclusters;
    stats{counter}.numclasses = cstats.numclasses;
    
    
    counter = counter + 1;
end
stats = cell2mat(stats);