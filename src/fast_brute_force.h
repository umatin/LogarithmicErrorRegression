#pragma once

#include <queue>
#include <vector>
#include <cmath>
#include <limits>

#include "interpolation.h"
#include "error_function.h"
#include "helpers/io_handler.h"
#include "log_regression.h"
#include "linear_regression.h"

struct q_item{
    double priority = 0;
    long index = 0;
    long error = 0;

    q_item(double p, long i, long e){
        this->priority = p;
        this->index = i;
        this->error = e;
    }

    bool operator<(const q_item& other) const{
        return this->priority < other.priority;
    }

    bool operator>(const q_item& other) const{
        return this->priority > other.priority;
    }
};

inline long pow2(long x){
    return (long)1 << x;
}

inline long comp_target(long i, long error, long index){
    long target = i;
    if (i < index){
        error += 1;
    }
    if(error > 0){
        target += pow2(error - 1);
    }
    else if(error < 0){
        target -= pow2(-error) - 1;
    }
    return target;
}

inline long error_fn(long i, double prediction){
    long delta = std::floor(prediction) - i;
    return (long) std::ceil(log2(1 + std::abs(delta)));
}

std::pair<linear_regression, long> build_regression_i(std::vector<double>& data, long index){
    std::priority_queue<q_item, std::vector<q_item>, std::greater<q_item>> pq;
    double epsilon = std::numeric_limits<double>::epsilon();//0.00000000000000001;

    long cost = 0;
    
    //Initalize cost and priority queue
    //with a = 0, b = index
    for(long i = 0; i < data.size(); i++){
        if (i != index){
            long dist = std::abs(index - i);

            //Initialize cost
            cost += std::abs(error_fn(i, index));

            //Insert queue item
            long error = index < i ? -(error_fn(i, index)-1) : error_fn(i, index)-1;
            long target = comp_target(i, error, index);
            linear_regression lr = fit_line(target, index, data[i], data[index]);
            pq.push(q_item(lr.a, i, error));
        }
    }

    linear_regression lr_opt(0, index);
    
    //Potential function measures the maximal possible reduction in cost 
    long potential = cost;
    long min_cost = cost;

    //We need to have all changes applied in order for the cost to be accurate
    bool valid = false;
    while((min_cost != 0 && min_cost >= cost - potential) || !valid){
        q_item element = pq.top();
        pq.pop();
        long i = element.index;

        valid = pq.top().priority != element.priority;

        long last_val =  i < index ? element.error+1 : element.error-1;
        cost -= std::abs(last_val);
        cost += std::abs(element.error);
        
        //Update potential
        if (i < index){
            if (last_val >= 1){
                potential -= 1;
            }
        }
        else{
            if (last_val <= -1){
                potential -= 1; 
            }
        }

        if (valid && cost < min_cost){
            //Update minimum
            double temp_a = element.priority + epsilon;
            double temp_b = index - data[index] * temp_a;
            min_cost = cost;
            lr_opt = linear_regression(temp_a, temp_b);
        }

        long new_error = i < index ? element.error-1 : element.error+1;
        long target = comp_target(i, new_error, index);
        linear_regression lr = fit_line(target, index, data[i], data[index]);
        pq.push(q_item(lr.a, i, new_error));
    }

    return std::pair<linear_regression, long>(lr_opt, min_cost);
}

linear_regression build_regression(std::vector<double>& data){
    long min_cost = std::numeric_limits<long>::max();
    linear_regression lr_opt;
    
    for(int i = 0; i < data.size(); i++){
        double a,b;
        std::pair<linear_regression, long> p = build_regression_i(data, i);
        linear_regression lr = p.first;
        long cost = p.second;
        if(cost < min_cost){
            min_cost = cost;
            lr_opt = lr;
        }
    }
    return lr_opt;
}

//Todo just check for equality
long find_min_idx(std::vector<double>& data, linear_regression lr, long selfi){
    long min_idx = -1;
    double min_err = std::numeric_limits<double>::infinity();
    for(int i = 0; i < data.size(); i++){
        if(i != selfi){
            double err = calculate_error_single_element<L1Norm, false, false>(data, lr.a, lr.b, i);
            if (err < min_err){
                min_idx = i;
                min_err = err;
            }
        }
    }
    return min_idx;
}

linear_regression build_regression_direct_descent(std::vector<double>& data, int num){
    srand(time(nullptr));
    long index = rand()%data.size();

    linear_regression lr;
    for(int i = 0; i < num; i++){
        double a,b;
        std::pair<linear_regression, long> p = build_regression_i(data, index);
        lr = p.first;
        long cost = p.second;
        index = find_min_idx(data, lr, index);
        //std::cout << calculate_error<DiscreteLogNorm>(data, lr) << std::endl;
    }
    return lr;
}

linear_regression build_regression_direct_descent(std::vector<double>& data){
    return build_regression_direct_descent(data, 3);
}
