// basic file operations
#include <iostream>
#include <fstream>
#include <random>
#include <cstdlib>
using namespace std;

int main(int argc, char *argv[]){

	int N;
	ifstream myfile;
	myfile.open("metadata_random_lcp.txt");
	myfile >> N;

	cout << N;
	myfile.close();

    int *lcp_array = new int [N];
    myfile.open("random_lcp.txt");
    for(int i = 0; i < N; ++i)
    {
        myfile >> lcp_array[i];
        cout << lcp_array[i] <<endl;
    }

    myfile.close();
	return 0;
}
