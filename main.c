#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

GLint jogoPausado = 0; // 0 jogo despausado e 1 jogo pausado
GLint gameOver = 0; // 1 fim de jogo, 0 jogo em andamento
GLint splash = 1; //jogo comeca na splash
GLint placar = 0;
GLint vidas = 3;
GLint venceuJogo = 0;

//variavel para mover a nave para esquerda e direita
GLfloat posNaveX = 50; //comecar a nave no centro da tela
GLfloat posTiroY = 100; //tiro fora da tela (invisivel inicialmente)
GLfloat posTiroX = -10; //tiro fora da tela (invisivel inicialmente)

GLint sentidoAliensX = 1; //sentido de movimento do eixo x: esquerda (0) e direita (1). Inicialmente ir para direita

GLuint idTexturaNave; //link da imagem (licensa publica) https://pixabay.com/pt/vectors/foguete-vetor-espa%c3%a7o-lan%c3%a7amento-2442125/

GLuint idTexturaFundo; //link da imagem (licensa publica https://pixabay.com/pt/photos/estrelas-espa%c3%a7o-noite-cosmos-black-964022/

GLuint idTexturaAlien; //link da imagem (licensa publica https://pixabay.com/pt/photos/estrelas-reino-unido-noite-1654074/

GLuint idTexturaVida; //link da imagem (licensa publica https://pixabay.com/pt/vectors/red-cora%c3%a7%c3%a3o-sa%c3%bade-amor-forma-304570/


GLuint carregaTextura(const char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(
                           arquivo,
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );

    if (idTextura == 0) {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }

    return idTextura;
}


struct alien {
  GLfloat posX;
  GLfloat posY;
  GLint estaVivo;
};

struct tiroAlien {
  GLfloat posX;
  GLfloat posY;
};

struct alien aliens[3][7]; // matriz de aliens 3x7
struct tiroAlien tiroalien;

void iniciaAliens() {
  int i, j;
  for(i=0; i<3; i++) {
    for(j=0; j<7; j++) {
      aliens[i][j].posX = j*10 + 10;
      aliens[i][j].posY = 100-(i*10 + 10);
      aliens[i][j].estaVivo = 1;
    }
  }

  //inicializa 2 tiros de aliens (Fora tela inicialmente)
  tiroalien.posX = 0;
  tiroalien.posY = -10; // comecar com o tiro do alien fora da tela
}

void desenhaAliens() {
    int i, j;
    for(i=0; i<3; i++) {
      for(j=0; j<7; j++) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, idTexturaAlien);
        glBegin(GL_POLYGON);
            if(aliens[i][j].estaVivo) { //so desenha o alien se ele estiver vivo

              glTexCoord2f(0, 0);
              glVertex3f(aliens[i][j].posX-3, aliens[i][j].posY-3, 0);
              glTexCoord2f(0, 1);
              glVertex3f(aliens[i][j].posX-3, aliens[i][j].posY+3, 0);
              glTexCoord2f(1, 1);
              glVertex3f(aliens[i][j].posX+3, aliens[i][j].posY+3, 0);
              glTexCoord2f(1, 0);
              glVertex3f(aliens[i][j].posX+3, aliens[i][j].posY-3, 0);
            }
            
        glEnd();
        glDisable(GL_TEXTURE_2D);
      }
    }
}

void verificaAlienChegouFim() {
  //verifica se algum alien conseguiu descer o eixo y todo
  int i, j;
  for(i=0; i<3; i++) {
    for(j=0; j<7; j++) {
      if(aliens[i][j].estaVivo) {
        if(aliens[i][j].posY < 12) {
          //alien ganhou
          gameOver = 1;
        }
      }
    }
  }
}

void deslocaAliens(GLfloat eixoX, GLfloat eixoY) {
  int i, j;
  //faz o movimento dos aliens na horizontal
  for(i=0; i<3; i++) {
    for(j=0; j<7; j++) {
      aliens[i][j].posX += eixoX;
      aliens[i][j].posY += eixoY;
    }
  }
}

void movimentaAliens() {
  int i, j;
  
  //menorX e maiorX recebe os piores casos possiveis
  int menorX = 100;
  int maiorX = 0;
  for(i=0; i<3; i++) {
    for(j=0; j<7; j++) {
      if(aliens[i][j].estaVivo) {
        if(aliens[i][j].posX > maiorX) {
          maiorX = aliens[i][j].posX;
        }
        if(aliens[i][j].posX < menorX) {
          menorX = aliens[i][j].posX;
        }
      }
    }
  }
  
  //pega o sentido x dos aliens (direita ou esquerda)
  if(maiorX >= 97) { // nao vai ate 100 pq cada alien tem 3 de comprimento. Logo 100-3=97
    sentidoAliensX = 0; //inverte o sentido (para de ir para direita e comeca ir pra esquerda)
    deslocaAliens(0, -3); //desloca os aliens para baixo em -3 posicao no eixo y
    verificaAlienChegouFim();
  }
  if(menorX <= 3) {// nao vai ate 0 pq cada alien tem 3 de comprimento.
    sentidoAliensX = 1;//inverte o sentido (para de ir para esquerda e comeca ir pra direita)
    deslocaAliens(0, -3); //desloca os aliens para baixo em -3 posicao no eixo y
    verificaAlienChegouFim();
  }
  
  //faz o movimento dos aliens na horizontal
  if(sentidoAliensX == 1) {
    deslocaAliens(0.5, 0); //desloca os aliens na horizontal em 0.3 posicoes
  } else {
    deslocaAliens(-0.5, 0); //desloca os aliens na horizontal em -0.3 posicoes
  }
    
}



void darTiroAliens() {
  if(tiroalien.posY <= -10) { //so dar outro tiro se o ultimo tiro estiver oculto (y menor que -10 = fora da tela)
    int deuTiro = 0;
    int xAleatorio, yAleatorio;
    int index = 0;
    //tenta dar um tiro. Nao termina ate achar um alien que de o tiro (max 50 tentativas para encontrar uma matriz valida)
    while(deuTiro == 0 && index <= 50) {
      xAleatorio = rand() % 3; //x tem ate 3 linhas
      yAleatorio = rand() % 7; //y tem ate 3 colunas
      if(aliens[xAleatorio][yAleatorio].estaVivo) {
        tiroalien.posX = aliens[xAleatorio][yAleatorio].posX;
        tiroalien.posY = aliens[xAleatorio][yAleatorio].posY -3;
        deuTiro = 1;
      }
      index++;
    }
  } else {
    tiroalien.posY--;
    
    // verificar se colidiu o tiro do alien com a nave
    if(tiroalien.posX >= posNaveX-3 && tiroalien.posX <= posNaveX+3) {
      if(tiroalien.posY >= 2 && tiroalien.posY <= 10) {
        //se acertou, joga o tiro para fora da tela, para nao dar game over com 3 vidas
        tiroalien.posY = -10;
        vidas--;
        if(vidas == 0) {
          gameOver = 1;
        }
      }
    }
  }
}

void desenhaNave() {
  
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaNave);

    // desenha um POLYGON por seus vertices
    glBegin(GL_POLYGON);
        // glColor3f(0, 1, 0); // cor da nave
        
        glTexCoord2f(0, 0);
        glVertex3f(posNaveX-4, 2, 0);
        
        glTexCoord2f(0, 1);
        glVertex3f(posNaveX-4, 11, 0);
        
        glTexCoord2f(1, 1);
        glVertex3f(posNaveX+4, 11, 0);
        
        glTexCoord2f(1, 0);
        glVertex3f(posNaveX+4, 2, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void movimentaTiro() {
  int i, j;
  posTiroY += 2;
  
  //verifica se o tiro encostou em algum aliens. Verifica todos os aliens
  if(posTiroY >= 0 && posTiroY <= 100) { //somente verificar se encostou se o tiro estiver na tela.
    for(i=0; i<3; i++) {
      for(j=0; j<7; j++) {
        if(posTiroY >= aliens[i][j].posY-3 && posTiroY <= aliens[i][j].posY+3) {
          if(posTiroX >= aliens[i][j].posX-3 && posTiroX <= aliens[i][j].posX+3) {
            //verifica se esse alien esta vivo. Se estiver morto, nao pode morrer de novo
            if(aliens[i][j].estaVivo) {
              //se cair aqui, significa que o tiro encostou no alien
              aliens[i][j].estaVivo = 0;
              posTiroX = -10;
              posTiroY = 100;
              placar++;
              if(placar == 21) {
                tiroalien.posY = -10; //quando vencer o jogo, sumir com o tiro da nave
                venceuJogo = 1;
              }
            }
          }
        }
      }
    }
  }
}

void movimentaItems() {
  if(!jogoPausado && !gameOver && !splash) { //nao movimenta os items se o jogo estiver pausado, ou gameover ou se estiver na splash
    movimentaTiro();
    movimentaAliens();
    darTiroAliens();
  }
  glutPostRedisplay();
  glutTimerFunc(33, movimentaItems, 1);
}

void desenhaTiro() {
    glBegin(GL_POLYGON);
        glColor3f(1, 1, 1); // cor do tiro
        glVertex3f(posTiroX-0.5, posTiroY-2, 0);
        glVertex3f(posTiroX-0.5, posTiroY+2, 0);
        glVertex3f(posTiroX+0.5, posTiroY+2, 0);
        glVertex3f(posTiroX+0.5, posTiroY-2, 0);
    glEnd();
}

void desenhaTiroAlien() {
    glBegin(GL_POLYGON);
        glColor3f(1, 1, 1); // cor do tiro do alien
        glVertex3f(tiroalien.posX-0.5, tiroalien.posY-2, 0);
        glVertex3f(tiroalien.posX-0.5, tiroalien.posY+2, 0);
        glVertex3f(tiroalien.posX+0.5, tiroalien.posY+2, 0);
        glVertex3f(tiroalien.posX+0.5, tiroalien.posY-2, 0);
    glEnd();
}

void desenhaTexto(char string[], int x, int y, int fonte) {
  glColor3f(1,1,1); //texto branco
  glRasterPos2f(x, y);
  int i;
  for (i = 0; i < (int)strlen(string); i++) {
    if(fonte == 1) {
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    } else if (fonte == 2) {
      glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
    }
  }
}

void desenhaPlacar()
{
  char string[20];
  snprintf(string, 20, "Placar: %d", placar);
  desenhaTexto(string, 75, 95, 1);
}


void desenhaVidas()
{
  desenhaTexto("Vidas", 5, 95, 1);
  int i;
  for(i=0;i<vidas;i++) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaVida);
  
    // desenha um POLYGON por seus vertices
    glBegin(GL_POLYGON);

        glTexCoord2f(0, 0);
        glVertex3f(18+(i*5), 94, 0);
        
        glTexCoord2f(0, 1);
        glVertex3f(18+(i*5), 98, 0);
        
        glTexCoord2f(1, 1);
        glVertex3f(22+(i*5), 98, 0);
        
        glTexCoord2f(1, 0);
        glVertex3f(22+(i*5), 94, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
  }
}


void desenhaFundo() {
  // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaFundo);

    // desenha um POLYGON por seus vertices
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(0, 0, 0);
        
        glTexCoord2f(0, 1);
        glVertex3f(0, 100, 0);
        
        glTexCoord2f(1, 1);
        glVertex3f(100, 100, 0);
        
        glTexCoord2f(1, 0);
        glVertex3f(100, 0, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void desenhaGameOver() {
    if(gameOver) {
      // desenha um POLYGON por seus vertices
      glBegin(GL_POLYGON);
          glColor3f(0, 0, 0);
          glVertex3f(0, 0, 0);
          
          glVertex3f(0, 100, 0);
          
          glVertex3f(100, 100, 0);
          
          glVertex3f(100, 0, 0);
      glEnd();
      glDisable(GL_TEXTURE_2D);
      desenhaTexto("Game Over. Aperte R para reiniciar", 15, 50, 1);
    }
}

void desenhaVenceuJogo() {
  if(venceuJogo) {
    desenhaTexto("Parabens! Voce salvou a Galaxia!", 15, 70, 1);
    desenhaTexto("Aperte R para jogar novamente", 18, 64, 1);
  }
}

void desenhaSplash() {
 if(splash) {
    // desenha um POLYGON por seus vertices
    glBegin(GL_POLYGON);
        glColor3f(0, 0, 0);
        glVertex3f(0, 0, 0);
        
        glVertex3f(0, 100, 0);
        
        glVertex3f(100, 100, 0);
        
        glVertex3f(100, 0, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    desenhaTexto("Bem vindo a Galaxia!", 29, 60, 1);
    desenhaTexto("Aperte Barra de Espaco para iniciar", 15, 54, 1);
    desenhaTexto("Instrucoes:", 10, 25, 2);
    desenhaTexto("Atirar: Barra de Espaco", 10, 20, 2);
    desenhaTexto("Mover nave: setas direita e esquerda", 10, 15, 2);
  }
}
// callback de desenho (nave)
void desenhaMinhaCena() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 100, 0, 100, -1, 1); //tela divida em 100 "pedacos" no eixo x e y
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    desenhaFundo();
    desenhaAliens();
    desenhaTiro();
    desenhaTiroAlien();
    desenhaNave();
    
    if(jogoPausado) {
      desenhaTexto("Jogo Pausado. Aperte P para voltar.", 15, 50, 1);
    }
    
    desenhaPlacar();
    desenhaVidas();
    desenhaVenceuJogo();
    
    desenhaSplash();
    desenhaGameOver();
    
    glFlush();
}

void inicializa() {
    // cor do fundo (Background) preto
    glClearColor(0, 0, 0, 1);

    // habilita mesclagem de cores, para termos suporte a texturas
    // com transparência
    glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    idTexturaNave = carregaTextura("img/nave.png");
    idTexturaFundo = carregaTextura("img/fundo.png");
    idTexturaAlien = carregaTextura("img/alien1.png");
    idTexturaVida = carregaTextura("img/vidas.png");
  
}

void reiniciaJogo() {
  iniciaAliens();
  jogoPausado = 0;
  posNaveX = 50; // reinicia a posicao da nave
  posTiroY = 100; // reinicia a posicao do tiro
  posTiroX = -10; // reinicia a posicao do tiro
  sentidoAliensX = 1;
  gameOver = 0;
  vidas = 3;
  placar = 0;
  venceuJogo = 0;
}


void tecla(unsigned char key, int x, int y) {
    // verifica qual tecla foi pressionada
    switch(key) {
    case 27:      // "ESC"
        exit(0);  // encerra a aplicacao
        break;
        
    case 32:      // barra de espaco
        if(splash) { // se estiver na tela de splash, entao inicia o jogo, senao, faz a logica do tiro
          splash = 0;
          reiniciaJogo();
        }
        else if(!jogoPausado){ //nao dar tiro com o jogo pausado
          // so pode dar um tiro de cada vez. Portanto, verifica se a posicao do tiro e maior que 100. Senao, precisa esperar o tiro para apertar de novo
          if(posTiroY >= 100) {
            posTiroY = 11; //posicao 11 quando da o tiro, para parecer que esta saindo da nave e nao no inicio da tela
            posTiroX = posNaveX;
          }
        }
        break;
    case 'p': //pausar jogo
        jogoPausado = !jogoPausado;
        break;
    case 'r': //reiniciar jogo
        reiniciaJogo();
        splash = 1;
        break;
    default:
        break;
    }
}

void teclaEspecial(GLint key, int x, int y) {
    int i;
    // verifica qual tecla foi pressionada
    switch(key) {
      case GLUT_KEY_LEFT: //seta para esquerda
        if(!jogoPausado && posNaveX >=4) { // nao deixar a nave sair da tela e nao movimentar se estiver pausado
          posNaveX -= 4;
        }
        break;
        
      case GLUT_KEY_RIGHT: //seta para direita
        if(!jogoPausado && posNaveX <= 96) { // nao deixar a nave sair da tela
          posNaveX += 4;
        }
        break;
          
      default:
        break;
    }
    desenhaMinhaCena();
}

// função principal
int main(int argc, char** argv) {
   glutInit(&argc, argv);
   
   glutInitContextVersion(1, 1);
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);

   glutCreateWindow("Galaxian");
   
   //inicializa a matriz 3x7 de aliens
   iniciaAliens();

   // registra callbacks para alguns eventos
   glutDisplayFunc(desenhaMinhaCena);
   glutKeyboardFunc(tecla);
   glutSpecialFunc(teclaEspecial);
   glutTimerFunc(33, movimentaItems, 1); // 1000/33 = 30 frames por segundo

   inicializa();
   
   glutMainLoop();
   return 0;
}