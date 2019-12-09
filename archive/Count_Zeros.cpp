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
	cout << N <<endl;
	N = N / 10;

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
    double end_time = omp_get_wtime();
    float seq_time = (end_time-start_time);
    printf("Sequentially %.3f seconds for %d Kmers.\n", seq_time, scount);
    myfile.close();

    myfile.open("random_lcp.txt");
    for(int i = 0; i < N; ++i)
    {
        myfile >> lcp_array[i];
    }
    double start_time2 = omp_get_wtime();
    
    omp_set_dynamic(0);
    omp_set_num_threads(12);
    #pragma omp parallel for
    for(int i = 0; i < N; ++i)
    {
        if(lcp_array[i] < 45)
            lcp_array[i] = 0;
        else
            lcp_array[i] = 1;
    }
   	int pcount = 0;

    #pragma omp parallel for reduction(+:pcount)
    for(int i = 0; i < N; ++i)
    {	
        if(lcp_array[i] == 0)
            pcount += 1;
	}

    double end_time2 = omp_get_wtime();
    float par_time = (end_time2-start_time2);
    printf("Parallelly %.3f seconds for %d Kmers. \n", pcount, par_time);
    printf("Speedup is: %.3f\n", seq_time/par_time);
    myfile.close();
	return 0;
}
