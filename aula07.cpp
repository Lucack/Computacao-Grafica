#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

// Variáveis globais para o catavento
float cataventoPosX = 0.0f, cataventoPosY = 0.0f; // Posição do catavento
float cataventoScale = 1.0f;                      // Escala do catavento
float rotationAngle = 0.0f;                       // Ângulo de rotação das hélices
float rotationSpeed = 0.05f;                      // Velocidade de rotação
bool isRotating = true;                           // Controle de rotação
float cores[4][3] = {{0.506f, 0.768f, 0.94f}, {1.0f, 0.623f, 0.615f}, {0.659f, 0.870f, 0.231f}, {1.0f, 0.957f, 0.231f}}; // Cores das hélices

// Variáveis de tamanho para a haste e as hélices do catavento
float sizeX = 0.05f;           // Largura da haste
float sizeY = 0.8f;            // Altura da haste
float helixOuterRadius = 0.3f; // Comprimento das hélices (distância até o vértice mais afastado)
float helixInnerRadius = 0.15f; // Distância até o vértice mais próximo

void desenhaCatavento()
{
    // Desenha a haste
    glPushMatrix();
    glTranslatef(cataventoPosX, cataventoPosY, 0.0f);
    glScalef(cataventoScale, cataventoScale, 1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-sizeX, -sizeY);
    glVertex2f(sizeX, -sizeY);
    glVertex2f(sizeX, 0.0f);
    glVertex2f(-sizeX, 0.0f);
    glEnd();

    // Desenha as hélices com triângulos retângulos
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
    for (int i = 0; i < 4; ++i)
    {
        glColor3fv(cores[i]);
        glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, 0.0f); // Vértice no centro do catavento
        glVertex2f(helixOuterRadius * cos(i * M_PI / 2), helixOuterRadius * sin(i * M_PI / 2)); // Vértice mais afastado
        glVertex2f(helixInnerRadius * cos(i * M_PI / 2), helixInnerRadius * sin(i * M_PI / 2)); // Vértice mais próximo
        glEnd();
    }
    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    desenhaCatavento();
    glutSwapBuffers();
}

void atualiza(int value)
{
    // Atualiza o ângulo de rotação se estiver rodando
    if (isRotating)
    {
        rotationAngle += rotationSpeed;
        if (rotationAngle > 360.0f)
            rotationAngle -= 360.0f;
    }

    glutPostRedisplay();                  // Solicita a atualização da tela
    glutTimerFunc(16, atualiza, 0);       // Chama esta função novamente após 16ms
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'r':
        rotationSpeed = fabs(rotationSpeed);
        isRotating = true;
        break;
    case 'l':
        rotationSpeed = -fabs(rotationSpeed);
        isRotating = true;
        break;
    case 'p':
        isRotating = !isRotating;
        break;
    case '+':
        cataventoScale += 0.1f;
        break;
    case '-':
        if (cataventoScale > 0.2f)
            cataventoScale -= 0.1f;
        break;
    case 'c':
        for (int i = 0; i < 4; ++i)
        {
            cores[i][0] = (float)rand() / RAND_MAX;
            cores[i][1] = (float)rand() / RAND_MAX;
            cores[i][2] = (float)rand() / RAND_MAX;
        }
        break;
    case '1':
        rotationSpeed = 0.02f;
        break;
    case '2':
        rotationSpeed = 0.05f;
        break;
    case '3':
        rotationSpeed = 0.1f;
        break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        cataventoPosY += 0.1f;
        break;
    case GLUT_KEY_DOWN:
        cataventoPosY -= 0.1f;
        break;
    case GLUT_KEY_LEFT:
        cataventoPosX -= 0.1f;
        break;
    case GLUT_KEY_RIGHT:
        cataventoPosX += 0.1f;
        break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // Salva a primeira cor temporariamente
        float tempColor[3] = {cores[0][0], cores[0][1], cores[0][2]};

        // Desloca as cores uma posição para frente
        for (int i = 0; i < 3; ++i)
        {
            cores[i][0] = cores[i + 1][0];
            cores[i][1] = cores[i + 1][1];
            cores[i][2] = cores[i + 1][2];
        }

        // Coloca a cor original da primeira hélice no final da lista
        cores[3][0] = tempColor[0];
        cores[3][1] = tempColor[1];
        cores[3][2] = tempColor[2];

        glutPostRedisplay(); // Redesenha a tela para exibir as novas cores
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        // Restaura a posição, rotação e escala do catavento
        cataventoPosX = 0.0f;
        cataventoPosY = 0.0f;
        cataventoScale = 1.0f;
        rotationAngle = 0.0f;
        glutPostRedisplay();
    }
}

void init()
{
    srand(time(NULL));
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Fundo branco
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Catavento Interativo");
    init();
    glutDisplayFunc(display);
    glutTimerFunc(16, atualiza, 0); // Chama `atualiza` pela primeira vez após 16ms
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
