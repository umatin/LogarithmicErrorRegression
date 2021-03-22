#pragma once

#include <vector>
#include <cmath>
#include "error_function.h"
#include <random>

long double weighted_mean_x(std::vector<double> data, std::vector<double>& weights){
    long double w_sum = 0;
    long double d_sum = 0;
    for(int i = 0; i < data.size(); i++){
        w_sum += weights[i];
        d_sum += weights[i] * (long double)data[i];
    }
    return d_sum/w_sum;
}

long double weighted_mean_y(std::vector<double>& weights){
    long double w_sum = 0;
    long double d_sum = 0;
    for(int i = 0; i < weights.size(); i++){
        w_sum += weights[i];
        d_sum += weights[i] * (long double)i;
    }
    return d_sum/w_sum;
}

linear_regression weighted_simple_regression(std::vector<double>& data, std::vector<double>& weights){
    long double x_mean = weighted_mean_x(data, weights);
    long double y_mean = weighted_mean_y(weights);

    long double top = 0;
    long double bot = 0;
    for(int i = 0; i < data.size(); i++){
        top += weights[i] * (data[i] - x_mean) * ((long double)i - y_mean);
        bot += weights[i] * (data[i] - x_mean) * (data[i] - x_mean);
    }

    double a = top/bot;
    double b = y_mean - (a * x_mean);
    return linear_regression(a,b);
}


/*
    A simple Implementation of the Iteratively rewieghted least squares algorithm for L1 regression
    https://en.wikipedia.org/wiki/Iteratively_reweighted_least_squares
*/
template<ERROR_TYPE E, bool ROUND = false, bool BOUNDED = false>
linear_regression irls_regression(std::vector<double>& data, int n){
    std::vector<double> weights;
    weights.resize(data.size());

    for (int i = 0; i < weights.size(); i++){
        weights[i] = 1;
    }
    linear_regression lr;
    for (int counter = 0; counter < n; counter++){
        lr = weighted_simple_regression(data, weights);
        for (int i = 0; i < weights.size(); i++){
            double err = calculate_error_single_element<E, ROUND, BOUNDED>(data, lr, i);
            weights[i] = 1.0 / std::max(err, 0.00001);
        }
    }
    return lr;
}

template<ERROR_TYPE E, bool ROUND = false, bool BOUNDED = false>
linear_regression irls_regression(std::vector<double>& data){
    return irls_regression<E, ROUND, BOUNDED>(data, 10);
}
