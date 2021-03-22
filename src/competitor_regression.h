#pragma once
#include <vector>
#include <cassert>
#include "linear_regression.h"

/*
    Exact solution to the simple linear regression problem with least squares error
*/
linear_regression simple_linear_regression(std::vector<double>& data){
    long double count = data.size();
    long double x_sum = 0;
    long double y_sum = 0;
    long double xx_sum = 0;
    long double xy_sum = 0;

    for(int i = 0; i < data.size(); i++){
        x_sum += data[i];
        y_sum += (long double)i;
        xx_sum += data[i] * data[i];
        xy_sum += (long double)i * data[i];
    }

    double a = ((count * xy_sum) - (x_sum * y_sum)) / ((count * xx_sum) - (x_sum * x_sum));
    double b = (y_sum - (a * x_sum)) / count;
    assert(a >= 0);
    return linear_regression(a,b);
}

linear_regression simple_linear_regression_stable(std::vector<double>& data){
    long double count = data.size();
    long double x_mean = 0;
    long double y_mean = 0;
    long double xx_sum = 0;
    long double xy_sum = 0;

    for(int i = 0; i < data.size(); i++){
        x_mean += data[i];
        y_mean += (long double)i;
    }
    x_mean /= count;
    y_mean /= count;

    for(int i = 0; i < data.size(); i++){
        xx_sum += std::pow<long double>((long double)data[i] - x_mean, 2);
        xy_sum += (long double)(i - y_mean) * (long double)(data[i] - x_mean);
    }

    double a = xy_sum / xx_sum;
    double b = y_mean - (a * x_mean);

    assert(a >= 0);
    return linear_regression(a,b);
}


/*
    Simple Linear Regression with indipendent x and y
*/
linear_regression simple_linear_regression(std::vector<double>& datax, std::vector<double>& datay){
    long double count = datax.size();
    long double x_sum = 0;
    long double y_sum = 0;
    long double xx_sum = 0;
    long double xy_sum = 0;

    for(int i = 0; i < datax.size(); i++){
        x_sum += datax[i];
        y_sum += datay[i];
        xx_sum += datax[i] * datax[i];
        xy_sum += datay[i] * datax[i];
    }

    double a = ((count * xy_sum) - (x_sum * y_sum)) / ((count * xx_sum) - (x_sum * x_sum));
    double b = (y_sum - (a * x_sum)) / count;
    return linear_regression(a,b);
}