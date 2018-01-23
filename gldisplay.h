#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#include <QGLWidget>
#include <mesh.h>
#include <QMouseEvent>

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

private:
    void drawSierpinski();
    Mesh mesh;

    float _angle;
    QPoint _position;
    
signals:
    
public slots:
    
};

#endif // GLDISPLAY_H
