/**
 * agglomerative.hpp
 * Agglomerative
 *
 * This header file contains the class rank order (RO) which uses RO distance to
 * measure distances between samples and the top k-NN, resulting in distance
 * matrix D. D is then compared to a threshold, and distances below threshold
 * clustered together.
 *
 * Class acts as follows:
 *  - Provided a distance matrix D, clustering is done via an agglomerative
 * approach: transitively merge w.r.t. single-links between pairs in 1 iteration
 *
 *
 * @todo
 * Created by Robinson, Joseph on 8/25/16.
 */

#ifndef agglomerative_hpp
#define agglomerative_hpp

#include "Clustering.hpp"

class agglomerative
// agglomerative clustering methods
{
public:
    agglomerative(){};
    ~agglomerative();

    void single_link(Clustering&, int k,  std::vector<int>&);
    void complete_link(Clustering&, int k,  std::vector<int>&);
    void single_pass(cv::Mat&, double,  std::vector<int>&);
    
};

#endif  /* agglomerative_hpp */
