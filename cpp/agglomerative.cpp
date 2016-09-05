//
//  agglomerative.cpp
//  Agglomerative
//
//  Created by Robinson, Joseph on 8/16/16.
#include "agglomerative.hpp"

/**
 * Destructor
 */
agglomerative::~agglomerative(){}

/**
 * Simple method to initialize a set of samples as singleton clusters.
 *
 * @param singletons    - reference to vector to return N singletons in
 * @param nsamples      - No. of samples (N) to cluster as singletons
 *
 * @author Joseph P. Robinson
 * @date 2016 August 20
 *
 */



/**
 * Complete- and single-link are merge closest clusters (w.r.t. each criteria)
 * until k clusters are found.
 */
void agglomerative::complete_link(Clustering& C, int k, std::vector<int>& cluster_ids){
    
    unsigned long nclusters = C.get_nclusters();
    
    while (nclusters > k)
    {// merge until k clusters is obtained
        
        // determine the two closest clusters
        if (constants::DO_DEBUG)
            std::cout << C.get_nclusters() << std::endl;
        
        C.merge_closest();
        nclusters = C.get_nclusters();
        
        
    }
    
    cluster_ids = C.get_cluster_ids();
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
void agglomerative::single_link(Clustering& C, int k, std::vector<int>& cluster_ids){
    
    unsigned long nclusters = C.get_nclusters();
    
    while (nclusters > k)
    {// merge until k clusters is obtained
        
        // determine the two closest clusters
        if (constants::DO_DEBUG)
            std::cout << C.get_nclusters() << std::endl;
        C.merge_closest();
        nclusters = C.get_nclusters();
        
        
    }
    
    cluster_ids = C.get_cluster_ids();
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
void agglomerative::single_pass(cv::Mat& D, double Eps, std::vector<int>& cluster_ids){
    
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



