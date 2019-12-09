// basic file operations
#include <iostream>
#include <fstream>
#include <random>
#include <cstdlib>
#include <omp.h>
using namespace std;

int main(int argc, char *argv[]){
	cout << omp_get_max_threads() << endl;
	int N;
    int K = 45;
	ifstream myfile;
	myfile.open("metadata_random_lcp.txt");
	myfile >> N;
	myfile.close();
	N = N / 2;
    cout << N << endl;

    int *lcp_array = new int [N];
    myfile.open("random_lcp.txt");
    for(int i = 0; i < N; ++i)
    {
        myfile >> lcp_array[i];
    }
    double start_time = omp_get_wtime();
    for(int i = 0; i < N; ++i)
    {
        if(lcp_array[i] < 45)
            lcp_array[i] = 0;
        else
            lcp_array[i] = 1;
    }
    int scount = 0;
    for(int i = 0; i < N; ++i)
    {
        if(lcp_array[i] == 0)
            scount += 1;
    }

    // NOW LOOK FOR 10 PATTERNS  ############################
    int *temp_0 = new int [N-1]; // store 0s
    int *temp_1 = new int [N-1]; // store 1s
    for(int i = 0; i < N - 1; ++i) // initialize to zero
    {
        temp_0[i] = 0;
        temp_1[i] = 0;
    }
    for(int i = 0; i < N - 1; ++i) // temp_1 = Vec[:-1] == 1
    {   
        if(lcp_array[i] == 1)
            temp_1[i] = 1;    
    }
    for(int i = 1; i < N; ++i) // temp_0 = Vec[1:] == 0
    {   
        if(lcp_array[i] == 0)
            temp_0[i-1] = 1;    
    }

    for(int i = 0; i < N - 1; ++i) // temp_1 & temp_0
    {
        temp_0[i] = temp_0[i] & temp_1[i];
    }

    int count_10 = 0; 
    for(int i = 0; i < N - 1; ++i) // np.sum(temp_1 & temp_0)
    {
        if(temp_0[i] == 1)
            count_10 += 1;
    }

    double end_time = omp_get_wtime();
    float seq_time = (end_time-start_time);
    printf("Sequentially %.3f seconds for %d Kmers, where %d appeared only once.\n", seq_time, scount, count_10);
    printf("%d kmers appear more than once\n", scount - count_10);

    // ############# START PARALLEL COMPUTING #################

    double start_time2 = omp_get_wtime();
    
    omp_set_dynamic(0);
    omp_set_num_threads(12);

    int pcount = 0;

    #pragma omp parallel for reduction(+:pcount)
    for(int i = 0; i < N; ++i)
    {   
        if(lcp_array[i] == 0)
            pcount += 1;
    }

    #pragma omp parallel for 
    for(int i = 0; i < N - 1; ++i) // temp_1 = Vec[:-1] == 1
    {	
        if(lcp_array[i] == 1)
            temp_1[i] = 1;
	}
    #pragma omp parallel for 
    for(int i = 0; i < N - 1; ++i) // temp_0 = Vec[1:] == 0
    {   
        if(lcp_array[i] == 0)
            temp_0[i-1] = 1;   
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
        if(temp_0[i] == 1)
            parallel_count_10 += 1;
    }
    double end_time2 = omp_get_wtime();
    float par_time = (end_time2-start_time2);
    printf("Parallelly %.3f seconds for %d Kmers, where %d appeared only once.\n", par_time, pcount, parallel_count_10);
    printf("%d kmers appear more than once\n", pcount - parallel_count_10);
    printf("Speedup is: %.3f\n", seq_time/par_time);
    myfile.close();
	return 0;
}
