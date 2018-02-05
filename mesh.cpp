#include "mesh.h"
#include <GL/gl.h>
#include <cstdlib>
#include <time.h>



point operator - (point a, point b) {return {a.x - b.x, a.y - b.y, a.z - b.z};}
point operator - (Vertex a, Vertex b) {return {a.x - b.x, a.y - b.y, a.z - b.z};}
point operator - (point a, Vertex b) {return {a.x - b.x, a.y - b.y, a.z - b.z};}
point operator - (Vertex a, point b) {return {a.x - b.x, a.y - b.y, a.z - b.z};}


point operator + (point a, point b) {return {a.x + b.x, a.y + b.y, a.z + b.z};}
point operator + (Vertex a, Vertex b) {return {a.x + b.x, a.y + b.y, a.z + b.z};}
point operator + (point a, Vertex b) {return {a.x + b.x, a.y + b.y, a.z + b.z};}
point operator + (Vertex a, point b) {return {a.x + b.x, a.y + b.y, a.z + b.z};}

double operator * (point a, point b) {return a.x * b.x + a.y * b.y + a.z * b.z;}
double operator * (Vertex a, Vertex b) {return a.x * b.x + a.y * b.y + a.z * b.z;}
double operator * (point a, Vertex b) {return a.x * b.x + a.y * b.y + a.z * b.z;}
double operator * (Vertex a, point b) {return a.x * b.x + a.y * b.y + a.z * b.z;}


point crossProduct(point u, point v) {return {u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x};}
point crossProduct(Vertex u, Vertex v) {return {u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x};}
point crossProduct(point u, Vertex v) {return {u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x};}
point crossProduct(Vertex u, point v) {return {u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x};}



point colors[] = {{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},{1.0,1.0,0.0}, {0.0,1.0,1.0}, {1.0,0.0,1.0}, {1.0,0.0,.7}, {.7,0.0,1.0}};


Mesh::Mesh()
{
    // setMeshToTetra();
    setMeshToEnclosingBox();

    insertPoint({0.1, 0.4, 0.0});
    insertPoint({0.4, 0.1, 0.0});


    flipEdge(7, 9);
}

void Mesh::setMeshToTetra()
{
    vertices.clear();
    triangles.clear();

    // vertices
    vertices.push_back(Vertex(0.0, 0.0, 1.0, 0)); // P0
    vertices.push_back(Vertex(0.0, 1.0, 0.0, 1)); // P1
    vertices.push_back(Vertex(-1.0, -0.5, 0.0, 3)); // P2
    vertices.push_back(Vertex(1.0, -0.5, 0.0, 2)); // P3

    // triangles
    triangles.push_back(Triangle(0, 2, 1, 1, 2, 3)); // T0 :  (P0, P1, P2), (T1, T2, T3)
    triangles.push_back(Triangle(1, 2, 3, 3, 2, 0)); // T1
    triangles.push_back(Triangle(1, 3, 0, 3, 0, 1)); // T2
    triangles.push_back(Triangle(0, 3, 2, 1, 0, 2)); // T3
}

void Mesh::setMeshToEnclosingBox()
{
    vertices.clear();
    triangles.clear();

    vertices.push_back(Vertex(0.0, 0.0, 0.0, 0));
    vertices.push_back(Vertex(1.0, 1.0, 0.0, 0));
    vertices.push_back(Vertex(0.0, 1.0, 0.0, 0));
    vertices.push_back(Vertex(1.0, 0.0, 0.0, 1));
    vertices.push_back(Vertex(0.5, 0.5, -1.0, 0));  // sommet infini

    triangles.push_back(Triangle(0, 1, 2, 3, 4, 1));
    triangles.push_back(Triangle(1, 0, 3, 5, 2, 0));
    triangles.push_back(Triangle(1, 3, 4, 5, 3, 1));
    triangles.push_back(Triangle(1, 4, 2, 4, 0, 2));
    triangles.push_back(Triangle(2, 4, 0, 5, 0, 3));
    triangles.push_back(Triangle(0, 4, 3, 2, 1, 4));
}

void Mesh::draw()
{
    // draw faces
    glBegin(GL_TRIANGLES);
    for (uint i = 0; i < triangles.size(); i++)
    {
        Triangle triangle = triangles[i];

        // change color for each triangle
        //glColor3dv(&colors[i%8].x);
        if (i == 7 || i == 9)
            glColor3ub(230, 160, 70);
        else
            glColor3ub(70, 160, 230);

        // for each vertices attached to the current triangle
        for (uint j = 0; j < 3; j++)
        {
            Vertex vertex = vertices[triangle.vertices_indexes[j]];
            glVertex3d(vertex.x, vertex.y, vertex.z);
        }
    }
    glEnd();

    // draw edges
    glBegin(GL_LINES);
    glColor3ub(0, 0, 0);
    for (uint i = 0; i < triangles.size(); i++)
    {
        Triangle triangle = triangles[i];

        // for each vertices attached to the current triangle
        for (uint j = 0; j < 3; j++)
        {
            Vertex vertex = vertices[triangle.vertices_indexes[j]];
            glVertex3d(vertex.x, vertex.y, vertex.z);
            vertex = vertices[triangle.vertices_indexes[(j+1)%3]];
            glVertex3d(vertex.x, vertex.y, vertex.z);
        }
    }
    glEnd();
}


bool Mesh::pointIsInsideTriangle(int tri_id, point p)
{
    Triangle &tri = triangles[tri_id];
    Vertex &a = vertices[tri.vertices_indexes[0]];
    Vertex &b = vertices[tri.vertices_indexes[1]];
    Vertex &c = vertices[tri.vertices_indexes[2]];

    bool p_left_ab = crossProduct(b-a, p-a) * crossProduct(b-a, c-b) >= 0;
    bool p_left_bc = crossProduct(c-b, p-b) * crossProduct(c-b, a-c) >= 0;
    bool p_left_ca = crossProduct(a-c, p-c) * crossProduct(a-c, b-a) >= 0;

    if (p_left_ab && p_left_bc && p_left_ca)
        return true;
    else
        return false;
}


bool Mesh::pointIsInsideTriangle(int tri_id, double x, double y, double z)
{
    return pointIsInsideTriangle(tri_id, {x, y, z});
}

int Mesh::pointInWhichTriangle(point p)
{
    for (uint i = 0; triangles.size(); i++)
    {
        if (pointIsInsideTriangle(i, p))
            return i;
    }
    return -1;
}


bool Mesh::trisAreIncident(int tri_id_1, int tri_id_2)
{
    for (uint i = 0; i < 3; i++)
        if (triangles[tri_id_1].triangles_indexes[i] == tri_id_2)
            return true;

    return false;
}


void Mesh::insertPoint(point p)
{
    // get the triangle the point p is going to be inserted in
    int i_g = pointInWhichTriangle(p);

    int i_g1 = triangles[i_g].triangles_indexes[1];
    int i_g2 = triangles[i_g].triangles_indexes[2];

    int i_nb = triangles.size();

    // point p is added to vertices
    vertices.push_back(Vertex(p.x, p.y, p.z, i_g));

    // modification of triangle n° i_g1
    int i_g1_r0;
    for (i_g1_r0 = 0; i_g1_r0 < 3; i_g1_r0++)
        if (triangles[i_g1].vertices_indexes[i_g1_r0] == triangles[i_g].vertices_indexes[0])
            break;
    triangles[i_g1].triangles_indexes[(i_g1_r0 + 2) % 3] = i_nb;

    // modification of triangle n° i_g2
    int i_g2_r1;
    for (i_g2_r1 = 0; i_g2_r1 < 3; i_g2_r1++)
        if (triangles[i_g2].vertices_indexes[i_g2_r1] == triangles[i_g].vertices_indexes[1])
            break;
    triangles[i_g2].triangles_indexes[(i_g2_r1 + 2) % 3] = i_nb + 1;

    // modification of triangle n° i_g (i_g0 not modified)
    triangles[i_g].triangles_indexes[1] = i_nb;
    triangles[i_g].triangles_indexes[2] = i_nb + 1;
    triangles[i_g].vertices_indexes[0] = vertices.size() - 1;



    // creation of triangle n° i_nb
    triangles.push_back(Triangle(triangles[i_g1].vertices_indexes[i_g1_r0],
                                 vertices.size() - 1,
                                 triangles[i_g1].vertices_indexes[(i_g1_r0 + 1) % 3],
                                 i_g, i_g1, i_nb + 1));

    // creation of triangle n° i_nb+1
    triangles.push_back(Triangle(triangles[i_g].vertices_indexes[1],
                                 triangles[i_g].vertices_indexes[0],
                                 triangles[i_g1].vertices_indexes[i_g1_r0],
                                 i_nb, i_g2, i_g));
}


void Mesh::flipEdge(int ig1, int ig2)
{
    if ( !trisAreIncident(ig1, ig2))
        return;

    // found 0 <= ir1 <= 2
    uint ir1;
    for (ir1 = 0; ir1 < 3 ; ir1++)
        if (triangles[ig1].triangles_indexes[(ir1+2)%3] == ig2)
            break;

    // found 0 <= ir2 <= 2
    uint ir2;
    for (ir2 = 0; ir2 < 3 ; ir2++)
        if (triangles[ig2].triangles_indexes[(ir2+2)%3] == ig1)
            break;

    // found ig3
    uint ig3 = triangles[ig1].triangles_indexes[(ir1+1)%3];

    // found ig4
    uint ig4 = triangles[ig2].triangles_indexes[(ir2+1)%3];

    // found 0 <= ir3 <= 2
    uint ir3;
    for (ir3 = 0; ir3 < 3 ; ir3++)
        if (triangles[ig3].triangles_indexes[ir3] == ig1)
            break;

    // found 0 <= ir4 <= 2
    uint ir4;
    for (ir4 = 0; ir4 < 3 ; ir4++)
        if (triangles[ig4].triangles_indexes[ir4] == ig2)
            break;

    // modification of triangle n°ig1
    //   ir1+1 --> ig2
    //   ir1+2 --> ig4
    //   ig1.vertex[ir1] --> ig2.vertex[ir2+2]
    triangles[ig1].triangles_indexes[(ir1+1)%3] = ig2;
    triangles[ig1].triangles_indexes[(ir1+2)%3] = ig4;
    triangles[ig1].vertices_indexes[ir1] = triangles[ig2].vertices_indexes[(ir2+2)%3];

    // modification of triangle n°ig2
    //   ir2+1 --> ig1
    //   ir2+2 --> ig3
    //   ig2.vertex[ir2] --> ig1.vertex[ir1+2]
    triangles[ig2].triangles_indexes[(ir2+1)%3] = ig1;
    triangles[ig2].triangles_indexes[(ir2+2)%3] = ig3;
    triangles[ig2].vertices_indexes[ir2] = triangles[ig1].vertices_indexes[(ir1+2)%3];

    // modification of triangle n°ig3
    //   ir3 --> ig2
    triangles[ig3].triangles_indexes[ir3] = ig2;

    // modification of triangle n°ig4
    //   ir4 --> ig1
    triangles[ig4].triangles_indexes[ir4] = ig1;
}


ostream& operator<<(ostream& os, Mesh& mesh)
{
    for (uint i = 0; i < mesh.triangles.size(); i++)
    {
        os << "triangle n° " << i << ":" << endl;
        os << mesh.triangles[i] << endl;
    }

    return os;
}
















