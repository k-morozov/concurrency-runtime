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

### DistributedPool Task 100,000 (std::mutex)

| Benchmark                                                       | Time    | CPU    | Iterations |
|-----------------------------------------------------------------|---------|--------|------------|
| DistributedPool_task_100000/iterations:10/repeats:5             | 254 ms  | 96.8 ms| 10         |
| DistributedPool_task_100000/iterations:10/repeats:5             | 492 ms  | 196 ms | 10         |
| DistributedPool_task_100000/iterations:10/repeats:5             | 563 ms  | 224 ms | 10         |
| DistributedPool_task_100000/iterations:10/repeats:5             | 254 ms  | 118 ms | 10         |
| DistributedPool_task_100000/iterations:10/repeats:5             | 175 ms  | 83.4 ms| 10         |
| DistributedPool_task_100000/iterations:10/repeats:5_mean        | 348 ms  | 144 ms | 5          |
| DistributedPool_task_100000/iterations:10/repeats:5_median      | 254 ms  | 118 ms | 5          |
| DistributedPool_task_100000/iterations:10/repeats:5_stddev      | 169 ms  | 62.7 ms| 5          |
| DistributedPool_task_100000/iterations:10/repeats:5_cv          | 48.67 % | 43.65 %| 5          |

### DistributedPool Task 100,000 (spinlock)

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

### DistributedPool Task 100,000 (queue spinlock)

| Benchmark                                                     | Time   | CPU    | Iterations |
|---------------------------------------------------------------|--------|--------|------------|
| DistributedPool_task_100000/iterations:10/repeats:5           | 304 ms | 126 ms | 10         |
| DistributedPool_task_100000/iterations:10/repeats:5           | 263 ms | 107 ms | 10         |
| DistributedPool_task_100000/iterations:10/repeats:5           | 205 ms | 79.7 ms| 10         |
| DistributedPool_task_100000/iterations:10/repeats:5           | 190 ms | 78.7 ms| 10         |
| DistributedPool_task_100000/iterations:10/repeats:5           | 460 ms | 189 ms | 10         |
| DistributedPool_task_100000/iterations:10/repeats:5_mean      | 285 ms | 116 ms | 5          |
| DistributedPool_task_100000/iterations:10/repeats:5_median    | 263 ms | 107 ms | 5          |
| DistributedPool_task_100000/iterations:10/repeats:5_stddev    | 108 ms | 45.5 ms| 5          |
| DistributedPool_task_100000/iterations:10/repeats:5_cv        | 37.98 %| 39.18 %| 5          |

### DistributedPool Task 100,000 (spinlock with weak mm)

| Benchmark | Time | CPU | Iterations |
| --- | --- | --- | --- |
| DistributedPool_task_100000/iterations:10/repeats:5 | 186 ms | 73.0 ms | 10 |
| DistributedPool_task_100000/iterations:10/repeats:5 | 132 ms | 62.2 ms | 10 |
| DistributedPool_task_100000/iterations:10/repeats:5 | 145 ms | 59.1 ms | 10 |
| DistributedPool_task_100000/iterations:10/repeats:5 | 126 ms | 58.9 ms | 10 |
| DistributedPool_task_100000/iterations:10/repeats:5 | 149 ms | 62.4 ms | 10 |
| DistributedPool_task_100000/iterations:10/repeats:5_mean | 148 ms | 63.1 ms | 5 |
| DistributedPool_task_100000/iterations:10/repeats:5_median | 145 ms | 62.2 ms | 5 |
| DistributedPool_task_100000/iterations:10/repeats:5_stddev | 23.5 ms | 5.76 ms | 5 |
| DistributedPool_task_100000/iterations:10/repeats:5_cv | 15.90 % | 9.13 % | 5 |

### Compare std::mutex and async_mutex with coro

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