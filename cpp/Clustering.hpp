//
//  Clustering.hpp
//  Agglomerative
//
//  Created by Robinson, Joseph on 8/25/16.
//

#ifndef Clustering_hpp
#define Clustering_hpp

#include "Cluster.hpp"

class Clustering
{
public:
    Clustering(): num_clusters(0), num_samples(0), num_nodes(0), link_criteria(0), reference_name(""){
        clusters = std::vector<Cluster>();
        cluster_ids = std::vector<int>();
    };
    ~Clustering(){};
    /* Preparing Distance Matrix */
    void initialize(cv::Mat& D);
    void initialize_utri(cv::Mat& D);
    void initialize_tuples(const std::string ifile);
    std::vector<dPoint1f> parse_sample_distances(cv::Mat& c_merge, cv::Mat& dis_vec, int x, int offset);
//    void parse_dMatrix(cv::Mat&);
    
    
    void add_cluster(Cluster& C);    
    /* Clustering*/
    void merge_closest();
    dPair2f get_nn();
    
    void find_sample(const int index, Cluster &C)const;
    
    /* Set Functions */
    void set_reference_name(const std::string& s){reference_name = s;};
    void set_linkage_criteria(const int ltype){link_criteria = ltype;};
    
    
    /* Get Functions */
    std::string get_reference_name()const{return reference_name;};
    int get_linkage_criteria()const{return link_criteria;};
    
    
    std::vector<Cluster> get_clusters()const{return clusters;};
    std::vector<int> get_cluster_ids()const{return cluster_ids;};
    unsigned long get_nclusters()const{return clusters.size();};
    
    
    // overloaded << (i.e., cout)
    friend std::ostream &operator<<(std::ostream &out, const Clustering& C)     //output
    {
        std::string hdr="****************************************************\n";
        out<< hdr;
        out<<"******\t\t"<<C.reference_name<<"\t\t******\n\n";
        out<< hdr;
        out<<"No. of samples:\t\t"<<C.num_samples<<"\n";
        out<<"No. of nodes:\t\t"<<C.num_nodes <<"\n";
        out<<"Linkage:\t\t\t"<<constants::vinit[ C.link_criteria] <<"\n";
        out<<"No. of clusters:\t"<<C.num_clusters <<"\n";
        out<< hdr << std::endl;
        return out;
    }
    
    
    /**
     * Methods for merging.
     */
    void single_link(int k,  std::vector<int>&);
    void complete_link(int k,  std::vector<int>&);
    void single_pass(cv::Mat&, double,  std::vector<int>&);
    
    
private:
    std::string reference_name; /* number of items that was clustered */
    int num_samples; /* number of samples that was clustered */
    int num_clusters; /* current number of root clusters */
    int num_nodes; /* number of leaf and merged clusters */
    int link_criteria; /* 0 - complete link; 1 - single link; 2 - single pass */
    std::vector<Cluster> clusters; /* leaf and merged clusters */
    std::vector<int> cluster_ids; /* clustering IDs w index corresponding to sample # */
    //    dMatrix distances; /* distance between leaves */
};
//class Cluster_Cutter
//{
//public:
//    Cluster_Cutter(){};
//    ~Cluster_Cutter(){};
//
//private:
//    Clustering tree;
//};

#endif /* Clustering_hpp */
