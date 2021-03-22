#pragma once
#include <vector>

#include "error_function.h"
#include "interpolation.h"
#include "median.h"
#include "linear_regression.h"


/*
    Theil Sen regression
*/
linear_regression theil_sen(std::vector<double>& data, long max_trails){
    std::vector<double> slopes;
    double a,b;
    long num = (data.size() * (data.size()-1)) / 2;

    for (int i = 0; i < max_trails; i++){
        double a_tmp, b_tmp;
        fit_line(data, rand()%data.size(), rand()%data.size(), a_tmp, b_tmp);
        slopes.push_back(a_tmp);
    }
    a = quickselect(slopes,slopes.size()/2,0,slopes.size()-1);

    std::vector<double> intercepts;
    for (int i = 0; i < max_trails; i++){
        int element = rand()%data.size();
        double intercept = (double)element - (a * data[element]);
        intercepts.push_back(intercept);
    }
    b = quickselect(intercepts,intercepts.size()/2,0,intercepts.size()-1);
    return linear_regression(a,b);
}

linear_regression theil_sen(std::vector<double>& data){
    return theil_sen(data, 100000);
}