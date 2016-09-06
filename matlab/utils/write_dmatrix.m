%% Write Distance Matrix to File.
% Write distance matrix D to filepath fpath as a set of tuples, i.e.,
% (i,j,d), where i is row number, j is column number, and d is distance (or
% edge). D is assumed symmetric, unless specified otherwise via flag. Thus,
% only the upper triangle is scanned.
% 
% File format is compatible with c++ clustering tools.
% 
% @param fpath  - Output filepath
% @param D      - Distance matrix
% 
% OPTIONAL
% @param is_sym - D matrix is symmetrical (Default: true)
% 
% @todo         - Adjust offset if not symmetric matrix (i.e., add
% condition in inner for-loop to add 1 to offset.
% 
% @author Joseph P. Robinson
% @date 2016 September 3
%% 

function write_dmatrix(fpath, D, is_sym)

if (nargin < 3), is_sym = true; end
if ~is_sym
    fprintf(2,'Assuming Matrix is symmetric');
    is_sym = true;
end
[nrows, ~] = size(D);

if (is_sym)
    % set lower triangle to Inf (i.e., ignore lower triangle if symmetric)
    D(tril(ones(size(D)))==1) = Inf;
end
% set diagonal to Inf, to ignore (i,j) where i == j
D(eye(size(D))==1) = Inf;

nelements = length(find(~isinf(D)));
tuple = cell(1,nelements);
counter = 1;

if (is_sym)
    offset = 1:(nrows-1) + 1;
else
    offset = zeros(1, nrows - 1);
end
%% iterating row-by-row conserves ordering
for x = 1:(nrows-1)
    % for each row
   dvec = D(x,offset(x):end);
   y = find(~isinf(dvec));
   for w = y
       tuple{counter} = [x (w + offset(x)) dvec(w)]';
       counter = counter + 1;
   end
end
tmatrix=cell2mat(tuple)';
clear tuple;
ttable = array2table(tmatrix,'VariableNames',{'i' 'j' 'd'});
clear tmatrix;
%% write csv file
writetable(ttable, fpath,'Delimiter',',');
end
