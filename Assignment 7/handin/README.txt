Calvin Cheng
36090132
o7x8

Question 1
The runtime performance seemed to indicate a linear relationship between the
time taken and the number of blocks read by the disk. More specifically, running
sRead with an argument of 10 took 0.110s, an argument of 100 took 1.075s, and an
argument of 1000 took 10.697s. Thereofore, we see that the time complexity of
program is roughly linear in terms of the number of blocks read.
This performance occurs because the disk reads are performed synchronously; each
read operation waits for the previous one to complete before executing itself,
and thus it will take roughly (n * 10) ms for the program to complete, as each
disk read takes 10 ms.

Question 2
The runtime performance did not seem to have a noticeable effect, even as the
input parameter increased. Running aRead with an argument of 10 took 0.013s, an
argument of 100 took 0.014s, and an argument of 1000 took 0.015s. Therefore, the
running time of the program did not increase significantly despite a much larger
increase in disk reads.
This performance occurs because the disk reads are performed asynchronously.
First, all of the disk reads are scheduled. Then, about 10ms later, all of the
disk reads complete in the same order in which the requests were enqueued, and
so the runtime of the program essentially consists of the time taken to enqueue
all of the scheduled disk reads, time taken to read from disk (~10 ms in total),
and finally the calls to handleRead() after each disk read is completed. As a
result, the running time of the program is much shorter than the synchronous
version, since all of the disk reads can be performed in parallel, as opposed to
one after another (as in sRead).
