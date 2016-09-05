#ifndef CLUSTER_HPP
#define CLUSTER_HPP
/**
 * Cluster.hpp
 * Agglomerative
 * Demonstrates workflow behin performing rank-order clustering.
 *
 *
 * [1] Otto, Charles, Dayong Wang, and Anil K. Jain. "Clustering millions
 * of faces by identity." arXiv preprint arXiv:1604.00989 (2016).
 *
 * Created by Robinson, Joseph on 8/19/2016
 *
 */

#include "Sample.hpp"

class Cluster
{
public:
    /* Constructors */
    // default
    Cluster(): cluster_id(0), num_samples(1), num_nodes(1) {};
    // ID passed in
    Cluster(int id): cluster_id(id),num_samples(1),num_nodes(1) {};
    // ID and 1st item passed in; add item and set mergelist as item.list
    Cluster(int id, Sample& samp)
    : cluster_id(id), num_samples(1), num_nodes(1)
    {
        add_sample(samp);
        merged_list = Neighbors(id, samp.get_neighbors().get_list());
    }
    Cluster(const Cluster &C)
    {
        cluster_id = C.get_id();
        indices = C.get_indices();
        merged_list = C.get_merged_list();
        merges = C.get_merges();
        num_nodes = C.get_nnodes();
        samples = C.get_samples();
        num_samples = (int)samples.size();
        
    }
    
    ~Cluster(){};
    
    void merge(const Cluster& c2, const int);
    
    void add_sample(const Sample);
    void merge_samples(const std::vector<Sample>);
    void merge_indices(const std::vector<int>);
    void merge_lists(const Neighbors&, const int);
    
    void sort_list(const int);
    
    void add_merge(const dPair2f pair){merges.push_back(pair);};
    // Finding the centroid of the cluster
//    void find_centroid();
//    void get_centroid();
    
    
    /* Set Functions */
    void set_id(const int id){cluster_id=id;};
//    void set_linkage_criteria(const int ltype){link_criteria =ltype;};
    void set_samples(const std::vector<Sample>& samples_in){samples = samples_in;};

    void set_merged_list(const Neighbors n){merged_list = n;};
    
    /* Get Functions */
//    int get_nitems()const{return num_items;};
    int get_id()const{return cluster_id;};
    int get_nnodes()const{return num_nodes;};
    bool has_sample(const int index)const;
    
    std::vector<Sample> get_samples()const{return samples;};
    Sample get_sample(int k)const{return samples.at(k);};
    
    std::vector<dPair2f> get_merges()const{return merges;};
    std::vector<int> get_indices()const{return indices;};
//    int get_criteria()const{return link_criteria;};
    unsigned long get_nsamps()const{return samples.size();};
    
    Neighbors get_merged_list()const{return merged_list;};
    dPoint1f get_nn()const{return merged_list.get_kth_nn(0);};
    int get_nneighbors()const{return merged_list.get_nncount();};
    bool has_neighbors()const{return (merged_list.get_nncount() > 0);};
    
private:
    
    int cluster_id; /* cluster number in the order of formation */
    int num_samples; /* number of items contained in this cluster */
//    int link_criteria; /* 0 - complete link; 1 - single link; 2 - single pass */
    std::vector<dPair2f> merges; /* indices and distances of items merged */
    
    //    std::vector<int> merges; /* indices of root clusters merged */
    std::vector<int> indices; /* array of leaf nodes indices inside merged clusters */
    
    int num_nodes; /* number of leaf and merged clusters */
    std::vector<Sample> samples; /* leaf and merged clusters */
    //    cv::Point centroid; /* centroid of this cluster */
    Neighbors merged_list;
    
    
    
};
//




#endif // CLUSTER_HPP
