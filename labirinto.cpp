#define GLUT_DISABLE_ATEXIT_HACK
/*
============================= JOGO DO LABIRINTO ========================
====MATEUS BERNINI ======
*/

#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define TAM_BLOCO 2
#define tamanhoMapa 45
#define windowSizeX 600
#define windowSizeY 500
#include<iostream>


typedef struct { /// STRUCT DOS TERRORISTAS

    int x;

    int y;

    int dir;

} Robot;

typedef struct { /// STRUCT DAS BOMBAS

    int x;

    int y;

    bool ativa;

} Bomba;


///VARIÁVEIS E FUNÇÕES
void keyboard(unsigned char key, int x, int y);

void Special_Keys(int key, int x, int y);

void Mouse(int button, int state, int x, int y);

void mover(char direcao, int x, int y);

void mover(Robot *robot);

void iniciarBomba(Bomba *bomba);

int movimentoBot();

void cenario();

void quadrado(int r, int g, int b);

void drawText(const char *text, int length, int x, int y);

int bombaTempo;

int pontos;

bool colizao(int x, int y);

Robot robot[10];

Bomba bomba[10];

int cont;

int Y;

int X; ///POSIÇÃO DO JOGADOR DENTRO DA MATRIZ

int proporcao = 3; ///TRABALHA COM AS TRANSFORMAÇÕES SCALEF E TRANSLATE F..

bool fim;

bool key;

///CONSTRUÇÃO DO LABIRINTO
int mapa[tamanhoMapa][tamanhoMapa] =/// 0 = ESPAÇOS VAZIOS(CORREDORES), 1 = PAREDES, 2 = JOGADOR
{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,///////0
  2,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,///////1
  1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,1,1,1,1,1,1,///////2
  1,0,1,0,1,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,///////3
  1,0,1,0,1,1,0,1,1,1,1,1,1,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,1,///////4
  1,0,1,0,0,0,0,1,4,0,0,0,1,0,1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,0,1,0,0,1,0,0,0,0,0,0,0,0,1,///////5
  1,0,1,0,1,1,0,1,0,0,0,0,1,0,1,1,0,0,0,1,1,0,1,1,1,1,1,1,0,1,1,0,1,1,0,1,0,1,0,1,1,1,1,0,1,///////6
  1,0,1,0,1,1,0,1,0,0,0,0,1,0,0,1,0,1,0,0,0,0,1,0,0,0,4,1,0,1,1,0,0,1,0,1,1,0,1,1,1,1,1,1,1,///////7
  1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,1,0,1,1,1,1,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,///////8
  1,0,1,1,0,1,0,1,0,1,1,1,1,0,1,1,0,1,0,0,1,0,1,0,0,0,0,1,0,1,0,0,1,0,1,1,0,1,0,0,0,0,0,0,1,///////9
  1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,1,1,0,1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,1,4,0,1,0,1,///////10
  1,0,1,0,0,1,0,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,1,0,1,1,0,1,0,0,0,0,0,0,1,///////11
  1,0,1,1,1,1,0,1,0,0,0,0,0,0,1,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0,1,1,0,1,0,0,0,0,1,0,1,1,1,///////12
  1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,1,1,1,///////13
  1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,0,1,0,0,1,0,1,0,0,0,0,0,0,1,///////14
  1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,1,1,0,0,1,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,///////15
  1,0,1,0,1,1,0,1,0,0,0,1,0,0,1,0,0,1,0,1,1,0,1,1,0,0,1,1,0,1,1,0,1,1,0,1,0,0,0,1,1,0,0,0,1,///////16
  1,0,1,0,1,1,0,1,0,0,0,1,0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,1,0,1,0,0,1,0,0,1,0,1,0,0,0,0,0,0,1,///////17
  1,0,1,0,1,1,0,0,0,0,0,0,0,0,1,0,0,1,0,1,1,0,0,0,0,0,0,0,0,1,0,1,1,0,0,1,0,0,0,1,1,0,1,0,1,///////18
  1,0,1,0,1,1,1,1,0,1,1,1,0,0,1,0,1,0,0,1,0,0,1,1,1,1,1,1,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,1,///////19
  1,0,1,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,0,1,1,0,1,0,0,0,0,1,0,1,///////20
  1,0,1,0,0,1,0,1,0,1,1,0,1,0,1,4,1,1,0,0,0,1,1,1,1,1,0,1,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,///////21
  1,0,1,1,1,1,0,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,1,0,0,0,0,0,0,1,///////22
  1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,1,0,1,1,0,1,1,0,1,0,0,0,0,1,0,1,0,1,///////23
  1,0,1,0,1,0,0,1,0,0,0,0,0,0,1,1,0,1,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,1,1,0,1,0,1,///////24
  1,0,1,0,1,0,0,1,0,0,0,4,1,0,1,1,0,1,0,1,1,0,0,0,0,0,0,0,4,1,1,0,1,0,1,1,0,1,0,0,0,0,1,1,1,///////25
  1,0,1,0,0,0,0,1,1,1,1,1,1,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,1,1,0,1,0,1,0,1,///////26
  1,0,1,0,1,1,0,1,0,0,0,0,1,0,1,1,0,1,0,1,0,0,1,0,0,0,0,1,0,1,1,0,1,1,1,1,0,1,0,0,0,0,0,0,1,///////27
  1,0,1,0,1,1,0,0,0,0,0,0,0,0,1,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,1,0,0,0,0,0,1,1,///////28
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,///////29
  1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,///////30
  1,0,1,0,1,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,///////31
  1,0,0,0,1,1,0,1,1,0,1,1,1,0,1,0,0,1,0,1,1,0,0,0,0,0,0,0,0,1,0,0,1,0,1,1,0,0,0,0,0,0,0,0,1,///////32
  1,0,1,0,0,0,0,1,0,0,0,0,1,0,1,0,0,1,0,1,1,1,1,1,0,1,1,1,1,1,1,0,1,0,0,1,0,0,1,1,1,0,0,0,1,///////33
  1,0,1,0,1,1,0,0,0,1,1,1,1,0,1,1,0,1,0,1,1,0,1,1,1,1,0,0,0,1,1,0,1,1,0,1,0,0,0,0,1,1,1,0,1,///////34
  1,0,0,0,1,1,0,1,0,0,0,0,1,0,0,1,0,1,0,0,0,0,1,0,0,0,4,1,0,1,1,0,0,1,0,1,1,0,1,0,0,1,1,1,1,///////35
  1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,1,0,1,0,1,1,0,1,1,1,0,1,0,0,0,0,0,0,0,1,1,1,0,1,1,1,1,///////36
  1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,1,0,1,0,0,1,0,1,0,0,0,0,1,0,1,0,0,1,0,1,1,0,1,0,0,0,0,0,0,1,///////37
  1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,1,1,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,0,0,1,1,1,///////38
  1,0,1,0,0,1,0,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,1,1,1,0,1,1,0,1,1,0,1,0,0,1,0,1,0,0,0,0,0,0,1,///////39
  1,0,1,1,1,1,0,1,0,0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1,0,0,1,1,0,1,0,0,0,1,1,0,1,1,1,///////40
  1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,1,0,0,0,0,1,1,1,///////41
  1,1,0,1,1,1,0,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,0,0,1,///////42
  1,0,4,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,1,///////43
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,///////44
  };

void iniciarGame(){

    Y = 0;

    X = 1;

    fim = false;

    key = false;

    cont=0;


    for(int  i = 0; i < 10; i++){

        iniciarBomba(&bomba[i]);

    }
    ///INICIALIZAR TEMPO
    bombaTempo = 1600;
    ///INICIALIZA BOMBAS COLETADAS
    pontos=0;

}

void iniciarBomba(Bomba *bomba){
    /// INSERE O VALOR DA BOMBA NA MATRIZ NÚMERO 6
    mapa[bomba->x][bomba->y]=6;

}
// Função callback para desenho
void desenha(void){

    // inicializar sistema de viz.

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    // Inicializa o sistema de coordenadas

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    //Define os limites dos eixos x e y

    //Argumentos da função: void gluOrtho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top)

    glOrtho(0.0f, windowSizeX, 0.0f, windowSizeY, 0.0f, 50.0f);

    //Limpa todos os pixels com a cor de fundo

	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f (1.0, 1.0, 0.0);

	for(int  i = 0; i < 10; i++){

        mover(&robot[i]);  ///mover terroristas

    }

    glPushMatrix();

        glTranslatef(100, 0,0.0);

        glScalef(proporcao,proporcao,0.0);

        cenario();

	glPopMatrix();

	glutSwapBuffers();

	Sleep(100);

}

void drawText(const char *text, int length, int x, int y){///FUNÇÃO PARA PRINTAR OS TEXTOS NA TELA
    glMatrixMode(GL_PROJECTION);
    double *matriz = new double[16];
    glGetDoublev(GL_PROJECTION_MATRIX, matriz);
    glLoadIdentity();
    glOrtho(0,800,0,600,-5,5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2i(x,y);
    for(int i = 0; i < length; i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)text[i]);
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matriz);
    glMatrixMode(GL_MODELVIEW);
}

///FUNÇÃO DO CONTADOR DE TEMPO
void myTimer(int value) {
int j, g_counter;
 if(j==0)
 {
 g_counter = value + 1;
 }
 else if(j==1)
 {
  g_counter = value + 0;
 }
 else
 {
  g_counter = 0;
 }
 glutPostRedisplay();
 glutTimerFunc(1000, myTimer, g_counter);
}

void cenario(){

    int x, y;

	int x_mun, y_mun;

    bombaTempo--;

    if(!fim && bombaTempo > 0 && pontos < 10){

        for (y = tamanhoMapa - 1; y >= 0; y--){ ///PRINTA O LABIRINTO

            for (x = 0; x < tamanhoMapa; x++){

                if (mapa[x][y] == 1){

                    x_mun = x * proporcao;

                    y_mun = y * proporcao;

                    glPushMatrix();

                        glTranslatef(x_mun, y_mun,0);

                        glScalef(proporcao,proporcao,0.0);

                        quadrado(0,0,0);

                    glPopMatrix();
                }

                if (mapa[x][y] == 2){
                        x_mun = x * proporcao;

                        y_mun = y * proporcao;

                        glPushMatrix();

                            glTranslatef(x_mun, y_mun,0);

                            glScalef(proporcao,proporcao,0.0);

                            quadrado(255,255,255);

                        glPopMatrix();
                }

                if (mapa[x][y] == 5){

                        x_mun = x * proporcao;

                        y_mun = y * proporcao;

                        glPushMatrix();

                            glTranslatef(x_mun, y_mun,0);

                            glScalef(proporcao,proporcao,0.0);

                            quadrado(0,222,0);

                        glPopMatrix();
                }

                if (mapa[x][y] == 6){
                        x_mun = x * proporcao;

                        y_mun = y * proporcao;

                        glPushMatrix();

                            glTranslatef(x_mun, y_mun,0);

                            glScalef(proporcao,proporcao,0.0);

                            quadrado(242,168,51);

                        glPopMatrix();

                }

            }


        }

    std::string text;

        glPushMatrix();
            /// PRINTA O TEMPO
            text.append("TEMPO (2min e 40seg) : ");
            text.append(std::to_string(bombaTempo));
            glColor3f(1.0,1.0,1.0);
            drawText(text.data(), text.size(), 5, 500);

        glPopMatrix();

        glPushMatrix();
            /// PRINTA AS BOMBAS COLETADAS
            text = "";
            text.append("PONTOS (Bombas Coletadas): ");
            text.append(std::to_string(pontos));
            glColor3f(1.0,1.0,1.0);
            drawText(text.data(), text.size(), 5, 550);

        glPopMatrix();

        glPushMatrix();///PRINTA COR DO JOGADOR
            text = "Cores: ";
            glColor3f(0.0,0.0,0.0);
            drawText(text.data(), text.size(), 5, 450);
        glPopMatrix();

        glPushMatrix();///PRINTA COR DO JOGADOR
            text = "Jogador ";
            glColor3f(1.0,1.0,1.0);
            drawText(text.data(), text.size(), 5, 400);
        glPopMatrix();

        glPushMatrix();///PRINTA COR DO TERRORISTA
            text = "Terrorista ";
            glColor3f(0.0,1.0,0.0);
            drawText(text.data(), text.size(), 5, 350);
        glPopMatrix();

        glPushMatrix();///PRINTA COR DA BOMBA
            text = "Bomba ";
            glColor3ub(242,168,51);
            drawText(text.data(), text.size(), 5, 300);
        glPopMatrix();

        ///CONTADOR DO TEMPO
            myTimer(bombaTempo);

    }

    else{
            if (pontos >=10){ ///VERIFICA SE TODAS AS 10 BOMBAS FORAM COLETADAS E FINALIZA O JOGO CONGRATULANDO O JOGADOR
                glPushMatrix();
            /// QUANDO O JOGADOR VENCE:
            std::string text;
            text =  "VOCE VENCEU!!PRESSIONE '' i '' PARA JOGAR NOVAMENTE";
            glColor3f(1.0,1.0,1.0);
            drawText(text.data(), text.size(), 70, 300);

        glPopMatrix();


            }else{
            glPushMatrix();
            /// QUANDO O JOGADOR PERDE:
            std::string text;
            text =  "GAME OVER!! PRESSIONE '' i '' PARA JOGAR NOVAMENTE";
            glColor3f(1.0,1.0,1.0);
            drawText(text.data(), text.size(), 70, 300);

        glPopMatrix();

            }




    }





}

void quadrado(int r, int g, int b){ ///FUNÇÃO QUE DESENHA JOGADOR, TERRORISTA, BOMBA, PASSANDO COR PELA REFERÊNCIA

    glBegin(GL_QUADS);

        glColor3ub(r,g,b);

        glVertex2f(0,0);

        glVertex2f(0,1);

        glVertex2f (1,1);

        glVertex2f (1,0);

    glEnd();

}

bool colizao(int x, int y, char tipo[]){/// FUNÇÃO QUE FAZ A VERIFICAÇÃO DE OBSTÁCULOS NO SENTIDO DO OBJETO (COLIZÃO)

    if((x >= 0 && y >= 0) && (x < tamanhoMapa && y < tamanhoMapa)){

        if(mapa[y][x]==0){

            return false;

        }

        if(mapa[y][x]==4 && tipo == "avatar"){

            key = true;

            return false;

        }

        if(mapa[y][x]==3 && key==true && tipo == "avatar"){

            key = false;

            return false;

        }

        if(mapa[y][x]==5 && tipo == "avatar" || mapa[y][x]==2 && tipo == "npc"){

            fim = true;

            return false;

        }

        if(mapa[y][x]==6 && tipo == "avatar"){


            pontos++;
            return false;

        }

    }

    return true;

}

void mover(Robot *robot){/// MOVIMENTA OS TERRORISTAS
    /// O TERRORISTA SE MOVE DE ACORDO COM UM VALOR DE 0 A 3 QUE RECEBE
    /// MANTENDO ESSE MOVIMENTO ATÉ ENCONTRAR UM OBSTÁCULO. AO ENCONTRAR O OBSTÁCULO ELE MUDA DE ROTA E CONTINUA SE MOVENDO.
    switch (robot->dir){
        case 0:
            if(!colizao(robot->x+1, robot->y, "npc")){ ///VERIFICA SE NÃO TEM OBJETO NO CAMINHO

                mapa[robot->y][robot->x] = 0; //POSIÇÃO ATUAL INICIALIZA COM 0

                robot->x = robot->x+1; //POSIÇÃO DA FRENTE RECEBE O MESMO VALOR DO TERRORISTA
            }
            else{

                robot->dir=rand() % 4; /// AO ENCONTRAR OBSTÁCULO, RECEBE UM VALOR SORTEADO NO QUAL MUDA SUA DIREÇÃO PARA QUALQUER SENTIDO

            }

            break;

        case 1:

            if(!colizao(robot->x-1, robot->y, "npc")){

                mapa[robot->y][robot->x] = 0;

                robot->x = robot->x-1;

            }

            else{

                robot->dir=rand() % 4;

            }

            break;

        case 2:

            if(!colizao(robot->x, robot->y+1, "npc")){

                mapa[robot->y][robot->x] = 0;

                robot->y = robot->y+1;

            }

            else{

                robot->dir=rand() % 4;

            }

            break;

        case 3:

             if(!colizao(robot->x, robot->y-1, "npc")){

                mapa[robot->y][robot->x]  = 0;

                robot->y = robot->y-1;

            }

            else{

                robot->dir=rand() % 4;

            }

            break;

   }



    mapa[robot->y][robot->x] = 5;

}
//Inicializa parâmetros
void init (void){

    ///COR DO BACKGROUND

    glClearColor(0.8f, 0.0f, 1.0f, 0.0f);

}

void keyboard(unsigned char key, int x, int y){///AUMENTA OU DIMINUI ZOOM DA TELA AO PRESSIONAR + OU -

   switch (key)

   {

       case  '+':

            proporcao++;

            break;



        case '-':

            proporcao--;

            break;

        case 'i':

            iniciarGame();

            break;

   }

   glutPostRedisplay();

}

void Special_Keys(int key, int x, int y){///

    int auxX=X;

    int auxY=Y;

    switch (key)

    {

        case GLUT_KEY_DOWN:

            auxY--;

            break;


        case GLUT_KEY_UP:

            auxY++;

            break;


        case GLUT_KEY_LEFT:

            auxX--;

            break;


        case GLUT_KEY_RIGHT:

            auxX ++;

            break;

    }

    if(!colizao(auxY, auxX, "avatar")){

        mapa[X][Y] = 0;

        Y = auxY;

        X = auxX;

        mapa[X][Y] = 2;

    }



    printf("X: %d Y: %d  | ", X, Y);



    printf("\nfim: %d\n", fim);

    glutPostRedisplay();

}
// Função callback chamada para gerenciar eventos do mouse
void Mouse(int button, int state, int x, int y){

    y = windowSizeY - y;


    if (button == GLUT_LEFT_BUTTON) {



        if (state == GLUT_DOWN) {

            printf("x: %d\ny: %d\n\n", x,y);

        }

    }


    glutPostRedisplay();

}
//Principal
int main(int argc, char *argv[]){

    srand(time(NULL));

    for(int  i = 0; i < 10; i++){///SETA OS TERRORISTAS EM POSIÇÕES RANDÔMICAS DO MAPA, MENOS NAS BORDAS.

        robot[i].x = rand() % 43 + 2;

        robot[i].y = rand() % 43 + 2;

        robot[i].dir = rand() % 4;

    }



    for(int  i = 0; i < 10; i++){///SETA AS BOMBAS EM POSIÇÕES RANDÔMICAS DO MAPA, MENOS NAS BORDAS.

        bomba[i].x = rand() % 43 + 1;

        bomba[i].y = rand() % 43 + 1;

        bomba[i].ativa = true;

    }


    iniciarGame();

    glutInit(&argc, argv);


	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowSize (windowSizeX, windowSizeY);

	//Configura a posição inicial da janela

	glutInitWindowPosition (100, 100);

	//Configura a janela

	glutCreateWindow("Teste do OpenGL");

	//Chama a função desenha

	glutDisplayFunc(desenha);

    //inicia o CallBack do teclado

    glutKeyboardFunc(keyboard);

    //teclas especiais

    glutSpecialFunc(Special_Keys);

    //mouse

    glutMouseFunc(Mouse);

    //habilita tela cheia

    //glutFullScreen();

	//Inicializa o programa

	init();

	//Habilita o loop (máquina de estados)

	glutMainLoop();

	//encerra o programa

	return 0;

	//FUNÇÃO DO TEMPO EM MILISEGUNDOS
	glutTimerFunc(1000,myTimer,1000);


}
