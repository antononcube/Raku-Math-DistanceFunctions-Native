# Math::DistanceFunctions::Native

[![Actions Status](https://github.com/antononcube/Raku-Math-DistanceFunctions-Native/actions/workflows/linux.yml/badge.svg)](https://github.com/antononcube/Raku-Math-DistanceFunctions-Native/actions)
[![Actions Status](https://github.com/antononcube/Raku-Math-DistanceFunctions-Native/actions/workflows/macos.yml/badge.svg)](https://github.com/antononcube/Raku-Math-DistanceFunctions-Native/actions)

[![License: Artistic-2.0](https://img.shields.io/badge/License-Artistic%202.0-0298c3.svg)](https://opensource.org/licenses/Artistic-2.0)

Raku package with distance functions implemented in C.
Apple's Accelerate library is used if available.

The primary motivation for making this library is to have fast sorting and nearest neighbors computations 
over collections of LLM-embedding vectors.

------

## Usage examples


### Regular vectors

Make a large (largish) collection of large vectors and find Euclidean distances over them:

```perl6
use Math::DistanceFunctions::Native;

my @vecs = (^1000).map({ (^1000).map({1.rand}).cache.Array }).Array;
my @searchVector = (^1000).map({1.rand});

my $start = now;
my @dists = @vecs.map({ euclidean-distance($_, @searchVector)});
my $tend = now;
say "Total time of computing {@vecs.elems} distances: {round($tend - $start, 10 ** -6)} s";
say "Average time of a single distance computation: {($tend - $start) / @vecs.elems} s";
```
```
# Total time of computing 1000 distances: 0.63326 s
# Average time of a single distance computation: 0.0006332598499999999 s
```

### `CArray` vectors

Use `CArray` vectors instead:

```perl6
use NativeCall;
my @cvecs = @vecs.map({ CArray[num64].new($_) });
my $cSearchVector = CArray[num64].new(@searchVector);

$start = now;
my @cdists = @cvecs.map({ euclidean-distance($_, $cSearchVector)});
$tend = now;
say "Total time of computing {@cvecs.elems} distances: {round($tend - $start, 10 ** -6)} s";
say "Average time of a single distance computation: {($tend - $start) / @cvecs.elems} s";
```
```
# Total time of computing 1000 distances: 0.002994 s
# Average time of a single distance computation: 2.994124e-06 s
```

I.e., we get ≈ 200 times speed-up using `CArray` vectors and the functions of this package. 

### Edit distance

The loading of this package automatically loads the (C-implemented) function `edit-distance` of
["Math::DistanceFunctions::Edit"](https://github.com/antononcube/Raku-Math-DistanceFunctions-Edit).
Here is an example usage:

```perl6
edit-distance('racoon', 'raccoon')
```
```
# 1
```
