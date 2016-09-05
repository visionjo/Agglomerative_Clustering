//
//  ClusterUtils.cpp
//  Agglomerative
//
//  Created by Robinson, Joseph on 8/25/16.
//  Copyright © 2016 Robinson, Joseph. All rights reserved.
//

#include "ClusterUtils.hpp"
dPair2f min_dis(cv::Mat m) {
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc( m, &minVal, &maxVal, &minLoc, &maxLoc );
    
    dPair2f p;
    p.i = (float)minLoc.y;
    p.j = (float)minLoc.x;
    p.distance = (float)minVal;
    return (p);
}

std::vector<size_t> sort_indices(std::vector<float> &v) {
    
    // initialize original index locations
    std::vector<size_t> idx(v.size());
    for (size_t i = 0; i != idx.size(); ++i) idx[i] = i;
    
    // sort indexes based on comparing values in v
    std::sort(idx.begin(), idx.end(),
              [&v](size_t i1, size_t i2) {return v[i1] < v[i2];});
    
    return idx;
}

void init_singletons(std::vector<int>& singletons, int nsamples){
    for (int x = 0; x < nsamples; x++)
    {   // initialize all samples to its own cluster 1<=k<=N
        
        singletons.push_back(x);
    }
}
std::vector<int> unique(const cv::Mat& input, bool sort)
{
    if (input.channels() > 1)
    {
        std::cerr << "unique !!! " << std::endl;
        return std::vector<int>();
    }
    
    std::vector<int> out;
    for (int y = 0; y < input.rows; ++y)
    {
        const int* row_ptr = input.ptr<int>(y);
        for (int x = 0; x < input.cols; ++x)
        {
            int value = row_ptr[x];
            
            if ( std::find(out.begin(), out.end(), value) == out.end() )
                out.push_back(value);
        }
    }
    
    if (sort)
        std::sort(out.begin(), out.end());
    
    return out;
}

void set_cluster_ids(std::vector<int>& cluster_ids){
    
    cv::Mat cmat = cv::Mat(cluster_ids);
    std::vector<int> cids = unique(cmat);

    
    
    int ncluster = (int)cids.size();
    for (int x = 0; x < ncluster; x++)
    {   // initialize all samples to its own cluster 1<=k<=N
        
       std::replace (cluster_ids.begin(), cluster_ids.end(), cids[x], x);
        
    }
    
}

