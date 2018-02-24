#include "gldisplay.h"

#define FRUSTUM_SIZE 2.0f
#define FRUSTUM_Z 1000.0
#define DEGREES_PER_DELTA 0.00064

GLDisplay::GLDisplay(QWidget *parent) :
    QGLWidget(parent),
    _angle(0.0f)
{
}

void GLDisplay::initializeGL()
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    glFrontFace(GL_CCW);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glColor3f(1.0, 1.0, 0.0);

    _zoom = 1.0f;
}

void GLDisplay::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadMatrixf(modelView.constData());

    mesh.draw();  
}

void GLDisplay::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    _viewport[0] = w;
    _viewport[1] = h;

    setProjection();
}

void GLDisplay::setProjection() {
    int w = _viewport[0];
    int h = _viewport[1];

    glMatrixMode(GL_PROJECTION);

    projection.setToIdentity();

    // Aspect ratio fix
    if( w >= h)
        projection.ortho(-FRUSTUM_SIZE * w / h * _zoom,
                          FRUSTUM_SIZE * w / h * _zoom,
                         -FRUSTUM_SIZE * _zoom,
                          FRUSTUM_SIZE * _zoom,
                         -FRUSTUM_Z,
                          FRUSTUM_Z);
    else
        projection.ortho(-FRUSTUM_SIZE * _zoom,
                          FRUSTUM_SIZE * _zoom,
                         -FRUSTUM_SIZE * h / w * _zoom,
                          FRUSTUM_SIZE * h / w * _zoom,
                         -FRUSTUM_Z,
                          FRUSTUM_Z);

    glLoadMatrixf(projection.constData());

    glMatrixMode(GL_MODELVIEW);
}

void GLDisplay::mouseMoveEvent(QMouseEvent *event)
{
    if( event != NULL ) {
        QPoint position = event->pos();

        QVector2D delta(position.x() - _position.x(),
                        position.y() - _position.y());

        QMatrix4x4 rotM;
        rotM.rotate(delta.y(), 1.0, 0.0, 0.0);
        rotM.rotate(delta.x(), 0.0, 1.0, 0.0);

        modelView = rotM * modelView;

        _position = position;

        updateGL();
    }
}

void GLDisplay::mousePressEvent(QMouseEvent *event)
{

    /*
     1 - left button
     2 - right button
     4 - middle button
    */
    if( event != NULL )
    {
        _position = event->pos();
        if (event->buttons() ==  2)
            insertPoint();
        else if (event->buttons() ==  1)
        {
            uint tri_id = getPickedTriangle();
            if (tri_id != -1)
                cout << "picked triangle : "<< tri_id << endl;
        }
        updateGL();
    }
}

void GLDisplay::wheelEvent(QWheelEvent * event)
{
    float numDegrees = (float)event->delta() * DEGREES_PER_DELTA;

    _zoom += numDegrees;

    setProjection();

    updateGL();
}


QVector3D GLDisplay::toWorld(float x, float y, float z)
{
    int resolution = devicePixelRatio(); //custom pixel resolution (different from standard pixel resolution)
    float width = _viewport[0] / resolution;
    float height = _viewport[1] / resolution;

    // Normalize to [-1 .. 1]
    float screenX = (2.0f * x / width) - 1.0f;
    float screenY = (height - (2.0f * y)) / height;
    float screenZ = (2.0f * z) - 1.0f;

    QVector4D screen(screenX, screenY, screenZ, 1.0f);

    QMatrix4x4 viewProjection = projection * modelView;
    QMatrix4x4 viewProjectionInverted = viewProjection.inverted();

    screen = viewProjectionInverted * screen;

    float w = screen.w();

    return QVector3D(screen / w);
}

void GLDisplay::insertPoint()
{
    GLdouble    x = _position.x();
    GLdouble    y = _position.y();
    GLdouble    minZ = 0.0;
    GLdouble    maxZ = 1.0;

    // Get the picking ray coordinates.
    QVector3D rayMin = toWorld(x, y, minZ);
    QVector3D rayMax = toWorld(x, y, maxZ);

    GLdouble t = rayMin.z() / (rayMin.z() - rayMax.z());

    x = rayMin.x() + t * (rayMax.x() - rayMin.x());
    y = rayMin.y() + t * (rayMax.y() - rayMin.y());

    mesh.insertPoint(x, y, 0.0);
}


int GLDisplay::getPickedTriangle()
{
    GLdouble    x = _position.x();
    GLdouble    y = _position.y();
    GLdouble    minZ = 0.0;
    GLdouble    maxZ = 1.0;

    // Get the picking ray coordinates.
    QVector3D rayMin = toWorld(x, y, minZ);
    QVector3D rayMax = toWorld(x, y, maxZ);

    GLdouble t = rayMin.z() / (rayMin.z() - rayMax.z());

    x = rayMin.x() + t * (rayMax.x() - rayMin.x());
    y = rayMin.y() + t * (rayMax.y() - rayMin.y());

    return mesh.pointInWhichTriangle({x, y, 0.0});
}




