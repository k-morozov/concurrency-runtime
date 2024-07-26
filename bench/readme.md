## Benchmark Results

### IntrusiveThreadPool Task 100,000

| Benchmark                                                      | Time    | CPU    | Iterations |
|----------------------------------------------------------------|---------|--------|------------|
| IntrusiveThreadPool_task_100000/iterations:10/repeats:5        | 304 ms  | 89.4 ms| 10         |
| IntrusiveThreadPool_task_100000/iterations:10/repeats:5        | 339 ms  | 86.4 ms| 10         |
| IntrusiveThreadPool_task_100000/iterations:10/repeats:5        | 345 ms  | 90.6 ms| 10         |
| IntrusiveThreadPool_task_100000/iterations:10/repeats:5        | 301 ms  | 98.1 ms| 10         |
| IntrusiveThreadPool_task_100000/iterations:10/repeats:5        | 309 ms  | 100.0 ms| 10        |
| IntrusiveThreadPool_task_100000/iterations:10/repeats:5_mean   | 320 ms  | 92.9 ms| 5          |
| IntrusiveThreadPool_task_100000/iterations:10/repeats:5_median | 309 ms  | 90.6 ms| 5          |
| IntrusiveThreadPool_task_100000/iterations:10/repeats:5_stddev | 20.8 ms | 5.86 ms| 5          |
| IntrusiveThreadPool_task_100000/iterations:10/repeats:5_cv     | 6.50 %  | 6.31 % | 5          |

### DistributedPool Task 100,000

| Benchmark                                                     | Time   | CPU     | Iterations |
|---------------------------------------------------------------|--------|---------|------------|
| DistributedPool_task_100000/iterations:10/repeats:5           | 143 ms | 60.0 ms | 10         |
| DistributedPool_task_100000/iterations:10/repeats:5           | 123 ms | 59.3 ms | 10         |
| DistributedPool_task_100000/iterations:10/repeats:5           | 124 ms | 59.3 ms | 10         |
| DistributedPool_task_100000/iterations:10/repeats:5           | 122 ms | 58.6 ms | 10         |
| DistributedPool_task_100000/iterations:10/repeats:5           | 123 ms | 59.2 ms | 10         |
| DistributedPool_task_100000/iterations:10/repeats:5_mean      | 127 ms | 59.3 ms | 5          |
| DistributedPool_task_100000/iterations:10/repeats:5_median    | 123 ms | 59.3 ms | 5          |
| DistributedPool_task_100000/iterations:10/repeats:5_stddev    | 8.96 ms| 0.477 ms| 5          |
| DistributedPool_task_100000/iterations:10/repeats:5_cv        | 7.05 % | 0.80 %  | 5          |

### Mutex

| Benchmark                                     | Time     | CPU     | Iterations |
|-----------------------------------------------|----------|---------|------------|
| AsyncMutex/iterations:10/repeats:5            | 29.0 ms  | 0.243 ms| 10         |
| AsyncMutex/iterations:10/repeats:5            | 29.1 ms  | 0.189 ms| 10         |
| AsyncMutex/iterations:10/repeats:5            | 29.7 ms  | 0.227 ms| 10         |
| AsyncMutex/iterations:10/repeats:5            | 28.3 ms  | 0.223 ms| 10         |
| AsyncMutex/iterations:10/repeats:5            | 28.7 ms  | 0.221 ms| 10         |
| AsyncMutex/iterations:10/repeats:5_mean       | 29.0 ms  | 0.221 ms| 5          |
| AsyncMutex/iterations:10/repeats:5_median     | 29.0 ms  | 0.223 ms| 5          |
| AsyncMutex/iterations:10/repeats:5_stddev     | 0.522 ms | 0.020 ms| 5          |
| AsyncMutex/iterations:10/repeats:5_cv         | 1.80 %   | 8.91 %  | 5          |
| StdMutex/iterations:10/repeats:5              | 154 ms   | 0.205 ms| 10         |
| StdMutex/iterations:10/repeats:5              | 156 ms   | 0.187 ms| 10         |
| StdMutex/iterations:10/repeats:5              | 157 ms   | 0.182 ms| 10         |
| StdMutex/iterations:10/repeats:5              | 158 ms   | 0.193 ms| 10         |
| StdMutex/iterations:10/repeats:5              | 159 ms   | 0.190 ms| 10         |
| StdMutex/iterations:10/repeats:5_mean         | 157 ms   | 0.191 ms| 5          |
| StdMutex/iterations:10/repeats:5_median       | 157 ms   | 0.190 ms| 5          |
| StdMutex/iterations:10/repeats:5_stddev       | 1.90 ms  | 0.009 ms| 5          |
| StdMutex/iterations:10/repeats:5_cv           | 1.21 %   | 4.54 %  | 5          |