#ifndef V2_H
#define V2_H


double clamp(double value, double min, double max);


/*
 *  Library for handling 2-dimensional vectors. Implements the basic operations and
 *  usefull functionality.
 */
typedef struct V2 V2;
struct V2
{
    double x, y;
};

V2     v2(double x, double y);
V2     v2Sample(V2 low, V2 high);
V2     v2Add(V2 a, V2 b);
V2     v2Subtract(V2 a, V2 b);
V2     v2Scale(V2 v, double s);
V2     v2Hadamard(V2 a, V2 b);
V2     v2Normalize(V2 v);
int    v2Equals(V2 a, V2 b);
double v2DotProduct(V2 a, V2 b);
double v2Length(V2 v);


#endif // V2_H
