###############
# given a binary array (0,1,1,0,1,0,1,1,1,0,0,0,1,0), returns how many 0's there are

import numpy as np
def countTotalUniqueKmers(Vec, K):
    
    num_zeros = (Vec == 0).sum()
    return num_zeros
    
countTotalUniqueKmers(np.array([0,1,0,0,1,1,0,1,1,1,1]), 45)

###############

# given an binary array, count the number of '10' patterns

import numpy as np
def count_1_Plus_FrequencyKmers(Vec):
    search_val = [1,0]
    positions_bool = ((Vec[:-1]==search_val[0]) & (Vec[1:]==search_val[1])).astype('int8')
    num_one_zeros = np.sum(positions_bool)
    return num_one_zeros

print(count_1_Plus_FrequencyKmers(np.array([0,1,0,0,1,1,0,1,1,1,1])))

###############

# given a np.array([134,123,2,45,7]), returns indices of substrings of k-mers of length > 1

def indices_of_chunks_beginning_with_1_slow_version(Vec, K):
    
    x = (Vec >= 45).astype('int8')
    part = np.concatenate([np.array([0]) , np.diff(x)])
    
    main_part = np.concatenate([ np.where(part==1)[0], np.where(part==-1)[0]])
    main_part.sort(kind='quicksort')
    
    # if odd number of items, add last element index to main_part
    # remember, np.concatenate is faster than np.append
    if(len(main_part) % 2 == 1):
        main_part = np.append(main_part, len(Vec)-1)
    
    return main_part

###############

# given a np.array ([134,123,2,45,7]), returns indices of substrings of k-mers of length == 1. 

def indices_of_chunks_beginning_with_0(Vec, K):
    #Vec will have 0 in its first index
    a = (Vec >= K).astype('int8')
    print(a)
    b = np.roll(a, 1)
    c = np.add(a,b)
    d = np.where(c==0)[0]
    
    if(a[-1] == 1):
        d = np.append(0,d)

    return d

# a = np.concatenate([np.array([0]) , np.random.choice(xrange(151), size=(10,))])
# print(a)
# d = indices_of_chunks_beginning_with_0(a , 45)
# print(d)
