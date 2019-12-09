# Programming Assignment PA2
Taksir Hasan and Thanh Vu  
Programming Assignment PA2  
Instructor: Dr. Jan Prins
COMP 633 Parallel Computing  
Fall 2019   

# Project Description
An LCP array is used as an auxiliary data structure for an MSBWT. Here, we demonstrate how the LCP array can be used to count the total number of KMERS and the number of Kmers among them that appear once. 
Kmers appearing once are considered as noises due to errors in reads. Also, we show the indices of the KMERS (that appear once) can also be obtained efficiently with parallel implementation.

Our reads are strings of length 151 with a '$' sign at the end as a marker. An element at LCP array, LCP[j] = k, whenever the jth read in an uncompressed MSBWT shares k longest common
prefix with the (j+1)th read in the same uncompressed msbwt. Therefore, the largest LCP value can be 151 and the lowest can be 0.

An LCP array can be: (151, 121, 42, 40, 10, 120, 0, 1)

Our implementation is based on the idea that we will not distinguish between ACGNT and $.
Our logic is as follows:
- Suppose, a Kmer appears T times. It will definitely have (T - 1) consecutive values >= K in the LCP array (from its starting position at the LCP array). 
- The next value will be less that K, indicating that it shares less than K characters at the beginning with the next read. 
- We convert the LCP array to an array consisting of 1 and 0s only based on this  idea. 
- If an element is >= 45, convert it to 1, else 0. Counting how many zeros there are is equivalent to counting the total number of Kmers.

The LCP array above will be now: (1, 1, 0, 0, 0, 1, 0, 0)

Simply count the zeros to get the number of Kmers.
```
def CountNumberofKmers(array LCP):

	for each element, e in LCP: // divided equally among the processors
		if e >= 45:
			temp_LCP[index of e] = 1
		else:
			temp_LCP[index of e] = 0
	for each element, e in temp_LCP: // divided equally among the processors
		use count the number of zeros locally and then add them parallely to get the final count of Kmers
```
Now, to count the number of kmers that appear once, we observe that a Kmer appearing only once has a 0 at its index, and is preceded by another 0. Such pattern is impossible to 
appear elsewhere.

```
def CountNumberofKmersAppearingOnce(array LCP):
    STEP 1: CONVERT LCP array to a binary_LCP array // divided equally among the processors
    STEP 2: get shifted_binary_LCP array by shifting all elements once to the right
    STEP 3: Perform AND operations among all elements of binary_LCP and shifted_binary_LCP
    STEP 4: Count the number of zeros in the result array in STEP 3
```

```
def get_indices_of_noisy_Kmers_appearing_once(Vec, K):
	a = (Vec >= K).astype('int8')
    print(a)
    b = np.roll(a, 1) // here np is short form of numpy in python
    c = np.add(a,b)
    d = np.where(c==0)[0] // d is a numpy array containing the indices of the noisy kmers
```

# To run
1. Generate data:
```
g++ -o Generate_Random_LCP Generate_Random_LCP.cpp
./Generate_Random_LCP <number-of-data-points>
```
2. Process data. For example, with main:
```
icpc -std=c++11 -Ofast -fopenmp -o main main.cpp ;
./main <number-of-data-points> <number-of-threads>;
```
3. To run our experiment, simply run the script main.sh, which will execute main.cpp with appropriate parameters:
```
./main.sh
```

# Experiments
In all experiments, only the currently experimented parameter changes, the rest is the same as those used in the base command.
- Base command
```
icpc -std=c++11 -O2 -fopenmp -o main main.cpp ;
./main 100000000 12
```
- Experimental parameters
	1. Optimization flags: { -O, -O1, -O2, -O3, -Ofast }
	2. Number of threads: {2, 4, 8, 16, 18, 20}
	3. Size of data: {1e7, 2e7, 5e7, 1e8, 5e8, 1e9}


# Flow of the project
1. Generate Random LCP
2. Read the Random LCP
3. Vectorize the Random LCP (sequential vs parallel)
4. Count Kmers that appear once (sequential vs parallel)
5. Count Kmers that appear once and twice (sequential vs parallel)
6. Count Kmers that appear once and twice + obtain once-Kmer indices (sequential vs parallel)


# TODO
## countTotalUniqueKmers()
- [ ] Sequential version in C
- [ ] Parallel version in C with OpenMP
- [ ] Compare versions and analyze the speedup
## count_1_Plus_FrequencyKmers()
- [ ] Sequential version in C
- [ ] Parallel version in C with OpenMP
- [ ] Compare versions and analyze the speedup
