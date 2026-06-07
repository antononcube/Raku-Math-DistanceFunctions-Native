use v6.d;


sub EXPORT {
    use Math::DistanceFunctions::Edit;
    Map.new:
            '&edit-distance' => &Math::DistanceFunctions::Edit::edit-distance,
}

unit module Math::DistanceFunctions::Native;

use NativeCall;
use NativeHelpers::Array;

my constant $library = %?RESOURCES<libraries/DistanceFunctions>;

sub SquaredEuclideanDistance(CArray[num64], CArray[num64], int32 --> num64) is native($library) {*}
sub SquaredEuclideanDistanceFloat(CArray[num32], CArray[num32], int32 --> num32) is native($library) {*}

sub EuclideanDistance(CArray[num64], CArray[num64], int32 --> num64) is native($library) {*}
sub EuclideanDistanceFloat(CArray[num32], CArray[num32], int32 --> num32) is native($library) {*}

sub CosineDistance(CArray[num64], CArray[num64], int32 --> num64) is native($library) {*}
sub CosineDistanceFloat(CArray[num32], CArray[num32], int32 --> num32) is native($library) {*}

sub DotProduct(CArray[num64], CArray[num64], int32 --> num64) is native($library) {*}
sub DotProductFloat(CArray[num32], CArray[num32], int32 --> num32) is native($library) {*}

sub VectorNorm(CArray[num64], int32, Str --> num64) is native($library) {*}
sub VectorNormFloat(CArray[num32], int32, Str --> num32) is native($library) {*}

sub ComplexDistanceFunction4Args(Str, CArray[num64], CArray[num64], CArray[num64], CArray[num64], int32 --> num64) is native($library) {*}

#-----------------------------------------------------------

sub is-complex-array($x) {$x ~~ (Array:D | List:D | Seq:D) && $x.all ~~ Numeric:D && $x.any ~~ Complex:D }

#-----------------------------------------------------------
our proto sub squared-euclidean-distance($v1, $v2 --> Numeric:D) is export {*}

multi sub squared-euclidean-distance($v1, $v2 --> Numeric:D) {
    if !(($v1 ~~ Positional:D) && ($v2 ~~ Positional:D) && $v1.elems == $v2.elems) {
        die "The arguments are expected to be both numeric arrays of the same length.";
    }

    if $v1 ~~ CArray[num64] || $v2 ~~ CArray[num64] {

        return SquaredEuclideanDistance(
                $v1 ~~ CArray[num64] ?? $v1 !! copy-to-carray($v1, num64),
                $v2 ~~ CArray[num64] ?? $v2 !! copy-to-carray($v2, num64),
                $v1.elems)

    } elsif $v1 ~~ CArray[num32] && $v2 ~~ CArray[num32] {

        return SquaredEuclideanDistanceFloat($v1, $v2, $v1.elems)

    } elsif is-complex-array($v1) || is-complex-array($v2) {

        return ComplexDistanceFunction4Args(
                'SquaredEuclideanDistance',
                copy-to-carray($v1».Complex».re, num64),
                copy-to-carray($v1».Complex».im, num64),
                copy-to-carray($v2».Complex».re, num64),
                copy-to-carray($v2».Complex».im, num64),
                $v1.elems)

    } else {

        return SquaredEuclideanDistance(
                $v1 ~~ CArray ?? $v1 !! copy-to-carray($v1, num64),
                $v2 ~~ CArray ?? $v2 !! copy-to-carray($v2, num64),
                $v1.elems)
    }
}


#-----------------------------------------------------------
our proto sub euclidean-distance($v1, $v2 --> Numeric:D) is export {*}

multi sub euclidean-distance($v1, $v2 --> Numeric:D) {
    if !(($v1 ~~ Positional:D) && ($v2 ~~ Positional:D) && $v1.elems == $v2.elems) {
        die "The arguments are expected to be both numeric arrays of the same length.";
    }

    if $v1 ~~ CArray[num64] || $v2 ~~ CArray[num64] {

        return EuclideanDistance(
                $v1 ~~ CArray[num64] ?? $v1 !! copy-to-carray($v1, num64),
                $v2 ~~ CArray[num64] ?? $v2 !! copy-to-carray($v2, num64),
                $v1.elems)

    } elsif $v1 ~~ CArray[num32] && $v2 ~~ CArray[num32] {

        return EuclideanDistanceFloat($v1, $v2, $v1.elems);

    } elsif is-complex-array($v1) || is-complex-array($v2) {

        return ComplexDistanceFunction4Args(
                'EuclideanDistance',
                copy-to-carray($v1».Complex».re, num64),
                copy-to-carray($v1».Complex».im, num64),
                copy-to-carray($v2».Complex».re, num64),
                copy-to-carray($v2».Complex».im, num64),
                $v1.elems)

    } else {
        return EuclideanDistance(
                $v1 ~~ CArray ?? $v1 !! copy-to-carray($v1, num64),
                $v2 ~~ CArray ?? $v2 !! copy-to-carray($v2, num64),
                $v1.elems)
    }
}

#-----------------------------------------------------------
our proto sub cosine-distance($v1, $v2 --> Numeric:D) is export {*}

multi sub cosine-distance($v1, $v2 --> Numeric:D) {
    if !(($v1 ~~ Positional:D) && ($v2 ~~ Positional:D) && $v1.elems == $v2.elems) {
        die "The arguments are expected to be both numeric arrays of the same length.";
    }

    if $v1 ~~ CArray[num64] || $v2 ~~ CArray[num64] {

        return CosineDistance(
                $v1 ~~ CArray[num64] ?? $v1 !! copy-to-carray($v1, num64),
                $v2 ~~ CArray[num64] ?? $v2 !! copy-to-carray($v2, num64),
                $v1.elems)

    } elsif $v1 ~~ CArray[num32] && $v2 ~~ CArray[num32] {

        return CosineDistanceFloat($v1, $v2, $v1.elems)

    } elsif is-complex-array($v1) || is-complex-array($v2) {

        return ComplexDistanceFunction4Args(
                'CosineDistance',
                copy-to-carray($v1».Complex».re, num64),
                copy-to-carray($v1».Complex».im, num64),
                copy-to-carray($v2».Complex».re, num64),
                copy-to-carray($v2».Complex».im, num64),
                $v1.elems)

    } else {

        return CosineDistance(
                $v1 ~~ CArray ?? $v1 !! copy-to-carray($v1, num64),
                $v2 ~~ CArray ?? $v2 !! copy-to-carray($v2, num64),
                $v1.elems);
    }
}

#-----------------------------------------------------------
our proto sub dot-product($v1, $v2 --> Numeric:D) is export {*}

multi sub dot-product($v1, $v2 --> Numeric:D) {
    if !(($v1 ~~ Positional:D) && ($v2 ~~ Positional:D) && $v1.elems == $v2.elems) {
        die "The arguments are expected to be both numeric arrays of the same length.";
    }

    if $v1 ~~ CArray[num64] || $v2 ~~ CArray[num64] {

        return DotProduct(
                $v1 ~~ CArray[num64] ?? $v1 !! copy-to-carray($v1, num64),
                $v2 ~~ CArray[num64] ?? $v2 !! copy-to-carray($v2, num64),
                $v1.elems)

    } elsif $v1 ~~ CArray[num32] && $v2 ~~ CArray[num32] {

        return DotProductFloat($v1, $v2, $v1.elems)

    } elsif is-complex-array($v1) || is-complex-array($v2) {

        return ComplexDistanceFunction4Args(
                'DotProduct',
                copy-to-carray($v1».Complex».re, num64),
                copy-to-carray($v1».Complex».im, num64),
                copy-to-carray($v2».Complex».re, num64),
                copy-to-carray($v2».Complex».im, num64),
                $v1.elems)

    } else {

        return DotProduct(
                $v1 ~~ CArray ?? $v1 !! copy-to-carray($v1, num64),
                $v2 ~~ CArray ?? $v2 !! copy-to-carray($v2, num64),
                $v1.elems)
    }
}

#-----------------------------------------------------------
our proto sub norm(| --> Numeric:D) is export {*}

multi sub norm(:v(:$vector)!, Str:D :p(:$type) = '2' --> Numeric:D) {
    return norm($vector, :$type);
}

multi sub norm($v, Str:D :p(:$type) = '2' --> Numeric:D) {

    if !(($v ~~ Positional:D) && ($v.all ~~ Numeric:D)) {
        die "The first argument is expected to be a numeric array.";
    }
    if $v ~~ CArray[num32] {
        return VectorNormFloat($v, $v.elems, $type);
    } else {
        return VectorNorm($v ~~ CArray ?? $v !! copy-to-carray($v, num64), $v.elems, $type);
    }
}