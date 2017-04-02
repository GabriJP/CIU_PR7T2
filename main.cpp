#include <stdio.h>
#include <cmath>

#include <GL/glew.h>
#include <GL/freeglut.h>

#define WINDOW_ANCHO 500
#define WINDOW_ALTO 500

// Espacio para las variables globales de la ventana
float gl_fovy = 60.0f;
float gl_fovy_min = 10.0f, gl_fovy_max = 90.0f, gl_fovy_paso = 1.0f;
float gl_cerca = 1.0f, gl_lejos = 100.0f;
float gl_center = 7.0f; // centro donde pondremos los objetos


// Espacio para otras variables globales
double theta = M_PI_2, phi = 0.0;
double radio = 7.0;

// Espacio para la declaraci�n de funciones
void InitGlew();

void InitGL();

void Display();

void ReshapeSize(int ancho, int alto);

void PosicionRaton(int x, int y);

void RuedaRaton(int rueda, int sentido, int x, int y);

//main
int main(int argc, char *argv[]) {

    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WINDOW_ANCHO, WINDOW_ALTO);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("OpenGL Practica ");
    InitGlew(); // despues de crear la primera ventana
    InitGL();
    glutDisplayFunc(Display); // registra la funcion de rendering
    glutReshapeFunc(ReshapeSize);
    glutMotionFunc(PosicionRaton);
    glutMouseWheelFunc(RuedaRaton);

    glutMainLoop(); // bucle principal

    return 0;
}

void InitGlew() {

    // para poder utilizar trasnparentemente todas las extensiones de OpenGL
    GLenum glew_init = glewInit();
    if (glew_init != GLEW_OK) {
        // Problem: glewInit failed, something is seriously wrong.
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_init));
    } else
        fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}

void setViewport() {
    int ancho = glutGet(GLUT_WINDOW_WIDTH);
    int alto = glutGet(GLUT_WINDOW_HEIGHT);
    glViewport(0, 0, ancho, alto); // utiliza la totalidad de la ventana
}

void setProjection() {
    int ancho = glutGet(GLUT_WINDOW_WIDTH);
    int alto = glutGet(GLUT_WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(gl_fovy, (float) ancho / (float) alto, gl_cerca, gl_lejos);
}

void setModelView() {

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void InitGL() {

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    // para cambiar el criterio de lo que se ve.
    // LESS es el de defecto: se ve lo que tiene menor profundidad
    glDepthFunc(GL_LESS);
    // TO DO

    setViewport();
    setProjection();
}

// cambio de dimensiones de la ventana, manteniendo la dimension de los objetos
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

void ReshapeSize(int ancho, int alto) {
#pragma clang diagnostic pop

    setViewport();
    setProjection();
    glutPostRedisplay();
}

void PosicionRaton(int x, int y) {

    int ancho = glutGet(GLUT_WINDOW_WIDTH);
    int alto = glutGet(GLUT_WINDOW_HEIGHT);

    theta = M_PI * (double) y / (double) alto;
    phi = -M_PI + 2 * M_PI * (double) x / (double) ancho;
    glutPostRedisplay();
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

void RuedaRaton(int rueda, int sentido, int x, int y) {
#pragma clang diagnostic pop

    gl_fovy -= gl_fovy_paso * (float) sentido;
    if (gl_fovy < gl_fovy_min) gl_fovy = gl_fovy_min;
    if (gl_fovy > gl_fovy_max) gl_fovy = gl_fovy_max;
    setProjection();
    glutPostRedisplay();
}

void caja(float x, float y, float z) {
    glPushMatrix();
    glScalef(x, y, z);
    glutWireCube(1.0f);
    glPopMatrix();
}

void body() {
    float cuerpo_ancho = 1.5f, cuerpo_alto = 2.0f, cuerpo_grueso = 0.5f;
    float cabeza = 1.0f, pierna = 2.5f, brazo = 2.0f;

    glPushMatrix();
    {
        //cabeza
        glPushMatrix();
        {
            glTranslatef(0.0f, (GLfloat) (cuerpo_alto / 2.0 + cabeza / 2.0), 0.0f);
            glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
            glutWireSphere(cabeza / 2.0, 10, 10);
        }
        glPopMatrix();

        //cuerpo
        caja(cuerpo_ancho, cuerpo_alto, cuerpo_grueso);

        //pierna
        glPushMatrix();
        {
            glTranslatef(0.8f * cuerpo_ancho / 2, (GLfloat) (-cuerpo_alto / 2.0), 0.0f);
            glRotatef(15.0f, 0.0f, 0.0f, 1.0f);
            glTranslatef(0.0f, -pierna / 2.0f, 0.0f);
            caja(0.2f, pierna, 0.2f);
        }
        glPopMatrix();

        //pierna
        glPushMatrix();
        {
            glTranslatef(-0.8f * cuerpo_ancho / 2, (GLfloat) (-cuerpo_alto / 2.0), 0.0f);
            glRotatef(-15.0f, 0.0f, 0.0f, 1.0f);
            glTranslatef(0.0f, -pierna / 2.0f, 0.0f);
            caja(0.2f, pierna, 0.2f);
        }
        glPopMatrix();

        //brazo
        glPushMatrix();
        {
            glTranslatef(cuerpo_ancho / 2, (GLfloat) (cuerpo_alto / 2.0), 0.0f);
            glRotatef(10.0f, 0.0f, 0.0f, 1.0f);
            glTranslatef(0.0f, -brazo / 2.0f, 0.0f);
            caja(0.2f, brazo, 0.2f);
        }
        glPopMatrix();

        //brazo
        glPushMatrix();
        {
            glTranslatef(-cuerpo_ancho / 2, (GLfloat) (cuerpo_alto / 2.0), 0.0f);
            glRotatef(-140.0f, 0.0f, 0.0f, 1.0f);
            glRotatef(40.0f, 1.0f, 0.0f, 0.0f);
            glTranslatef(0.0f, -brazo / 2.0f, 0.0f);
            caja(0.2f, brazo, 0.2f);
        }
        glPopMatrix();
    }
    glPopMatrix();
}

// funci�n de gestion de ventana
void Display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // borra todo lo existente en el framebuffer
    setModelView();

    // poicionamos el centro de coordenadas
    glTranslatef(0.0f, 0.0f, -gl_center);

    // el eje cenital es el Y
    float x = (float) (radio * sin(theta) * sin(phi));
    float y = (float) (radio * cos(theta));
    float z = (float) (radio * sin(theta) * cos(phi));
    gluLookAt(x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f); // mira al (0,0,0)

    // TO DO
    body();

    glFlush(); // actualiza el framebuffer
    glutSwapBuffers(); // en caso de animacion
}