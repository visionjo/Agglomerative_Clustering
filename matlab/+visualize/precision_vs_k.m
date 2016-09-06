%% Generate confusion matrix of clustering (pair-wise)
%
% precision     - precision scores for various number of clusters
% 
% kspan         - (OPTIONAL) k values corresponding to precision. If not
%                   provided, k is assumed to span 1:length(precision)
% 
% labels        - (OPTIONAL) labels for legend. No legend added is not
%                   passed in as arg.
%
% @author Joseph P. Robinson
% @date 2016 July 25
%% Build
function fh = precision_vs_k(precision, kspan, labels)
% generate confusion matrices
fh = figure;
nruns = length(precision);
if nargin == 1
    kspan = 1:nruns;
end
plot(kspan,precision, 'linewidth', 2);
set(gca, 'box', 'on', 'xgrid', 'on', 'ygrid', 'on');
xlabel('No. of Clusters','FontSize',14);
ylabel('Pairwise Precision','FontSize',14);
axis([0 1 0 1]);

if exist('labels','var')
    lh = legend(labels{:}); 
    set(lh,'FontSize',16,'Location','Best');
end

end


% figure
% plot([stats2.Eps],[stats2.fscore],'linewidth',1.5)
% xlabel('Eps','FontSize',14);
% ylabel('F-Score','FontSize',14);
% title(['Rank-Order Clustering (F-Score)'],'FontWeight','bold','FontSize',16);
% % axis equal
% export_fig('figures/rank_ordering_fscores.pdf', '-m2.5')
% set(gca, 'box', 'on', 'xgrid', 'on', 'ygrid', 'on');
% box(gca,'on');
% % Set the remaining axes properties
% set(gca,'XGrid','on','XMinorTick','on','XTick',...
%     [0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20],'YGrid','on');
