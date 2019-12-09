// basic file operations
#include <iostream>
#include <fstream>
#include <random>
#include <cstdlib>
#include <deque>
#include <omp.h>
using namespace std;

int main(int argc, char *argv[]){
	cout << omp_get_max_threads() << endl;
    int N;
    int K = 45;
    int x;
    ifstream myfile;
    if(argc > 1){
        N = atoi(argv[1]);
    }
    else{
        myfile.open("metadata_random_lcp.txt");
        myfile >> N;
        myfile.close();
    }
    cout << N << endl;

    uint8_t *lcp_array = new uint8_t [N];
    myfile.open("random_lcp.txt");
    for(int i = 0; i < N; ++i)
    {
        myfile >> x;
        lcp_array[i] = (uint8_t)x;
    }
    double start_time = omp_get_wtime();
    for(int i = 0; i < N; ++i)
    {
        if(lcp_array[i] < (uint8_t)K)
            lcp_array[i] = (uint8_t)0;
        else
            lcp_array[i] = (uint8_t)1;
    }

    int valid_kmer_count = 0;
    deque<int64_t> vect;
    int j;

    for(int i = N - 1; i >= 3; --i)
    {
        if(lcp_array[i] == (uint8_t)0)
        {
            for(j = i - 1; j >= i - 3; --j)
            { 
                if(lcp_array[j] == (uint8_t)0)
                    break; 
            }
            if(j == (i - 4))
            {
                vect.push_front((int64_t)i);
                valid_kmer_count++;
            }
            i = j;
        }
    }
    double end_time = omp_get_wtime();
    float seq_time = (end_time-start_time);
    printf("Sequentially %.3f seconds for counting %d valid-Kmers.\n", seq_time, valid_kmer_count);
    cout << "Here are first 10 indices of valid kmers." << endl;
    deque<int64_t>::iterator it = vect.begin();
    int iter = 0; 
    while(iter < 10){
        cout << *it << ", ";
        it++;
        iter++;
    }
    cout << endl;

    myfile.close();

 //    myfile.open("random_lcp.txt");
 //    for(int i = 0; i < N; ++i)
 //    {
 //        myfile >> x;
 //        lcp_array[i] = (uint8_t)x;
 //    }
 //    double start_time2 = omp_get_wtime();
    
 //    omp_set_dynamic(0);
 //    omp_set_num_threads(12);
 //    #pragma omp parallel for
 //    for(int i = 0; i < N; ++i)
 //    {
 //        if(lcp_array[i] < (uint8_t)K)
 //            lcp_array[i] = (uint8_t)0;
 //        else
 //            lcp_array[i] = (uint8_t)1;
 //    }
 //   	int pcount = 0;

 //    #pragma omp parallel for reduction(+:pcount)
 //    for(int i = 0; i < N; ++i)
 //    {	
 //        if(lcp_array[i] == (uint8_t)0)
 //            pcount += 1;
	// }

 //    double end_time2 = omp_get_wtime();
 //    float par_time = (end_time2-start_time2);
 //    printf("Parallelly %.3f seconds for %d Kmers. \n", pcount, par_time);
 //    printf("Speedup is: %.3f\n", seq_time/par_time);
 //    myfile.close();
	return 0;
}
