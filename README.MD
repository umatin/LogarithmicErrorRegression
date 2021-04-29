# Logarithmic Error Regression
This repository implements a microbenchmark for all regressions discussed in "A Tailored Regression for Learned Indexes : Logarithmic Error Regression"

Run using:
```
make
./bin/benchmark
```
Returns
```
Specify Dataset and Size
Available option are:
path_to_dataset
rand
normal
lognormal
randint
debug
exp
outlier
```

Example inputs/outputs:

```
./bin/benchmark normal 2000
SLR data_name:normal data size:2000 lookups size:1000000 mean lookup ns:62.16 median lookup ns:61.5635 log error:12054 discrete log error:12835 mse error:28584486 build time:6442
LogTE data_name:normal data size:2000 lookups size:1000000 mean lookup ns:53.4539 median lookup ns:53.0545 log error:8783 discrete log error:9513 mse error:96096300 build time:1053017
DLogTE data_name:normal data size:2000 lookups size:1000000 mean lookup ns:53.2141 median lookup ns:52.715 log error:8767 discrete log error:9498 mse error:97257552 build time:336671
2P data_name:normal data size:2000 lookups size:1000000 mean lookup ns:53.8817 median lookup ns:53.3618 log error:9078 discrete log error:9844 mse error:91762440 build time:10198276
TheilSen data_name:normal data size:2000 lookups size:1000000 mean lookup ns:58.0887 median lookup ns:57.415 log error:10643 discrete log error:11551 mse error:43867653 build time:9673370
LAD data_name:normal data size:2000 lookups size:1000000 mean lookup ns:57.4404 median lookup ns:56.9566 log error:9822 discrete log error:10692 mse error:67778485 build time:20448857443

./bin/benchmark ../data/wiki_ts_200M_uint64 100
SLR data_name:../data/wiki_ts_200M_uint64 data size:100 lookups size:1000000 mean lookup ns:24.6285 median lookup ns:24.4854 log error:157 discrete log error:179 mse error:819 build time:560
LogTE data_name:../data/wiki_ts_200M_uint64 data size:100 lookups size:1000000 mean lookup ns:22.6714 median lookup ns:22.4476 log error:169 discrete log error:190 mse error:2458 build time:61310
DLogTE data_name:../data/wiki_ts_200M_uint64 data size:100 lookups size:1000000 mean lookup ns:22.4809 median lookup ns:22.3673 log error:163 discrete log error:180 mse error:3382 build time:23327
2P data_name:../data/wiki_ts_200M_uint64 data size:100 lookups size:1000000 mean lookup ns:22.4554 median lookup ns:22.328 log error:169 discrete log error:185 mse error:4033 build time:203075
TheilSen data_name:../data/wiki_ts_200M_uint64 data size:100 lookups size:1000000 mean lookup ns:23.8788 median lookup ns:23.6187 log error:157 discrete log error:179 mse error:898 build time:9171293
LAD data_name:../data/wiki_ts_200M_uint64 data size:100 lookups size:1000000 mean lookup ns:24.1859 median lookup ns:23.9408 log error:155 discrete log error:175 mse error:852 build time:5102775

./bin/benchmark outlier 1000
SLR data_name:outlier data size:1000 lookups size:1000000 mean lookup ns:82.1875 median lookup ns:81.2651 log error:7533 discrete log error:7977 mse error:82863060 build time:3851
LogTE data_name:outlier data size:1000 lookups size:1000000 mean lookup ns:37.2033 median lookup ns:36.9197 log error:2410 discrete log error:2699 mse error:-9223372036854775808 build time:517912
DLogTE data_name:outlier data size:1000 lookups size:1000000 mean lookup ns:37.4408 median lookup ns:37.1932 log error:2401 discrete log error:2712 mse error:-9223372036854775808 build time:218178
2P data_name:outlier data size:1000 lookups size:1000000 mean lookup ns:37.3031 median lookup ns:36.9523 log error:2403 discrete log error:2690 mse error:-9223372036854775808 build time:3778144
TheilSen data_name:outlier data size:1000 lookups size:1000000 mean lookup ns:37.6076 median lookup ns:37.389 log error:2430 discrete log error:2736 mse error:-9223372036854775808 build time:9070631
LAD data_name:outlier data size:1000 lookups size:1000000 mean lookup ns:37.8289 median lookup ns:37.3604 log error:2426 discrete log error:2743 mse error:-9223372036854775808 build time:2647179195
```


Results are stored in results/benchmark.csv

The modified RMI with logarithmic error linear regressions is hosted at [https://github.com/umatin/RMI](https://github.com/umatin/RMI)