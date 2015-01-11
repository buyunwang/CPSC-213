PARTNER - MARK JOINTLY

Calvin Cheng
36090132
o7x8

Joseph So
15472137
x1g9

-------------------------------------------------------------------------------
smoke.c

Below is the output for five test runs of the program, with an iteration size
of 10000 rounds. The categories in the counters refer to the type of resource
that each smoker has (i.e. tobacco, paper, and matches, respectively):
--
Expected: [ 3350 3319 3331 ]	Sum: 10000
Actual:   [ 3350 3319 3331 ]	Sum: 10000
--
Expected: [ 3264 3396 3340 ]	Sum: 10000
Actual:   [ 3264 3396 3340 ]	Sum: 10000
--
Expected: [ 3346 3328 3326 ]	Sum: 10000
Actual:   [ 3346 3328 3326 ]	Sum: 10000
--
Expected: [ 3355 3287 3358 ]	Sum: 10000
Actual:   [ 3355 3287 3358 ]	Sum: 10000
--
Expected: [ 3301 3440 3259 ]	Sum: 10000
Actual:   [ 3301 3440 3259 ]	Sum: 10000
--
From the outputs above, it is clear that the smokers seem to be chosen more or
less randomly, and that the expected outputs match the actual outputs of the
histogram, thus verifying the correctness of the program.
-------------------------------------------------------------------------------
washroom.c

Below is the output for five test runs of the program:
--
Created 12 male thread(s) and 8 female thread(s), running 100 iterations each.
        1   2   3    T
Male:   85  142 973  1200
Female: 94  173 533  800
Total:  179 315 1506 2000
Histogram: [ 276 88 126 95 87 80 65 64 54 61 62 39 63 54 41 57 46 52 43 32 ]
Overflow: 515
--
Created 6 male thread(s) and 14 female thread(s), running 100 iterations each.
        1   2   3    T
Male:	  79	148	373  600
Female: 87  123	1190 1400
Total:  166	271	1563 2000
Histogram: [ 302 95 111 64 68 76 55 71 79 82 67 62 59 48 53 48 45 46 33 29 ]
Overflow: 507
--
Created 6 male thread(s) and 14 female thread(s), running 100 iterations each.
        1   2   3    T
Male:   77  154 369  600
Female: 84  127 1189 1400
Total:  161 281 1558 2000
Histogram: [ 281 103 112 66 77 84 67 82 57 72 70 46 60 58 48 52 47 39 35 39 ]
Overflow: 505
--
Created 13 male thread(s) and 7 female thread(s), running 100 iterations each.
        1   2   3    T
Male:   84  121 1095 1300
Female: 80  169 451  700
Total:  164 290 1546 2000
Histogram: [ 284 105 101 81 104 83 73 79 82 66 52 64 49 49 60 44 49 37 40 27 ]
Overflow: 471
--
Created 19 male thread(s) and 1 female thread(s), running 100 iterations each.
        1   2   3    T
Male:   96  121 1683 1900
Female: 100 0   0    100
Total:  196 121 1683 2000
Histogram: [ 126 37 80 105 122 122 123 107 103 94 63 64 61 56 48 38 50 46 42 32 ]
Overflow: 481
--
The counters seem to suggest that the program is relatively fair in terms of
allowing males/females to enter, as the majority of the occupancy is at 3.
Likewise, the histogram of waiting times suggests that many employees do not
have to wait for very long in order to enter the washroom. Therefore, we can
conclude that the algorithm is relatively efficient, and that the program
properly accesses all of the shared resources while running threads
concurrently.
-------------------------------------------------------------------------------
pc_sem.c

Below is the output for five test runs of the program, with an iteration size
of 25000 rounds for each consumer/producer thread (with two of each), and thus
100000 total changes in the number of items:
--
Histogram: [ 1772 5762 9540 15236 14241 9066 13972 14690 8789 5246 1686 ]
Sum: 100000
--
Histogram: [ 1771 5866 9724 14485 13759 9793 13856 14367 9182 5489 1708 ]
Sum: 100000
--
Histogram: [ 1937 6173 9793 15303 14310 8964 13475 14252 8886 5308 1599 ]
Sum: 100000
--
Histogram: [ 1748 5501 9045 14617 14030 9366 14229 14879 9201 5637 1747 ]
Sum: 100000
--
Histogram: [ 1808 5647 9340 14836 13827 9135 13546 14193 9535 6189 1944 ]
Sum: 100000
--
From the outputs above, we can see that the total number of counts in the
histogram matches with what is expected, and because of the relatively random
distribution of frequencies across the histograms, we can also conclude that
the program concurrently produces/consumes items correctly.
-------------------------------------------------------------------------------
smoke_sem.c

Below is the output for five test runs of the program, with an iteration size
of 1000000 rounds. The categories in the counters refer to the type of resource
that each smoker has (i.e. tobacco, paper, and matches, respectively):
--
Expected: [ 332851 334030 333119 ]	Sum: 1000000
Actual:   [ 332851 334030 333119 ]	Sum: 1000000
--
Expected: [ 333300 332870 333830 ]	Sum: 1000000
Actual:   [ 333300 332870 333830 ]	Sum: 1000000
--
Expected: [ 333772 332655 333573 ]	Sum: 1000000
Actual:   [ 333772 332655 333573 ]	Sum: 1000000
--
Expected: [ 333968 333126 332906 ]	Sum: 1000000
Actual:   [ 333968 333126 332906 ]	Sum: 1000000
--
Expected: [ 333687 333146 333167 ]	Sum: 1000000
Actual:   [ 333687 333146 333167 ]	Sum: 1000000
--
From the outputs above, it is clear that the smokers seem to be chosen more or
less randomly, and that the expected outputs match the actual outputs of the
histogram, thus verifying the correctness of the program.
-------------------------------------------------------------------------------
washroom_sem.c

Below is the output for five test runs of the program:
--
Created 4 male thread(s) and 16 female thread(s), running 100 iterations each.
        1   2   3   T
Male:   410 6   1   417
Female: 395 415 773 1583
Total:  805 492 614 1984
Histogram: [ 10 14 29 10 2 3 2 2 1 3 8 5 7 16 167 257 519 468 417 2 ]
Overflow: 58
--
Created 3 male thread(s) and 17 female thread(s), running 100 iterations each.
        1   2   3   T
Male:	  314 13  1   328
Female: 291 391 990 1672
Total:  605 404 991 2000
Histogram: [ 28 13 24 6 5 4 5 6 10 14 4 3 13 40 322 310 454 400 258 1 ]
Overflow: 80
--
Created 6 male thread(s) and 14 female thread(s), running 100 iterations each.
        1   2   3   T
Male:   501 99  10  610
Female: 494 426 470 1390
Total:  995 525 480 2000
Histogram: [ 12 19 24 8 3 1 3 4 2 1 5 5 7 11 86 163 517 607 459 2 ]
Overflow: 61
--
Created 1 male thread(s) and 19 female thread(s), running 100 iterations each.
        1   2   3    T
Male:   210 1   0    212
Female: 106 256 1426 1788
Total:  164 290 1546 2000
Histogram: [ 92 28 21 4 14 10 21 28 37 39 13 29 44 150 534 300 213 116 93 4 ]
Overflow: 210
--
Created 5 male thread(s) and 15 female thread(s), running 100 iterations each.
        1   2   3   T
Male:   422 84  16  522
Female: 406 406 666 1478
Total:  828 490 682 2000
Histogram: [ 17 24 19 8 6 4 5 1 6 4 4 6 4 19 183 294 520 461 350 7 ]
Overflow: 58
--
The counters seem to suggest that the program is relatively fair in terms of
allowing males/females to enter, as the waiting times suggests that many
employees do not have to wait for too long in order to enter the washroom. In
this case, we sacrificed fairness slightly for efficiency, but nonetheless,
it appears that all employees do eventually gain access to the washroom (that
is, none of the threads become starved). The numbers in the counter, however,
are a little inaccurate because I was unable to find a way to atomically update
the counters immediately after a semaphore wait completes.
