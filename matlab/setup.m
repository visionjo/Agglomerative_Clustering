function varargout =  setup()
%%
%     Configures workspace for clustering toolbox.
%           - adds directories to path.
%
%     AUTHOR    : Joseph Robinson
%     DATE      : July-2015
%     Revision  : 1.0
%     DEVELOPED : 2015b
%     FILENAME  : setup.m
%
dirnames = {'utils','rankorder','demo'};

warning('OFF', 'MATLAB:dispatcher:nameConflict') 
cur_root = pwd;
DIR_ROOT = [fileparts(mfilename('fullpath')), '/'];
% DIR_ROOT = fpath(1:tmp(end));   

addpath(DIR_ROOT);

cellfun(@addpath,strcat(DIR_ROOT,dirnames))

cd (cur_root);

fprintf(1,'\nConfigured & Ready for Clustering!\n');

if nargout == 1,    varargout{1} = DIR_ROOT;  end

end

