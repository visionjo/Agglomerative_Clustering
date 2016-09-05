//
//  Item.hpp
//  Clustering_Faces
//
//  Created by Robinson, Joseph on 8/25/16.
//  Copyright © 2016 Robinson, Joseph. All rights reserved.
//

#ifndef Item_hpp
#define Item_hpp
#include "ClusterUtils.hpp"

/**
 *
 */
//--------------------------------------------------------------------------
// Neigbors are contained with every Node, i.e., each Node contains a list of NN
// Neigbors are represented as dPoint1f (distance & corresponding item index)
class Neighbors {
public:
    
    /* Constructors */
    Neighbors(int ids = 0, std::vector<dPoint1f> ilist = std::vector<dPoint1f>()) {
        index = ids;
        if (ilist.size() == 0){
            list = ilist;
            return;
        }
        for (int x = 0; x < ilist.size(); x++) {
            list.push_back(ilist.at(x));
        }
        sort_nn();
        
    };
    // copy constructor
    Neighbors(Neighbors const &N) {
        this->list = N.list;
        this->index = N.index;
    };
    
    /* Utils */
    ~Neighbors(){};
    void sort_nn();

    void add_neighbor(dPoint1f &dpt){
        list.push_back(dpt);
    };
    
    /* Get Functions */
    std::vector<dPoint1f>& get_list() {return list;};
    std::vector<float> get_distances() const;
    std::vector<int> get_sampleids() const;
//    std::vector<dPoint1f> get_samples() {return list;}

    const std::vector<dPoint1f> &get_samples() const
    {
        return list;
    }
    
    
    int get_index() const{return index;};
    int get_nncount() const{return (int)list.size();};
    dPoint1f get_kth_nn(const int k) const{return list.at(k);};
    
    /* Set Functions */
    void set_index(const int i){index=i;};
    void set_list(const std::vector<dPoint1f>& l){
        list=l;
        sort_nn();
    };
    
    /* Overloaded Operators */
    void operator=(const Neighbors &N );

    
    // overloaded << (i.e., cout)
    friend std::ostream &operator<<(std::ostream &out, const Neighbors& N)     //output
    {
        std::string hdr="****************************************************\n";
        out<< hdr;
        out<<"******\t\tNN List for Sample "<<N.index<<"\t\t******\n";
        std::vector<int> ids = N.get_sampleids();
        std::vector<float> dis = N.get_distances();
        out<<"[ID]\t distance\n____________\n";
        for (int x = 0; x < ids.size(); x++)
        {
            out<<"[" << ids[x] << "]\t " << dis[x] << std::endl;
        }
        out<< hdr << std::endl;
        return out;
    }
    
private:
    std::vector<dPoint1f> list;  // set of distances and corresponding indices
    int index;  // index reference of object. Should be same as Node.label
};

/**
 * Node represents items from point of singletons through there cluster assignments.
 * Node object allows for better tracking of
 */
class Sample {
    
public:
    Sample(int id = 0, bool r=true, int h = 0)  /* Constructor */
    : index(id), root(r), height(h){};
    Sample(int id, bool r, int h, Neighbors ilist)  /* Constructor */
    : index(id), root(r), height(h), ineighbors(ilist){
        
    };
    ~Sample(){};
    
    // access to root member (i.e., root is Item that has yet to merge)
    void is_root(const bool r){ root = r;};
    bool is_root()const{return root;};
    
    /* Set Functions */
    void set_neighbors(const Neighbors& n);
    void set_index(const int id){ index = id;};
    void set_height(const int h){ height = h;};
    
    
    /* Get Functions */
    Neighbors get_neighbors()const{    return ineighbors;   };
    int get_nneighbors() const  { //        return nneighbors;
        return ineighbors.get_nncount();
    };
    int get_height()const{return height;};
    int get_index()const{return index;};
    
    // overloaded << (i.e., cout)
    friend std::ostream &operator<<(std::ostream &out, const Sample& S)     //output
    {
        out<< "\nSample No. " << S.index << ":\n\t";
        out<< "isroot:\t" << S.root << "\n\t";
        out<<"height:\t\t"<<S.height<<"\n\t";
        out<<"No. Neighbors:\t"<<S.ineighbors.get_nncount() <<"\n";
        return out;
    }
    
private:
    bool root;    /* true if cluster hasn't merged with another */
    int height;     /* height of node from the bottom */
    int index;      /* index of sample (i.e., leaf node) */
    Neighbors ineighbors;  /* sorted linked list of distances to roots */
    
};



#endif /* Item_hpp */