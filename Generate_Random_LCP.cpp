// basic file operations
#include <iostream>
#include <fstream>
#include <random>
#include <cstdlib>
using namespace std;

int main(int argc, char *argv[]){
	int N;
	if(argc == 2)
	{
		N = atoi(argv[1]);
	}
	else N = 1000;

	srand(100);
	ofstream myfile;
	myfile.open ("metadata_random_lcp.txt");
	myfile << N;
	myfile.flush();
	myfile.close();
    printf("%d\n", N);

	myfile.open("random_lcp.txt");
	for(int i = 0 ; i < N; ++i){
		myfile << rand() % 152;
		myfile << '\n';
	}
	myfile.flush();

	myfile.close();
	return 0;
}
