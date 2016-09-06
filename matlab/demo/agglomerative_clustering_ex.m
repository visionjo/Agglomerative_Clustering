%% Agglomerative Clustering Demo (CPP Implementation)
% This script calls CPP Implementation of agglomerative clustering,
% demonstrating both the use and options of the program, along with a
% synthetic experiment to visually show the program in action.

%% Set Parameters
%%% I/O Values
% Path to executable
EXEPATH='~/WORK/janus/sandbox/jrobinson/cpp/Agglomerative/build/Debug/Agglomerative';

% Dirs and file paths to input D matrix (CSV) and output cluster IDs (TXT)
d_demo = fileparts(which('agglomerative_clustering_ex.m'));
d_data = strcat(d_demo,'/data/');
d_out = strcat(d_demo,'/results/');

f_data = strcat(d_data,'synthetic_data_200_samples.csv');
if ~exist(d_out,'dir'), mkdir(d_out);end

%% Command Line Options
% Call executable with arg to print 'help'
command = [EXEPATH, ' --help'];%-i ', f_data, ' -o ', f_out, ' -k ', num2str(8)];
display(command);

%% Run Agglomerative Clustering
try
    system(command);
catch
    warning(command);
end

%% Complete-link agglomerative clustering
% Specify input filepath, output filepath, k = 4, and linkage = 1 (single link)
f_out = strcat(d_out,'cluster_ids_s-l.txt');
command = [EXEPATH, ' -i ', f_data, ' -o ', f_out, ' -k ', num2str(4) ' -l 2'];
display(command);

%% Run Agglomerative Clustering
try
    system(command);
catch
    warning(command);
end

%% Single-link agglomerative clustering
% Specify input filepath, output filepath, k = 4, and linkage = 1 (single link)
f_out = strcat(d_out,'cluster_ids_s-l.txt');
command = [EXEPATH, ' -i ', f_data, ' -o ', f_out, ' -k ', num2str(4) ' -l 1'];
display(command);

%% Run Agglomerative Clustering
try
    system(command);
catch
    warning(command);
end

%% Single-pass (approximate) agglomerative clustering
% Specify input filepath, output filepath, eps = 2, and linkage = 2 (single pass)
f_out = strcat('cluster_ids_s-p.txt');
command = [EXEPATH, ' -i ', f_data, ' -o ', f_out, ' -e ', num2str(10) ' -l 2'];
display(command);
%% Run Agglomerative Clustering
try
    system(command);
catch
    warning(command);
end