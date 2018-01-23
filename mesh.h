#ifndef MESH_H
#define MESH_H


#include "vertex.h"
#include "triangle.h"
#include <vector>

using namespace std;

class Mesh
{
public:
    Mesh();
    Mesh(vector<Vertex>* _v, vector<Triangle>* _t){vertices = *_v; triangles = *_t;}

    vector<Vertex> vertices;
    vector<Triangle> triangles;

    void draw();

    void set_mesh_to_tetra();
    void set_mesh_to_cube();


};

#endif // MESH_H
