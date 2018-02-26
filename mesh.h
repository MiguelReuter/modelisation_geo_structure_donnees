#ifndef MESH_H
#define MESH_H


#include "vertex.h"
#include "triangle.h"
#include <vector>
#include <iostream>
#include <GL/gl.h>
#include <cstdlib>
#include <time.h>
#include "math.h"

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

    // drawing
    void draw() const;
    void drawVoronoi() const;

    // mesh definition
    void setMeshToTetra();
    void setMeshToEnclosingBox();

    // changes on mesh
    void insertPoint(point p);
    void insertPoint(double x, double y, double z){return insertPoint({x, y, z});}
    void flipEdge(int id_tri_1, int id_tri_2);

    //get infos
    double getAngle(uint tri_id, uint a_id, uint b_id, uint c_id) const;
    Vertex getTriangleCenter(uint tri_id) const;

    int inWhichTriangle(point p);

    // Delaunay
    void LawsonAlgorithm();
    void incrementalLawson(vector<uint> tris_id, uint vertex_id);
    bool isDelaunayTriangulation();

    // predicats
    //  - point
    bool isInsideTriangle(int tri_id, double x, double y, double z) {return isInsideTriangle(tri_id, {x, y, z});}
    bool isInsideTriangle(int tri_id, point p);

    bool isInCircle(point s, point p, point q, point r) const;
    bool isInCircle(point s, const Triangle& triangle) const;
    bool isInCircle(const Vertex& s, const Triangle& triangle) const {return isInCircle({s.x, s.y, s.z}, triangle);}
    bool isInCircle(const Vertex& s, const Vertex& p, const Vertex& q, const Vertex& r) const;
    //  - edge
    bool isEdgeLocallyDelaunay(int tri_id1, int tri_id2) const;
    //  - triangle
    bool areIncident(int tri_id_1, int tri_id_2) const;
    bool isTriangleInfinite(int tri_id) const;

    friend ostream& operator<<(ostream& os, Mesh& mesh);
};

#endif // MESH_H
