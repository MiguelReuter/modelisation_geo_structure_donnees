#ifndef VERTEX_H
#define VERTEX_H


class Vertex
{
public:
    Vertex();
    Vertex(double _x, double _y, double _z, int tri_ind);

    int triangle_index;
    double x, y, z;

};

#endif // VERTEX_H
