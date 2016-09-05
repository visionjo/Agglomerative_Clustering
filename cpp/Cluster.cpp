



#include "Cluster.hpp"

void Cluster::merge(const Cluster& c2, const int linkage_criteria)
{
    merge_samples(c2.get_samples());
    merge_indices(c2.get_indices());
    merge_lists(c2.get_merged_list(), linkage_criteria);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Cluster
//--------------------------------------------------------------------------
void Cluster::add_sample(const Sample samp)
{
    samples.push_back(samp);
    num_samples = (int)samples.size();
    indices.push_back(samp.get_index());
}

void Cluster::merge_samples(const std::vector<Sample> samps){
    int nsamps = (int)samps.size();
    for (int x = 0; x < nsamps; x++)
    {
        samples.push_back(samps.at(x));
    }
    
    num_samples = (int)samples.size();
    num_nodes += 1;
}
void Cluster::merge_indices(const std::vector<int> ids){
    int nsamps = (int)ids.size();
    for (int x = 0; x < nsamps; x++)
    {
        indices.push_back(ids.at(x));
    }
}

bool Cluster::has_sample(const int index) const
{
    for (int x = 0; x < samples.size(); x++)
    {
        if (samples.at(x).get_index() == index)
            return true;
    }
    return false;
}
void Cluster::sort_list(const int link_criteria){
//    //    neighbors
    
    std::vector<float> nn_dis=merged_list.get_distances();
    
    std::vector<int> nn_ids=merged_list.get_sampleids();
    
    
    std::vector<size_t>nn_sorted= sort_indices(nn_dis);
    
    if (link_criteria == 0) // complete-link (flip to keep max distance)
        std::reverse(std::begin(nn_sorted), std::end(nn_sorted));
    
    int num_nn = (int)nn_dis.size();
    
    std::vector<dPoint1f> nlist = std::vector<dPoint1f>();
    int ind;
    for(int y = 0; y < num_nn; y++)
    {
        ind = (int)nn_sorted.at(y);
        nlist.push_back(dPoint1f(nn_ids.at(ind), nn_dis.at(ind)));
        
    }
    
    merged_list = Neighbors(merged_list.get_index(), nlist);
}
/**
 * Merge the list of this cluster with that of (Neighbors&) N
 */
void Cluster::merge_lists(const Neighbors& N, const int link_criteria){

    // get list of pairwise distances for both lists
    std::vector<dPoint1f> pdis1 = merged_list.get_samples();
    std::vector<dPoint1f> pdis2 = N.get_samples();
    std::vector<dPoint1f> pdis_tmp;
    if (link_criteria == 0)
    {   // append vector only for common NN
        // complete-link ANDs NN (i.e., both must contain it)
        
//        cv::bitwise_and(pdis1, pdis2, pdis1);
        
        std::vector<int> ids2 = N.get_sampleids();
        std::vector<int> ids1 = merged_list.get_sampleids();
        
        std::vector<float> dis2 = N.get_distances();
        std::vector<float> dis1 = merged_list.get_distances();
        
        dPoint1f tmp;
        for(std::vector<int>::size_type i = 0; i != ids2.size(); i++) {
            
            if ( has_sample(ids2.at(i))){
                tmp=dPoint1f(ids1[i],dis1[i]);
                pdis_tmp.push_back(tmp);

            }
        }
        merged_list.set_list(pdis_tmp);
        for(std::vector<int>::size_type i = 0; i != ids1.size(); i++) {
            if ( has_sample(ids1.at(i))){
                tmp=dPoint1f(ids2[i],dis2[i]);
                pdis_tmp.push_back(tmp);
                
            }
        }
        
        merged_list.set_list(pdis_tmp);
    }
    else if (link_criteria == 1){
        // append vector of distances to merged_list of this
        // single-link ORs NN (i.e., add if either one or both contains it)
        
        
        pdis1.insert( pdis1.end(), pdis2.begin(), pdis2.end() );
        merged_list.set_list(pdis1);
        
    }
   
    sort_list(link_criteria);
    
    // check for duplicate indices. In other words, only keep single distance if
    // both lists contain distances to same sample
    std::vector<int> ids = merged_list.get_sampleids();
    std::vector<float> dis = merged_list.get_distances();
    
    std::vector<dPoint1f> uset;
    std::vector<unsigned int> linenoVec; // Holds the line numbers of the unique instances only
    
    dPoint1f dpt;
    
    
    for (unsigned int i = 0; i < ids.size(); i++)
    {   // dupecheck
        bool exists = false;
        
        for (unsigned int x = 0; x < samples.size(); x++)
        {// first check that sample whether sample is in this cluster
            if (ids[i] == samples[x].get_index())
            {
                exists = true;
            }
        }
        if (exists == false)
        { // if not in cluster, then
            for (unsigned int x = 0; x < uset.size(); x++)
            {
                if (ids[i] == uset[x].j)
                {
                    exists = true;
                }
            }
        }
        if (exists == false)
        {
            dpt = dPoint1f(ids[i], dis[i]);
            uset.push_back(dpt);
        }
    }
    merged_list = Neighbors(merged_list.get_index(), uset);
}

