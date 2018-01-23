#include "triangle.h"

Triangle::Triangle()
{
    triangles_indexes[0] = -1;
    triangles_indexes[1] = -1;
    triangles_indexes[2] = -1;

    vertices_indexes[0] = -1;
    vertices_indexes[1] = -1;
    vertices_indexes[2] = -1;
}


Triangle::Triangle(int s0, int s1, int s2, int t0, int t1, int t2)
{
    triangles_indexes[0] = s0;
    triangles_indexes[1] = s1;
    triangles_indexes[2] = s2;

    vertices_indexes[0] = t0;
    vertices_indexes[1] = t1;
    vertices_indexes[2] = t2;
}
