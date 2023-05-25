# 并发数据结构
## concurrrency counter
- simple_counter.c 基本的基数器
- thread_safe_counter.c 线程安全的计数器

## thread_safe_counter.c
运行统计
single thread 1000000
```
time ./a.out 1 1000000
thread exit
counter value = 1000000
./a.out 1 1000000  0.09s user 0.00s system 98% cpu 0.097 total
```
- two thread
```
❯ time ./a.out 2 1000000
thread exit
counter value = 2000000
./a.out 2 1000000  0.35s user 0.10s system 195% cpu 0.231 total
```

- four thread
```
❯ time ./a.out 4 1000000
thread exit
counter value = 4000000
./a.out 4 1000000  1.08s user 0.66s system 383% cpu 0.452 total
```

- sixten thread

```
 time ./a.out 16 1000000
thread exit
counter value = 16000000
./a.out 16 1000000  7.54s user 11.41s system 753% cpu 2.516 total

```
理想情况下，你会看到多处理上运行的多线程
就像单线程一样快。达到这种状态称为完美扩展
（perfect scaling）。虽然总工作量增多，但是并行执
行后，完成任务的时间并没有增加
