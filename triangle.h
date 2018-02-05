#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <iostream>

using namespace std;

class Triangle
{
public:
    Triangle();
    Triangle(int s0, int s1, int s2, int t0, int t1, int t2);

    int triangles_indexes[3];
    int vertices_indexes[3];

    friend ostream& operator<<(ostream& os, const Triangle& triangle);
};

#endif // TRIANGLE_H
