#pragma once

#include <vector>
#include "linear_regression.h"

inline linear_regression fit_line(std::vector<double>& data, long fr, long to){
    double a = (double)(to-fr) / (double)(data[to] - data[fr]);
    double b = (double) fr - ((double)data[fr]) * a;
    return linear_regression(a,b);
}

/*
    Fits a linear interpolation function to 2 points
*/
void fit_line(std::vector<double>& data, long fr, long to, double & a, double & b){
    a = (double)(to-fr) / (double)(data[to] - data[fr]);
    b = (double) fr - ((double)data[fr]) * a;
}

/*
    Fits a linear interpolation function to 2 points
*/
linear_regression fit_line(std::vector<double>& datax, std::vector<double>& datay, long fr, long to){
    double a = (double)(datay[to] - datay[fr]) / (double)(datax[to] - datax[fr]);
    double b = (double) datay[fr] - ((double)datax[fr]) * a;
    return linear_regression(a,b);
}


/*
    Fits a linear interpolation function to 2 points
*/
void fit_line(std::vector<double>& datax, std::vector<double>& datay, long fr, long to, double & a, double & b){
    a = (double)(datay[to] - datay[fr]) / (double)(datax[to] - datax[fr]);
    b = (double) datay[fr] - ((double)datax[fr]) * a;
}


linear_regression fit_line(long fr, long to, double dfr, double dto){
    double a = (to - fr) / (dto - dfr);
    double b = to - (dto * a);
    if (a == -std::numeric_limits<double>::infinity()){
        a = std::numeric_limits<double>::infinity();
    }
    return linear_regression(a,b);
}