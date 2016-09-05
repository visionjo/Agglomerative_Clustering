function metrics=ClusterMetrics(g,c)
% function metrics=ClusterMetrics(g,c);
%
% This function provides standard metrics for clustering 
% Inputs:
%   g- ground truth cluster labels
%   c- evaluated cluster labels
%  Outputs:
%    metrics- metrics 
%      numsamples- number of samples provided
%      numclasses- number of ground truth clusters
%      numclusters- number of evaluated clusters
%      precision/recall- Pairwise precision and recall
%      wprecision/wrecall- Pairwise weighted prec and rec
%

%% usage notes
%g=clust.ind2sid(:,2);
%c=idx;
%metrics=ClusterMetrics(g,c);


%% setup data

g=g(:);
c=c(:);

guids=unique(g);
cuids=unique(c);

metrics.numsamples=length(g);
metrics.numclusters=length(cuids);
metrics.numclasses=length(guids);

%% ground truth evaluation (for precision)

gstats=zeros(length(guids),5);
for i=1:length(guids)
    guid=guids(i);
    cs=c(g==guid);
    l=numel(cs);
    TP=sum(sum(repmat(cs,[1,l])==repmat(cs',[l,1])))-l;
    gstats(i,1:3)=[TP,l^2-l,l];
    gstats(i,4)=gstats(i,1)/gstats(i,2);
    gstats(i,5)=sum(cs==mode(cs));
end
metrics.precision=sum(gstats(:,1))/sum(gstats(:,2));
w=gstats(:,3);w=w/sum(w);
metrics.wprecision=sum(w.*gstats(:,1)./gstats(:,2));
metrics.gpurity=mean(gstats(:,5)./gstats(:,3));

%% cluster evaluation (for recall)

cstats=zeros(length(cuids),5);
for i=1:length(cuids)
    cuid=cuids(i);
    gs=g(c==cuid);
    l=numel(gs);
    TP=sum(sum(repmat(gs,[1,l])==repmat(gs',[l,1])))-l;
    if (l==1)
        cstats(i,1:5)=[0,0,1,0,1];
    else
        cstats(i,1:3)=[TP,l^2-l,l];
        cstats(i,4)=cstats(i,1)/cstats(i,2);
        cstats(i,5)=sum(gs==mode(gs));
    end
end
if (sum(cstats(:,2))==0)
    metrics.recall=1;
    metrics.wrecall=1;
else
    metrics.recall=sum(cstats(:,1))/sum(cstats(:,2));
    w=cstats(:,3);w=w/sum(w);
    mask=cstats(:,2)~=0;
    metrics.wrecall=sum(w(mask).*cstats(mask,1)./cstats(mask,2));
end;    
metrics.cpurity=mean(cstats(:,5)./cstats(:,3));

metrics.fscore = 2*bsxfun(@times,[metrics.recall],[metrics.precision])./([metrics.recall]'+[metrics.precision]')';
