#pragma once

#include <algorithm>
#include <chrono>
#include <iostream>
#include <functional>
#include <fstream>
#include <thread>
#include <vector>
#include <cassert>
#include <cmath>
#include <math.h>
#include <cstring>
#include <climits>
#include <random>
#include <string>


template<typename T>
static std::vector<T> load_data(char * filename_cs) {
    std::string filename(filename_cs);
    std::vector<T> data;

    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "unable to open " << filename << std::endl;
        exit(EXIT_FAILURE);
    }
    // Read size.
    uint64_t size;
    in.read(reinterpret_cast<char*>(&size), sizeof(uint64_t));
    data.resize(size);
    // Read values.
    in.read(reinterpret_cast<char*>(data.data()), size*sizeof(T));
    in.close();
    
    return data;
}


std::vector<u_int64_t> cache_data;
/*
    Parses first 2 arguments to create data
*/
std::vector<double> parse_arguments(int argc, char *argv[]){
    if(argc < 3){
        std::cout << "Specify Dataset and Size" << std::endl;
        std::cout << "Available option are:" << std::endl;
        std::cout << "path_to_dataset" << std::endl;
        std::cout << "rand" << std::endl;
        std::cout << "normal" << std::endl;
        std::cout << "lognormal" << std::endl;
        std::cout << "randint" << std::endl;
        std::cout << "debug" << std::endl;
        std::cout << "exp" << std::endl;
        std::cout << "outlier" << std::endl;
        exit(0);
    }
    
    long num_data = strtol(argv[2], nullptr, 0);
    std::vector<double> data;

    std::random_device rd;
    std::mt19937 gen(rd());
    
    if(strcmp(argv[1],"rand") == 0){
        std::uniform_real_distribution<double> dis(0, 2147483647);
        for (long i = 0; i < num_data; i++){
            data.push_back(dis(gen));
        }
    }
    else if(strcmp(argv[1],"normal") == 0){
        std::normal_distribution<double> dis(0.0,4.0);
        for (long i = 0; i < num_data; i++){
            data.push_back(dis(gen));
        }
    }
    else if(strcmp(argv[1],"lognormal") == 0){
        std::lognormal_distribution<double> dis(0.0, 4.0);
        for (long i = 0; i < num_data; i++){
            data.push_back(dis(gen));
        }
    }
    else if(strcmp(argv[1],"randint") == 0){
        for (long i = 0; i < num_data; i++){
            long val = rand();
            data.push_back(val);
        }
    }
    else if(strcmp(argv[1],"debug") == 0){
        for (long i = 0; i < num_data; i++){
            long val = rand()%30;
            data.push_back(val);
        }
    }
    else if(strcmp(argv[1],"randfixed") == 0){
        for (long i = 0; i < num_data; i++){
            std::uniform_real_distribution<double> dis(0, 10000);
            data.push_back(dis(gen));
        }
    }
    else if(strcmp(argv[1],"exp") == 0){
        for (long i = 0; i < num_data; i++){
            long val = std::pow(2,i);
            data.push_back(val);
        }
    }
    else if(strcmp(argv[1],"manual") == 0){
        for (long i = 2; i < argc; i++){
            data.push_back(strtol(argv[i], nullptr, 0));
        }
        std::cout << data.size() << std::endl;
        std::sort(data.begin(), data.end());
        return data;
    }
    else if(strcmp(argv[1],"outlier") == 0){
        int num_outliers = 1;
        for (long i = 0; i < num_data - num_outliers; i++){
            std::uniform_real_distribution<double> dis(0, 100000);
            data.push_back(dis(gen));
        }
        for (long i = 0; i < num_outliers; i++){
            std::uniform_real_distribution<double> dis(200000000000000, 300000000000000);
            data.push_back(dis(gen));
        }
    }
    else{
        if (cache_data.size() == 0){
            std::cout << argv[1] << "\n";
            cache_data = load_data<u_int64_t>(argv[1]);
        }

        data.reserve(num_data);
        std::uniform_int_distribution<long> dis(0, std::max<long>((long)cache_data.size()-1-num_data, 0));
        int start = dis(gen);
        for(long i = 0; cache_data[i] < LONG_MAX && i < num_data && i < cache_data.size(); i++){
            data.push_back((double)cache_data[start+i]);
        }
    }
    std::sort(data.begin(), data.end());
    //data.erase( unique( data.begin(), data.end() ), data.end() );
    //std::sort(data.begin(), data.end());
    return data;
}