#include <iostream>
#include <fstream>
#include "src/log_regression.h"
#include "src/competitor_regression.h"
#include "src/irls.h"

#include "src/helpers/regression_benchmark.h"
#include "src/helpers/io_handler.h"
#include "src/theil_sen.h"
#include "src/fast_brute_force.h"

int main(int argc, char *argv[]){
    srand(time(NULL));

    std::vector<double> data = parse_arguments(argc, argv);

    std::string data_name = argv[1];
    std::string outfile = "results/benchmark.csv";
    int num_lookups = 1000000;
    
    std::vector<double> lookups;
    lookups.resize(num_lookups);
    for (int i = 0; i < lookups.size(); i++){
        lookups[i] = data[rand()%data.size()];
    }

    benchmark_regression<simple_linear_regression_stable>(data,lookups,"SLR",data_name,outfile);
    benchmark_regression<create_regression_tournament_selection<LogNorm>>(data,lookups,"LogTE",data_name,outfile);
    benchmark_regression<create_regression_tournament_selection<FastDiscreteLogNorm>>(data,lookups,"DLogTE",data_name,outfile);
    benchmark_regression<build_regression_direct_descent>(data,lookups,"2P",data_name,outfile);
    benchmark_regression<theil_sen>(data,lookups,"TheilSen",data_name,outfile);
    benchmark_regression<create_regression_optimal<L1Norm>>(data,lookups,"LAD",data_name,outfile);
}
