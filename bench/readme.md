## Benchmark Results

### IntrusiveThreadPool Task 100,000

| Benchmark                                                      | Time       | CPU      | Iterations |
|----------------------------------------------------------------|------------|----------|------------|
| IntrusiveThreadPool_task_100000/iterations:10/repeats:5        | 323 ms     | 93.4 ms  | 10         |
| IntrusiveThreadPool_task_100000/iterations:10/repeats:5        | 315 ms     | 99.5 ms  | 10         |
| IntrusiveThreadPool_task_100000/iterations:10/repeats:5        | 453 ms     | 144 ms   | 10         |
| IntrusiveThreadPool_task_100000/iterations:10/repeats:5        | 485 ms     | 150 ms   | 10         |
| IntrusiveThreadPool_task_100000/iterations:10/repeats:5        | 478 ms     | 153 ms   | 10         |
| IntrusiveThreadPool_task_100000/iterations:10/repeats:5_mean   | 411 ms     | 128 ms   | 5          |
| IntrusiveThreadPool_task_100000/iterations:10/repeats:5_median | 453 ms     | 144 ms   | 5          |
| IntrusiveThreadPool_task_100000/iterations:10/repeats:5_stddev | 84.5 ms    | 29.0 ms  | 5          |
| IntrusiveThreadPool_task_100000/iterations:10/repeats:5_cv     | 20.57 %    | 22.69 %  | 5          |

### DistributedPool Task 100,000

| Benchmark                                                     | Time       | CPU      | Iterations |
|---------------------------------------------------------------|------------|----------|------------|
| DistributedPool_task_100000/iterations:10/repeats:5           | 400 ms     | 60.9 ms  | 10         |
| DistributedPool_task_100000/iterations:10/repeats:5           | 400 ms     | 59.3 ms  | 10         |
| DistributedPool_task_100000/iterations:10/repeats:5           | 400 ms     | 57.9 ms  | 10         |
| DistributedPool_task_100000/iterations:10/repeats:5           | 400 ms     | 59.2 ms  | 10         |
| DistributedPool_task_100000/iterations:10/repeats:5           | 400 ms     | 59.2 ms  | 10         |
| DistributedPool_task_100000/iterations:10/repeats:5_mean      | 400 ms     | 59.3 ms  | 5          |
| DistributedPool_task_100000/iterations:10/repeats:5_median    | 400 ms     | 59.2 ms  | 5          |
| DistributedPool_task_100000/iterations:10/repeats:5_stddev    | 0.027 ms   | 1.09 ms  | 5          |
| DistributedPool_task_100000/iterations:10/repeats:5_cv        | 0.01 %     | 1.83 %   | 5          |