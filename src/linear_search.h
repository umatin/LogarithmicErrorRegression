#pragma once
#include <vector>
#include <iostream>

/*
    Linear Search
    This is only for benchmarking and atm ignores some edge cases
*/
int linear_search_lower_bound(int m, double key, std::vector<double> & data) {
    if(key < data[m]){
        while (key < data[m] && m > 0){
            m--;
        }
        return m+1;
    }
    else{
        while (key >= data[m] && m < data.size()-1){
            m++;
        }
        return m;
    }
}
