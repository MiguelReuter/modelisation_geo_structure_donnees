#include "mesh.h"
#include <GL/gl.h>
#include <cstdlib>
#include <time.h>


struct point {
    double x, y, z;
};


point colors[] = {{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},{1.0,1.0,0.0}};


Mesh::Mesh()
{
    set_mesh_to_tetra();
}

void Mesh::set_mesh_to_tetra()
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




void Mesh::draw()
{
    glBegin(GL_TRIANGLES);

    for (int i = 0; i < triangles.size(); i++)
    {
        glColor3dv(&colors[i%4].x);

        Triangle* triangle = &(triangles[i]);

        for (int j = 0; j < 3; j++)
        {
            Vertex* vertex = &vertices[triangle->vertices_indexes[j]];

            // specify vertex
            glVertex3d(vertex->x, vertex->y, vertex->z);
        }

    }
    glEnd();
}
