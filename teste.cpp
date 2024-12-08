#include <GL/freeglut.h>
#include <GL/gl.h>
#include <cmath>

// Textura do chão
GLuint sandTexture;

// Ângulo de rotação do trator
float tractorAngle = 0.0f;

// Função para criar uma textura de areia simples
void createSandTexture() {
    const int texSize = 64;
    unsigned char texture[texSize][texSize][3];

    for (int i = 0; i < texSize; i++) {
        for (int j = 0; j < texSize; j++) {
            unsigned char c = rand() % 32 + 192;
            texture[i][j][0] = c;
            texture[i][j][1] = c - 40;
            texture[i][j][2] = c - 80;
        }
    }

    glGenTextures(1, &sandTexture);
    glBindTexture(GL_TEXTURE_2D, sandTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texSize, texSize, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

// Função para desenhar um cubo
void drawCube() {
    glBegin(GL_QUADS);
    // Frente
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    // Trás
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    // Topo
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    // Base
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    // Direita
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    // Esquerda
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glEnd();
}

// Função para desenhar uma roda
void drawWheel() {
    glPushMatrix();
    glColor3f(0.2f, 0.2f, 0.2f);
    glutSolidTorus(0.1, 0.2, 20, 20);
    glPopMatrix();
}

// Função para desenhar o trator
void drawTractor() {
    // Corpo principal
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    glScalef(2.0f, 1.0f, 1.0f);
    drawCube();
    glPopMatrix();

    // Cabine
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, 0.75f, 0.0f);
    glScalef(1.0f, 0.5f, 0.8f);
    drawCube();
    glPopMatrix();

    // Rodas
    glPushMatrix();
    glTranslatef(-0.8f, -0.5f, 0.6f);
    drawWheel();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.8f, -0.5f, 0.6f);
    drawWheel();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.8f, -0.5f, -0.6f);
    drawWheel();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.8f, -0.5f, -0.6f);
    drawWheel();
    glPopMatrix();
}

// Função para desenhar o chão
void drawGround() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, sandTexture);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, -1.0f, -10.0f);
    glTexCoord2f(10.0f, 0.0f); glVertex3f(10.0f, -1.0f, -10.0f);
    glTexCoord2f(10.0f, 10.0f); glVertex3f(10.0f, -1.0f, 10.0f);
    glTexCoord2f(0.0f, 10.0f); glVertex3f(-10.0f, -1.0f, 10.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

// Função de desenho principal
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(5.0f, 3.0f, 5.0f,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);

    drawGround();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(tractorAngle, 0.0f, 1.0f, 0.0f);
    drawTractor();
    glPopMatrix();

    glutSwapBuffers();
}

// Função de atualização
void update(int value) {
    tractorAngle += 0.5f;
    if (tractorAngle > 360.0f) {
        tractorAngle -= 360.0f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// Configuração da iluminação
void setupLighting() {
    GLfloat ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat position[] = {5.0f, 5.0f, 5.0f, 0.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Trator na Areia");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);

    setupLighting();
    createSandTexture();

    glutDisplayFunc(display);
    glutTimerFunc(25, update, 0);

    glutMainLoop();
    return 0;
}
