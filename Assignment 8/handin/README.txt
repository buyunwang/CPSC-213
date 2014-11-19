Calvin Cheng
36090132
o7x8

Question 1

The data and observations for the programs run with different parameters are
listed below.

For 100 disk reads:
sRead   1.070s
aRead   0.014s
tRead   0.015s
For 500 disk reads:
sRead   5.355s
aRead   0.011s
tRead   0.017s
For 1000 disk reads:
sRead   10.688s
aRead   0.016s
tRead   0.037s
For 5000 disk reads:
sRead   53.443s
aRead   0.023s
tRead   0.302s

As we can see, the sRead program runs the most slowly, by far. Although tRead
tends to take slightly more time than aRead, we can assume that this is because
the implementation of the thread abstraction creates slightly more overhead.
Nonetheless, we notice that tRead is still much faster than sRead, as it allows
for parallelism; a new thread is created for each disk read, and so the
advantages of parallel processing can be taken advantage of, much like in aRead.

As for the system time of aRead and tRead, it appears that tRead spends much
more system running the program, while aRead uses a lot more user time. An
explanation for this is that the threads (in tRead) make use of system calls
within the kernel to implement the abstraction of asynchronicity, whereas in
aRead, the asynchronous calls are all handled within the program, which is
considered to be "user" time.



Testing - pc_spinlock.c

By running the program several times for varying iteration sizes, I was able to
ascertain that the sum of all the values in the histogram corresponded correctly
with NUM_ITERATIONS * (NUM_PRODUCERS + NUM_CONSUMERS), as each thread would
attempt to increment/decrement the number of items NUM_ITERATIONS times.

Looking at the histogram, I also found the distribution to be fairly random, as
each successive execution of the program would produce quite a different spread
of values, as is expected. Likewise, the wait times for both kinds of threads
varied greatly between executions, as all of these depend on different factors
occuring in the operating system at a particular time.

Nonetheless, the wait times for both consumers and producers were usually fairly
similar given a large enough iteration size, which makes sense, as the number of
items is equally likely to reach MAX_ITEMS as it is to reach 0, and given enough
iterations, it is probable that the two numbers end up quite similar to each
other.



Testing - pc_mutex_cond.c

By running the program several times for varying iteration sizes, I was able to
ascertain that the sum of all the values in the histogram corresponded correctly
with NUM_ITERATIONS * (NUM_PRODUCERS + NUM_CONSUMERS), as each thread would
attempt to increment/decrement the number of items NUM_ITERATIONS times.

Looking at the histogram, I also found the distribution to be fairly random, as
each successive execution of the program would produce quite a different spread
of values, as is expected. Likewise, the wait times for both kinds of threads
varied greatly between executions, as all of these depend on different factors
occuring in the operating system at a particular time.

Nonetheless, the wait times for both consumers and producers were usually fairly
similar given a large enough iteration size, which makes sense, as the number of
items is equally likely to reach MAX_ITEMS as it is to reach 0, and given enough
iterations, it is probable that the two numbers end up quite similar to each
other.

It is also interesting to note that pc_spinlock often had a higher total number
of waits for each kind of thread, which makes sense, as the "spinning" nature of
the code would result in the CPU checking for the condition to evaluate to true
many more times than with the blocking lock, which would put the thread to sleep
until it is woken up to check the condition again.

However, the pc_mutex_cond program took much longer to run for larger input
sizes. This is most likely due to the overhead of the handling of threads, which
becomes quite expensive over time. pc_spinlock does not have to worry about
blocking/unblocking threads, and thus runs much more quickly (albeit perhaps
less efficiently).
