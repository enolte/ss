# ss

C++20 / 23 hobby project.

Mono-repo: *No submodules*

This repo includes a collection of generative algorithms for operations on subsets of $\mathcal{P}(X)$,
the power set of a set $X$. I've used these algorithms for sundry projects over time. They're re-written
here to the C++23 standard, optimized for time and memory, and with full generality of scope.

The underlying set is modeled as the usual abstraction, $X = \left\\{0,\\; ...,\\; N-1\right\\}$. In other words, an integer is a set. In practice, $X$ itself is usually an external index into some other data structure, so there is no loss of generality here.

## Scope

This repo implements 3 kinds of algorithms.

* Fixed-size subset iteration
* Multi-set iteration
* Subset sum accumulation

Each of these does what it sounds like it does. I might add others later.

Each of these algorithms has a legacy implementation.
This repo replaces those impls with memory-efficient bitwise versions with equivalent functionality.
Time complexity asymptotics are mentioned below.

As of today (Sunday, November 09, 2025), only the new impls for fixed-size and multi-sets are uploaded here.
When the updated impls are complete and performance comparisons tests are at least minimally complete, the older impls will be uploaded here for comparison.
Power sets are inherently extremely large (in the sense that they grow very quickly), so some discussion of execution speed and memory consumption is also to be included.

Commentary for these topics in this document is also in progress.


## Status

(_2:01 PM Sunday, November 09, 2025_)

I might be pausing work on this repo for a brief period. Current status of all iteration implementations:

|                              |forward iteration|backward iteration|random access iteration|uploaded|
|:-----------------------------|:----------------|:-----------------|:----------------------|--------|
|fixed-size (bitwise)          | done            | done             | working (1)           | yes    |
|fixed-size (`std::bitset`)    | done            | done             | not started           | no     |
|fixed-size (`std::size_t`s)   | done            | done             | working (1)           | no     |
|multi-sets (bitwise)          | done            | done             | in progress           | yes    |
|multi-sets (`std::bitset`) (2)| not started     | not started      | not started           | no     |
|multi-sets (`std::size_t`s)   | done            | done             | in progress           | no     |
|power set  (bitwise)          | done            | done             | not planned (3)       | no     |

(1) For now, random access iteration requires restarting from `begin` state. Removing this requirement is
a TODO. Iteration is _not_ done by successive forward states; it is currently implemented by successive binomial strides.

(2) Multi-sets done with a std::bitset are probably an indicator implementation. This may or may not be done for this repo.

(3) This is arbitrary-sized integer arithmetic, which is not the intent of this repo.


Subset sums

|                   |           |uploaded|
|:------------------|:----------|--------|
| positive sets     |done       | yes    |
| nonnegative sets  |in progress| no     |
| arbitrary sets    |in progress| no     |


<details>
<summary><h3>Previous updates</h3></summary>

(_2:38 PM Wednesday, November 05, 2025_)

Random access iteration for fixed-size subsets is now cyclic.
Still needs improvements, but it works as is.

(_8:01 PM Monday, November 03, 2025_)

Progress with fixed-size random access. With $C:=\binom{N}{K}$, getting the $C^{th}$ set by random access now correctly produces the zeroth set. `fixed_size::subset::get(i)` with $i \gt C$ is still not working, but it's close.

(_6:39 PM Friday, October 31, 2025_)

Progress with multi-set iteration.

* Fixed-size subset iteration: bit-packed impl random-access is pending. Otherwise complete.
* Multi-set iteration: bit-packed impl now does bidirectional iteration. Random access is pending.
* Subset summation: working for positive sets, in progress for the general case. Needs minor unit test improvements for the positive-set case.

(_7:57 PM Friday, October 24, 2025_)

Work in progress. Addign code to this repo beginning today. I have working versions of each implementation described below, but I'm in progress with updating the newer, faster ones for multiindices, and also with completing the general case of subset summation. Until then, they won't be included in this repo.

* Fixed-size subset iteration: Random access iteration is pending. Otherwise complete.
* Multi-set iteration: offset-based impl is complete (not included in this repo yet), bit-packed implementation is in progress
* Subset summation: working for positive sets, in progress for the general case. Needs minor unit test improvements for the positive-set case.

</details>

## Unit tests

From the repo root, this

```sh
$ g++ --std=c++23 test/main.cpp
```
compiles and links all unit tests. Verification requires only a small number of tests for this repo.

Unit tests cover fixed-size iteration, multiindex iteration, and subset summations.

## Subset accumulation

A simple dynamic programming implementation. Each previous subset sum is re-used by adding the next encountered
point to that sum. The result is a histogram of subset sums, returned as `std::unordered_map<std::int64_t, std::uint64_t>`.

This is essentially computing but not evaluating a generating polynomial, where each exponent in the polynomial
is a partial sum of set entries. With each encountered point, the polynomial is updated, and the accumulated sums are updated by rescanning the exponents. This is implicit in the dynamic programming implementation.

This is currently implemented only for sets (no duplicate elements). This may change later.

### Example

If $S = \\{1, 4, 5, 7\\}$, the generator $p$ is initialized to $p(x) = 1 = 1 + x^0$. The 0 exponent is the sum of points in the empty set.
Points are processed in encounter order.

* When the point 1 is encountered, $p$ changes: $p \leftarrow p(1 + x^1) = 1 + x$. The exponents 0 and 1 are the subset sums for $\left\\{\emptyset, \\{1\\}\right\\}$.

* When the point 4 is encountered, $p \leftarrow p(1 + x^4) = 1 + x + x^4 + x^5$. The exponents are now 0, 1, 4, 5,
for the subsets $\left\\{\emptyset, \\{1\\}, \\{4\\}, \\{1, 4\\}\right\\}$.

* For point 5, $p \leftarrow p(1 + x^5) = 1   + x   + x^4 + 2x^5 + x^6 + x^9 + x^{10}$, for $\left\\{\emptyset, \\{1\\}, \\{4\\}, \\{1, 4\\}, \\{5\\}, \\{1,5\\}, \\{4, 5\\}, \\{1, 4, 5\\}\right\\}$.
There are 2 subsets whose sum is 5.

* For point 7, $p \leftarrow p(1 + x^7) = 1   + x   + x^4    + 2x^5   + x^6 + x^7 + x^8 +  x^9   + x^{10} + x^{11} + 2x^{12}+ x^{13}+ x^{16}+ x^{17}$. The exponents are for the sets
$\left\\{\emptyset, \\{1\\},   \\{4\\},    \\{1, 4\\},    \\{5\\},   \\{1, 5\\},    \\{4, 5\\},    \\{1, 4, 5\\},
\\{7\\},   \\{1,7\\}, \\{4, 7\\}, \\{1, 4, 7\\}, \\{5, 7\\},\\{1, 5, 7\\}, \\{4, 5, 7\\}, \\{1, 4, 5, 7\\}\right\\} = \mathcal{P}(S)$,
and there are two sets with sum = 5 and two with sum = 12.

In table form, the implementation deduces the following:

|sum|count|subsets|
|--:|-----|-------|
| 0 |  1  | $\emptyset$
| 1 |  1  | $\left\\{1\right\\}$
| 4 |  1  | $\left\\{4\right\\}$
| 5 |  2  | $\left\\{5\right\\}, \left\\{1,4\right\\}$
| 6 |  1  | $\left\\{1, 5\right\\}$
| 7 |  1  | $\left\\{7\right\\}$
| 8 |  1  | $\left\\{1, 7\right\\}$
| 9 |  1  | $\left\\{4, 5\right\\}$
|10 |  1  | $\left\\{1, 4, 5\right\\}$
|11 |  1  | $\left\\{4, 7\right\\}$
|12 |  2  | $\left\\{1, 4, 7\right\\}, \left\\{5, 7\right\\}$
|13 |  1  | $\left\\{1, 5, 7\right\\}$
|16 |  1  | $\left\\{4, 5, 7\right\\}$
|17 |  1  | $\left\\{1, 4, 5, 7\right\\} = S$

The result is returned as a histogram / table of <sum, count>, the first 2 columns from the table above.

TODO 3:18 PM Sunday, November 09, 2025. Return only those counts for a sum in a specififed [min, max].

TODO 3:33 PM Sunday, November 09, 2025. Return a third component, the offset index of each subset in iteration order.



## Fixed-size subset iteration

This enumerates every subset of $X$ of some chosen size $K$, $0 \le K \le N = \vert{X\vert}$.
There are $\binom{N}{K}$ many such subsets. $N > 0$ is arbitrary, subject to storage constraints.

Iteration is over every integer < $2^N$ with exactly $K$ many one bits in its binary representation,
in numerically increasing order. With the LSB == rightmost bit, the resulting bit sequences are also
lexicographically ordered.

If the chain graph of $\mathcal{P}(X)$ is traversed with a breadth-first search starting from either $\emptyset$
or from $X$, this traversal iterates the sets at a given BFS level.

The naïve implementation of this generates every set in $\Theta(N2^N)$, by iterating the full power set. This is not necessary. Both the older impl this repo replaces, and the new one here, iterate  _directly_ from a set of size K to a next set of size K. Every set is generated in $\Theta(K(N-K)\binom{N}{K})$ time with constant additional stack memory (for counters, about 40 bytes).

For $n = \lvert X \rvert = 6$ and $k = 3$, the full bitwise subset iteration looks like this (zeroes omitted):

|   | 0 | 1 | 2 | 3 | 4 | 5 |
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
| 0 | 1 | 1 | 1 |   |   |   |
| 1 | 1 | 1 |   | 1 |   |   |
| 2 | 1 |   | 1 | 1 |   |   |
| 3 |   | 1 | 1 | 1 |   |   |
| 4 | 1 | 1 |   |   | 1 |   |
| 5 | 1 |   | 1 |   | 1 |   |
| 6 |   | 1 | 1 |   | 1 |   |
| 7 | 1 |   |   | 1 | 1 |   |
| 8 |   | 1 |   | 1 | 1 |   |
| 9 |   |   | 1 | 1 | 1 |   |
|10 | 1 | 1 |   |   |   | 1 |
|11 | 1 |   | 1 |   |   | 1 |
|12 |   | 1 | 1 |   |   | 1 |
|13 | 1 |   |   | 1 |   | 1 |
|14 |   | 1 |   | 1 |   | 1 |
|15 |   |   | 1 | 1 |   | 1 |
|16 | 1 |   |   |   | 1 | 1 |
|17 |   | 1 |   |   | 1 | 1 |
|18 |   |   | 1 |   | 1 | 1 |
|19 |   |   |   | 1 | 1 | 1 |


### Example usage

The above sequence of 20 subsets is produced by the following program. (N.B. op<< serializes MSB to LSB, so the
output is the left-to-right reverse of the above table.)

```c++
/*
  Compiled with

    g++ --std=c++23 -I. examples/fixed-size-all-subsets/example.cpp

  from the ss repo root.
 */

#include "subsets/fixed_size.h"
#include <iostream>

int main()
{
  int count{};

  using namespace ss::fixed_size;

  // Write to stdout every 3-subset of a 6-set.
  for(subset<6> subset{begin, 3}; subset != end; ++subset)
  {
    std::cout << count << " " << subset << std::endl;
    ++count;
  }

  return 0;
}
```

This code is in [examples/fixed-size-all-subsets/example.cpp](examples/fixed-size-all-subsets/example.cpp)

The following program code prints the first 10 subsets of $(N, K) = (89, 65)$ to stdout.

```c++
/*
  Compiled with

    g++ --std=c++23 -I. examples/00/example.cpp

  from the ss repo root.
 */

#include "subsets/fixed_size.h"
#include <iostream>

int main()
{
  int count{};

  // Select subsets of size 65 from a set of 89 points.
  for(ss::fixed_size::subset<89> subset{ss::fixed_size::begin, 65}; subset != ss::fixed_size::end; ++subset)
  {
    std::cout << subset << std::endl;

    if(++count == 10)
      break;
  }

  return 0;
}

```

This code is in [examples/00/example.cpp](examples/00/example.cpp)



### Bidirectional iteration

Bidirectional iteration is supported.

For the `ss::subset` type, it is circular. `end` are `rend` are distinct states, with distinct implementations. They are non-terminal, and a `subset` in either state is valid for "read" purposes. I.e., `end` and `rend` are not
so-called sentinels.

Sentinal singletons are provided, as `ss::end`, `ss::begin`, etc, to support optimized comparisons such as `susbet == end`.

The state transitions among `rend`, `begin`, `rbegin`, and `end` are very simple, and follow a natural pattern.
The intent is that `end` (`rend`) is entered only by forward (backward) iteration.

![docs/cyclic-iteration.png](docs/cyclic-iteration.png)

So for forward iteration: `rbegin --> end --> begin` and `rend --> begin`.

For backward iteration: `rbegin <-- rend <-- begin` and `rbegin <-- end`.




### Random Access Iteration

Random access support is implemented.

This uses the same cyclic semantics that bidirectional iteration does.
At the moment, it looks something like this:

```c++

subset<7> s{4, begin};
// `s` is now the subset {0, 1, 2, 3} = bits sequence 0001111

s.get(13);
// `s` is now the subset {1, 3, 4, 5} = bits sequence 0111010
```

A better API for this is pending.

For now, this is constrained by the requirement that $C(N,K) < 2^{64}$. Removing this requirement is in progress.

TODO 2:48 PM Wednesday, November 05, 2025. For now, this is done by resetting to the `begin` state before
iteration forward. I have a strategy for removing this requirement; there are a couple of implemetations
that come to mind. When I've selected one, that will proceed.



### Performance tests

In progress.

These tests compare different implementations for each of the
subset types in this repo. That are completed for fixed-size subsets. When I have time, I'll finish perf tests
for multi-sets and upload the results.

On my host, the following tables are a brief sample of results for fixed-size subsets described below.
* After the N and K columns, the next column in each table is the fastest absolute (`(abs)`) execution time
for forward or backward iteration.
* The columns thereafter are exec times relative to those two columns.

Note: Times in the `(abs)` column are for static allocation with *on-frame access only*.
Accessing a subset off-frame incurs a penalty of ~4ns on my host system.

Source impls compared here:
* `ss::index`: a `std::vector<std::uint64_t>`, sparse offset index into the underlying set.
This is the previous impl I used for these utilites.
* `ss::bits`: a `std::array<std::uint64_t>`, bit-packed and dense. This is the new impl.

Data collected over a minimum of 10,000,000 iterations for each
value.

Table units are `[ns / subset]`.

Forward iteration:

|  N   |  K  |ss::bits (abs) |  ss::index (rel)  |std::bitset (rel)  |
|:----:|:---:|--------------:|------------------:|------------------:|
|  502 | 200 |     0.3350    |        93.4468    |      1325.8696    |
|  848 | 318 |     0.3344    |       165.9450    |      2210.8060    |
|  999 | 233 |     0.3339    |       120.3990    |      2400.7920    |
|  999 | 712 |     0.3344    |       424.5800    |      3241.7576    |
| 1114 | 884 |     0.3372    |       532.3165    |      3731.2569    |
| 8850 |4117 |     0.3350    |      2570.9802    |     25396.9750    |

Backward iteration:

|  N   |  K  |ss::bits (abs) |  ss::index (rel)  |std::bitset (rel)  |
|:----:|:---:|--------------:|------------------:|------------------:|
|  502 | 200 |     0.3350    |        93.4305    |      1474.8884    |
|  848 | 318 |     0.3352    |       165.5847    |      2611.1661    |
|  999 | 233 |     0.3348    |       120.0779    |      3370.2465    |
|  999 | 712 |     0.3350    |       423.8702    |      2479.6406    |
| 1114 | 884 |     0.3401    |       527.7299    |      2599.5476    |
| 8850 |4117 |     0.3386    |      2543.8706    |     26419.8678    |

Notes:

1. `std::bitset` is slow. `ss::index` is ~10.4 x faster than `std::bitset`, even though `ss::index`
dynamically allocates, where `std::bitset` does not.

2. `ss::bits` execution period is essentially constant up to an array length
of 139 (implied max $N$ = 8895). This is due to iterating `ss::bits` on-frame.

3. `ss::bits` execution period is ~336 picoseconds / subset, with std dev = ~10 ps / subset / row (average).
These exec times are only on my host, so obviously not reproducible on another host, but
should give an estimate of what to expect.


## Fixed-length multi-set iteration

Updated impl is in progress.


For a given a set $X$, a (dense) multiindex is just an ordered sequence of counts $\left\\{m_0, ..., m_{N-1}\right\\}$,
where each $m_i \ge 0$ is the number of occurrences of the $i^{th}$ point in $X$.

Generating all multiindices of a fixed length $L = \sum m_i$ is done in a natural order.
There are $\binom{N + L - 1}{L}$ many such multiindices.

E.g., with N = 4 and L = 3, the entire sequence is this (zeroes omitted):

|   | 0 | 1 | 2 | 3 |
|:-:|:-:|:-:|:-:|:-:|
| 0 | 3 |   |   |   |
| 1 | 2 | 1 |   |   |
| 2 | 1 | 2 |   |   |
| 3 |   | 3 |   |   |
| 4 | 2 |   | 1 |   |
| 5 | 1 | 1 | 1 |   |
| 6 |   | 2 | 1 |   |
| 7 | 1 |   | 2 |   |
| 8 |   | 1 | 2 |   |
| 9 |   |   | 3 |   |
|10 | 2 |   |   | 1 |
|11 | 1 | 1 |   | 1 |
|12 |   | 2 |   | 1 |
|13 | 1 |   | 1 | 1 |
|14 |   | 1 | 1 | 1 |
|15 |   |   | 2 | 1 |
|16 | 1 |   |   | 2 |
|17 |   | 1 |   | 2 |
|18 |   |   | 1 | 2 |
|19 |   |   |   | 3 |


The legacy implementation (not included here yet) is a dense `std::array` of counts, literally as described here.

The replacement impl uses bit-packed integers, to reduce cache reloads. This will be performance tested with the `std::array` of counts. When that is complete, the old impl and comparison results will be added to this repo.


### Bidirectional iteration

Bidirectional iteration is supported, with the same circular semantics as described for fixed-size subset iteration.

### Random access

Random access is supported. Documentation pending.




