#ifndef VECTOR_H
#define VECTOR_H


typedef struct V2 V2;
struct V2 {
    double x;
    double y;
};

V2 v2(double x, double y);

V2     v2Add(V2 a, V2 b);
V2     v2Subtract(V2 a, V2 b);
double v2DotProduct(V2 a, V2 b);
V2     v2Normalize(V2 v);
V2     v2Scale(V2 v, double factor);
int    v2Equals(V2 a, V2 b);

#endif /* VECTOR_H */
