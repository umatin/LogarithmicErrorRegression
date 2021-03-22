#pragma once
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <random>
#include <cassert>
#include <limits>

#include "error_function.h"
#include "fast_random.h"
#include "interpolation.h"
#include "linear_regression.h"

/*
    A brute force algorithm to create the optimal regression function for a dataset
    Only works for error functions in O(error)
*/
template<ERROR_TYPE E, bool ROUND = true, bool BOUNDED = true>
linear_regression create_regression_optimal(std::vector<double>& data){
    double e = INFINITY;
    double a, b;
    for(long fr = 0; fr < data.size(); fr++){
        for(long to = fr+1; to < data.size(); to++){
            double a_tmp;
            double b_tmp;
            fit_line(data, fr, to, a_tmp, b_tmp);
            double error_tmp = calculate_error<E,ROUND,BOUNDED>(data, a_tmp, b_tmp);
            if(error_tmp < e){
                a = a_tmp;
                b = b_tmp;
                e = error_tmp;
            }
        }
    }
    return linear_regression(a,b);
}

/*
    A recursive helper function for create_regression_tournament_selection
*/
template<ERROR_TYPE E, bool ROUND = true, bool BOUNDED = true>
linear_regression create_regression_tournament_selection_recursive(std::vector<double>& data, std::vector<int>& tournament, int fr, int to, double & error){
    assert(to - fr >= 2);

    if (to - fr == 2){
        error = 0;
        return fit_line(data, tournament[fr], tournament[to-1]);;
    }
    double errorl = 0;
    linear_regression lr_l = create_regression_tournament_selection_recursive<E,ROUND,BOUNDED>(data, tournament, fr, (fr+to)/2, errorl);

    double errorr = 0;
    linear_regression lr_r = create_regression_tournament_selection_recursive<E,ROUND,BOUNDED>(data, tournament, (fr+to)/2, to, errorr);

    for (int i = (fr+to)/2; i < to; i++){
        errorl += calculate_error_single_element<E,ROUND,BOUNDED> (data, lr_l, tournament[i]);
    }
    for (int i = fr; i < (fr+to)/2; i++){
        errorr += calculate_error_single_element<E,ROUND,BOUNDED> (data, lr_r, tournament[i]);
    }
    if (errorl < errorr){
        error = errorl;
        return lr_l;
    }
    else{
        error = errorr;
        return lr_r;
    }
}

/*
    A randomized approximation algorihm for the logarithm error regression.
    Only works properly for Logarithmic errors.
    Works by facing candidtate regression functions off in a tounament to obtain a good candidate.
    Runtime is in O(nlogn)
    The function rounds the size or the data up to the next highest power of 2.
*/
template<ERROR_TYPE E, bool ROUND = true, bool BOUNDED = true>
linear_regression create_regression_tournament_selection(std::vector<double>& data){
    long s = std::pow(2, (int)std::ceil(log2(data.size())));
    std::vector<int> tournament;
    tournament.resize(s);
    for (int i = 0; i < s/2; i++){
        int fr = rand() % data.size();
        int to;
        do{
            to = rand() % data.size();
        }while(fr == to);
        tournament[2*i] = fr;
        tournament[2*i + 1] = to;
    }
    double error;
    return create_regression_tournament_selection_recursive<E,ROUND,BOUNDED>(data, tournament, 0, s, error);
}

/*
    A repeated execution of create_regression_tournament_selection
*/
template<ERROR_TYPE E, bool ROUND = true, bool BOUNDED = true>
linear_regression create_regression_tournament_selection_repeated(std::vector<double>& data, int n){
    double min_error = std::numeric_limits<double>::infinity();
    linear_regression lr;
    for (int i = 0; i < n; i++){
        double candidate_a, candidate_b;
        linear_regression lr_tmp = create_regression_tournament_selection<E,ROUND,BOUNDED>(data);
        double candidate_error = calculate_error<E,ROUND,BOUNDED>(data, lr_tmp);
        if(candidate_error < min_error){
            min_error = candidate_error;
            lr = lr_tmp;
        }
    }
    return lr;
}

template<ERROR_TYPE E, bool ROUND = true, bool BOUNDED = true>
void create_regression_tournament_selection_fast(std::vector<double> & data, int & fr, int & to, int height){
    if(height == 0){
        fr = xorshf96()%data.size();
        to = xorshf96()%data.size();
        while(to == fr){
            to = xorshf96()%data.size();
        }
        return;
    }
    int fr_l, to_l;
    int fr_r, to_r;
    create_regression_tournament_selection_fast<E,ROUND,BOUNDED>(data, fr_l, to_l, height-1);
    create_regression_tournament_selection_fast<E,ROUND,BOUNDED>(data, fr_r, to_r, height-1);

    linear_regression lr_l = fit_line(data,fr_l,to_l);
    linear_regression lr_r = fit_line(data,fr_r,to_r);
    double err_l = 0;
    double err_r = 0;
    err_l += calculate_error_single_element<E,ROUND,BOUNDED>(data, lr_l, fr_r);
    err_l += calculate_error_single_element<E,ROUND,BOUNDED>(data, lr_l, to_r);
    err_r += calculate_error_single_element<E,ROUND,BOUNDED>(data, lr_r, fr_l);
    err_r += calculate_error_single_element<E,ROUND,BOUNDED>(data, lr_r, to_l);
    
    for(int i = 0; i < (2<<(height-1)); i++){
        int ele = xorshf96()%data.size();
        err_l += calculate_error_single_element<E,ROUND,BOUNDED>(data, lr_l, ele);
        err_r += calculate_error_single_element<E,ROUND,BOUNDED>(data, lr_r, ele);
    }

    if(err_l < err_r){
        fr = fr_l;
        to = to_l;
    }
    else{
        fr = fr_r;
        to = to_r;
    }
}

template<ERROR_TYPE E, bool ROUND = true, bool BOUNDED = true>
linear_regression create_regression_tournament_selection_fast(std::vector<double> & data){
    int fr,to;
    int height = std::ceil(log2(data.size()));
    create_regression_tournament_selection_fast<E,ROUND,BOUNDED>(data, fr, to, height);
    return fit_line(data,fr,to);
}

template<ERROR_TYPE E, bool ROUND = true, bool BOUNDED = true>
void create_regression_tournament_selection(std::vector<double> & datax, std::vector<double> & datay, int & fr, int & to, int height){
    if(height == 0){
        fr = xorshf96()%datax.size();
        to = xorshf96()%datax.size();
        while(to == fr){
            to = xorshf96()%datax.size();
        }
        return;
    }
    int fr_l, to_l;
    int fr_r, to_r;
    create_regression_tournament_selection<E,ROUND,BOUNDED>(datax, datay, fr_l, to_l, height-1);
    create_regression_tournament_selection<E,ROUND,BOUNDED>(datax, datay, fr_r, to_r, height-1);
    double a_l,b_l;
    double a_r,b_r;
    linear_regression lr_l = fit_line(datax,datay,fr_l,to_l);
    linear_regression lr_r = fit_line(datax,datay,fr_r,to_r);
    double err_l = 0;
    double err_r = 0;
    err_l += calculate_error_single_element<E,ROUND,BOUNDED>(datax, datay, a_l, b_l, fr_r);
    err_l += calculate_error_single_element<E,ROUND,BOUNDED>(datax, datay, a_l, b_l, to_r);
    err_r += calculate_error_single_element<E,ROUND,BOUNDED>(datax, datay, a_r, b_r, fr_l);
    err_r += calculate_error_single_element<E,ROUND,BOUNDED>(datax, datay, a_r, b_r, to_l);
    
    for(int i = 0; i < (2<<(height-1)) && i < datax.size(); i++){
        int ele = xorshf96()%datax.size();
        err_l += calculate_error_single_element<E,ROUND,BOUNDED>(datax, datay, a_l, b_l, ele);
        err_r += calculate_error_single_element<E,ROUND,BOUNDED>(datax, datay, a_r, b_r, ele);
    }

    if(err_l < err_r){
        fr = fr_l;
        to = to_l;
    }
    else{
        fr = fr_r;
        to = to_r;
    }
}
