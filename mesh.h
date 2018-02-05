#ifndef MESH_H
#define MESH_H


#include "vertex.h"
#include "triangle.h"
#include <vector>
#include <iostream>

struct point {
    double x, y, z;
};


using namespace std;

class Mesh
{
public:
    Mesh();
    Mesh(vector<Vertex>* _v, vector<Triangle>* _t){vertices = *_v; triangles = *_t;}

    vector<Vertex> vertices;
    vector<Triangle> triangles;

    void draw();

    void setMeshToTetra();
    void setMeshToEnclosingBox();

    bool pointIsInsideTriangle(int tri_id, double x, double y, double z);
    bool pointIsInsideTriangle(int tri_id, point p);

    bool trisAreIncident(int tri_id_1, int tri_id_2);

    int pointInWhichTriangle(point p);

    void insertPoint(point p);
    void insertPoint(double x, double y, double z){return insertPoint({x, y, z});}

    void flipEdge(int id_tri_1, int id_tri_2);

    friend ostream& operator<<(ostream& os, Mesh& mesh);

};

#endif // MESH_H
