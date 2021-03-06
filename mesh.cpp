#include "mesh.h"


#define EDGE_MARGIN 0.00005



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
    setMeshToEnclosingBox();
    LawsonAlgorithm();
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

    vertices.push_back(Vertex(-1.01, -1.0, 0.0, 0));
    vertices.push_back(Vertex(1.0, 1.0, 0.0, 0));
    vertices.push_back(Vertex(-1.0, 1.0, 0.0, 0));
    vertices.push_back(Vertex(1.0, -1.0, 0.0, 1));
    vertices.push_back(Vertex(0.0, 0.0, -1.0, 0));  // sommet infini

    triangles.push_back(Triangle(0, 1, 2, 3, 4, 1));
    triangles.push_back(Triangle(1, 0, 3, 5, 2, 0));
    triangles.push_back(Triangle(1, 3, 4, 5, 3, 1));
    triangles.push_back(Triangle(1, 4, 2, 4, 0, 2));
    triangles.push_back(Triangle(2, 4, 0, 5, 0, 3));
    triangles.push_back(Triangle(0, 4, 3, 2, 1, 4));
}


void Mesh::insertPoint(point p)
{
    // get the triangle the point p is going to be inserted in
    int i_g = inWhichTriangle(p);

    if (i_g == -1)
        return;

    // if there is an existing point at the same position
    for (uint i = 0; i < 3; i++)
        if ( p.x == vertices[triangles[i_g].vertices_indexes[i]].x
             && p.y == vertices[triangles[i_g].vertices_indexes[i]].y
             && p.z == vertices[triangles[i_g].vertices_indexes[i]].z )
            return;

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




    // incremental Delaunay triangulation
    vector<uint> v;
    v.push_back(i_g);
    v.push_back(i_nb);
    v.push_back(i_nb + 1);
    incrementalLawson(v, vertices.size() - 1);
}

void Mesh::flipEdge(int ig1, int ig2)
{
    if ( !areIncident(ig1, ig2))
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


void Mesh::draw() const
{
    // draw faces
    glBegin(GL_TRIANGLES);
    for (uint i = 0; i < triangles.size(); i++)
    {
        if (isTriangleInfinite(i))
            glColor3ub(90, 140, 210);
        else
            glColor3ub(70, 160, 230);

        // for each vertices attached to the current triangle
        for (uint j = 0; j < 3; j++)
        {
            Vertex vertex = vertices[triangles[i].vertices_indexes[j]];
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
            glVertex3d(vertex.x, vertex.y, vertex.z + EDGE_MARGIN);
            vertex = vertices[triangle.vertices_indexes[(j+1)%3]];
            glVertex3d(vertex.x, vertex.y, vertex.z + EDGE_MARGIN);
        }
    }
    glEnd();
}

void Mesh::drawVoronoi() const
{
    glBegin(GL_LINES);
    glColor3ub(255, 0, 0);

    for (uint i = 0; i < triangles.size(); i++)
        for (uint j = 0; j < 3; j++)
        {
            Vertex center = getTriangleCenter(i);
            Vertex adj_center = getTriangleCenter(triangles[i].triangles_indexes[j]);

            if (!isTriangleInfinite(i))
            {
                glVertex3d(center.x, center.y, EDGE_MARGIN);

                if (isTriangleInfinite(triangles[i].triangles_indexes[j]))
                    glVertex3d(adj_center.x * 1000, adj_center.y * 1000, EDGE_MARGIN);
                else
                    glVertex3d(adj_center.x, adj_center.y, EDGE_MARGIN);
            }
        }
    glEnd();
}


double Mesh::getAngle(uint tri_id, uint a_id, uint b_id, uint c_id) const
{
    // get angle between [ab] and [ac] (a,b and c defined by a_id, b_id and c_id <= 2)
    Vertex a = vertices[triangles[tri_id].vertices_indexes[a_id]];
    Vertex b = vertices[triangles[tri_id].vertices_indexes[b_id]];
    Vertex c = vertices[triangles[tri_id].vertices_indexes[c_id]];

    double ab[2] = {b.x - a.x, b.y - a.y};
    double kab = 1 / sqrt(pow(ab[0], 2) + pow(ab[1], 2));
    ab[0] = kab * ab[0];
    ab[1] = kab * ab[1];

    double ac[2] = {c.x - a.x, c.y - a.y};
    double kac = 1 / sqrt(pow(ac[0], 2) + pow(ac[1], 2));
    ac[0] = kac * ac[0];
    ac[1] = kac * ac[1];

    return acos(ab[0] * ac[0] + ab[1] * ac[1]);
}

Vertex Mesh::getTriangleCenter(uint tri_id) const
{
    double alpha = getAngle(tri_id, 0, 1, 2);
    double beta = getAngle(tri_id, 1, 2, 0);
    double gamma = getAngle(tri_id, 2, 0, 1);

    double tan_bc = tan(beta) + tan(gamma);
    double tan_ca = tan(gamma) + tan(alpha);
    double tan_ab = tan(alpha) + tan(beta);

    Vertex a = vertices[triangles[tri_id].vertices_indexes[0]];
    Vertex b = vertices[triangles[tri_id].vertices_indexes[1]];
    Vertex c = vertices[triangles[tri_id].vertices_indexes[2]];

    double x = (a.x * tan_bc + b.x * tan_ca + c.x * tan_ab) / (tan_bc + tan_ca + tan_ab);
    double y = (a.y * tan_bc + b.y * tan_ca + c.y * tan_ab) / (tan_bc + tan_ca + tan_ab);

    return Vertex(x, y, 0.0, -1);
}

int Mesh::inWhichTriangle(point p)
{
    for (uint i = 0; i < triangles.size(); i++)
        if (isInsideTriangle(i, p))
            if (!isTriangleInfinite(i))
                return i;
    return -1;
}


bool Mesh::isInsideTriangle(int tri_id, point p)
{
    Triangle &tri = triangles[tri_id];
    Vertex &a = vertices[tri.vertices_indexes[0]];
    Vertex &b = vertices[tri.vertices_indexes[1]];
    Vertex &c = vertices[tri.vertices_indexes[2]];

    bool p_left_ab = crossProduct(b-a, p-a) * crossProduct(b-a, c-b) >= 0;
    bool p_left_bc = crossProduct(c-b, p-b) * crossProduct(c-b, a-c) >= 0;
    bool p_left_ca = crossProduct(a-c, p-c) * crossProduct(a-c, b-a) >= 0;

    return (p_left_ab && p_left_bc && p_left_ca);
}

bool Mesh::isInCircle(point s, point p, point q, point r) const
{
    // return true if point s is in circle defined by points p,q and r

    float det = (q.x - p.x) * ( (r.y - p.y) * ((s.x - p.x)*(s.x - p.x) + (s.y - p.y)*(s.y - p.y)) - ((r.x - p.x)*(r.x - p.x) + (r.y - p.y)*(r.y - p.y)) * (s.y - p.y));
    det -= (q.y - p.y) * ( (r.x - p.x)*((s.x - p.x)*(s.x - p.x) + (s.y - p.y)*(s.y - p.y)) - ((r.x - p.x)*(r.x - p.x) + (r.y - p.y)*(r.y - p.y))*(s.x - p.x));
    det += ((q.x - p.x)*(q.x - p.x) + (q.y - p.y)*(q.y - p.y)) * ((r.x - p.x)*(s.y - p.y) - (r.y-p.y)*(s.x-p.x));
    det *= -1;

    return det >= 0;
}

bool Mesh::isInCircle(point s, const Triangle &triangle) const
{
    point p = {this->vertices[triangle.vertices_indexes[0]].x, this->vertices[triangle.vertices_indexes[0]].y, this->vertices[triangle.vertices_indexes[0]].z};
    point q = {this->vertices[triangle.vertices_indexes[1]].x, this->vertices[triangle.vertices_indexes[1]].y, this->vertices[triangle.vertices_indexes[1]].z};
    point r = {this->vertices[triangle.vertices_indexes[2]].x, this->vertices[triangle.vertices_indexes[2]].y, this->vertices[triangle.vertices_indexes[2]].z};

    return isInCircle(s, p, q, r);
}

bool Mesh::isInCircle(const Vertex &s, const Vertex &p, const Vertex &q, const Vertex &r) const
{
    return isInCircle({s.x, s.y, s.z}, {p.x, p.y, p.z}, {q.x, q.y, q.z}, {r.x, r.y, r.z});
}

bool Mesh::isEdgeLocallyDelaunay(int tri_id1, int tri_id2) const
{
    // v1 : vertex in tri 1, in front off tri 2
    uint i;
    for (i = 0; i < 3; i++)
        if (triangles[tri_id1].triangles_indexes[i] == tri_id2)
            break;
    int v1 = triangles[tri_id1].vertices_indexes[i];

    // v2 : vertex in tri 2, in front off tri 1
    for (i = 0; i < 3; i++)
        if (triangles[tri_id2].triangles_indexes[i] == tri_id1)
            break;
    int v2 = triangles[tri_id2].vertices_indexes[i];

    return !(isInCircle(vertices[v1], triangles[tri_id2]) || isInCircle(vertices[v2], triangles[tri_id1]));
}

bool Mesh::areIncident(int tri_id_1, int tri_id_2) const
{
    for (uint i = 0; i < 3; i++)
        if (triangles[tri_id_1].triangles_indexes[i] == tri_id_2)
            return true;
    return false;
}

bool Mesh::isTriangleInfinite(int tri_id) const
{
    for (uint i = 0; i < 3; i++)
        if (triangles[tri_id].vertices_indexes[i] == 4)
            return true;
    return false;
}


void Mesh::LawsonAlgorithm()
{
    // edges to flip
    vector <pair <uint, uint> > edges_to_flip;
    for (uint i = 0; i < triangles.size(); i++)
        for (uint j = 0; j < 3; j++)
            if (triangles[i].triangles_indexes[j] > i)
                if (!isEdgeLocallyDelaunay(i, triangles[i].triangles_indexes[j]))
                    edges_to_flip.push_back(make_pair(i, triangles[i].triangles_indexes[j]));


    while (!edges_to_flip.empty())
    {
        pair <uint, uint> edge = edges_to_flip.back();
        edges_to_flip.pop_back();
        if (areIncident(edge.first, edge.second))
            if (!isEdgeLocallyDelaunay(edge.first, edge.second) && !isTriangleInfinite(edge.first) && !isTriangleInfinite(edge.second))
            {
                flipEdge(edge.first, edge.second);

                for (uint i = 0; i < 3; i++)
                {
                    if (triangles[edge.first].triangles_indexes[i] != edge.second)
                        edges_to_flip.push_back(make_pair(edge.first, triangles[edge.first].triangles_indexes[i]));

                    if (triangles[edge.second].triangles_indexes[i] != edge.first)
                        edges_to_flip.push_back(make_pair(edge.second, triangles[edge.second].triangles_indexes[i]));
                }
            }
    }
}

void Mesh::incrementalLawson(vector<uint> tris_id, uint vertex_id)
{
    vector<pair <uint, uint> > edges_to_flip;

    // def of conflict_zone
    for (uint i = 0; i < tris_id.size(); i++)
        for (uint j = 0; j < 3; j++)
            if (triangles[tris_id[i]].vertices_indexes[j] == vertex_id)
                edges_to_flip.push_back(make_pair(tris_id[i], triangles[tris_id[i]].triangles_indexes[j]));

    while (!edges_to_flip.empty())
    {
        pair <uint, uint> edge = edges_to_flip.back();
        edges_to_flip.pop_back();
        if (areIncident(edge.first, edge.second))
            if (!isEdgeLocallyDelaunay(edge.first, edge.second) && !isTriangleInfinite(edge.first) && !isTriangleInfinite(edge.second))
            {
                flipEdge(edge.first, edge.second);

                for (uint i = 0; i < 3; i++)
                {
                    if (triangles[edge.first].triangles_indexes[i] != edge.second)
                        edges_to_flip.push_back(make_pair(edge.first, triangles[edge.first].triangles_indexes[i]));

                    if (triangles[edge.second].triangles_indexes[i] != edge.first)
                        edges_to_flip.push_back(make_pair(edge.second, triangles[edge.second].triangles_indexes[i]));
                }
            }
    }
}

bool Mesh::isDelaunayTriangulation()
{
    for (uint i = 0; i < triangles.size(); i++)
        for (uint j = 0; j < 3; j++)
            if (triangles[i].triangles_indexes[j] > i)
                if ( !isEdgeLocallyDelaunay(i, triangles[i].triangles_indexes[j]) && !isTriangleInfinite(i) && !isTriangleInfinite(triangles[i].triangles_indexes[j]))
                    return false;
    return true;
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
















