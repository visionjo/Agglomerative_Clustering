//
//  Item.cpp
//  Agglomerative
//
//  Created by Robinson, Joseph on 8/25/16.
//  Copyright © 2016 Robinson, Joseph. All rights reserved.
//

#include "Sample.hpp"

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// NEIGHBORS
//--------------------------------------------------------------------------
void Neighbors::sort_nn(){
    //    neighbors
    std::vector<float> nn_dis=get_distances();
    
    std::vector<int> nn_ids=get_sampleids();
    
    
    std::vector<size_t>nn_sorted= sort_indices(nn_dis);
    
    
    
    int num_nn = (int)nn_dis.size();
    
    std::vector<dPoint1f> nlist = std::vector<dPoint1f>();
    int ind;
    for(int y = 0; y < num_nn; y++)
    {
        ind = (int)nn_sorted.at(y);
        nlist.push_back(dPoint1f(nn_ids.at(ind), nn_dis.at(ind)));
        
    }
    list = nlist;
}

void Neighbors::operator=(const Neighbors &N )
{
    list = N.list;
    index = N.index;
}


cv::vector<float> Neighbors::get_distances() const{
    int nvals = (int)list.size();
    cv::vector<float> dvec = cv::vector<float>(nvals);
    
    for (int x = 0; x < nvals; x++){
        dvec.at(x) = list.at(x).distance;
    }
    return dvec;
}

std::vector<int> Neighbors::get_sampleids() const{
    int nvals = (int)list.size();
    std::vector<int> ivec;
 
    for (int x = 0; x < nvals; x++){
        ivec.push_back(list.at(x).j);
//        ivec.at(x) = list.at(x).j;
    }
    return ivec;
}


void Sample::set_neighbors(const Neighbors& n){
    ineighbors=Neighbors(n);
    ineighbors.sort_nn();
};