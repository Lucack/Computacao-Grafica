#include <GL/glut.h>
#include <math.h>

float color[3] = {1.0f, 0.0f, 0.0f};
float xPos = 0.0, yPos = 0.0;
float size = 0.2;
int speed = 20;

#define PI 3.14159265358979323846
#define NUM_SEGMENTOS 100

int animacaoAtiva = 0;

void desenhaPonto(float x, float y)
{
    glColor3f(color[0], color[1], color[2]);

    glBegin(GL_POINTS);
    glVertex2f(xPos, yPos);
    glEnd();
}

void desenhaQuadrado(float x, float y)
{
    glColor3f(color[0], color[1], color[2]);

    glBegin(GL_QUADS);
    glVertex2f(xPos - size, yPos - size);
    glVertex2f(xPos + size, yPos - size);
    glVertex2f(xPos + size, yPos + size);
    glVertex2f(xPos - size, yPos + size);
    glEnd();
}

void desenhaCirculo(float x, float y, float raio)
{
    glColor3f(color[0], color[1], color[2]);

    glBegin(GL_POLYGON);
    for (int i = 0; i <= NUM_SEGMENTOS; i++)
    {
        float angulo = 2.0f * PI * i / NUM_SEGMENTOS;
        float dx = raio * cos(angulo);
        float dy = raio * sin(angulo);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    desenhaQuadrado(0.0, 0.0);
    glFlush();
}

void inicializa()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glPointSize(10.0f);
}

void mouse(int botao, int estado, int x, int y)
{
    if (botao == GLUT_LEFT_BUTTON && estado == GLUT_DOWN)
    {
        xPos = (x / (float)glutGet(GLUT_WINDOW_WIDTH)) * 2 - 1;
        yPos = (glutGet(GLUT_WINDOW_HEIGHT) - y) / (float)glutGet(GLUT_WINDOW_HEIGHT) * 2 - 1;
    }
    glutPostRedisplay();
}

void mousePressionado(int botao, int estado, int x, int y)
{
    if (estado == GLUT_DOWN)
    {
        if (botao == GLUT_LEFT_BUTTON)
        {
            size += 0.05;
        }
        else if (botao == GLUT_MIDDLE_BUTTON)
        {
            size = 0.2;
        }
        else if (botao == GLUT_RIGHT_BUTTON)
        {
            if (size > 0.05)
            {
                size -= 0.05;
            }
        }
    }
    glutPostRedisplay();
}

void menu(int opcao)
{
    switch (opcao)
    {
    case 1: // vermelho
        color[0] = 1;
        color[1] = 0;
        color[2] = 0;
        break;
    case 2: // verde
        color[0] = 0;
        color[1] = 1;
        color[2] = 0;
        break;
    case 3: // azul
        color[0] = 0;
        color[1] = 0;
        color[2] = 1;
        break;
    }

    glutPostRedisplay();
}

void movimento(int x, int y)
{
    xPos = (x / (float)glutGet(GLUT_WINDOW_WIDTH)) * 2 - 1;
    yPos = (glutGet(GLUT_WINDOW_HEIGHT) - y) / (float)glutGet(GLUT_WINDOW_HEIGHT) * 2 - 1;

    glutPostRedisplay();
}

void atualizaPosicao(int valor)
{

    if (animacaoAtiva)
    {
        xPos += 0.01;
        if (xPos > 1)
        {
            xPos = -1;
        }
        glutPostRedisplay();
        glutTimerFunc(speed, atualizaPosicao, 0);
    }
}

void teclado(unsigned char tecla, int x, int y)
{
    switch (tecla)
    {
    case 'i':
    case 'I':
        if (animacaoAtiva == 0)
        {
            animacaoAtiva = 1;
            glutTimerFunc(speed, atualizaPosicao, 0);
        }
        break;
    case 'p':
    case 'P':
        animacaoAtiva = 0;
        break;
     case '-':
    
        speed+=10;
        
        break;
    case '=':
    
        speed-=10;
        speed <0 ? speed = 1 : speed = speed;
        break;

        glutPostRedisplay();
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Mouse, Menu e Animação");

    inicializa();

    // Criação do submenu
    int submenu = glutCreateMenu(menu);
    glutAddMenuEntry("Vermelho", 1);

    // Criação do menu principal
    glutCreateMenu(menu);
    glutAddMenuEntry("Verde", 2);
    glutAddMenuEntry("Azul", 3);
    glutAddSubMenu("Mais Cores", submenu); // Adicionando o submenu corretamente

    // Anexando o menu ao botão direito do mouse
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutDisplayFunc(display);
    glutMouseFunc(mousePressionado);  // botoes do mouse realizando ações
    glutMotionFunc(movimento);        // preciona o botao e consegue movimentar
    glutPassiveMotionFunc(movimento); // sem precisar pressionar pra mexer
    glutKeyboardFunc(teclado);
    glutMainLoop();

    return 0;
}