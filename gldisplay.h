#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#include <QGLWidget>
#include <mesh.h>
#include <QMouseEvent>
#include <QMatrix4x4>

class GLDisplay : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLDisplay(QWidget *parent = 0);

    virtual void initializeGL();

    virtual void paintGL(); // Déclenche l'affichage du gasket

    virtual void resizeGL(int w, int h); // Spécifie également la partie de la scène à afficher

protected:
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void wheelEvent(QWheelEvent *event);

private:
    void drawSierpinski();
    Mesh mesh;

    float _angle;
    float _zoom;
    QPoint _position;


    int _viewport[2];
    QMatrix4x4 modelView;
    QMatrix4x4 projection;
    QVector3D toWorld(float x, float y, float z);
    void setProjection();
    void insertPoint();
    int getPickedTriangle();
    
signals:
    
public slots:
    
};

#endif // GLDISPLAY_H
