/*
Authors: Md Taksir Hasan Majumder (PID: 730176270), Thanh Vu (PID: 730268213)
Target: COMP 633 FINAL Project
INSTRUCTOR: Dr. JAN PRINS
GITHUB REPO: https://github.com/thanhmvu/parallel633

An LCP array is used as an auxiliary data structure for an MSBWT. Here, we demonstrate how the LCP array can be used to count the total number of KMERS and the number
of Kmers among them that appear once. 
Kmers appearing once are considered as noises due to errors in reads. Also, we show the indices of the KMERS (that appear once) can also be obtained efficiently 
with parallel implementation.

Our reads are strings of length 151 with a '$' sign at the end as a marker. An element at LCP array, LCP[j] = k, whenever the jth read in an uncompressed MSBWT shares k longest common
prefix with the (j+1)th read in the same uncompressed msbwt. Therefore, the largest LCP value can be 151 and the lowest can be 0.

An LCP array can be: (151, 121, 42, 40, 10, 120, 0, 1)

Our implementation is based on the idea that we will not distinguish between ACGNT and $.
Our logic is as follows:

Suppose, a Kmer appears T times. It will definitely have (T - 1) consecutive values >= K in the LCP array (from its starting position at the LCP array). The next value will be
less that K, indicating that it shares less than K characters at the beginning with the next read. We convert the LCP array to an array consisting of 1 and 0s only based on this 
idea. If an element is >= 45, convert it to 1, else 0. Counting how many zeros there are is equivalent to counting the total number of Kmers.

The LCP array above will be now: (1, 1, 0, 0, 0, 1, 0, 0)

Simply count the zeros to get the number of Kmers.

def CountNumberofKmers(array LCP):
	do
		for each element, e in LCP: // divided equally among the processors
			if e >= 45:
				temp_LCP[index of e] = 1
			else:
				temp_LCP[index of e] = 0
		for each element, e in temp_LCP: // divided equally among the processors
			use count the number of zeros locally and then add them parallely to get the final count of Kmers
	end

Now, to count the number of kmers that appear once, we observe that a Kmer appearing only once has a 0 at its index, and is preceded by another 0. Such pattern is impossible to 
appear elsewhere.

Algo CountNumberofKmersAppearingOnce(array LCP):
	STEP 1: CONVERT LCP array to a binary_LCP array // divided equally among the processors
	STEP 2: get shifted_binary_LCP array by shifting all elements once to the right
	STEP 3: Perform AND operations among all elements of binary_LCP and shifted_binary_LCP
	STEP 4: Count the number of zeros in the result array in STEP 3

def get_indices_of_noisy_Kmers_appearing_once(Vec, K):
	a = (Vec >= K).astype('int8')
    print(a)
    b = np.roll(a, 1) // here np is short form of numpy in python
    c = np.add(a,b)
    d = np.where(c==0)[0] // d is a numpy array containing the indices of the noisy kmers
*/


#include <iostream>
#include <fstream>
#include <random>
#include <cstdlib>
#include <omp.h>
#include <bits/stdc++.h> 
using namespace std;

int main(int argc, char *argv[]){
    cout << "Max Threads  = " << omp_get_max_threads() << endl; 
    int N, temp_n;
    int K = 45;
    int P, x;
    ifstream myfile;
    if(argc == 2){
        N = atoi(argv[1]);
      	myfile.open("metadata_random_lcp.txt");
        myfile >> temp_n;
        if(N > temp_n)
        	N = temp_n;
        myfile.close();
    }
    else{
        myfile.open("metadata_random_lcp.txt");
        myfile >> N;
        // N = N / 10;
        myfile.close();
    }
    cout << "Total lcp values = " << N << endl;
    if(argc == 3){
        N = atoi(argv[1]);
        P = atoi(argv[2]);
        if(P > omp_get_max_threads())
            P = omp_get_max_threads();
        cout << "Total threads to be used = " << P << endl;
    }

    uint8_t *lcp_array = new uint8_t [N];
    myfile.open("random_lcp.txt");
    for(int i = 0; i < N; ++i)
    {
        myfile >> x;
        lcp_array[i] = (uint8_t)x;
    }
    for(int i = 0; i < N; ++i)
    {
        if(lcp_array[i] < (uint8_t)K)
            lcp_array[i] = (uint8_t)0;
        else
            lcp_array[i] = (uint8_t)1;
    }
    double start_time = omp_get_wtime();
    int scount = 0;
    for(int i = 0; i < N; ++i)
    {
        if(lcp_array[i] == (uint8_t)0)
            scount += 1;
    }

    // NOW LOOK FOR 10 PATTERNS  ############################
    uint8_t *temp_0 = new uint8_t [N-1]; // store 0s
    uint8_t *temp_1 = new uint8_t [N-1]; // store 1s
    for(int i = 0; i < N - 1; ++i) // initialize to zero
    {
        temp_0[i] = (uint8_t)0;
        temp_1[i] = (uint8_t)0;
    }
    for(int i = 0; i < N - 1; ++i) // temp_1 = Vec[:-1] == 1
    {   
        if(lcp_array[i] == (uint8_t)1)
            temp_1[i] = (uint8_t)1;    
    }
    for(int i = 1; i < N; ++i) // temp_0 = Vec[1:] == 0
    {   
        if(lcp_array[i] == (uint8_t)0)
            temp_0[i-1] = (uint8_t)1;    
    }

    for(int i = 0; i < N - 1; ++i) // temp_1 & temp_0
    {
        temp_0[i] = temp_0[i] & temp_1[i];
    }

    int count_10 = 0; 
    for(int i = 0; i < N - 1; ++i) // np.sum(temp_1 & temp_0)
    {
        if(temp_0[i] == (uint8_t)1)
            count_10 += 1;
    }

    uint8_t * shifted_lcp_array = new uint8_t [N];
    shifted_lcp_array[0] = lcp_array[N - 1];
    for(int i = 1; i < N; ++i){
        shifted_lcp_array[i] = lcp_array[i - 1];
    }

    uint8_t *temp_lcp_array = new uint8_t [N];
    for(int i = 1; i < N; ++i){
        temp_lcp_array[i] = lcp_array[i] + shifted_lcp_array[i];
    }

    vector<int64_t> vect; 
    for(int i = 1; i < N; ++i){
        if(temp_lcp_array[i] == (uint8_t)0)
            vect.push_back((int64_t)i);
    }

    double end_time = omp_get_wtime();
    float seq_time = (end_time-start_time);
    printf("Sequentially %.3f seconds for %d Kmers, where %d appeared only once.\n", seq_time, scount, count_10);
    printf("%d kmers appear more than once\n", scount - count_10);

    cout << "Here are first 10 indices of noisy kmers." << endl;
    vector<int64_t>::iterator it = vect.begin();
    int iter = 0; 
    while(iter < 10){
        cout << *it << ", ";
        it++;
        iter++;
    }
    cout << endl;
    vect.clear();
    cout << "Vector is cleared. new size: " << vect.size() << endl << endl;

    // ############# START PARALLEL COMPUTING #################

    double start_time2 = omp_get_wtime();
    
    omp_set_dynamic(0);
    omp_set_num_threads(12);
    int pcount = 0;

    #pragma omp parallel for reduction(+:pcount)
    for(int i = 0; i < N; ++i)
    {   
        if(lcp_array[i] == (uint8_t)0)
            pcount += 1;
    }

    #pragma omp parallel for 
    for(int i = 0; i < N - 1; ++i) // temp_1 = Vec[:-1] == 1
    {   
        if(lcp_array[i] == (uint8_t)1)
            temp_1[i] = (uint8_t)1;
    }
    #pragma omp parallel for 
    for(int i = 0; i < N - 1; ++i) // temp_0 = Vec[1:] == 0
    {   
        if(lcp_array[i] == (uint8_t)0)
            temp_0[i-1] = (uint8_t)1;   
    }
    #pragma omp parallel for 
    for(int i = 0; i < N - 1; ++i) // temp_1 & temp_0
    {
        temp_0[i] = temp_0[i] & temp_1[i];
    }
    int parallel_count_10 = 0;
    #pragma omp parallel for reduction(+:parallel_count_10)
    for(int i = 0; i < N - 1; ++i) // np.sum(temp_1 & temp_0)
    {
        if(temp_0[i] == (uint8_t)1)
            parallel_count_10 += 1;
    }

    shifted_lcp_array[0] = lcp_array[N - 1];
    #pragma omp parallel for
    for(int i = 1; i < N; ++i){
        shifted_lcp_array[i] = lcp_array[i - 1];
    }
    #pragma omp parallel for
    for(int i = 1; i < N; ++i){
        temp_lcp_array[i] = lcp_array[i] + shifted_lcp_array[i];
    }
    //////////////////////////////////////////////////////////////////////////////////////
    vector <int64_t> par_vect;
    cout << "Entering parallel vector region." << endl; 
    #pragma omp parallel shared(temp_lcp_array)
    {
        vector<int64_t> vec_private;
        #pragma omp for nowait schedule(static)
        for(int i=0; i<N; i++) {
            if(temp_lcp_array[i] == 0) 
                vec_private.push_back((int64_t)i);
        }
        #pragma omp for schedule(static) ordered
        for(int i=0; i<omp_get_num_threads(); i++) {
            #pragma omp ordered
            par_vect.insert(par_vect.end(), vec_private.begin(), vec_private.end());
        }
    }

    double end_time2 = omp_get_wtime();
    float par_time = (end_time2-start_time2);
    cout << "Size of parallel vector is: " << par_vect.size() << endl;
    printf("Parallelly %.3f seconds for %d Kmers, where %d appeared only once.\n", par_time, pcount, parallel_count_10);
    printf("%d kmers appear more than once\n", pcount - parallel_count_10);
    printf("Speedup is: %.3f\n", seq_time/par_time);

    cout << "Here are first 10 indices of noisy kmers." << endl;
    it = par_vect.begin();
    it++; // first elem is zero, neglect it.
    iter = 0; 
    while(iter < 10){
        cout << *it << ", ";
        it++;
        iter++;
    }
    cout << endl;

    myfile.close();
    return 0;
}


// void readRandomLcp(){
//  myfile.open("metadata_random_lcp.txt");
//  myfile >> N;

//  cout << N;
//  myfile.close();

//     myfile.open("random_lcp.txt");
//     for(int i = 0; i < N; ++i)
//     {
//         myfile >> lcp_array[i];
//         cout << lcp_array[i] <<endl;
//     }
//     myfile.close();
// }