
#ifndef ClusterUtils_hpp
#define ClusterUtils_hpp

#include <stdio.h>

#include <queue>
#include <string>
#include <vector>
#include <list>
#include <map>

#include <fstream>
#include <sstream>
#include <iostream>
#include <ostream>

#include <iomanip>

#include <cstddef>

#include <algorithm>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include "Sample.hpp"

std::vector<size_t> sort_indices(std::vector<float>&);
namespace constants
{
    const float MAX_DISTANCE = 100.0;
    const std::string vinit[] =   // different merging criteria
    {"completelink","singlelink","singlepass"};
    const bool DO_DEBUG = true;
    
    
} // namespace constants
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Pair of indices, target cluster
class Pair2i {
public:
    int i;  // row index (y) corresponds to (current) feature index
    int j;  // col index (x) correponds to (other) feature index
    int cluster_ids; // cluster to be assinged to
    Pair2i(int i_pt, int j_pt, int c_pt) {
        i = i_pt;
        j = j_pt;
        cluster_ids = c_pt;
    };
    void operator=(const Pair2i &P )
    {
        i = P.i;
        j = P.j;
        cluster_ids = P.cluster_ids;
    }
};

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Points of distance matrix D; Matrix D is assumed to be a sparse matrix, i.e.,
// many samples are an infinite distance from one another; Thus, this class is
// used within a vector of vectors to represent distances if ~isinf(distance)
class dPair2f {
public:
    int i;  // row index (y) corresponds to (current) feature index
    int j;  // col index (x) correponds to (other) feature index
    float distance; // distance between ith and jth samples
    dPair2f(int i_pt=0, int j_pt=0, float d_pt = 0.0) {
        i = i_pt;
        j = j_pt;
        distance = d_pt;
    };
    void operator=(const dPair2f &D )
    {
        i = D.i;
        j = D.j;
        distance = D.distance;
    }
    // overloaded << (i.e., cout)
    friend std::ostream &operator<<(std::ostream &out, const dPair2f& P)     //output
    {
        out<< "\nPairwise distance d(" << P.i << ", " << P.j << ")";
        out<< " = " << P.distance << "\n";
        return out;
    }
    
    
    void compare(dPair2f &D1)
    {
        if (D1.distance < distance)
        {
            i = D1.i;
            j = D1.j;
            distance = D1.distance;
        }
    }
};


//--------------------------------------------------------------------------
// distance matrix containing points (i.e., type dPoint2f)
class dMatrix {
public:
    std::vector<std::vector<dPair2f>> D;  // row index (y) corresponds to (current) feature index
    dMatrix() {
        D = std::vector<std::vector<dPair2f>>();
    };
};

//--------------------------------------------------------------------------
// each instanct is a distance from sample j
class dPoint1f {
public:
    int j;  // col index (x) correponds to (other) feature index
    float distance; // distance between ith and jth samples
    dPoint1f(int j_pt=0, float d_pt = 0.0) {
        j = j_pt;
        distance = d_pt;
    };
};

dPair2f min_dis(cv::Mat m);
std::vector<size_t> sort_indices(std::vector<float> &v);
void init_singletons(std::vector<int>&, int);
void set_cluster_ids(std::vector<int>& cluster_ids);
std::vector<int> unique(const cv::Mat& input, bool sort = false);
void read_tuple(const std::string filename, cv::Mat& data);
#endif