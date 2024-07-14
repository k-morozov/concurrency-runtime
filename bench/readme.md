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