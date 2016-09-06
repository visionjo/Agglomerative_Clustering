/**
 * main.cpp
 * Clustering
 * This program demonstrates how to use STR's Agglomerative Clustering C++ Tools.
 *
 *
 *
 * [1] Otto, Charles, Dayong Wang, and Anil K. Jain. "Clustering millions
 * of faces by identity." arXiv preprint arXiv:1604.00989 (2016).
 *
 * Created by Robinson, Joseph on 6/16/2016
 * Copyright © 2016 Robinson, Joseph. All rights reserved.
 *
 */

#include <time.h>
#include "Clustering.hpp"
#include "cxxopts.hpp"



using namespace cv;
using namespace std;

template<typename T> string tostr(const T&);
void writeVecToFile(std::vector<int>&, const string);
static void read_csv(const string, cv::Mat&, int);
void writeVecOfVecToFile(std::vector<std::vector<float>>&, const string);

double eps = 2.0;

void app_agglomerative_demo();
void agglomerative_demo();
//void agglomerative_complete_link_ex();

string ifile;
string ofile;
string odir;
string linktype;
int ltype = 1;
int k;
// Global vars
clock_t t_init, t_final;
static void show_usage(string name, const int nargs, cxxopts::Options options, string missing = NULL);
static void print_help(const int nargs, const string name, cxxopts::Options options);
static void display_inputs(const int nargs, const string, cxxopts::Options options);
int main(int argc, char* argv[])
{
    /**
     * Parse command line inputs.
     */
    cxxopts::Options options(argv[0], " Agglomerative Clustering C++ Tools");
    try
    {
        options.add_options("Requirements")
        ("i,ifile", "Input CSV file containing distance matrix", cxxopts::value<string>(ifile), "    FILE")
        ("o,ofile", "Output filepath", cxxopts::value<string>(ofile), "    FILE");
        
        options.add_options("Optional")
        ("l,ltype", "Merge Criteria (0=complete-link; 1=single-link; 2=single-pass)", cxxopts::value<int>(ltype)->default_value("1"), "    INT")
        ("k,nclusters", "Number of clusters to predict", cxxopts::value<int>(k)->default_value("2"), "INT")
        ("e,eps", "Threshold for single-pass clustering", cxxopts::value<double>(eps)->default_value("2.6"), "      DOUBLE")
        ("h,help", "Print help");
        
        options.parse(argc, argv);
        
        string str(argv[0]);
        display_inputs(argc, str, options);
        
    } catch (const cxxopts::OptionException& e)
    {
        cout << "error parsing options: " << e.what() << endl;
        exit(1);
    }
    /**
     * Check that both input and output were specified; else, determine which is 
     * missing to print (display warning), then terminate.
     */
    if (ifile == "" || ofile == "")
    {
        string missing;
        
        if (ifile == "" && ofile == "")
            missing = "Missing INPUT data (.csv) & OUTPUT filepath for cluster IDs.";
        else if (ifile == "")
            missing = "Missing INPUT data (.csv).";
        else
            missing = "Missing OUTPUT filepath for cluster IDs.";
        
        string str(argv[0]);
        
        show_usage(str, argc, options, missing);
        return 0;
    }
    
    
    if (ltype == 0 || ltype == 1){
        if (ltype == 0)
        {
            cout << "Complete-link is buggy. Use single-link.\n";
            cout << "WARNING: Switching link criteria to single-link.";
            ltype = 1;
        }
        
        agglomerative_demo();
    }
    else if(ltype == 2){
        app_agglomerative_demo();}
    else{
            cout << "Linkage criteria " << ltype << " is undefined." << endl;
            cout << "-l [0:2] are supported. See --help for more details";
            cout << endl << endl;
    }
    
    return 0;
}
/**
 * Demo of doing agglomerative clustering using various linkage criteria.
 * Linkage Types:
 *          - Single Link
 */
void agglomerative_demo(){
    Mat dmatrix;
    
    
    /* Load Data */
//    if (constants::DO_DEBUG)
//        cout << "Agglomerative Clustering Demo (BEGIN)!\n\n";
    
//    read_csv(ifile, dmatrix, -1);

    
    
    string ref_name = "Agglomerative Clustering Demo";
    
    /* Clustering object C is*/
    Clustering C;
//    C.read_tuples(ifile, dmatrix);
//    
//    if (constants::DO_DEBUG)
//        cout << "Loaded " << dmatrix.rows << " samples to cluster.\n";
    
    C.set_reference_name(ref_name);
    C.set_linkage_criteria(ltype); // set to single-link
     C.initialize_tuples(ifile);
//    C.initialize_utri(dmatrix);
    if (constants::DO_DEBUG)
        cout << C;
    
    std::vector<int> cluster_ids;
    C.single_link(k, cluster_ids);
    
    
    if (constants::DO_DEBUG){
        for (int x = 0; x < cluster_ids.size(); x++){
            cout<< "[" << x << "]\t " << cluster_ids.at(x) << endl;
        }
    }
    writeVecToFile(cluster_ids, ofile);
    
    //    
}
/**
 * Demo of doing transitive merging through a single pass.
 */
void app_agglomerative_demo(){
    Mat dmatrix;
    /* Load Data */
    if (constants::DO_DEBUG)
        cout << "Approximate Agglomerative Clustering Demo (BEGIN)!\n\n";
    
    // write distances estimated via ADC to file
    read_csv(ifile, dmatrix, -1);        // read training data
    //    vector<float> temp = dmatrix.row(0);
    if (constants::DO_DEBUG)
        cout << "Loaded " << dmatrix.rows << " samples to cluster.\n";
    
    
    std::vector<int> cluster_ids;
    Clustering C;
    C.single_pass(dmatrix, eps, cluster_ids);
    
    if (constants::DO_DEBUG)
        cout << "\nWriting " << ofile << endl;
    
    writeVecToFile(cluster_ids, ofile);
    
    
    if (constants::DO_DEBUG)
        cout << "Rank Order Clustering Demo (END)!\n\n";
    
    
}
/**
 * Displays options of executable. Called when --help (or -h) is pass in as arg or when
 * required input args (i.e., ifile and ofile) are not specified.
 */
static void show_usage(string name, const int nargs, cxxopts::Options options, string missing)
{
    cout << "ERROR: missing required inputs ** " << name << " **" << endl << endl;
    cout << "Program requires -i and -o files. For example:"<< endl << "\t";
    cout << name << " -i data_in.csv -o cluster_ids.txt" << endl << endl;
    cout << missing << endl<< endl;
    for(int x = 0; x < 18; x++){
        cout << "****";
    }cout << endl;
    cout << "\t\t\t\t********** HELP **********" << endl;
    for(int x = 0; x < 18; x++){
        cout << "****";
    }cout << endl;
    print_help(nargs, name, options);
    
}
/**
 * Detailed printout specifying program usage.
 */
static void print_help(const int nargs, const string name, cxxopts::Options options)
{
    for(int x = 0; x < 18; x++){
        cout << "****";
    }cout << endl;
    cout << options.help({"Requirements","Optional"});
    for(int x = 0; x < 18; x++){
        cout << "****";
    }cout << endl << endl;
    
    cout << "Sample command line call:" << endl << "\t" << name;
    cout << " -i <DATADIR>/distance_matrix.csv -o";
    cout << " <OUTDIR>cluster_ids.txt -l 1 -k 4" << endl << endl;
    
    cout << "The usage exemplified above would generate 4 clusters for 'distance_matrix.csv'";
    cout << " using single-linkage criteria and dumping results (i.e., cluster ids) in 'cluster_ids.txt'." << endl << endl;
    
    cout << "Equivalently:" << endl << "\t" << name;
    cout << " -ifile <DATADIR>/distance_matrix.csv -ofile";
    cout << " <OUTDIR>cluster_ids.txt -ltype 1 -nclusters 4" << endl << endl;
    
    cout << "Also equivalent:" << endl << "\t" << name;
    cout << " -i <DATADIR>/distance_matrix.csv -d <OUTBIN> -o";
    cout << " cluster_ids.txt -l 1 -k 4" << endl << endl;
    
    cout << "Note, command line arguments can be passed in in any order. However, flags are required; else, the input argument will be ignored" << endl << endl;
    for(int x = 0; x < 18; x++){
        cout << "****";
    }cout << endl << endl;
    
    exit(0);
}



static void display_inputs(const int nargs, const string name, cxxopts::Options options)
{

    if (options.count("help") || nargs == 0)
    {
//        std::cout << options.help({"Required","Optional"}) << std::endl;
        print_help(nargs, name, options);
    }
    
    if (options.count("ifile"))
    {
        cout << "Input = " << options["ifile"].as<string>()
        << endl;
    }
    
    if (options.count("odir"))
    {
        cout << "Output = " << options["odir"].as<string>()
        << endl;
    }
    
    if (options.count("l"))
    {
        cout << "linkage type = " << options["ltype"].as<int>() << endl;
    }
    if (options.count("k"))
    {
        cout << "No. of clusters = " << options["nclusters"].as<int>() << endl;
    }
    if (constants::DO_DEBUG)
        cout << "Arguments remain = " << nargs << endl;
}

/**
 * (TEMP) Function to read in data (i.e., SIFT features) in CSV file format.
 */
static void read_csv(const string filename, cv::Mat& data, int nrows) {
    
    ifstream inputfile(filename);
    string current_line;
    // vector allows you to add data without knowing the exact size beforehand
    std::vector<std::vector<float> > all_data;
    int counter = 1;
    // Start reading lines as long as there are lines in the file
    while(getline(inputfile, current_line)){
        // Now inside each line we need to seperate the cols
        std::vector<float> values;
        stringstream temp(current_line);
        string single_value;
        while(getline(temp,single_value,',')){
            // convert the string element to a integer value
            values.push_back(stof(single_value.c_str()));
        }
        // add the row to the complete data vector
        all_data.push_back(values);
        
        if (counter == nrows)
            break;
        counter++;
        
    }
    
    if ((nrows == -1) || (nrows > (int)all_data.size()))
        nrows = (int)all_data.size();
    
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
void writeVecToFile(std::vector<int>& m, const string filename)
{
    ofstream fout(filename);
    
    if(!fout)
    {
        cout<<"File Not Opened"<<endl;  return;
    }
    
    for(int i=0; i<m.size(); i++)
    {
        fout<<m.at(i)<<"\t";
    }
    
    fout.close();
}
template <typename T> string tostr(const T& t) {
    ostringstream os;
    os<<t;
    return os.str();
}

void writeVecOfVecToFile(std::vector<std::vector<float>>& v2D, const string filename)
{
    ofstream fout(filename, ofstream::binary);
    for(size_t i = 0; i < v2D.size(); i++ )
    {
        if ( v2D[i].size() > 0 )
        {
            std::vector<float> cvec = v2D.at(i);
            const char* buffer = (const char*)(&cvec);
            //            const char* buffer = static_cast<const char*>(&v2D[i][0]);
            fout.write(buffer, v2D[i].size());
        }
    }
    
    fout.close();
}


