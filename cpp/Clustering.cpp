//
//  Clustering.cpp
//  Agglomerative
//
//  Created by Robinson, Joseph on 8/25/16.
//  Copyright © 2016 Robinson, Joseph. All rights reserved.
//

#include "Clustering.hpp"
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Clustering
//--------------------------------------------------------------------------


/**
 *
 */
std::vector<dPoint1f> Clustering::parse_sample_distances(cv::Mat& c_merge, cv::Mat& dis_vec, int x, int offset){
    
    cv::Mat idx;
    //cv::Mat c_merge = merge_mask.row(x).colRange(x+1, num_samples);
    cv::findNonZero(c_merge, idx);
    
    // convert type Mat to stl vector
    std::vector<int> m_row=std::vector<int>(idx.begin<int>(), idx.end<int>());
    int n_to_merge = (int)m_row.size();
    
    std::vector<dPoint1f> nlist = std::vector<dPoint1f>();
    for(int y = 0; y < n_to_merge; y++)
    {
        // index the jth index of the D matrix's upper triangle
        int j = m_row.at(y) + x + offset;
        float dis = dis_vec.at<float>(m_row.at(y));
        nlist.push_back(dPoint1f(j, dis));
        
    }
    return nlist;
}

/**
 * Initialize all Samples, each are put in its own Cluster (i.e., singletons).
 *
 * In essence, this function parses the distance Matrix D such that it is 
 * structured- initializes each sample (row) as an item containing reference to 
 * its sample index, along with indices and distances of the respective nearest 
 * neighbors, i.e., all samples for which fall below a threshold. It's important
 * to note that this threshold is only to ignore pairs of samples seperated by 
 * large distances (i.e., inf). NN list is then sorted for the given Item; the 
 * Item is then passed to Cluster() to instantiate a singleton cluster 
 * (this is repeated for all samples).
 *
 * Under the assumption D(i,j) = D(j,i) & diag(D) = 0 only the upper triangle of
 * the distance matrix is processed. Thus, sample N will not have any NNs, as it
 * its position bottom right of the upper triangle; however, pairwise distances
 * are recorded w.r.t. to the other sample as 'i' in (i,j).
 *
 */

void Clustering::initialize(cv::Mat& D)
{
    num_samples = D.rows;
    init_singletons(cluster_ids, num_samples);
    
    cv::Mat idx, merge_mask;
    // generate mask to filter large distances, where 0 if less MAX_DISTANCE; else, 1;
    cv::threshold(D,merge_mask,constants::MAX_DISTANCE,
                  1.0,cv::THRESH_BINARY_INV);
    merge_mask.convertTo(merge_mask,CV_8UC1);
    
    // Initialize each samples to its own cluster.
    // Note, distance (i,j) and (j,i) are assumed equal, however, distance from
    // j w.r.t. ith sample and distance from i w.r.t. to jth sample are both add
    // to respective lists. Reason is to avoid complications when merging, i.e.,
    // complete-link would always cluster the Nth sample by itself, as it would
    // not have any NN if only the upper triangle of the symmetric D matrix was
    // parsed.
    // However, if it is undesirable to track this redudant information (i.e.,
    // consider (i,j) but not (j,i), then see Clustering::initialize_utri(),
    // which could be most efficient for a single-link criteria.
    std::vector<int> m_row; Cluster Cc;
    int offset = 0;
    for (int x = 0; x < num_samples; x++)
    {   // for each sample
        
        // get ids of distances for current sample w corresponding row of mask
        cv::Mat c_merge = merge_mask.row(x);
        c_merge.at<int>(x) = 0; // not to record when i == j (i.e., diagonal)
        cv::Mat samp_dis = D.row(x);
        std::vector<dPoint1f> nlist = parse_sample_distances(c_merge, samp_dis, x, offset);
        
        // instantiate samp with index x, (true) is_root, height 0 (base of tree),
        // and Neighbors
        Sample samp (x,true,0, Neighbors (x,nlist));
        // assign Cluster w ID x (note, cluster_ids(x) == x, as each sample
        // starts of as a singleton, i.e., each sample in itself is a cluster)
        // and containing sample samp
        Cc= Cluster(cluster_ids.at(x), samp);
//        Cc.set_criteria(link_criteria);
        add_cluster(Cc);

    }
}

/**
 * Similiar to Clustering::initialize, but with the following difference:
 *
 * Under the assumption D(i,j) = D(j,i) && diag(D) = 0 only the upper triangle 
 * of the distance matrix is processed. Thus, sample (or Item) N will not have 
 * any NN, as it is the bottom right point of the upper triangle; however, all 
 * distances it shares between other samples.
 *
 * Case that the two methods would yield different outcomes is when it comes 
 * time for merging NN lists, provided certain merging schemes (i.e., complete 
 * link), the bottom of the upper triangle (i.e., samples w smaller NN lists) 
 * could drastically impact the final clustering, making such union of NN lists 
 * could reduce down to no neighbors (in the case with the Nth sample). For 
 * singlelink, this will not make difference, as NN lists get XOR (opposed to &)
 */

void Clustering::initialize_utri(cv::Mat& D)
{
    num_samples = D.rows;
    init_singletons(cluster_ids, num_samples);
    
    cv::Mat idx, merge_mask;
    // generate mask to filter large distances, where 0 if less MAX_DISTANCE; else, 1;
    cv::threshold(D,merge_mask,constants::MAX_DISTANCE,
                  1.0,cv::THRESH_BINARY_INV);
    merge_mask.convertTo(merge_mask,CV_8UC1);
    
    /**
     * Initialize each samples to its own cluster.
     *
     * Again, only considering the upper triangle.
     */
    Cluster Cc;
    int offset = 1;
    for (int x = 0; x < num_samples; x++)
    {   // for each sample
        
        cv::Mat c_merge = merge_mask.row(x).colRange(x+offset, num_samples);
        cv::Mat samp_dis = D.row(x).colRange(x+offset, num_samples);
        
        std::vector<dPoint1f> nlist = parse_sample_distances(c_merge, samp_dis, x, offset);
        
        // instantiate samp with index x, (true) is_root, height 0 (base of tree),
        // and Neighbors
        Sample samp (x,true,0, Neighbors (x,nlist));
        // assign Cluster w ID x (note, cluster_ids(x) == x, as each sample
        // starts of as a singleton, i.e., each sample in itself is a cluster)
        // and containing sample samp
        Cc= Cluster(cluster_ids.at(x), samp);
//        Cc.set_linkage_criteria(link_criteria);
        add_cluster(Cc);
        
    }
}

void Clustering::add_cluster(Cluster& C)
{
    clusters.push_back(C);
    num_clusters += 1;

};

void Clustering::find_sample(const int index, Cluster &C)const
{
    for (int x = 0; x < clusters.size(); x++)
    {
        Cluster cclust = clusters.at(x);
        
        if (cclust.has_sample(index))
        {
            C = cclust;
            return;
        }
    }
    
}
/**
 * Find the two nearest samples from different clusters and merge.
 * Assuming there were K clusters prior to calling this method, there would be 
 * (K - 1) clusters thereafter.
 */
void Clustering::merge_closest()
{
    
    // find closest pair of samples
    dPair2f closest_pair = get_nn();

    if (constants::DO_DEBUG)
        std::cout << "Closest Pairs: " << closest_pair << "\n";
    
    
    // Find clusters containing 2 samples
    Cluster c1, c2;
    find_sample(closest_pair.i, c1);
    find_sample(closest_pair.j, c2);
    
    /**
     * cluster c2 gets merged into cluster c1; cluster ID is
     */
    c1.add_merge(closest_pair);
    
    c1.merge(c2, link_criteria);
    
    
    // update all samples in cluster w proper label (i.e., min of all labels)
    std::vector<int> vec = c1.get_indices();
    int ids = *std::min_element(std::begin(vec), std::end(vec));
    
    for (int x = 0; x < c1.get_samples().size(); x++)
    {   // update all IDs for samples in c1
        cluster_ids[c1.get_sample(x).get_index()] = ids;
    }

    std::vector<Cluster> uset;
    for(int x = 0; x < clusters.size(); x++)
    {// get all clusters that were not merged above (i.e., all but c1 and c2)
        Cluster ccluster = clusters.at(x);
        
        if(c1.get_id() == ccluster.get_id() || c2.get_id() == ccluster.get_id() )
            continue;
        
        uset.push_back(ccluster);
    }
    // add newly merged cluster as last element of vector
    uset.push_back(c1);
    
    // update clusters and num_clusters
    clusters = uset;
    num_clusters = (int)clusters.size();
    
}
dPair2f Clustering::get_nn()
{
//    Pair2i minp(0,0,0);
    std::list<float> dis_list;
    
    // store smallest distance of any pair
    dPair2f minpoint(0,0,constants::MAX_DISTANCE);
    
    std::vector<Cluster>::iterator iter = clusters.begin();
    while(iter != clusters.end())
    {   // for each cluster
        Cluster& c = *iter;
        // get NN list
        
        if (c.has_neighbors())
        { // compare smallest pairwise distance w.r.t. samples in current cluster
            Neighbors n = c.get_merged_list();
            
            // get smallest of all pairwise distances for this cluster's NN list
            dPoint1f cpoint = n.get_kth_nn(0);
            // instantiate dPair2f object to compare with current min (placeholder)
            
            int i = c.get_id();
            int j = cpoint.j;
            
            if (cluster_ids[i] != cluster_ids[j])
            {
                dPair2f d(i,j,cpoint.distance);
                minpoint.compare(d);  // compare, setting minpoint to closer pair
            }
        }
        
        iter++;
    }
    
    
    return minpoint;
}




/**
 * Complete- and single-link are merge closest clusters (w.r.t. each criteria)
 * until k clusters are found.
 */
void Clustering::complete_link(int k, std::vector<int>& cluster_ids){
    
    unsigned long nclusters = get_nclusters();
    
    while (nclusters > k)
    {// merge until k clusters is obtained
        
        // determine the two closest clusters
        if (constants::DO_DEBUG)
            std::cout << get_nclusters() << std::endl;
        
        merge_closest();
        nclusters = get_nclusters();
        
        
    }
    
    cluster_ids = get_cluster_ids();
    // set cluster labels to span from 0:nclusters
    set_cluster_ids(cluster_ids);
}
/**
 * Provided a Clustering object, this method merges based on closest pairs from
 * all clusters NN lists. Merging clusters also involves merging NN lists, which
 * is where this method differs from agglomerative.complete_link().
 *
 * To merge NN lists for a given pair of clusters, the min distance of shared
 * neighbors and the only distance of unshared neighbors make up the newly
 * merged NN list.
 *
 * @param C   - Clustering object already fit to D matrix (i.e., initialized)
 * @param k   - Number of clusters to predict
 * @param cluster_tags   - Cluster ID assignments
 *
 * @return cluster_tags   - Cluster ID assignments
 *
 * @todo - add constraint matrix C
 *
 * @author Joseph P. Robinson
 * @date 2016 August 18
 *
 */
void Clustering::single_link(int k, std::vector<int>& cluster_ids){
    
    unsigned long nclusters = get_nclusters();
    
    while (nclusters > k)
    {// merge until k clusters is obtained
        
        // determine the two closest clusters
        if (constants::DO_DEBUG)
            std::cout << get_nclusters() << std::endl;
        
        merge_closest();
        nclusters = get_nclusters();
        
        
    }
    
    cluster_ids = get_cluster_ids();
    // set cluster labels to span from 0:nclusters
    set_cluster_ids(cluster_ids);
}


/**
 * Threshold distance matrix using single-link agglomerative clustering.
 * Transitively step through matrix, merging each pairs w distances below
 * threshold Eps into same cluster.
 *
 * Assuming symmetrical matrix, this function steps through the upper triangle
 * row-by-row. With each sample initialized in its own cluster, this function
 * 1st determines which pairs are below a threshold (for a given row), and then
 * groups each into the cluster w smallest label-- assigning to min cluster IDs
 * avoids reassigning clusters that were formed prior (i.e., starts at cluster 0
 * and steps to N-1).
 *
 * @param D   - Rank order distance matrix
 * @param Eps - Rank order distance threshold (i.e., epsilon) [default 1.6]
 * @param C   - Constraint matrix [default zeros(size(D))]
 *
 * @return cluster_tags   - Cluster ID assignments
 *
 * @todo - add constraint matrix C
 *
 * @author Joseph P. Robinson
 * @date 2016 August 18
 *
 */
void Clustering::single_pass(cv::Mat& D, double Eps, std::vector<int>& cluster_ids){
    
    cv::Mat merge_mask;// = D < this->Eps;                 // RO distance matrix
    // threshold D using Eps, where 0 if less than; else, 1;
    cv::threshold(D,merge_mask,Eps,1.0,cv::THRESH_BINARY_INV);
    
    merge_mask.convertTo(merge_mask,CV_8UC1);
    
    int nsamples = D.rows;
    init_singletons(cluster_ids, nsamples);
    cv::Mat idx;
    std::vector<float> x_distances;
    std::vector<int> m_row;
    for (int x = 0; x < (nsamples-1); x++)
    {   // initialize all samples in own cluster
        cv::Mat c_merge = merge_mask.row(x).colRange(x, nsamples);
        cv::findNonZero(c_merge, idx);
        
        // convert type Mat to stl vector
        m_row=std::vector<int>(idx.begin<int>(), idx.end<int>());
        int n_to_merge = (int)m_row.size();
        
        
        std::vector<int> ids_to_merge, labs_to_merge;
        for (int y = 0; y < n_to_merge; y++)
        { // loop over
            labs_to_merge.push_back(cluster_ids.at(m_row.at(y) + x));
            ids_to_merge.push_back(m_row.at(y) + x);
        }
        double minVal;
        cv::minMaxLoc(labs_to_merge, &minVal, NULL, NULL, NULL);
        
        // add current index and label
        int clabel = (int)minVal;
        for (int y = 0; y < ids_to_merge.size(); y++)
        {   // set samples @ indices w distances below threshold to same cluster
            cluster_ids.at(ids_to_merge.at(y)) = clabel;
            
        }
    }
    // set cluster labels to span from 0:nclusters
    set_cluster_ids(cluster_ids);
    
}


