#pragma once

struct linear_regression{
    double a = 0;
    double b = 0;
    linear_regression(double a_, double b_){
        this->a = a_;
        this->b = b_;
    }
    linear_regression(){

    }
    inline double predict(double x){
        return fma(this->a, x, this->b);
    }
};