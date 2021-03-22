#pragma once

#include <vector>
#include <cmath>
#include "linear_regression.h"

/*
    All supported types of errors supported
*/
enum ERROR_TYPE{L1Norm, LogNorm, DiscreteLogNorm, L2Norm, FastDiscreteLogNorm, FPLogNorm, SquaredLogNorm, HardwareNorm, ExpNorm};

int alignment = 0;
#define CACHE_LINE_SIZE 8
#define LOG2(X) ((unsigned) (8*sizeof (unsigned long long) - __builtin_clzll((X)) - 1))

/*
    Applies the specified Errorfunction
    ROUND -> round values to obtain better fit to actual data
    BOUNDED -> clamp values inside the array to obtain a more realistic estimate of the error
*/
template<ERROR_TYPE E, bool ROUND = true, bool BOUNDED = true>
inline double apply_errorfn(double prediction, double y, int max_bound){
    if(BOUNDED){
        prediction = std::max<long double>(std::min<long double>(prediction, max_bound), 0);
    }
    if(ROUND){
        prediction = std::floor(prediction);
    }

    long double error = y - prediction;
    
    if (E == L1Norm){
        return std::abs(error);
    }
    else if (E == LogNorm){
        return log2(1 + std::abs(error));
    }
    else if (E == SquaredLogNorm){
        return log2(1 + (error)*(error));
    }
    else if (E == DiscreteLogNorm){
        return ceil(log2(1 + std::abs(error)));
    }
    else if (E == L2Norm){
        return std::pow(error, 2);
    }
    else if (E == FastDiscreteLogNorm){
        return LOG2(1 + std::abs(error));
    }
    else if (E == HardwareNorm){
        // Attempt to maximize hits on the same cache line
        // Obtain using either:
        // cat /sys/devices/system/cpu/cpu0/cache/index2/coherency_line_size
        // getconf LEVEL1_DCACHE_LINESIZE
        int cache_line_y = ((int)y-alignment) / CACHE_LINE_SIZE;
        int cache_line_p = ((int)prediction-alignment) / CACHE_LINE_SIZE;
        return cache_line_y == cache_line_p ? LOG2(1 + std::abs(error)) : LOG2(1 + std::abs(error)) + 1;
    }
    else if (E == FPLogNorm){
        //Directly uses the exponent of the number to calculate the discretized logarithm
        int exp;
        frexp(error+1, &exp);
        return exp;
    }
    if (E == ExpNorm){
        //Metric used by IRLS
        return std::pow(2, std::abs(error));
    }
}

/*
    Calculates the error for a single element for a certain linear function
*/
template<ERROR_TYPE E, bool ROUND = true, bool BOUNDED = true>
inline double calculate_error_single_element (std::vector<double>& data, linear_regression lr, int i){
    double x = data[i];
    double y = i;
    double prediction = lr.predict(x);
    
    return apply_errorfn<E, ROUND, BOUNDED>(prediction, y, data.size()-1);
}

/*
    Calculates the error for a single element for a certain linear function
*/
template<ERROR_TYPE E, bool ROUND = true, bool BOUNDED = true>
inline double calculate_error_single_element (std::vector<double>& data, double a, double b, int i){
    double x = data[i];
    double y = i;
    double prediction = (a*x)+b;
    
    return apply_errorfn<E, ROUND, BOUNDED>(prediction, y, data.size()-1);
}

/*
    Calculates the error for a single element for a certain linear function for x,y points
*/
template<ERROR_TYPE E, bool ROUND = true, bool BOUNDED = true>
inline double calculate_error_single_element (std::vector<double>& datax, std::vector<double>& datay, linear_regression lr, int i){
    double x = datax[i];
    double y = datay[i];
    double prediction = lr.predict(x);
    
    return apply_errorfn<E, ROUND, BOUNDED>(prediction, y, datax.size()-1);
}

/*
    Calculates the error for a single element for a certain linear function for x,y points
*/
template<ERROR_TYPE E, bool ROUND = true, bool BOUNDED = true>
inline double calculate_error_single_element (std::vector<double>& datax, std::vector<double>& datay, double a, double b, int i){
    double x = datax[i];
    double y = datay[i];
    double prediction = (a*x)+b;
    
    return apply_errorfn<E, ROUND, BOUNDED>(prediction, y, datax.size()-1);
}


/*
    Calculates the total error for a linear function
*/
template<ERROR_TYPE E, bool CORRECT = true, bool BOUNDED = true>
long double calculate_error(std::vector<double>& data, linear_regression lr){
    long double total_error = 0;
    for(long i = 0; i < data.size(); i++){
        total_error += calculate_error_single_element<E, CORRECT, BOUNDED>(data, lr, i);
    }
    return total_error;
}

/*
    Calculates the total error for a linear function
*/
template<ERROR_TYPE E, bool CORRECT = true, bool BOUNDED = true>
long double calculate_error(std::vector<double>& data, double a, double b){
    long double total_error = 0;
    for(long i = 0; i < data.size(); i++){
        total_error += calculate_error_single_element<E, CORRECT, BOUNDED>(data, a, b, i);
    }
    return total_error;
}

/*
    Calculates the total error for a linear function for x,y points
*/
template<ERROR_TYPE E, bool CORRECT = true, bool BOUNDED = true>
double calculate_error(std::vector<double>& datax, std::vector<double>& datay, const linear_regression lr){
    double total_error = 0;
    for(long i = 0; i < datax.size(); i++){
        total_error += calculate_error_single_element<E, CORRECT, BOUNDED>(datax, datay, lr, i);
    }
    return total_error;
}

/*
    Calculates the total error for a linear function for x,y points
*/
template<ERROR_TYPE E, bool CORRECT = true, bool BOUNDED = true>
double calculate_error(std::vector<double>& datax, std::vector<double>& datay, double a, double b){
    double total_error = 0;
    for(long i = 0; i < datax.size(); i++){
        total_error += calculate_error_single_element<E, CORRECT, BOUNDED>(datax, datay, a, b, i);
    }
    return total_error;
}