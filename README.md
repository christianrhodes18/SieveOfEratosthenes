# SieveOfEratosthenes with Multithreading
An ancient algorithm to compute prime numbers up to a given limit.

The Sieve of Eratosthenes works within a lower and upper bound. The lower bound is hard coded to equal 2. The upper bound can be changed to the highest prime number to be computed up to. This value can be changed on line 13, represented by M. The Sieve of Eratosthenes starts with the base value and iteratively marks the multiples of each prime (from the lower bound to the upper bound) as composite. 

This program utilizes multi-threading in C. To modify the number of threads, change NUM_THREADS on line 12.
