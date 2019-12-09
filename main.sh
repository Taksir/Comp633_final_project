#!/bin/bash

echo "==================== Experiment 1: Optimization Flag";
echo; 
echo ">>> optim = -O <<<"; 
icpc -std=c++11 -O -fopenmp -o main main.cpp ;
./main 100000000 12;
echo; 
echo ">>> optim = -O1 <<<";
icpc -std=c++11 -O1 -fopenmp -o main main.cpp ;
./main 100000000 12;
echo;
echo ">>> optim = -O2 <<<"; 
icpc -std=c++11 -O2 -fopenmp -o main main.cpp ;
./main 100000000 12;
echo; 
echo ">>> optim = -O3 <<<"; 
icpc -std=c++11 -O3 -fopenmp -o main main.cpp ;
./main 100000000 12;
echo; 
echo ">>> optim = -Ofast <<<"; 
icpc -std=c++11 -Ofast -fopenmp -o main main.cpp ;
./main 100000000 12;


echo "==================== Experiment 2: Number of threads";
icpc -std=c++11 -Ofast -fopenmp -o main main.cpp ;
echo; 
echo ">>> threads = 2 <<<";
./main 100000000 2;
echo; 
echo ">>> threads = 4 <<<";
./main 100000000 4;
echo; 
echo ">>> threads = 8 <<<";
./main 100000000 8;
echo; 
echo ">>> threads = 16 <<<";
./main 100000000 16;
echo; 
echo ">>> threads = 18 <<<";
./main 100000000 18;
echo; 
echo ">>> threads = 20 <<<";
./main 100000000 20;


echo "==================== Experiment 3: Size of data";
icpc -std=c++11 -O2 -fopenmp -o main main.cpp ;
echo; 
echo ">>> data = 1e7 <<<";
./main 10000000 12;
echo; 
echo ">>> data = 2e7 <<<";
./main 20000000 12;
echo; 
echo ">>> data = 5e7 <<<";
./main 50000000 12;
echo; 
echo ">>> data = 1e8 <<<";
./main 100000000 12;
echo; 
echo ">>> data = 5e8 <<<";
./main 500000000 12;
echo; 
echo ">>> data = 1e9 <<<";
./main 1000000000 12;
