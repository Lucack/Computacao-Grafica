#include <GL/glut.h>
#include <cmath>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GLuint texturaRoda;

bool farolLigado = false; // Estado do farol


float cameraAngleX = 0.0f, cameraAngleY = 0.0f;
float cameraDistance = 10.0f;

float anguloRodas = 0.0f; // Ângulo de rotação das rodas


float posX = 0.0f, posZ = 0.0f; // Posição do trator
float direcao = 0.0f; // Direção do trator (ângulo em graus)

float velocidade = 0.5f; // Velocidade do trator

GLfloat lightPosition[] = { 5.0f, 5.0f, 5.0f, 1.0f }; // Posição da luz
GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };  // Luz ambiente
GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };  // Luz difusa
GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Luz especular

void desenhaFarol() {
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.8f); // Cor amarelada para o farol
    glTranslatef(-.83f, 0.3f, .3f); // Posição do farol (ajustar conforme necessário)
    glutSolidCube(.2);   // Farol como uma esfera
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.8f); // Cor amarelada para o farol
    glTranslatef(-.83f, 0.3f, -.3f); // Posição do farol (ajustar conforme necessário)
    glutSolidCube(.2);   // Farol como uma esfera
    glPopMatrix();
}

void carregarTextura(const char* arquivo, GLuint& texturaID) {
    int largura, altura, canais;
    unsigned char* dados = stbi_load(arquivo, &largura, &altura, &canais, 0);
    if (dados) {
        
        glGenTextures(1, &texturaID);
        glBindTexture(GL_TEXTURE_2D, texturaID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, largura, altura, 0, GL_RGB, GL_UNSIGNED_BYTE, dados);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, largura, altura, GL_RGB, GL_UNSIGNED_BYTE, dados);

        // Parâmetros de filtro da textura
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(dados);
    } else {
        printf("Erro ao carregar a textura: %s\n", arquivo);
    }
}

void atualizaFarol() {
    // Posição e direção do farol direito
    GLfloat posicaoLuzDireita[] = {
        posX - 0.82f * cosf(direcao * M_PI / 180.0f),
        0.3f,
        posZ + 0.3f * sinf(direcao * M_PI / 180.0f),
        1.0f
    };
    GLfloat direcaoLuzDireita[] = {
        -cosf(direcao * M_PI / 180.0f),
        0.0f,
        sinf(direcao * M_PI / 180.0f)
    };

    // Posição e direção do farol esquerdo
    GLfloat posicaoLuzEsquerda[] = {
        posX - 0.82f * cosf(direcao * M_PI / 180.0f),
        0.3f,
        posZ - 0.3f * sinf(direcao * M_PI / 180.0f),
        1.0f
    };
    GLfloat direcaoLuzEsquerda[] = {
        -cosf(direcao * M_PI / 180.0f),
        0.0f,
        sinf(direcao * M_PI / 180.0f)
    };

    // Atualiza as luzes
    glLightfv(GL_LIGHT1, GL_POSITION, posicaoLuzDireita);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direcaoLuzDireita);

    glLightfv(GL_LIGHT2, GL_POSITION, posicaoLuzEsquerda);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, direcaoLuzEsquerda);
}


void inicializa() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_TEXTURE_2D); // Habilita texturas
    carregarTextura("pneu.png", texturaRoda);
}

void atualizaIluminacao() {
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glutPostRedisplay();
}

void resetarTrator() {
    // Resetar apenas as variáveis relevantes
    posX = 0.0f;
    posZ = 0.0f;
    direcao = 0.0f;

    // Atualizar a exibição
    glutPostRedisplay();
}



void menu(int opcao) {
    switch (opcao) {
    case 1: // Aumentar intensidade da luz difusa
        for (int i = 0; i < 3; i++) {
            lightDiffuse[i] = std::min(lightDiffuse[i] + 0.1f, 1.0f);
        }
        break;
    case 2: // Diminuir intensidade da luz difusa
        for (int i = 0; i < 3; i++) {
            lightDiffuse[i] = std::max(lightDiffuse[i] - 0.1f, 0.0f);
        }
        break;
    case 3: // Mover luz para a direita
        lightPosition[0] += 2.0f;
        break;
    case 4: // Mover luz para a esquerda
        lightPosition[0] -= 2.0f;
        break;
    case 5: // Mover luz para cima
        lightPosition[1] += 2.0f;
        break;
    case 6: // Mover luz para baixo
        lightPosition[1] -= 2.0f;
        break;
    case 7: // Resetar o trator
        resetarTrator();
        break;
    case 0: // Sair
        exit(0);
    }
    atualizaIluminacao();
}

void desenhaRoda(float raio, float largura) {
    glPushMatrix();
    glRotatef(anguloRodas, 0.0f, 0.0f, 1.0f);

    int numLados = 32;
    float anguloIncremento = 2.0f * M_PI / numLados;

    // Ativar textura
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaRoda);
    

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= numLados; i++) {
        float angulo = i * anguloIncremento;
        float x = cos(angulo) * raio;
        float y = sin(angulo) * raio;

        glNormal3f(cos(angulo), sin(angulo), 0.0f);
        glTexCoord2f((float)i / numLados, 1.0f);
        glVertex3f(x, y, largura / 2.0f);
        glTexCoord2f((float)i / numLados, 0.0f);
        glVertex3f(x, y, -largura / 2.0f);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D); // Desativar textura após uso
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Tampões (sem textura)
    glColor3f(0.1f, 0.1f, 0.1f);
    //glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, largura / 2.0f);
    for (int i = 0; i <= numLados; i++) {
        float angulo = i * anguloIncremento;
        glVertex3f(cos(angulo) * raio, sin(angulo) * raio, largura / 2.0f);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, -largura / 2.0f);
    for (int i = 0; i <= numLados; i++) {
        float angulo = i * anguloIncremento;
        glVertex3f(cos(angulo) * raio, sin(angulo) * raio, -largura / 2.0f);
    }
    glEnd();

    // Parte central (encaixe amarelo)
    float raioEncaixe = raio * 0.5f;
    glColor3f(0.968f, 0.58f, 0.11f); // Cor amarelo alaranjado

    // Frente do encaixe
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, largura / 2.0f + 0.01f);
    for (int i = 0; i <= numLados; i++) {
        float angulo = i * anguloIncremento;
        glVertex3f(cos(angulo) * raioEncaixe, sin(angulo) * raioEncaixe, largura / 2.0f + 0.01f);
    }
    glEnd();

    // Traseira do encaixe
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.0f, 0.0f, -largura / 2.0f - 0.01f);
    for (int i = 0; i <= numLados; i++) {
        float angulo = i * anguloIncremento;
        glVertex3f(cos(angulo) * raioEncaixe, sin(angulo) * raioEncaixe, -largura / 2.0f - 0.01f);
    }
    glEnd();

    glPopMatrix();
}

void desenhaCabine() {
    // Dimensões da cabine
    float larguraTeto = 2.f;  // Largura do teto
    float profundidadeTeto = 1.5f; // Profundidade do teto
    float alturaCabine = 1.8f; // Altura da cabine a partir do topo do trator
    float espessuraHaste = 0.2f; // Espessura das hastes

    // Hastes (quatro cantos da cabine)
    glColor3f(0.1f, 0.1f, 0.1f); // Preto
    for (int i = 0; i < 4; ++i) {
        float x = (i % 2 == 0) ? larguraTeto / 2 : -larguraTeto / 2;
        float z = (i < 2) ? profundidadeTeto / 2 : -profundidadeTeto / 2;

        glPushMatrix();
        glTranslatef(x, alturaCabine / 2, z);
        glScalef(espessuraHaste, alturaCabine, espessuraHaste);
        glutSolidCube(1.0f); // Cubo para representar a haste
        glPopMatrix();
    }

    // Teto da cabine
    glColor3f(1.0f, 0.5f, 0.0f); // Amarelo alaranjado
    glPushMatrix();
    glTranslatef(0.0f, alturaCabine, 0.0f);
    glScalef(larguraTeto, espessuraHaste, profundidadeTeto);
    glutSolidCube(1.0f); // Teto como um cubo achatado
    glPopMatrix();
}


float anguloBraco = 300.0f;  // Ângulo de rotação do braço
float anguloPá = 270.0f;     // Ângulo de rotação da pá

void desenhaPá() {
    // Dimensões da pá
    float larguraPá = 1.0f;       // Largura da pá
    float alturaPá = 0.8f;        // Altura da pá (para comportar mais coisas)
    float profundidadePá = 2.0f;  // Profundidade da pá

    // Desenhar a pá com base funda e laterais tampadas
    glPushMatrix();
    glColor3f(0.968f, 0.58f, 0.11f); // Amarelo alaranjado

    // Base da pá (trapezoidal)
    glBegin(GL_QUADS);
    glVertex3f(-larguraPá / 2, -alturaPá / 2, -profundidadePá / 2); // Inferior esquerdo traseiro
    glVertex3f(larguraPá / 2, -alturaPá / 2, -profundidadePá / 2);  // Inferior direito traseiro
    glVertex3f(larguraPá / 2 * 0.8f, alturaPá / 2, profundidadePá / 2); // Superior direito frontal
    glVertex3f(-larguraPá / 2 * 0.8f, alturaPá / 2, profundidadePá / 2); // Superior esquerdo frontal
    glEnd();

    // Lado esquerdo da pá
    glBegin(GL_QUADS);
    glVertex3f(-larguraPá / 2, -alturaPá / 2, -profundidadePá / 2); // Inferior traseiro esquerdo
    glVertex3f(-larguraPá / 2 * 0.8f, alturaPá / 2, -profundidadePá / 2); // Superior traseiro esquerdo
    glVertex3f(-larguraPá / 2 * 0.8f, alturaPá / 2, profundidadePá / 2);  // Superior frontal esquerdo
    glVertex3f(-larguraPá / 2, -alturaPá / 2, profundidadePá / 2);        // Inferior frontal esquerdo
    glEnd();

    // Lado direito da pá
    glBegin(GL_QUADS);
    glVertex3f(larguraPá / 2, -alturaPá / 2, -profundidadePá / 2); // Inferior traseiro direito
    glVertex3f(larguraPá / 2, -alturaPá / 2, profundidadePá / 2);  // Inferior frontal direito
    glVertex3f(larguraPá / 2 * 0.8f, alturaPá / 2, profundidadePá / 2); // Superior frontal direito
    glVertex3f(larguraPá / 2 * 0.8f, alturaPá / 2, -profundidadePá / 2); // Superior traseiro direito
    glEnd();

    // Traseira da pá
    glBegin(GL_QUADS);
    glVertex3f(-larguraPá / 2, -alturaPá / 2, -profundidadePá / 2); // Inferior esquerdo traseiro
    glVertex3f(larguraPá / 2, -alturaPá / 2, -profundidadePá / 2);  // Inferior direito traseiro
    glVertex3f(larguraPá / 2 * 0.8f, alturaPá / 2, -profundidadePá / 2); // Superior direito traseiro
    glVertex3f(-larguraPá / 2 * 0.8f, alturaPá / 2, -profundidadePá / 2); // Superior esquerdo traseiro
    glEnd();

    glPopMatrix();
}

void desenhaBracoDuplo() {
    // Dimensões dos braços
    float larguraBraco = 0.3f;
    float alturaBraco = 3.0f;
    float profundidadeBraco = 0.2f;

    // Posição inicial dos braços (laterais do trator)
    float baseY = 1.0f;   // Altura do eixo dos braços
    float baseZDireita = 1.1f; // Deslocamento lateral do braço direito
    float baseZEsquerda = -1.1f; // Deslocamento lateral do braço esquerdo
    float baseX = -1.0f;  // Posição na frente do corpo do trator

    // Braço direito
    glPushMatrix();
    glTranslatef(baseX, baseY, baseZDireita);       // Posição do eixo de rotação (direita)
    glRotatef(anguloBraco, 0.0f, 0.0f, 1.0f);      // Rotação do braço em torno do eixo Z
    glTranslatef(0.0f, -alturaBraco / 2, 0.0f);    // Ajusta a posição do braço para girar corretamente

    // Desenha o braço direito
    glPushMatrix();
    glColor3f(0.1f, 0.1f, 0.1f); // Cinza escuro
    glScalef(larguraBraco, alturaBraco, profundidadeBraco);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPopMatrix();

    // Braço esquerdo
    glPushMatrix();
    glTranslatef(baseX, baseY, baseZEsquerda);      // Posição do eixo de rotação (esquerda)
    glRotatef(anguloBraco, 0.0f, 0.0f, 1.0f);      // Rotação do braço em torno do eixo Z
    glTranslatef(0.0f, -alturaBraco / 2, 0.0f);    // Ajusta a posição do braço para girar corretamente

    // Desenha o braço esquerdo
    glPushMatrix();
    glColor3f(0.1f, 0.1f, 0.1f); // Cinza claro
    glScalef(larguraBraco, alturaBraco, profundidadeBraco);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPopMatrix();

    // Desenhar a pá na ponta dos braços
    glPushMatrix();
    glTranslatef(baseX, baseY, 0.0f);              // Posição inicial da pá no eixo
    glRotatef(anguloBraco, 0.0f, 0.0f, 1.0f);      // Mesma rotação dos braços
    glTranslatef(0.0f, -alturaBraco, 0.0f);        // Ajusta a pá para a ponta dos braços
    glRotatef(anguloPá, 0.0f, 0.0f, 1.0f);         // Rotação da pá
    desenhaPá();
    glPopMatrix();
}

void desenhaCorpoTrator() {
    // Variáveis para ajustar o corpo do trator
    float larguraBase = 2.0f;  // Largura da base do corpo
    float alturaBase = 1.5f;   // Altura da base do corpo
    float profundidadeBase = 2.0f; // Profundidade do corpo
    float topoAjuste = 0.8f;   // Proporção da largura no topo em relação à base
    float alturaTopo = 0.7f;   // Altura relativa do topo (posição vertical máxima)

    glPushMatrix();

    // Escala geral do corpo do trator
    glScalef(larguraBase, alturaBase, profundidadeBase);

    glBegin(GL_QUADS);

    // Face frontal
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -0.5f, 0.5f);           // Inferior esquerdo
    glVertex3f(1.0f, -0.5f, 0.5f);            // Inferior direito
    glVertex3f(topoAjuste, alturaTopo, 0.5f); // Superior direito
    glVertex3f(-topoAjuste, alturaTopo, 0.5f);// Superior esquerdo

    // Face traseira
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, -0.5f, -0.5f);          // Inferior esquerdo
    glVertex3f(1.0f, -0.5f, -0.5f);           // Inferior direito
    glVertex3f(topoAjuste, alturaTopo, -0.5f);// Superior direito
    glVertex3f(-topoAjuste, alturaTopo, -0.5f);// Superior esquerdo

    // Face superior
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-topoAjuste, alturaTopo, -0.5f); // Traseiro esquerdo
    glVertex3f(topoAjuste, alturaTopo, -0.5f);  // Traseiro direito
    glVertex3f(topoAjuste, alturaTopo, 0.5f);   // Frontal direito
    glVertex3f(-topoAjuste, alturaTopo, 0.5f);  // Frontal esquerdo

    // Face inferior
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-1.0f, -0.5f, -0.5f);           // Traseiro esquerdo
    glVertex3f(1.0f, -0.5f, -0.5f);            // Traseiro direito
    glVertex3f(1.0f, -0.5f, 0.5f);             // Frontal direito
    glVertex3f(-1.0f, -0.5f, 0.5f);            // Frontal esquerdo

    // Face lateral direita
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, -0.5f, -0.5f);            // Inferior traseiro
    glVertex3f(topoAjuste, alturaTopo, -0.5f); // Superior traseiro
    glVertex3f(topoAjuste, alturaTopo, 0.5f);  // Superior frontal
    glVertex3f(1.0f, -0.5f, 0.5f);             // Inferior frontal

    // Face lateral esquerda
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -0.5f, -0.5f);           // Inferior traseiro
    glVertex3f(-topoAjuste, alturaTopo, -0.5f);// Superior traseiro
    glVertex3f(-topoAjuste, alturaTopo, 0.5f); // Superior frontal
    glVertex3f(-1.0f, -0.5f, 0.5f);            // Inferior frontal

    glEnd();
    
    // Desenha o farol
    desenhaFarol();

    glPopMatrix();
}
void inicializaFarol() {
    // Configuração da luz do farol direito
    GLfloat luzAmbiente[] = { 0.7f, 0.7f, 0.7f, 1.0f }; // Ambiente mais claro
    GLfloat luzDifusa[] = { 3.5f, 3.5f, 3.0f, 1.0f };   // Luz mais forte
    GLfloat luzEspecular[] = { 4.0f, 4.0f, 3.5f, 1.0f }; // Reflexos intensos


    // Farol direito (GL_LIGHT1)
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT1, GL_SPECULAR, luzEspecular);

    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0f);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2000.0f);

    // Farol esquerdo (GL_LIGHT2)
    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT2, GL_SPECULAR, luzEspecular);

    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45.0f);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 2000.0f);

    // Inicialmente, as luzes estarão desligadas
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
}

float anguloBracoTraseiro = 150.0f;  // Ângulo de rotação do braço traseiro
float anguloAntebraco = 250.0f;      // Ângulo do antebraço traseiro
float anguloPaTraseira = 180.0f;     // Ângulo da pá traseira

void desenhaPáTraseira() {
    // Dimensões da pá traseira
    float larguraPá = 1.5f;       // Largura da pá
    float alturaPá = 0.7f;        // Altura da pá
    float profundidadePá = 1.0f;  // Profundidade da pá

    glPushMatrix();
    glColor3f(0.968f, 0.58f, 0.11f); // Amarelo alaranjado

    // Base da pá (fundo inclinado)
    glBegin(GL_QUADS);
    glVertex3f(-larguraPá / 2, -alturaPá / 2, -profundidadePá / 2); // Inferior traseiro esquerdo
    glVertex3f(larguraPá / 2, -alturaPá / 2, -profundidadePá / 2);  // Inferior traseiro direito
    glVertex3f(larguraPá / 2 * 0.8f, alturaPá / 2, profundidadePá / 2); // Superior frontal direito
    glVertex3f(-larguraPá / 2 * 0.8f, alturaPá / 2, profundidadePá / 2); // Superior frontal esquerdo
    glEnd();

    // Lado esquerdo da pá
    glBegin(GL_QUADS);
    glVertex3f(-larguraPá / 2, -alturaPá / 2, -profundidadePá / 2); // Inferior traseiro esquerdo
    glVertex3f(-larguraPá / 2, -alturaPá / 2, profundidadePá / 2);  // Inferior frontal esquerdo
    glVertex3f(-larguraPá / 2 * 0.8f, alturaPá / 2, profundidadePá / 2); // Superior frontal esquerdo
    glVertex3f(-larguraPá / 2 * 0.8f, alturaPá / 2, -profundidadePá / 2); // Superior traseiro esquerdo
    glEnd();

    // Lado direito da pá
    glBegin(GL_QUADS);
    glVertex3f(larguraPá / 2, -alturaPá / 2, -profundidadePá / 2); // Inferior traseiro direito
    glVertex3f(larguraPá / 2, -alturaPá / 2, profundidadePá / 2);  // Inferior frontal direito
    glVertex3f(larguraPá / 2 * 0.8f, alturaPá / 2, profundidadePá / 2); // Superior frontal direito
    glVertex3f(larguraPá / 2 * 0.8f, alturaPá / 2, -profundidadePá / 2); // Superior traseiro direito
    glEnd();

    // Traseira da pá
    glBegin(GL_QUADS);
    glVertex3f(-larguraPá / 2, -alturaPá / 2, -profundidadePá / 2); // Inferior traseiro esquerdo
    glVertex3f(larguraPá / 2, -alturaPá / 2, -profundidadePá / 2);  // Inferior traseiro direito
    glVertex3f(larguraPá / 2 * 0.8f, alturaPá / 2, -profundidadePá / 2); // Superior traseiro direito
    glVertex3f(-larguraPá / 2 * 0.8f, alturaPá / 2, -profundidadePá / 2); // Superior traseiro esquerdo
    glEnd();

    glPopMatrix();
}

void desenhaBracoTraseiro() {
    // Dimensões do braço e antebraço
    float larguraBraco = 0.3f;
    float alturaBraco = 2.4f;
    float profundidadeBraco = 0.3f;

    // Posição inicial do braço traseiro
    float baseX = 1.5f;  // Parte traseira do trator
    float baseY = 1.0f;  // Altura da base do braço
    float baseZ = 0.0f;  // Centralizado na parte traseira

    // Braço principal
    glPushMatrix();
    glTranslatef(baseX, baseY, baseZ);             // Posição do eixo de rotação
    glRotatef(anguloBracoTraseiro, 0.0f, 0.0f, 1.0f); // Rotação do braço principal em torno do eixo Z
    glTranslatef(0.0f, -alturaBraco / 2, 0.0f);    // Ajusta a posição para rotação correta

    // Desenhar o braço principal
    glPushMatrix();
    glColor3f(0.1f, 0.1f, 0.1f); // Cinza escuro
    glScalef(larguraBraco, alturaBraco, profundidadeBraco);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Antebraço (conectado ao braço principal)
    glPushMatrix();
    glTranslatef(0.0f, -alturaBraco / 2, 0.0f);    // Conecta o antebraço ao final do braço principal
    glRotatef(anguloAntebraco, 0.0f, 0.0f, 1.0f);  // Rotação do antebraço
    glTranslatef(0.0f, -alturaBraco / 2, 0.0f);    // Ajusta para o eixo correto de rotação

    // Desenhar o antebraço
    glPushMatrix();
    glColor3f(0.1f, 0.1f, 0.1f); // Cinza escuro
    glScalef(larguraBraco, alturaBraco, profundidadeBraco);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Pá na ponta do antebraço
    glPushMatrix();
    glTranslatef(0.3f, -alturaBraco / 2, 0.0f);    // Conecta a pá ao final do antebraço
    glRotatef(anguloPaTraseira, 0.0f, 0.0f, 1.0f); // Rotação da pá traseira
    desenhaPáTraseira();                           // Substituição pelo desenho da nova pá
    glPopMatrix();

    glPopMatrix(); // Final do antebraço
    glPopMatrix(); // Final do braço principal
}
void desenhaMapa() {
    glPushMatrix();
    glColor3f(0.5f, 0.8f, 0.5f); // Verde claro para o chão
    glBegin(GL_QUADS);
    glVertex3f(-50.0f, -0.5f, -50.0f);
    glVertex3f(-50.0f, -0.5f, 50.0f);
    glVertex3f(50.0f, -0.5f, 50.0f);
    glVertex3f(50.0f, -0.5f, -50.0f);
    glEnd();
    glPopMatrix();
}


void desenhaTrator() {
    glPushMatrix();
    // Aplica a posição e rotação do trator
    glTranslatef(posX, 0.0f, posZ);
    glRotatef(direcao, 0.0f, 1.0f, 0.0f);

    // Desenhar o corpo do trator
    glColor3f(0.968f, 0.58f, 0.11f); // Amarelo alaranjado
    desenhaCorpoTrator();

    // Desenhar a cabine do motorista
    glPushMatrix();
    glTranslatef(0.5f, 1.0f, 0.0f); // Posição da cabine deslocada
    desenhaCabine();
    glPopMatrix();

    // Desenhar o braço frontal da retroescavadeira
    desenhaBracoDuplo();

    // Desenhar o braço traseiro da retroescavadeira
    desenhaBracoTraseiro();

    // Configuração das rodas
    float raioRodasTraseiras = 0.8f;   // Raio das rodas traseiras (maiores)
    float larguraRodasTraseiras = 0.7f; // Largura das rodas traseiras
    float raioRodasDianteiras = 0.6f;  // Raio das rodas dianteiras (menores)
    float larguraRodasDianteiras = 0.4f; // Largura das rodas dianteiras
    float posicaoRodasY = -0.5f;       // Altura das rodas
    float posicaoRodasTraseirasX = 1.3f;  // Posição X das rodas traseiras
    float posicaoRodasDianteirasX = -1.5f; // Posição X das rodas dianteiras
    float posZ = 1.2f;

    // Rodas traseiras - direita
    glPushMatrix();
    glColor3f(0.1f, 0.1f, 0.1f); // Preto
    glTranslatef(posicaoRodasTraseirasX, posicaoRodasY, posZ);
    desenhaRoda(raioRodasTraseiras, larguraRodasTraseiras);
    glPopMatrix();

    // Rodas traseiras - esquerda
    glPushMatrix();
    glTranslatef(posicaoRodasTraseirasX, posicaoRodasY, -posZ);
    desenhaRoda(raioRodasTraseiras, larguraRodasTraseiras);
    glPopMatrix();

    // Rodas dianteiras - direita
    glPushMatrix();
    glTranslatef(posicaoRodasDianteirasX, posicaoRodasY, posZ);
    desenhaRoda(raioRodasDianteiras, larguraRodasDianteiras);
    glPopMatrix();

    // Rodas dianteiras - esquerda
    glPushMatrix();
    glTranslatef(posicaoRodasDianteirasX, posicaoRodasY, -posZ);
    desenhaRoda(raioRodasDianteiras, larguraRodasDianteiras);
    glPopMatrix();

    glPopMatrix(); // Finaliza as transformações do trator
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Posicionar a câmera
    float camX = cameraDistance * sin(cameraAngleY) * cos(cameraAngleX);
    float camY = cameraDistance * sin(cameraAngleX);
    float camZ = cameraDistance * cos(cameraAngleY) * cos(cameraAngleX);
    gluLookAt(camX, camY, camZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    // Atualizar a posição e direção do farol
    atualizaFarol();

    // Desenhar o trator completo (corpo + cabine + rodas)
    desenhaTrator();

    glutSwapBuffers();
}


void teclado(unsigned char tecla, int x, int y) {
    switch (tecla) {
    case 's': // Mover para frente
        posX += velocidade * cos(direcao * M_PI / 180.0f);
        posZ -= velocidade * sin(direcao * M_PI / 180.0f);
        anguloRodas -= (360.0f * velocidade) / (2.0f * M_PI * 0.6f); // Roda girando para frente
        break;
    case 'w': // Mover para trás
        posX -= velocidade * cos(direcao * M_PI / 180.0f);
        posZ += velocidade * sin(direcao * M_PI / 180.0f);
        anguloRodas += (360.0f * velocidade) / (2.0f * M_PI * 0.6f); // Roda girando para trás
        break;
    case 'a': // Girar para a esquerda
        direcao += 5.0f;
        if (direcao >= 360.0f) direcao -= 360.0f; // Mantém o ângulo entre 0 e 360
        break;
    case 'd': // Girar para a direita
        direcao -= 5.0f;
        if (direcao < 0.0f) direcao += 360.0f; // Mantém o ângulo entre 0 e 360
        break;
    case '+':
        cameraDistance = std::max(cameraDistance - 0.5f, 5.0f);
        break;
    case '-':
        cameraDistance = std::min(cameraDistance + 0.5f, 30.0f);
        break;
    case 'u': // Levanta o braço
        anguloBraco = std::min(anguloBraco + 5.0f, 320.0f); // Limite superior
        break;
    case 'j': // Abaixa o braço
        anguloBraco = std::max(anguloBraco - 5.0f, 180.0f); // Limite inferior
        break;
    case 'i': // Gira a pá para cima
        anguloPá = (anguloPá + 5.0f); // Limite superior
        break;
    case 'k': // Gira a pá para baixo
        anguloPá = (anguloPá - 5.0f); // Limite inferior
        break;
    case 'o': // Levanta o braço traseiro
        anguloBracoTraseiro = std::min(anguloBracoTraseiro + 5.0f, 170.0f);
        break;
    case 'l': // Abaixa o braço traseiro
        anguloBracoTraseiro = std::max(anguloBracoTraseiro - 5.0f, 50.0f);
        break;
    case 'p': // Inclina o antebraço traseiro para cima
        anguloAntebraco = std::min(anguloAntebraco + 5.0f, 355.0f);
        break;
    case ';': // Inclina o antebraço traseiro para baixo
        anguloAntebraco = std::max(anguloAntebraco - 5.0f, 200.0f);
        break;
    case '[': // Inclina a pá traseira para cima
        anguloPaTraseira = anguloPaTraseira + 5.0f;
        break;
    case ']': // Inclina a pá traseira para baixo
        anguloPaTraseira = anguloPaTraseira - 5.0f;
        break;
    case 'f': // Liga/desliga os faróis
        farolLigado = !farolLigado;
        if (farolLigado) {
            glEnable(GL_LIGHT1);
            glEnable(GL_LIGHT2);
        } else {
            glDisable(GL_LIGHT1);
            glDisable(GL_LIGHT2);
        }
        break;


    case 27:  // ESC
        exit(0);
        break;
    }
    glutPostRedisplay();
}

void teclasEspeciais(int tecla, int x, int y) {
    switch (tecla) {
    case GLUT_KEY_UP: // Seta para cima
        cameraAngleX = std::min(cameraAngleX + 0.1f, static_cast<float>(M_PI_2));
        break;
    case GLUT_KEY_DOWN: // Seta para baixo
        cameraAngleX = std::max(cameraAngleX - 0.1f, -static_cast<float>(M_PI_2));
        break;
    case GLUT_KEY_LEFT: // Seta para a esquerda
        cameraAngleY -= 0.1f;
        break;
    case GLUT_KEY_RIGHT: // Seta para a direita
        cameraAngleY += 0.1f;
        break;
    }
    glutPostRedisplay(); // Redesenha a cena após a interação
}


void reshape(int largura, int altura) {
    glViewport(0, 0, largura, altura);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, static_cast<float>(largura) / static_cast<float>(altura), 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Retroescavadeira");

    inicializa();
    inicializaFarol(); // Inicializa o farol


    // Criar menu
    glutCreateMenu(menu);
    glutAddMenuEntry("Aumentar intensidade da luz", 1);
    glutAddMenuEntry("Diminuir intensidade da luz", 2);
    glutAddMenuEntry("Mover luz para a direita", 3);
    glutAddMenuEntry("Mover luz para a esquerda", 4);
    glutAddMenuEntry("Mover luz para cima", 5);
    glutAddMenuEntry("Mover luz para baixo", 6);
    glutAddMenuEntry("Posicao Inicial", 7); // Corrigir duplicidade e nome
    glutAddMenuEntry("Sair", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON); // Associa ao botão direito do mouse

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(teclasEspeciais);
    glutKeyboardFunc(teclado);
    glutMainLoop();
    return 0;
}
