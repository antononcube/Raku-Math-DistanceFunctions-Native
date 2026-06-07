#include <math.h>
#include <complex.h>
#include <string.h>
#include <stdlib.h>

#ifdef __APPLE__
    #include <Accelerate/Accelerate.h>
#endif

//=====================================================================
double SquaredEuclideanDistance(const double *vectorA, const double *vectorB, int count) {
    double result = 0.0;

#ifdef __APPLE__
    vDSP_distancesqD(vectorA, 1, vectorB, 1, &result, (vDSP_Length)count);
#else
    for (int i = 0; i < count; i++) {
        double diff = vectorA[i] - vectorB[i];
        result += diff * diff;
    }
#endif

    return result;
}

//-------------------------------------------------------------------
float SquaredEuclideanDistanceFloat(const float *vectorA, const float *vectorB, int count) {
    float result = 0.0f;

#ifdef __APPLE__
    vDSP_distancesq(vectorA, 1, vectorB, 1, &result, (vDSP_Length)count);
#else
    for (int i = 0; i < count; i++) {
        float diff = vectorA[i] - vectorB[i];
        result += diff * diff;
    }
#endif

    return result;
}

//-------------------------------------------------------------------
double SquaredEuclideanDistanceComplex(const complex double *vectorA, const complex double *vectorB, int count) {
    double result = 0.0;

#ifdef __APPLE__
    // Every complex double consists of two sequential doubles.
    vDSP_Length totalElements = (vDSP_Length)count * 2;
    vDSP_distancesqD((const double *)vectorA, 1, (const double *)vectorB, 1, &result, totalElements);
#else
    for (int i = 0; i < count; i++) {
        complex double diff = vectorA[i] - vectorB[i];
        double re = creal(diff);
        double im = cimag(diff);
        result += (re * re) + (im * im);
    }
#endif

    return result;
}

//-------------------------------------------------------------------
float SquaredEuclideanDistanceComplexFloat(const complex float *vectorA, const complex float *vectorB, int count) {
    float result = 0.0f;

#ifdef __APPLE__
    // Every complex float consists of two sequential floats.
    vDSP_Length totalElements = (vDSP_Length)count * 2;
    vDSP_distancesq((const float *)vectorA, 1, (const float *)vectorB, 1, &result, totalElements);
#else
    for (int i = 0; i < count; i++) {
        complex float diff = vectorA[i] - vectorB[i];
        float re = crealf(diff);
        float im = cimagf(diff);
        result += (re * re) + (im * im);
    }
#endif

    return result;
}

//=====================================================================
double EuclideanDistanceComplex(const complex double *vectorA, const complex double *vectorB, int count) {
    double squaredDistance = SquaredEuclideanDistanceComplex(vectorA, vectorB, count);
    return sqrt(squaredDistance);
}

//-------------------------------------------------------------------
float EuclideanDistanceComplexFloat(const complex float *vectorA, const complex float *vectorB, int count) {
    float squaredDistance = SquaredEuclideanDistanceComplexFloat(vectorA, vectorB, count);
    return sqrtf(squaredDistance);
}

//=====================================================================
double EuclideanDistance(const double *vectorA, const double *vectorB, int count) {
    double squaredDistance = SquaredEuclideanDistance(vectorA, vectorB, count);
    return sqrt(squaredDistance);
}

//-------------------------------------------------------------------
float EuclideanDistanceFloat(const float *vectorA, const float *vectorB, int count) {
    float squaredDistance = SquaredEuclideanDistanceFloat(vectorA, vectorB, count);
    return sqrtf(squaredDistance);
}

//=====================================================================
double DotProduct(const double *vectorA, const double *vectorB, int count) {
    double result = 0.0;

#ifdef __APPLE__
    vDSP_dotprD(vectorA, 1, vectorB, 1, &result, (vDSP_Length)count);
#else
    for (int i = 0; i < count; i++) {
        result += vectorA[i] * vectorB[i];
    }
#endif

    return result;
}

//-------------------------------------------------------------------
float DotProductFloat(const float *vectorA, const float *vectorB, int count) {
    float result = 0.0f;

#ifdef __APPLE__
    vDSP_dotpr(vectorA, 1, vectorB, 1, &result, (vDSP_Length)count);
#else
    for (int i = 0; i < count; i++) {
        result += vectorA[i] * vectorB[i];
    }
#endif

    return result;
}

//=====================================================================
complex double DotProductComplex(const complex double *vectorA, const complex double *vectorB, int count) {
    complex double result = 0.0 + 0.0 * I;

#ifdef __APPLE__
    cblas_zdotu_sub(count, vectorA, 1, vectorB, 1, &result);
#else
    for (int i = 0; i < count; i++) {
        result += vectorA[i] * vectorB[i];
    }
#endif

    return result;
}

//-------------------------------------------------------------------
complex float DotProductComplexFloat(const complex float *vectorA, const complex float *vectorB, int count) {
    complex float result = 0.0f + 0.0f * I;

#ifdef __APPLE__
    cblas_cdotu_sub(count, vectorA, 1, vectorB, 1, &result);
#else
    for (int i = 0; i < count; i++) {
        result += vectorA[i] * vectorB[i];
    }
#endif

    return result;
}

//=====================================================================
double CosineDistance(const double *vectorA, const double *vectorB, int count) {
    double dotProduct = 0.0;
    double normA = 0.0;
    double normB = 0.0;

#ifdef __APPLE__
    vDSP_dotprD(vectorA, 1, vectorB, 1, &dotProduct, (vDSP_Length)count);
    vDSP_svesqD(vectorA, 1, &normA, (vDSP_Length)count);
    vDSP_svesqD(vectorB, 1, &normB, (vDSP_Length)count);
#else
    for (int i = 0; i < count; i++) {
        dotProduct += vectorA[i] * vectorB[i];
        normA += vectorA[i] * vectorA[i];
        normB += vectorB[i] * vectorB[i];
    }
#endif

    normA = sqrt(normA);
    normB = sqrt(normB);

    if (normA == 0.0 || normB == 0.0) {
        return NAN;
    }

    return 1.0 - (dotProduct / (normA * normB));
}

//-------------------------------------------------------------------
float CosineDistanceFloat(const float *vectorA, const float *vectorB, int count) {
    float dotProduct = 0.0f;
    float normA = 0.0f;
    float normB = 0.0f;

#ifdef __APPLE__
    vDSP_dotpr(vectorA, 1, vectorB, 1, &dotProduct, (vDSP_Length)count);
    vDSP_svesq(vectorA, 1, &normA, (vDSP_Length)count);
    vDSP_svesq(vectorB, 1, &normB, (vDSP_Length)count);
#else
    for (int i = 0; i < count; i++) {
        dotProduct += vectorA[i] * vectorB[i];
        normA += vectorA[i] * vectorA[i];
        normB += vectorB[i] * vectorB[i];
    }
#endif

    normA = sqrtf(normA);
    normB = sqrtf(normB);

    if (normA == 0.0f || normB == 0.0f) {
        return NAN;
    }

    return 1.0f - (dotProduct / (normA * normB));
}

//=====================================================================
double CosineDistanceComplex(const complex double *vectorA, const complex double *vectorB, int count) {
    complex double dotProduct = 0.0 + 0.0 * I;
    double normA = 0.0;
    double normB = 0.0;

#ifdef __APPLE__
    cblas_zdotc_sub(count, vectorA, 1, vectorB, 1, &dotProduct);
    normA = cblas_dznrm2(count, vectorA, 1);
    normB = cblas_dznrm2(count, vectorB, 1);
#else
    for (int i = 0; i < count; i++) {
        dotProduct += conj(vectorA[i]) * vectorB[i];
        normA += creal(vectorA[i]) * creal(vectorA[i]) + cimag(vectorA[i]) * cimag(vectorA[i]);
        normB += creal(vectorB[i]) * creal(vectorB[i]) + cimag(vectorB[i]) * cimag(vectorB[i]);
    }

    normA = sqrt(normA);
    normB = sqrt(normB);
#endif

    if (normA == 0.0 || normB == 0.0) {
        return NAN;
    }

    return 1.0 - (cabs(dotProduct) / (normA * normB));
}

//-------------------------------------------------------------------
float CosineDistanceComplexFloat(const complex float *vectorA, const complex float *vectorB, int count) {
    complex float dotProduct = 0.0f + 0.0f * I;
    float normA = 0.0f;
    float normB = 0.0f;

#ifdef __APPLE__
    cblas_cdotc_sub(count, vectorA, 1, vectorB, 1, &dotProduct);
    normA = cblas_scnrm2(count, vectorA, 1);
    normB = cblas_scnrm2(count, vectorB, 1);
#else
    for (int i = 0; i < count; i++) {
        dotProduct += conjf(vectorA[i]) * vectorB[i];
        normA += crealf(vectorA[i]) * crealf(vectorA[i]) + cimagf(vectorA[i]) * cimagf(vectorA[i]);
        normB += crealf(vectorB[i]) * crealf(vectorB[i]) + cimagf(vectorB[i]) * cimagf(vectorB[i]);
    }

    normA = sqrtf(normA);
    normB = sqrtf(normB);
#endif

    if (normA == 0.0f || normB == 0.0f) {
        return NAN;
    }

    return 1.0f - (cabsf(dotProduct) / (normA * normB));
}

//=====================================================================
double VectorNorm(const double *vector, int count, const char *type) {
    double result = 0.0;

#ifdef __APPLE__
    if (strcmp(type, "1") == 0) {
        vDSP_svemgD(vector, 1, &result, (vDSP_Length)count);
    } else if (strcmp(type, "infinity") == 0 || strcmp(type, "max") == 0) {
        vDSP_maxmgvD(vector, 1, &result, (vDSP_Length)count);
    } else if (strcmp(type, "2") == 0 || strcmp(type, "euclidean") == 0) {
        vDSP_svesqD(vector, 1, &result, (vDSP_Length)count);
        result = sqrt(result);
    } else {
        result = NAN;
    }
#else
    if (strcmp(type, "1") == 0) {
        for (int i = 0; i < count; i++) {
            result += fabs(vector[i]);
        }
    } else if (strcmp(type, "infinity") == 0 || strcmp(type, "max") == 0) {
        for (int i = 0; i < count; i++) {
            double mag = fabs(vector[i]);
            if (mag > result) {
                result = mag;
            }
        }
    } else if (strcmp(type, "2") == 0 || strcmp(type, "euclidean") == 0) {
        for (int i = 0; i < count; i++) {
            result += vector[i] * vector[i];
        }
        result = sqrt(result);
    } else {
        result = NAN;
    }
#endif

    return result;
}

//-------------------------------------------------------------------
double VectorNormComplex(const complex double *vector, int count, const char *type) {
    double result = 0.0;

#ifdef __APPLE__
    if (strcmp(type, "2") == 0 || strcmp(type, "euclidean") == 0) {
        return cblas_dznrm2(count, vector, 1);
    }

    DSPDoubleSplitComplex split;
    split.realp = (double *)malloc((size_t)count * sizeof(double));
    split.imagp = (double *)malloc((size_t)count * sizeof(double));
    double *magnitudes = (double *)malloc((size_t)count * sizeof(double));

    if (split.realp == NULL || split.imagp == NULL || magnitudes == NULL) {
        free(split.realp);
        free(split.imagp);
        free(magnitudes);
        return NAN;
    }

    vDSP_ctozD((const DSPDoubleComplex *)vector, 2, &split, 1, (vDSP_Length)count);
    vDSP_zvabsD(&split, 1, magnitudes, 1, (vDSP_Length)count);

    if (strcmp(type, "1") == 0) {
        vDSP_sveD(magnitudes, 1, &result, (vDSP_Length)count);
    } else if (strcmp(type, "infinity") == 0 || strcmp(type, "max") == 0) {
        vDSP_maxvD(magnitudes, 1, &result, (vDSP_Length)count);
    } else {
        result = NAN;
    }

    free(split.realp);
    free(split.imagp);
    free(magnitudes);
#else
    if (strcmp(type, "1") == 0) {
        for (int i = 0; i < count; i++) {
            result += cabs(vector[i]);
        }
    } else if (strcmp(type, "infinity") == 0 || strcmp(type, "max") == 0) {
        for (int i = 0; i < count; i++) {
            double mag = cabs(vector[i]);
            if (mag > result) {
                result = mag;
            }
        }
    } else if (strcmp(type, "2") == 0 || strcmp(type, "euclidean") == 0) {
        for (int i = 0; i < count; i++) {
            result += creal(vector[i]) * creal(vector[i]) + cimag(vector[i]) * cimag(vector[i]);
        }
        result = sqrt(result);
    } else {
        result = NAN;
    }
#endif

    return result;
}

//-------------------------------------------------------------------
float VectorNormFloat(const float *vector, int count, const char *type) {
    float result = 0.0f;

#ifdef __APPLE__
    if (strcmp(type, "1") == 0) {
        vDSP_svemg(vector, 1, &result, (vDSP_Length)count);
    } else if (strcmp(type, "infinity") == 0 || strcmp(type, "max") == 0) {
        vDSP_maxmgv(vector, 1, &result, (vDSP_Length)count);
    } else if (strcmp(type, "2") == 0 || strcmp(type, "euclidean") == 0) {
        vDSP_svesq(vector, 1, &result, (vDSP_Length)count);
        result = sqrtf(result);
    } else {
        result = NAN;
    }
#else
    if (strcmp(type, "1") == 0) {
        for (int i = 0; i < count; i++) {
            result += fabsf(vector[i]);
        }
    } else if (strcmp(type, "infinity") == 0 || strcmp(type, "max") == 0) {
        for (int i = 0; i < count; i++) {
            float mag = fabsf(vector[i]);
            if (mag > result) {
                result = mag;
            }
        }
    } else if (strcmp(type, "2") == 0 || strcmp(type, "euclidean") == 0) {
        for (int i = 0; i < count; i++) {
            result += vector[i] * vector[i];
        }
        result = sqrtf(result);
    } else {
        result = NAN;
    }
#endif

    return result;
}

//-------------------------------------------------------------------
float VectorNormComplexFloat(const complex float *vector, int count, const char *type) {
    float result = 0.0f;

#ifdef __APPLE__
    if (strcmp(type, "2") == 0 || strcmp(type, "euclidean") == 0) {
        return cblas_scnrm2(count, vector, 1);
    }

    DSPSplitComplex split;
    split.realp = (float *)malloc((size_t)count * sizeof(float));
    split.imagp = (float *)malloc((size_t)count * sizeof(float));
    float *magnitudes = (float *)malloc((size_t)count * sizeof(float));

    if (split.realp == NULL || split.imagp == NULL || magnitudes == NULL) {
        free(split.realp);
        free(split.imagp);
        free(magnitudes);
        return NAN;
    }

    vDSP_ctoz((const DSPComplex *)vector, 2, &split, 1, (vDSP_Length)count);
    vDSP_zvabs(&split, 1, magnitudes, 1, (vDSP_Length)count);

    if (strcmp(type, "1") == 0) {
        vDSP_sve(magnitudes, 1, &result, (vDSP_Length)count);
    } else if (strcmp(type, "infinity") == 0 || strcmp(type, "max") == 0) {
        vDSP_maxv(magnitudes, 1, &result, (vDSP_Length)count);
    } else {
        result = NAN;
    }

    free(split.realp);
    free(split.imagp);
    free(magnitudes);
#else
    if (strcmp(type, "1") == 0) {
        for (int i = 0; i < count; i++) {
            result += cabsf(vector[i]);
        }
    } else if (strcmp(type, "infinity") == 0 || strcmp(type, "max") == 0) {
        for (int i = 0; i < count; i++) {
            float mag = cabsf(vector[i]);
            if (mag > result) {
                result = mag;
            }
        }
    } else if (strcmp(type, "2") == 0 || strcmp(type, "euclidean") == 0) {
        for (int i = 0; i < count; i++) {
            result += crealf(vector[i]) * crealf(vector[i]) + cimagf(vector[i]) * cimagf(vector[i]);
        }
        result = sqrtf(result);
    } else {
        result = NAN;
    }
#endif

    return result;
}

//===================================================================
double ComplexDistanceFunction4Args(
    const char *name,
    const double *vectorARe,
    const double *vectorAIm,
    const double *vectorBRe,
    const double *vectorBIm,
    int count
) {
    if (count < 0) {
        return NAN;
    }

    complex double *vectorA = (complex double *)malloc((size_t)count * sizeof(complex double));
    complex double *vectorB = (complex double *)malloc((size_t)count * sizeof(complex double));

    if (vectorA == NULL || vectorB == NULL) {
        free(vectorA);
        free(vectorB);
        return NAN;
    }

    for (int i = 0; i < count; i++) {
        vectorA[i] = vectorARe[i] + vectorAIm[i] * I;
        vectorB[i] = vectorBRe[i] + vectorBIm[i] * I;
    }

    double result;
    if (strcmp(name, "SquaredEuclideanDistance") == 0) {
        result = SquaredEuclideanDistanceComplex(vectorA, vectorB, count);
    } else if (strcmp(name, "EuclideanDistance") == 0) {
        result = EuclideanDistanceComplex(vectorA, vectorB, count);
    } else if (strcmp(name, "CosineDistance") == 0) {
        result = CosineDistanceComplex(vectorA, vectorB, count);
    } else {
        // Unknown method
        result = NAN;
    }
    // Cannot be use that easily since the DotProduct result is complex
    //    } else if (strcmp(name, "DotProduct") == 0) {
    //       complex_result = DotProductComplex(vectorA, vectorB, count);
    //    }

    free(vectorA);
    free(vectorB);

    return result;
}