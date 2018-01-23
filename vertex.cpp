#include "vertex.h"

Vertex::Vertex()
{
    x = -1;
    y = -1;
    z = -1;
    triangle_index = -1;
}

Vertex::Vertex(double _x, double _y, double _z, int tri_ind)
{
    x = _x;
    y = _y;
    z = _z;
    triangle_index = tri_ind;
}
