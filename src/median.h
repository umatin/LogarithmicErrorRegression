#pragma once
#include <vector>

template<class T>
int hoare_partition(std::vector<T>& data, int lo, int hi, T pivot){
    int i = lo - 1;
    int j = hi + 1;
    while(true){
        do{
            i += 1;
        }while (data[i] < pivot);
        do{
            j -= 1;
        }while (data[j] > pivot);
        if(i >= j){
            return j;
        }
        std::swap(data[i], data[j]);
    }
}

/*
    Computes the k smallest element in the array
*/
template<class T>
T quickselect(std::vector<T>& data, int k, int lo, int hi){
    int i = hoare_partition(data, lo, hi, data[(lo+hi)/2]);
    if(i == k){
        return data[i];
    }
    else if(i < k){
        return quickselect(data, k, i+1, hi);
    }
    else{
        return quickselect(data, k, lo, i-1);
    }
}