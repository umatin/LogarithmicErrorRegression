#pragma once

#include <chrono>
#include <random>
#include <iostream>
#include <algorithm>
#include <string>
#include "../exponential_search.h"
#include "../error_function.h"
#include "../linear_regression.h"

template <class Tp>
inline void DoNotOptimize(Tp const& value) {
    asm volatile("" : : "r,m"(value) : "memory");
}

/*
    Benchmark Linear Regressions by measuring lookuptimes
*/
long benchmark_lookup_linear_regression(std::vector<double> & data, std::vector<double> lookups, linear_regression lr){ 
    int data_size = data.size()-1;
    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i = 0; i < lookups.size(); i++){
        long pos = lr.predict(lookups[i]);
        pos = std::max<long>(std::min<long>(pos, data_size), 0);
        DoNotOptimize(exponential_search_lower_bound_linear_head(pos, lookups[i], data));
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

    return duration.count();
}

typedef linear_regression (*build_function)(std::vector<double> &);
std::vector<long> perform_benchmark(linear_regression lr, std::vector<double>& data, std::vector<double>& lookups, int num){
    std::vector<long> measurements;
    for (int i = 0; i < num; i++){
        long time = benchmark_lookup_linear_regression(data, lookups, lr);
        measurements.push_back(time);
    }

    std::sort(measurements.begin(), measurements.end());
    return measurements;
}


template<build_function f>
void benchmark_regression(std::vector<double> & data, std::vector<double> & lookups, std::string regression_name, std::string data_name, std::string outfile){
    auto start = std::chrono::high_resolution_clock::now();
    linear_regression lr = f(data);
    auto stop = std::chrono::high_resolution_clock::now();
    long build_time = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();

    std::vector<long> measurements = perform_benchmark(lr, data, lookups, 30);

    double mean = 0;
    double median = measurements[measurements.size()/2];
    for(int i = 0; i < measurements.size(); i++){
        mean += measurements[i];
    }
    mean /= measurements.size();

    mean /= lookups.size();
    median /= lookups.size();

    long log_error = calculate_error<LogNorm, true, false>(data, lr);
    long d_log_error = calculate_error<DiscreteLogNorm, true, false>(data, lr);
    long mse_error = calculate_error<L2Norm, true, false>(data, lr);

    std::ofstream file;
    file.open(outfile, std::ios_base::app);
    file << regression_name << ";" << data_name << ";" << data.size() << ";" << lookups.size() << ";" << mean << ";" << median << ";" << log_error << ";" << d_log_error << ";" << mse_error << ";" << build_time << std::endl;
    file.close();
    
    std::cout << regression_name << " data_name:" << data_name << " data size:" << data.size() << " lookups size:" << lookups.size() << " mean lookup ns:" << mean << " median lookup ns:" << median << " log error:" << log_error << " discrete log error:" << d_log_error << " mse error:" << mse_error << " build time:" << build_time << std::endl;
}