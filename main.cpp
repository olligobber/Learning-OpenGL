#include <GL/gl.h>
#include <GL/glut.h>

static int fade = 0;

void display(void) {
    glClear (GL_COLOR_BUFFER_BIT);
    glColor3f (0.5 + fade/2000.0, 0.5 + fade/2000.0, 1.0);
    glBegin(GL_POLYGON);
        glVertex3f (-0.5, -0.5, 0.0);
        glVertex3f (-0.5, 0.5, 0.0);
        glVertex3f (0.5, 0.5, 0.0);
        glVertex3f (0.5, -0.5, 0.0);
    glEnd();
    glutSwapBuffers();
}

void idle(void) {
    fade++;
    if (fade > 1000) fade = 0;
    glutPostRedisplay();
}

int main (int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (250, 250);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("hello");
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
