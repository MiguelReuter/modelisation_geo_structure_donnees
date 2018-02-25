#ifndef MESH_H
#define MESH_H


#include "vertex.h"
#include "triangle.h"
#include <vector>
#include <iostream>
#include <GL/gl.h>
#include <cstdlib>
#include <time.h>

#include <queue>

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

    bool edgeIsLocallyDelaunay(int tri_id1, int tri_id2) const;

    void LawsonAlgorithm();
    void incrementalLawson(vector<uint> tris_id, uint vertex_id);
    bool isDelaunayTriangulation();


    bool trisAreIncident(int tri_id_1, int tri_id_2) const;

    bool isTriangleInfinite(int tri_id);


    bool pointIsInCircle(point s, point p, point q, point r) const;
    bool pointIsInCircle(point s, const Triangle& triangle) const;
    bool pointIsInCircle(const Vertex& s, const Triangle& triangle) const {return pointIsInCircle({s.x, s.y, s.z}, triangle);}
    bool pointIsInCircle(const Vertex& s, const Vertex& p, const Vertex& q, const Vertex& r) const;

    int pointInWhichTriangle(point p);

    void insertPoint(point p);
    void insertPoint(double x, double y, double z){return insertPoint({x, y, z});}

    void flipEdge(int id_tri_1, int id_tri_2);

    friend ostream& operator<<(ostream& os, Mesh& mesh);

};

#endif // MESH_H
