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



/**
 *
 */
void read_tuple(const std::string filename, cv::Mat& data) {
    
    std::ifstream inputfile(filename);
    std::string current_line;
    // vector allows you to add data without knowing the exact size beforehand
    std::vector<std::vector<float> > all_data;
    int counter = 1;
    getline(inputfile, current_line);   // read/ skip header
    // Start reading lines as long as there are lines in the file
    while(getline(inputfile, current_line)){
        // Now inside each line we need to seperate the cols
        std::vector<float> values;
        std::stringstream temp(current_line);
        std::string single_value;
        while(getline(temp,single_value,',')){
            // convert the string element to a integer value
            values.push_back(std::stof(single_value.c_str()));
        }
        // add the row to the complete data vector
        all_data.push_back(values);
        counter++;
        
    }
    
    int nrows = (int)all_data.size();
    
    // Now add all the data into a Mat element
    data = cv::Mat1f(nrows, (int)all_data[0].size());
    
    // Loop over vectors and add the data
    for(int rows = 0; rows < nrows; rows++){
        for(int cols= 0; cols< (int)all_data[0].size(); cols++){
            data.at<float>(rows,cols) = all_data[rows][cols];
        }
    }
    
    //    cout << "M = "<< endl << " "  << data << endl << endl;
}
