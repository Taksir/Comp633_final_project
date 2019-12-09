// basic file operations
#include <iostream>
#include <fstream>
#include <random>
#include <cstdlib>
#include <omp.h>
using namespace std;

int main(int argc, char *argv[]){
	int N;
    int K = 45;
	ifstream myfile;
	myfile.open("metadata_random_lcp.txt");
	myfile >> N;
	cout << N <<endl;
	myfile.close();

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
    double end_time = omp_get_wtime();
    float seq_time = (end_time-start_time);
    printf("Sequentially %.3f seconds required for binarization. \n", seq_time);
    myfile.close();
    myfile.open("random_lcp.txt");

    for(int i = 0; i < N; ++i)
    {
        myfile >> lcp_array[i];
    }
    double start_time2 = omp_get_wtime();
    #pragma omp parallel for
    for(int i = 0; i < N; ++i)
    {
        if(lcp_array[i] < 45)
            lcp_array[i] = 0;
        else
            lcp_array[i] = 1;
    }
    double end_time2 = omp_get_wtime();
    float par_time = (end_time2-start_time2);
    printf("Parallelly %.3f seconds required for binarization. \n", par_time);
    printf("Speedup is: %.3f\n", seq_time/par_time);
    myfile.close();
	return 0;
}
