#pragma once
#include <vector>
#include <iostream>

/*
    Binary search 
*/
inline int binary_search_lower_bound(int l, int r, const double key, std::vector<double> & data) {
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (data[mid] >= key) {
            r = mid;    
        } else {
            l = mid + 1;
        }
    }
    return l;
}

/*

*/
int exponential_search_lower_bound(int m, double key, std::vector<double> & data) {
    // Continue doubling the bound until it contains the lower bound. Then use
    // binary search.
    int bound = 1;
    int l, r;  // will do binary search in range [l, r)
    if (data[m] >= key) {
        int size = m;
        while (bound < size && data[m - bound] >= key) {
            bound *= 2;
        }
        l = m - std::min<int>(bound, size);
        r = m - bound / 2;
    } else {
        int size = data.size() - m;
        while (bound < size && data[m + bound] < key) {
            bound *= 2;
        }
        l = m + bound / 2;
        r = m + std::min<int>(bound, size);
    }
    return binary_search_lower_bound(l, r, key, data);
}

/*
    Exponential search with a linear scan in the beginning
*/
int exponential_search_lower_bound_linear_head(int m, double key, std::vector<double> & data) {
    if(data[m] == key){
        return m;
    }
    int bound = 4;
    int l, r;  // will do binary search in range [l, r)
    if (data[m] >= key) {
        for(int i = 1; i < 4; i++){
            if(data[m-i] < key){
                return m-i;
            }
        }
        int size = m;
        while (bound < size && data[m - bound] > key) {
            bound *= 2;
        }
        l = m - std::min<int>(bound, size);
        r = m - bound / 2;
    } else {
        for(int i = 1; i < 4; i++){
            if(data[m+1] > key){
                return m+i;
            }
        }
        int size = data.size() - m;
        while (bound < size && data[m + bound] < key) {
            bound *= 2;
        }
        l = m + bound / 2;
        r = m + std::min<int>(bound, size);
    }
    return binary_search_lower_bound(l, r, key, data);
}

/*
    Exponential search
*/
int exponential_search_lower_bound_steps(int m, double key, std::vector<double> & data, int & steps) {
    // Continue doubling the bound until it contains the lower bound. Then use
    // binary search.
    steps = 1;
    int bound = 1;
    int l, r;  // will do binary search in range [l, r)
    if (data[m] >= key) {
        int size = m;
        while (bound < size && data[m - bound] >= key) {
            bound *= 2;
            steps++;
        }
        l = m - std::min<int>(bound, size);
        r = m - bound / 2;
    } else {
        int size = data.size() - m;
        while (bound < size && data[m + bound] < key) {
            bound *= 2;
            steps++;
        }
        l = m + bound / 2;
        r = m + std::min<int>(bound, size);
    }
    return binary_search_lower_bound(l, r, key, data);
}