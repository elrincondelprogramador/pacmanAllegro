#include <allegro.h>
#define CAMBIOPACMANSPRITE       15
#define CAMBIOPACMANMOVIMIENTO   1
#define CAMBIOFANTASMASPRITE     45
#define CAMBIOFANTASMAMOVIMIENTO 1
#define CAMBIOCOMIDA             100
#define CAMBIODIRECCION          40
#define TIEMPOABRIR              3000
/* Para el tamaño del mapa */
#define COLUMNAS 29
#define FILAS 32

/* tamaño de cada sprite */
#define SPRITEX 16
#define SPRITEY 16

/* Funciones utilizadas */
void Inicializar_Variables(); /* Inicializa una serie de variables varias */
void Inicializar_Funciones(); /* Inicializa una serie de funciones varias */
int Inicializar_Graficos(int ancho, int alto); /* Inicializa el modo gráfico */
void Inicializar_Temporizador();
void Timer0();
int Crear_Sprites(BITMAP *imagen, PALETTE palette, char *ruta); /* Crea sprites sacado del bitmap seleccionado */
void Destruir_Sprites(BITMAP *imagen); /* Destruye los sprites puestos en el bitmap seleccionado */
void Inicializar_Pacman(int x, int y, int sprite, int direccion); /* Posiciona a pacman en la posicion X e Y inicial en la estructura correspondiente */
void Cargar_Mapa(int numeroMapa);
void Dibujar_Mapa(int x, int y, int numeroMapa);
void Colision(int incrementoPacmanX, int incrementoPacmanY, int direccion, int incX);
void Buscar_Transportacion(char letra);
void Colision_Fantasma1(int incrementoFantasmaX, int incrementoFantasmaY, int direccion, int incX);
void Colision_Fantasma2(int incrementoFantasmaX, int incrementoFantasmaY, int direccion, int incX);
void Colision_Fantasma3(int incrementoFantasmaX, int incrementoFantasmaY, int direccion, int incX);
void Colision_Fantasma4(int incrementoFantasmaX, int incrementoFantasmaY, int direccion, int incX);

/* Variables utilizadas */
volatile int contadorPacmanMovimiento = 0, contadorPacmanSprite = 0;
volatile int contadorFantasma1Movimiento = 0, contadorFantasma1Sprite = 0;
volatile int contadorComidaCambio = 0;
volatile int contadorCambioDireccionFantasma1 = 0;
volatile int abrirPuerta = 0;
/* incrementos x e y para pacman */
int incrementoPacmanX = 0, incrementoPacmanY = 0;
/* incrementos x e y para fantasma1 */
int incrementoFantasma1X = 0, incrementoFantasma1Y = 0;
char almacenarFantasma1 = ' ';
/* incrementos x e y para fantasma2 */
int incrementoFantasma2X = 0, incrementoFantasma2Y = 0;
char almacenarFantasma2 = ' ';
/* incrementos x e y para fantasma3 */
int incrementoFantasma3X = 0, incrementoFantasma3Y = 0;
char almacenarFantasma3 = ' ';
/* incrementos x e y para fantasma4 */
int incrementoFantasma4X = 0, incrementoFantasma4Y = 0;
char almacenarFantasma4 = ' ';

char Mapa[FILAS][COLUMNAS];

struct EstructuraComida
{
       BITMAP *spriteComida;
}spriteComida[3];

struct EstructuraMapa
{
       BITMAP *spriteColumnaMapa;
}spriteColumnaMapa[10];

struct EstructuraPacman
{
       struct EstructuraMovimiento
       {
              BITMAP *pacman;
       }spritePacman[3];
}spritePacman[4];

struct EstructuraPacman2
{
       int x, y;
       int sprite, direccion;
}movimientoPacman;

struct EstructuraFantasma1
{
       struct EstructuraMovimiento
       {
              BITMAP *fantasma;
       }spriteFantasma1[2];
}spriteFantasma1[4];

struct EstructuraFantasma11
{
       int x, y;
       int sprite, direccion;
}movimientoFantasma1;

struct EstructuraFantasma2
{
       struct EstructuraMovimiento
       {
              BITMAP *fantasma;
       }spriteFantasma2[2];
}spriteFantasma2[4];

struct EstructuraFantasma22
{
       int x, y;
       int sprite, direccion;
}movimientoFantasma2;

struct EstructuraFantasma3
{
       struct EstructuraMovimiento
       {
              BITMAP *fantasma;
       }spriteFantasma3[2];
}spriteFantasma3[4];

struct EstructuraFantasma33
{
       int x, y;
       int sprite, direccion;
}movimientoFantasma3;

struct EstructuraFantasma4
{
       struct EstructuraMovimiento
       {
              BITMAP *fantasma;
       }spriteFantasma4[2];
}spriteFantasma4[4];

struct EstructuraFantasma44
{
       int x, y;
       int sprite, direccion;
}movimientoFantasma4;

void Inicializar_Variables()
{               
}

void Inicializar_Funciones()
{
     allegro_init();
     install_keyboard();
     install_timer();   
     Inicializar_Temporizador();
}

int Inicializar_Graficos(int ancho, int alto)
{
    if(set_gfx_mode(GFX_SAFE, ancho, alto, 0, 0) != 0)
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("No se ha conseguido inicializar el modo gráfico.\n\n%s", allegro_error);
        return 1;
    }
    return 0;
}

void Inicializar_Temporizador()
{
   /* Rutinas de temporización. Bloqueamos las variables y la función del temporizador */
   LOCK_VARIABLE(contadorPacmanMovimiento);
   LOCK_VARIABLE(contadorPacmanSprite);   
   LOCK_VARIABLE(contadorFantasma1Movimiento);
   LOCK_VARIABLE(contadorFantasma1Sprite);    
   LOCK_VARIABLE(contadorComidaCambio);  
   LOCK_VARIABLE(contadorCambioDireccionFantasma1);
   LOCK_VARIABLE(abrirPuerta);
   LOCK_FUNCTION(Timer0);
 
   /* Y ponemos el temporizador en marcha: cada 10 milisegundos(cada centésima de segundo) */
   install_int(Timer0, 5);
}

void Timer0()
{
     contadorPacmanMovimiento++;
     contadorPacmanSprite++;
     contadorFantasma1Movimiento++;
     contadorFantasma1Sprite++;
     contadorComidaCambio++;
     contadorCambioDireccionFantasma1++;
     abrirPuerta++;
}

int Crear_Sprites(BITMAP *imagen, PALETTE palette, char *ruta)
{   
      int i, j;
      
      imagen = load_bitmap(ruta, palette);
      if (!imagen)
      {
                set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
                allegro_message("No se ha conseguido cargar la imagen\n");
                return 1;
      }
      set_palette(palette);
      
      for(i = 0; i < 4; i++)
            for(j = 0; j < 3; j++)      
                  spritePacman[i].spritePacman[j].pacman = create_bitmap(SPRITEX, SPRITEY);             
      
      blit(imagen, spritePacman[0].spritePacman[0].pacman, 6, 26, 0, 0, SPRITEX, SPRITEY);
      blit(imagen, spritePacman[0].spritePacman[1].pacman, 25, 26, 0, 0, SPRITEX, SPRITEY);
      blit(imagen, spritePacman[0].spritePacman[2].pacman, 46, 6, 0, 0, SPRITEX, SPRITEY);
      
      blit(imagen, spritePacman[1].spritePacman[0].pacman, 6, 6, 0, 0, SPRITEX, SPRITEY);
      blit(imagen, spritePacman[1].spritePacman[1].pacman, 27, 6, 0, 0, SPRITEX, SPRITEY);
      blit(imagen, spritePacman[1].spritePacman[2].pacman, 46, 6, 0, 0, SPRITEX, SPRITEY);      
      
      blit(imagen, spritePacman[2].spritePacman[0].pacman, 6, 46, 0, 0, SPRITEX, SPRITEY);
      blit(imagen, spritePacman[2].spritePacman[1].pacman, 25, 46, 0, 0, SPRITEX, SPRITEY);
      blit(imagen, spritePacman[2].spritePacman[2].pacman, 46, 6, 0, 0, SPRITEX, SPRITEY);
      
      blit(imagen, spritePacman[3].spritePacman[0].pacman, 6, 66, 0, 0, SPRITEX, SPRITEY);
      blit(imagen, spritePacman[3].spritePacman[1].pacman, 25, 64, 0, 0, SPRITEX, SPRITEY);
      blit(imagen, spritePacman[3].spritePacman[2].pacman, 46, 6, 0, 0, SPRITEX, SPRITEY);

      /* Fantasma 1 */
      for(i = 0; i < 4; i++)
            for(j = 0; j < 2; j++)      
                  spriteFantasma1[i].spriteFantasma1[j].fantasma = create_bitmap(SPRITEX, SPRITEY);
      
      blit(imagen, spriteFantasma1[0].spriteFantasma1[0].fantasma, 126, 86, 0, 0, SPRITEX, SPRITEY);
      blit(imagen, spriteFantasma1[0].spriteFantasma1[1].fantasma, 146, 86, 0, 0, SPRITEX, SPRITEY);      

      blit(imagen, spriteFantasma1[1].spriteFantasma1[0].fantasma, 86, 86, 0, 0, SPRITEX, SPRITEY);
      blit(imagen, spriteFantasma1[1].spriteFantasma1[1].fantasma, 106, 86, 0, 0, SPRITEX, SPRITEY);               
      
      blit(imagen, spriteFantasma1[2].spriteFantasma1[0].fantasma, 6, 86, 0, 0, SPRITEX, SPRITEY);
      blit(imagen, spriteFantasma1[2].spriteFantasma1[1].fantasma, 26, 86, 0, 0, SPRITEX, SPRITEY);
      
      blit(imagen, spriteFantasma1[3].spriteFantasma1[0].fantasma, 46, 86, 0, 0, SPRITEX, SPRITEY);      
      blit(imagen, spriteFantasma1[3].spriteFantasma1[1].fantasma, 66, 86, 0, 0, SPRITEX, SPRITEY);
      
      /* Fantasma 2 */
      for(i = 0; i < 4; i++)
            for(j = 0; j < 2; j++)      
                  spriteFantasma2[i].spriteFantasma2[j].fantasma = create_bitmap(SPRITEX, SPRITEY);
      
      blit(imagen, spriteFantasma2[0].spriteFantasma2[0].fantasma, 6, 106, 0, 0, SPRITEX, SPRITEY);
      blit(imagen, spriteFantasma2[0].spriteFantasma2[1].fantasma, 26, 106, 0, 0, SPRITEX, SPRITEY);
      
      blit(imagen, spriteFantasma2[1].spriteFantasma2[0].fantasma, 46, 106, 0, 0, SPRITEX, SPRITEY);      
      blit(imagen, spriteFantasma2[1].spriteFantasma2[1].fantasma, 66, 106, 0, 0, SPRITEX, SPRITEY);
      
      blit(imagen, spriteFantasma2[2].spriteFantasma2[0].fantasma, 86, 106, 0, 0, SPRITEX, SPRITEY);
      blit(imagen, spriteFantasma2[2].spriteFantasma2[1].fantasma, 106, 106, 0, 0, SPRITEX, SPRITEY);
      
      blit(imagen, spriteFantasma2[3].spriteFantasma2[0].fantasma, 126, 106, 0, 0, SPRITEX, SPRITEY);
      blit(imagen, spriteFantasma2[3].spriteFantasma2[1].fantasma, 146, 106, 0, 0, SPRITEX, SPRITEY);                          
      
      /* Fantasma 3 */
      for(i = 0; i < 4; i++)
            for(j = 0; j < 2; j++)      
                  spriteFantasma3[i].spriteFantasma3[j].fantasma = create_bitmap(SPRITEX, SPRITEY);
      
      blit(imagen, spriteFantasma3[0].spriteFantasma3[0].fantasma, 6, 126, 0, 0, SPRITEX, SPRITEY);
      blit(imagen, spriteFantasma3[0].spriteFantasma3[1].fantasma, 26, 126, 0, 0, SPRITEX, SPRITEY);
      
      blit(imagen, spriteFantasma3[1].spriteFantasma3[0].fantasma, 46, 126, 0, 0, SPRITEX, SPRITEY);      
      blit(imagen, spriteFantasma3[1].spriteFantasma3[1].fantasma, 66, 126, 0, 0, SPRITEX, SPRITEY);
      
      blit(imagen, spriteFantasma3[2].spriteFantasma3[0].fantasma, 86, 126, 0, 0, SPRITEX, SPRITEY);
      blit(imagen, spriteFantasma3[2].spriteFantasma3[1].fantasma, 106, 126, 0, 0, SPRITEX, SPRITEY);

      blit(imagen, spriteFantasma3[3].spriteFantasma3[0].fantasma, 126, 126, 0, 0, SPRITEX, SPRITEY);
      blit(imagen, spriteFantasma3[3].spriteFantasma3[1].fantasma, 146, 126, 0, 0, SPRITEX, SPRITEY);       
      
      /* Fantasma 4 */
      for(i = 0; i < 4; i++)
            for(j = 0; j < 2; j++)      
                  spriteFantasma4[i].spriteFantasma4[j].fantasma = create_bitmap(SPRITEX, SPRITEY);
      
      blit(imagen, spriteFantasma4[0].spriteFantasma4[0].fantasma, 6, 146, 0, 0, SPRITEX, SPRITEY);
      blit(imagen, spriteFantasma4[0].spriteFantasma4[1].fantasma, 26, 146, 0, 0, SPRITEX, SPRITEY);

      blit(imagen, spriteFantasma4[1].spriteFantasma4[0].fantasma, 46, 146, 0, 0, SPRITEX, SPRITEY);      
      blit(imagen, spriteFantasma4[1].spriteFantasma4[1].fantasma, 66, 146, 0, 0, SPRITEX, SPRITEY);
      
      blit(imagen, spriteFantasma4[2].spriteFantasma4[0].fantasma, 86, 146, 0, 0, SPRITEX, SPRITEY);
      blit(imagen, spriteFantasma4[2].spriteFantasma4[1].fantasma, 106, 146, 0, 0, SPRITEX, SPRITEY);

      blit(imagen, spriteFantasma4[3].spriteFantasma4[0].fantasma, 126, 146, 0, 0, SPRITEX, SPRITEY);
      blit(imagen, spriteFantasma4[3].spriteFantasma4[1].fantasma, 146, 146, 0, 0, SPRITEX, SPRITEY);       
      
      
      /* Columna del mapa */
      for(i = 0; i < 10; i++)
            spriteColumnaMapa[i].spriteColumnaMapa = create_bitmap(SPRITEX, SPRITEY);      
      blit(imagen, spriteColumnaMapa[0].spriteColumnaMapa, 5, 341, 0, 0, SPRITEX, SPRITEY);                                                      
      blit(imagen, spriteColumnaMapa[1].spriteColumnaMapa, 23, 341, 0, 0, SPRITEX, SPRITEY);                                                      
      blit(imagen, spriteColumnaMapa[2].spriteColumnaMapa, 41, 341, 0, 0, SPRITEX, SPRITEY);                                                      
      blit(imagen, spriteColumnaMapa[3].spriteColumnaMapa, 59, 341, 0, 0, SPRITEX, SPRITEY);                                                      
      blit(imagen, spriteColumnaMapa[4].spriteColumnaMapa, 77, 341, 0, 0, SPRITEX, SPRITEY);                                                      
      blit(imagen, spriteColumnaMapa[5].spriteColumnaMapa, 95, 341, 0, 0, SPRITEX, SPRITEY);                                                      
      blit(imagen, spriteColumnaMapa[6].spriteColumnaMapa, 113, 341, 0, 0, SPRITEX, SPRITEY);                                                      
      blit(imagen, spriteColumnaMapa[7].spriteColumnaMapa, 131, 341, 0, 0, SPRITEX, SPRITEY);                                                      
      blit(imagen, spriteColumnaMapa[8].spriteColumnaMapa, 149, 341, 0, 0, SPRITEX, SPRITEY);                                                      
      blit(imagen, spriteColumnaMapa[9].spriteColumnaMapa, 167, 341, 0, 0, SPRITEX, SPRITEY);                                                      
                                                            
      /* Comida pacman */
      spriteComida[0].spriteComida = create_bitmap(SPRITEX, SPRITEY);      
      spriteComida[1].spriteComida = create_bitmap(SPRITEX, SPRITEY);      
      spriteComida[2].spriteComida = create_bitmap(SPRITEX, SPRITEY);                        
      blit(imagen, spriteComida[0].spriteComida, 5, 365, 0, 0, SPRITEX, SPRITEY);      
      blit(imagen, spriteComida[1].spriteComida, 34, 365, 0, 0, SPRITEX, SPRITEY);            
      blit(imagen, spriteComida[2].spriteComida, 60, 363, 0, 0, SPRITEX, SPRITEY);            
            
      //draw_sprite(screen, spriteColumnaMapa[9].spriteColumnaMapa, 200, 200);                        
      //blit(bmp,screen,0,0,0,0,bmp->w,bmp->h); 
      //readkey();
      clear_bitmap(screen);      
      return 0;
}

void Destruir_Sprites(BITMAP *imagen)
{
     destroy_bitmap(imagen);
}

void Inicializar_Pacman(int x, int y, int sprite, int direccion)
{     
      /*
        direccion:
                  0         ->     Pacman ve hacia la derecha
                  1         ->     Pacman ve hacia la izquierda
                  2         ->     Pacman ve hacia arriba
                  3         ->     Pacman ve hacia abajo                  
        sprite:
                  0         ->     Primer sprite, pacman con la boca abierta
                  1         ->     Segundo sprite, pacman con la boca semiabierta
                  2         ->     Tercer sprite, pacman con la boca cerrada
        x, y:     Coordenadas de pacman en la ventana gráfica.
      */
      movimientoPacman.x = x;
      movimientoPacman.y = y;
      movimientoPacman.sprite = sprite;
      movimientoPacman.direccion = direccion;
      
//      draw_sprite(screen, spritePacman[direccion].spritePacman[sprite].pacman, movimientoPacman.x, movimientoPacman.y);
}

void Inicializar_Fantasma1(int x, int y, int sprite, int direccion)
{     
      /*
        direccion:
                  0         ->     Pacman ve hacia la derecha
                  1         ->     Pacman ve hacia la izquierda
                  2         ->     Pacman ve hacia arriba
                  3         ->     Pacman ve hacia abajo                  
        sprite:
                  0         ->     Primer sprite, pacman con la boca abierta
                  1         ->     Segundo sprite, pacman con la boca semiabierta
                  2         ->     Tercer sprite, pacman con la boca cerrada
        x, y:     Coordenadas de pacman en la ventana gráfica.
      */
      movimientoFantasma1.x = x;
      movimientoFantasma1.y = y;
      movimientoFantasma1.sprite = sprite;
      movimientoFantasma1.direccion = direccion;
}

void Inicializar_Fantasma2(int x, int y, int sprite, int direccion)
{     
      /*
        direccion:
                  0         ->     Pacman ve hacia la derecha
                  1         ->     Pacman ve hacia la izquierda
                  2         ->     Pacman ve hacia arriba
                  3         ->     Pacman ve hacia abajo                  
        sprite:
                  0         ->     Primer sprite, pacman con la boca abierta
                  1         ->     Segundo sprite, pacman con la boca semiabierta
                  2         ->     Tercer sprite, pacman con la boca cerrada
        x, y:     Coordenadas de pacman en la ventana gráfica.
      */
      movimientoFantasma2.x = x;
      movimientoFantasma2.y = y;
      movimientoFantasma2.sprite = sprite;
      movimientoFantasma2.direccion = direccion;            
}

void Inicializar_Fantasma3(int x, int y, int sprite, int direccion)
{     
      /*
        direccion:
                  0         ->     Pacman ve hacia la derecha
                  1         ->     Pacman ve hacia la izquierda
                  2         ->     Pacman ve hacia arriba
                  3         ->     Pacman ve hacia abajo                  
        sprite:
                  0         ->     Primer sprite, pacman con la boca abierta
                  1         ->     Segundo sprite, pacman con la boca semiabierta
                  2         ->     Tercer sprite, pacman con la boca cerrada
        x, y:     Coordenadas de pacman en la ventana gráfica.
      */
      movimientoFantasma3.x = x;
      movimientoFantasma3.y = y;
      movimientoFantasma3.sprite = sprite;
      movimientoFantasma3.direccion = direccion;            
}

void Inicializar_Fantasma4(int x, int y, int sprite, int direccion)
{     
      /*
        direccion:
                  0         ->     Pacman ve hacia la derecha
                  1         ->     Pacman ve hacia la izquierda
                  2         ->     Pacman ve hacia arriba
                  3         ->     Pacman ve hacia abajo                  
        sprite:
                  0         ->     Primer sprite, pacman con la boca abierta
                  1         ->     Segundo sprite, pacman con la boca semiabierta
                  2         ->     Tercer sprite, pacman con la boca cerrada
        x, y:     Coordenadas de pacman en la ventana gráfica.
      */
      movimientoFantasma4.x = x;
      movimientoFantasma4.y = y;
      movimientoFantasma4.sprite = sprite;
      movimientoFantasma4.direccion = direccion;            
}

void Cargar_Mapa(int numeroMapa, int x, int y)
{
     int i, j, auxX = x;
      
char aux[FILAS][COLUMNAS] = {
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  "XcccccccccccXXXXcccccccccccX",
  "XcXXXXcXXXXcXXXXcXXXXcXXXXcX",
  "XccccccccXXcXXXXcXXccccccccX",
  "XZXXXXcXccccccccccccXcXXXXZX",
  "XcXXXXcXcXXXXXXXXXXcXcXXXXcX",
  "XcXXXXcXcXXXXXXXXXXcXcXXXXcX",
  "XccccccXccccXXXXccccXccccccX",
  "XXXXXXcXXXXcXXXXcXXXXcXXXXXX",
  "     XcXXXccccccccXXXcX     ",
  "     XcXXXcXXXXXXcXXXcX     ",
  "XXXXXXcXXXZXMRYBXZXXXcXXXXXX",
  "cccccccccccXXXXXXccccccccccc",
  "XXXXXXcXXXcccPccccXXXcXXXXXX",
  "     XcXXXcXXXXXXcXXXcX     ",
  "     XcXXXcccXXcccXXXcX     ",
  "XXXXXXcXXXcXcXXcXcXXXcXXXXXX",
  "XccccccccccXccccXccccccccccX",
  "XcXXXXXXXXcXXXXXXcXXXXXXXXcX",
  "XcXXXXXXXXcXccccXcXXXXXXXXcX",
  "XccccccccccccXXccccccccccccX",
  "XcXXXXXXXXXXcXXcXXXXXXXXXXcX",
  "XccccccccccccXXccccccccccccX",
  "XZXcXcXXXXXXcXXcXXXXXXcXcXZX",
  "XcXcXcXXXXXXcXXcXXXXXXcXcXcX",
  "XcXcXccccccccccccccccccXcXcX",
  "XcXcXXXXXXXXXccXXXXXXXXXcXcX",
  "XcXccccccccccccccccccccccXcX",
  "XcXXXXXcXXXXXccXXXXcXXXXXXcX",
  "XccccccccccccccccccccccccccX",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXX"          
}; 

char aux2[FILAS][COLUMNAS] = {
  "XXXXXXXXXXXXXccXXXXXXXXXXXXX",
  "XZccccccccccXccXccccccccccZX",
  "XcXXXXcXXXXcXccXcXXXXcXXXXcX",
  "XcXXXXcccXXcXccXcXXcccXXXXcX",
  "XccccccXccccccccccccXccccccX",
  "XcXXXXcXcXXXXXXXXXXcXcXXXXcX",
  "XccccccXcXXXXXXXXXXcXccccccX",
  "XcXXXXcXccccXXXXccccXcXXXXcX",
  "XZcccccXXXXcXXXXcXXXXcccccZX",
  "XXXXXXcXXXccccccccXXXcXXXXXX",
  "     XcXXXcXXXXXXcXXXcX     ",
  "XXXXXXcXXXcXMYRBXcXXXcXXXXXX",
  "cccccccccccXXXXXXccccccccccc",
  "XXXXXXcXXXcccPccccXXXcXXXXXX",
  "     XcXXXcXXXXXXcXXXcX     ",
  "XXXXXXcXXXcccXXcccXXXcXXXXXX",
  "XccccccXXXcXcXXcXcXXXccccccX",
  "XXXXXXcccccXccccXcccccXXXXXX",
  "     XcXXXcXXXXXXcXXXcX     ",
  "XXXXXXcXXXcXccccXcXXXcXXXXXX",
  "cccccccccccccXXccccccccccccc",
  "XXXXXXXXXXXXcXXcXXXXXXXXXXXX",
  "XZcccccccccccXXcccccccccccZX",
  "XcXcXcXcXcXXcXXcXXcXcXcXcXcX",
  "XccccccccccccccccccccccccccX",
  "XcXcXcXcXcXXcXXcXXcXcXcXcXcX",
  "XcXcXcXcXcXXcXXcXXcXcXcXcXcX",
  "XcXccccccccccccccccccccccXcX",
  "XcXXXXXXXXXXXXXXXXXXXXXXXXcX",
  "XZccccccccccccccccccccccccZX",
  "XXXXXXXXXXXXXccXXXXXXXXXXXXX"          
};                                  
      
     switch(numeroMapa)
     {
         case 0:
                 rectfill(screen, 0, 0, 600, 600, palette_color[1]);                                                                                            
                 for(i = 0; i < FILAS; i++)     
                       for(j = 0; j < COLUMNAS-1; j++)           
                             Mapa[i][j] = aux[i][j];                    
              break;              
         case 1:
                 rectfill(screen, 0, 0, 600, 600, palette_color[1]);                                                                                            
                 for(i = 0; i < FILAS; i++)     
                       for(j = 0; j < COLUMNAS-1; j++)           
                             Mapa[i][j] = aux2[i][j];                    
              break;                            
     }
     for(i = 0; i < FILAS; i++)
     {
           for(j = 0; j < COLUMNAS-1; j++)
           {
                      if(Mapa[i][j] == 'X')   
                                    draw_sprite(screen, spriteColumnaMapa[numeroMapa].spriteColumnaMapa, x, y);                      
                      else if(Mapa[i][j] == 'P')
                      {
                                   draw_sprite(screen, spritePacman[0].spritePacman[0].pacman, x, y);                                   
                                   Inicializar_Pacman(j, i, 0, 0);
                      }
                      else if(Mapa[i][j] == 'R')
                      {
                                   draw_sprite(screen, spriteFantasma1[0].spriteFantasma1[0].fantasma, x, y);
                                   Inicializar_Fantasma1(j, i, 0, 0);
                      }
                      else if(Mapa[i][j] == 'Y')
                      {
                                   draw_sprite(screen, spriteFantasma2[0].spriteFantasma2[0].fantasma, x, y);
                                   Inicializar_Fantasma2(j, i, 0, 0);
                      }
                      else if(Mapa[i][j] == 'M')
                      {
                                   draw_sprite(screen, spriteFantasma3[0].spriteFantasma3[0].fantasma, x, y);                                    
                                   Inicializar_Fantasma3(j, i, 0, 0);
                      }
                      else if(Mapa[i][j] == 'B')
                      {
                                   draw_sprite(screen, spriteFantasma4[0].spriteFantasma4[0].fantasma, x, y);                                   
                                   Inicializar_Fantasma4(j, i, 0, 0);
                      }
                      else if(Mapa[i][j] == 'Z')
                                   draw_sprite(screen, spriteComida[0].spriteComida, x, y);                                                                
                      else if(Mapa[i][j] == 'c')
                                   draw_sprite(screen, spriteComida[1].spriteComida, x, y);                                                                
                      x+=SPRITEX;
           }
           x = auxX;
           y+=SPRITEY;
     }     
}

int Mover_Pacman(int direccion, int sprite)
{
     int incX = 0, incY = 0;
     
     switch(direccion)
     {
          case 0: /* Derecha */
               if(Mapa[movimientoPacman.y][(movimientoPacman.x)+1] != 'X' && incrementoPacmanY == 0 && ((movimientoPacman.x)+1 < (COLUMNAS-1)))
               {
                    incX = incrementoPacmanX/16;
                    incY = 0;
                    Inicializar_Pacman(movimientoPacman.x + incX, movimientoPacman.y+incY, sprite, direccion);
                    Colision(incrementoPacmanX, incrementoPacmanY, direccion, incX);
                     if(incrementoPacmanX < 16)
                          incrementoPacmanX++;
                     else
                          incrementoPacmanX = 0;
                     return 0;
               }
               else if((movimientoPacman.x)+1 >= (COLUMNAS-1))
               {
                    Mapa[movimientoPacman.y][movimientoPacman.x] = ' ';
                    movimientoPacman.x = 0;
                    if(Mapa[movimientoPacman.y][movimientoPacman.x] == 'c')
                    {
                    }
                    Mapa[movimientoPacman.y][movimientoPacman.x] = 'P';                                        
               }
               break;
          case 1: /* Izquierda */
               if(Mapa[movimientoPacman.y][(movimientoPacman.x)-1] != 'X' && incrementoPacmanY == 0 && (movimientoPacman.x) != 0)
               {          
                    incX = (incrementoPacmanX/16);
                    incY = 0;
                    Inicializar_Pacman(movimientoPacman.x + incX, movimientoPacman.y+incY, sprite, direccion);
                    Colision(incrementoPacmanX, incrementoPacmanY, direccion, incX);
                     if(incrementoPacmanX > -16)
                          incrementoPacmanX--;
                     else
                          incrementoPacmanX = 0;
                     return 0;
               }
               else if((movimientoPacman.x) == 0)
               {
                    Mapa[movimientoPacman.y][movimientoPacman.x] = ' ';
                    movimientoPacman.x = COLUMNAS-1;
                    if(Mapa[movimientoPacman.y][movimientoPacman.x] == 'c')
                    {
                    }
                    Mapa[movimientoPacman.y][movimientoPacman.x] = 'P';                                        
               }               
               break;
          case 2: /* Arriba */
               if(Mapa[(movimientoPacman.y)-1][movimientoPacman.x] != 'X' && incrementoPacmanX == 0 && movimientoPacman.y != 0)
               {          
                    incX = 0;
                    incY = (incrementoPacmanY/16);
                    Inicializar_Pacman(movimientoPacman.x + incX, movimientoPacman.y+incY, sprite, direccion);
                    Colision(incrementoPacmanX, incrementoPacmanY, direccion, incX);
                     if(incrementoPacmanY > -16)
                          incrementoPacmanY--;
                     else
                          incrementoPacmanY = 0;                    
                     return 0;
               }
               else if((movimientoPacman.y) == 0)
               {
                    Mapa[movimientoPacman.y][movimientoPacman.x] = ' ';
                    movimientoPacman.y = FILAS-1;
                    if(Mapa[movimientoPacman.y][movimientoPacman.x] == 'c')
                    {
                    }
                    Mapa[movimientoPacman.y][movimientoPacman.x] = 'P';
               }                              
               break;
          case 3: /* Abajo */
               if(Mapa[(movimientoPacman.y)+1][movimientoPacman.x] != 'X' && incrementoPacmanX == 0 && (movimientoPacman.y)+1 < FILAS-1)
               {              
                    incX = 0;
                    incY = incrementoPacmanY/16;
                    Inicializar_Pacman(movimientoPacman.x + incX, movimientoPacman.y+incY, sprite, direccion);
                    Colision(incrementoPacmanX, incrementoPacmanY, direccion, incX);
                     if(incrementoPacmanY < 16)
                          incrementoPacmanY++;
                     else
                          incrementoPacmanY = 0;
                     return 0;
               }
               else if((movimientoPacman.y)+1 >= (FILAS-1))
               {
                    Mapa[movimientoPacman.y][movimientoPacman.x] = ' ';
                    movimientoPacman.y = 0;
                    if(Mapa[movimientoPacman.y][movimientoPacman.x] == 'c')
                    {
                    }
                    Mapa[movimientoPacman.y][movimientoPacman.x] = 'P';                                        
               }               
               break;
     }
     return 1;
     //textprintf(screen, font, 10, 10, palette_color[15], "%d,%d - %d - %d - %c",movimientoPacman.x,movimientoPacman.y, incrementoPacmanX, incX, Mapa[movimientoPacman.y][(movimientoPacman.x)]);
     //readkey();     
     
     //textprintf(screen, font, 300, 10, palette_color[15], "%c - %c - %c",Mapa[movimientoPacman.y][(movimientoPacman.x)], Mapa[movimientoPacman.y][(movimientoPacman.x)+1], Mapa[13][13]);
     //readkey();     
}

int Mover_Fantasma1(int direccion, int sprite)
{
     int incX = 0, incY = 0;
     
     switch(direccion)
     {
          case 0: /* Derecha */
               if(Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)+1] != 'X' && Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)+1] != 'B' && Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)+1] != 'M' && Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)+1] != 'Y' && incrementoFantasma1Y == 0 && ((movimientoFantasma1.x)+1 < (COLUMNAS-1)))
               {
                    incX = incrementoFantasma1X/16;
                    incY = 0;
                    if(incX == 1)
                            Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)] = almacenarFantasma1;
                            //almacenarFantasma1 = Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)+1];

                    Inicializar_Fantasma1(movimientoFantasma1.x + incX, movimientoFantasma1.y+incY, sprite, direccion);
                    Colision_Fantasma1(incrementoFantasma1X, incrementoFantasma1Y, direccion, incX);
                     if(incrementoFantasma1X < 16)
                          incrementoFantasma1X++;
                     else
                          incrementoFantasma1X = 0;
                     return 0;
               }
               else if((movimientoFantasma1.x)+1 >= (COLUMNAS-1))
               {
                    Mapa[movimientoFantasma1.y][movimientoFantasma1.x] = ' ';
                    movimientoFantasma1.x = 0;
                    if(Mapa[movimientoFantasma1.y][movimientoFantasma1.x] == 'c')
                    {
                    }
                    Mapa[movimientoFantasma1.y][movimientoFantasma1.x] = 'R'; 
               }
               break;
          case 1: /* Izquierda */
               if(Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)-1] != 'X' && Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)-1] != 'B' && Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)-1] != 'M' && Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)-1] != 'Y' && incrementoFantasma1Y == 0 && (movimientoFantasma1.x) != 0)
               {          
                    incX = (incrementoFantasma1X/16);
                    incY = 0;
                    if(incX == -1)
                            Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)] = almacenarFantasma1;
                            //almacenarFantasma1 = Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)-1];                    
                    Inicializar_Fantasma1(movimientoFantasma1.x + incX, movimientoFantasma1.y+incY, sprite, direccion);
                    Colision_Fantasma1(incrementoFantasma1X, incrementoFantasma1Y, direccion, incX);
                     if(incrementoFantasma1X > -16)
                          incrementoFantasma1X--;
                     else
                          incrementoFantasma1X = 0;
                     return 0;
               }
               else if((movimientoFantasma1.x) == 0)
               {
                    Mapa[movimientoFantasma1.y][movimientoFantasma1.x] = ' ';
                    movimientoFantasma1.x = COLUMNAS-1;
                    if(Mapa[movimientoFantasma1.y][movimientoFantasma1.x] == 'c')
                    {
                    }
                    Mapa[movimientoFantasma1.y][movimientoFantasma1.x] = 'R';                                        
               }                            
               break;
          case 2: /* Arriba */
               if(Mapa[(movimientoFantasma1.y)-1][movimientoFantasma1.x] != 'X' && Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)-1] != 'B' && Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)-1] != 'M' && Mapa[(movimientoFantasma1.y)-1][(movimientoFantasma1.x)] != 'Y' && incrementoFantasma1X == 0 && movimientoFantasma1.y != 0)
               {          
                    incX = 0;
                    incY = (incrementoFantasma1Y/16);
                    if(incY == -1)
                            Mapa[(movimientoFantasma1.y)][(movimientoFantasma1.x)] = almacenarFantasma1;
                            //almacenarFantasma1 = Mapa[(movimientoFantasma1.y)-1][(movimientoFantasma1.x)]; 
                    Inicializar_Fantasma1(movimientoFantasma1.x + incX, movimientoFantasma1.y+incY, sprite, direccion);
                    Colision_Fantasma1(incrementoFantasma1X, incrementoFantasma1Y, direccion, incX);
                     if(incrementoFantasma1Y > -16)
                          incrementoFantasma1Y--;
                     else
                          incrementoFantasma1Y = 0;                    
                     return 0;
               }
               else if((movimientoFantasma1.y) == 0)
               {
                    Mapa[movimientoFantasma1.y][movimientoFantasma1.x] = ' ';
                    movimientoFantasma1.y = FILAS-1;
                    if(Mapa[movimientoFantasma1.y][movimientoFantasma1.x] == 'c')
                    {
                    }
                    Mapa[movimientoFantasma1.y][movimientoFantasma1.x] = 'R';
               }                              
               break;
          case 3: /* Abajo */
               if(Mapa[(movimientoFantasma1.y)+1][movimientoFantasma1.x] != 'X' && Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)-1] != 'B' && Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)-1] != 'M' && Mapa[(movimientoFantasma1.y)+1][(movimientoFantasma1.x)] != 'Y' && incrementoFantasma1X == 0 && (movimientoFantasma1.y)+1 < FILAS-1)
               {              
                    incX = 0;
                    incY = incrementoFantasma1Y/16;
                    if(incY == 1)
                            Mapa[(movimientoFantasma1.y)][(movimientoFantasma1.x)] = almacenarFantasma1;                    
                            //almacenarFantasma1 = Mapa[(movimientoFantasma1.y)+1][(movimientoFantasma1.x)];                     
                    Inicializar_Fantasma1(movimientoFantasma1.x + incX, movimientoFantasma1.y+incY, sprite, direccion);
                    Colision_Fantasma1(incrementoFantasma1X, incrementoFantasma1Y, direccion, incX);
                     if(incrementoFantasma1Y < 16)
                          incrementoFantasma1Y++;
                     else
                          incrementoFantasma1Y = 0;
                     return 0;
               }
               else if((movimientoFantasma1.y)+1 >= (FILAS-1))
               {
                    Mapa[movimientoFantasma1.y][movimientoFantasma1.x] = ' ';
                    movimientoFantasma1.y = 0;
                    if(Mapa[movimientoFantasma1.y][movimientoFantasma1.x] == 'c')
                    {
                    }
                    Mapa[movimientoFantasma1.y][movimientoFantasma1.x] = 'R';                                        
               }               
               break;
     }
     
     return 1;
}

int Mover_Fantasma2(int direccion, int sprite)
{
     int incX = 0, incY = 0;
     
     switch(direccion)
     {
          case 0: /* Derecha */
               if(Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)+1] != 'X' && Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)+1] != 'B' && Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)+1] != 'M' && Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)+1] != 'R' && incrementoFantasma2Y == 0 && ((movimientoFantasma2.x)+1 < (COLUMNAS-1)))
               {
                    incX = incrementoFantasma2X/16;
                    incY = 0;
                    if(incX == 1)
                            Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)] = almacenarFantasma2;
                            //almacenarFantasma2 = Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)+1];

                    Inicializar_Fantasma2(movimientoFantasma2.x + incX, movimientoFantasma2.y+incY, sprite, direccion);
                    Colision_Fantasma2(incrementoFantasma2X, incrementoFantasma2Y, direccion, incX);
                     if(incrementoFantasma2X < 16)
                          incrementoFantasma2X++;
                     else
                          incrementoFantasma2X = 0;
                     return 0;
               }
               else if((movimientoFantasma2.x)+1 >= (COLUMNAS-1))
               {
                    Mapa[movimientoFantasma2.y][movimientoFantasma2.x] = ' ';
                    movimientoFantasma2.x = 0;
                    if(Mapa[movimientoFantasma2.y][movimientoFantasma2.x] == 'c')
                    {
                    }
                    Mapa[movimientoFantasma2.y][movimientoFantasma2.x] = 'Y'; 
               }
               break;
          case 1: /* Izquierda */
               if(Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)-1] != 'X' && Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)-1] != 'B' && Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)-1] != 'M' && Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)-1] != 'R' && incrementoFantasma2Y == 0 && (movimientoFantasma2.x) != 0)
               {          
                    incX = (incrementoFantasma2X/16);
                    incY = 0;
                    if(incX == -1)
                            Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)] = almacenarFantasma2;
                            //almacenarFantasma2 = Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)-1];                    
                    Inicializar_Fantasma2(movimientoFantasma2.x + incX, movimientoFantasma2.y+incY, sprite, direccion);
                    Colision_Fantasma2(incrementoFantasma2X, incrementoFantasma2Y, direccion, incX);
                     if(incrementoFantasma2X > -16)
                          incrementoFantasma2X--;
                     else
                          incrementoFantasma2X = 0;
                     return 0;
               }
               else if((movimientoFantasma2.x) == 0)
               {
                    Mapa[movimientoFantasma2.y][movimientoFantasma2.x] = ' ';
                    movimientoFantasma2.x = COLUMNAS-1;
                    if(Mapa[movimientoFantasma2.y][movimientoFantasma2.x] == 'c')
                    {
                    }
                    Mapa[movimientoFantasma2.y][movimientoFantasma2.x] = 'Y';                                        
               }                            
               break;
          case 2: /* Arriba */
               if(Mapa[(movimientoFantasma2.y)-1][movimientoFantasma2.x] != 'X' && Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)-1] != 'B' && Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)-1] != 'M' && Mapa[(movimientoFantasma2.y)-1][(movimientoFantasma2.x)] != 'R' && incrementoFantasma2X == 0 && movimientoFantasma2.y != 0)
               {          
                    incX = 0;
                    incY = (incrementoFantasma2Y/16);
                    if(incY == -1)
                            Mapa[(movimientoFantasma2.y)][(movimientoFantasma2.x)] = almacenarFantasma2;
                            //almacenarFantasma2 = Mapa[(movimientoFantasma2.y)-1][(movimientoFantasma2.x)]; 
                    Inicializar_Fantasma2(movimientoFantasma2.x + incX, movimientoFantasma2.y+incY, sprite, direccion);
                    Colision_Fantasma2(incrementoFantasma2X, incrementoFantasma2Y, direccion, incX);
                     if(incrementoFantasma2Y > -16)
                          incrementoFantasma2Y--;
                     else
                          incrementoFantasma2Y = 0;                    
                     return 0;
               }
               else if((movimientoFantasma2.y) == 0)
               {
                    Mapa[movimientoFantasma2.y][movimientoFantasma2.x] = ' ';
                    movimientoFantasma2.y = FILAS-1;
                    if(Mapa[movimientoFantasma2.y][movimientoFantasma2.x] == 'c')
                    {
                    }
                    Mapa[movimientoFantasma2.y][movimientoFantasma2.x] = 'Y';
               }                              
               break;
          case 3: /* Abajo */
               if(Mapa[(movimientoFantasma2.y)+1][movimientoFantasma2.x] != 'X' && Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)-1] != 'B' && Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)-1] != 'M' && Mapa[(movimientoFantasma2.y)+1][(movimientoFantasma2.x)] != 'R' && incrementoFantasma2X == 0 && (movimientoFantasma2.y)+1 < FILAS-1)
               {              
                    incX = 0;
                    incY = incrementoFantasma2Y/16;
                    if(incY == 1)
                            Mapa[(movimientoFantasma2.y)][(movimientoFantasma2.x)] = almacenarFantasma2;                    
                            //almacenarFantasma2 = Mapa[(movimientoFantasma2.y)+1][(movimientoFantasma2.x)];                     
                    Inicializar_Fantasma2(movimientoFantasma2.x + incX, movimientoFantasma2.y+incY, sprite, direccion);
                    Colision_Fantasma2(incrementoFantasma2X, incrementoFantasma2Y, direccion, incX);
                     if(incrementoFantasma2Y < 16)
                          incrementoFantasma2Y++;
                     else
                          incrementoFantasma2Y = 0;
                     return 0;
               }
               else if((movimientoFantasma2.y)+1 >= (FILAS-1))
               {
                    Mapa[movimientoFantasma2.y][movimientoFantasma2.x] = ' ';
                    movimientoFantasma2.y = 0;
                    if(Mapa[movimientoFantasma2.y][movimientoFantasma2.x] == 'c')
                    {
                    }
                    Mapa[movimientoFantasma2.y][movimientoFantasma2.x] = 'Y';                                        
               }               
               break;
     }
     
     return 1;
}

int Mover_Fantasma3(int direccion, int sprite)
{
     int incX = 0, incY = 0;
     
     switch(direccion)
     {
          case 0: /* Derecha */
               if(Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)+1] != 'X' && Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)+1] != 'B' && Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)+1] != 'Y' && Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)+1] != 'R' && incrementoFantasma3Y == 0 && ((movimientoFantasma3.x)+1 < (COLUMNAS-1)))
               {
                    incX = incrementoFantasma3X/16;
                    incY = 0;
                    if(incX == 1)
                            Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)] = almacenarFantasma3;
                            //almacenarFantasma3 = Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)+1];

                    Inicializar_Fantasma3(movimientoFantasma3.x + incX, movimientoFantasma3.y+incY, sprite, direccion);
                    Colision_Fantasma3(incrementoFantasma3X, incrementoFantasma3Y, direccion, incX);
                     if(incrementoFantasma3X < 16)
                          incrementoFantasma3X++;
                     else
                          incrementoFantasma3X = 0;
                     return 0;
               }
               else if((movimientoFantasma3.x)+1 >= (COLUMNAS-1))
               {
                    Mapa[movimientoFantasma3.y][movimientoFantasma3.x] = ' ';
                    movimientoFantasma3.x = 0;
                    if(Mapa[movimientoFantasma3.y][movimientoFantasma3.x] == 'c')
                    {
                    }
                    Mapa[movimientoFantasma3.y][movimientoFantasma3.x] = 'M'; 
               }
               break;
          case 1: /* Izquierda */
               if(Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)-1] != 'X' && Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)-1] != 'B' && Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)-1] != 'Y' && Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)-1] != 'R' && incrementoFantasma3Y == 0 && (movimientoFantasma3.x) != 0)
               {          
                    incX = (incrementoFantasma3X/16);
                    incY = 0;
                    if(incX == -1)
                            Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)] = almacenarFantasma3;
                            //almacenarFantasma3 = Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)-1];                    
                    Inicializar_Fantasma3(movimientoFantasma3.x + incX, movimientoFantasma3.y+incY, sprite, direccion);
                    Colision_Fantasma3(incrementoFantasma3X, incrementoFantasma3Y, direccion, incX);
                     if(incrementoFantasma3X > -16)
                          incrementoFantasma3X--;
                     else
                          incrementoFantasma3X = 0;
                     return 0;
               }
               else if((movimientoFantasma3.x) == 0)
               {
                    Mapa[movimientoFantasma3.y][movimientoFantasma3.x] = ' ';
                    movimientoFantasma3.x = COLUMNAS-1;
                    if(Mapa[movimientoFantasma3.y][movimientoFantasma3.x] == 'c')
                    {
                    }
                    Mapa[movimientoFantasma3.y][movimientoFantasma3.x] = 'M';                                        
               }                            
               break;
          case 2: /* Arriba */
               if(Mapa[(movimientoFantasma3.y)-1][movimientoFantasma3.x] != 'X' && Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)-1] != 'B' && Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)-1] != 'Y' && Mapa[(movimientoFantasma3.y)-1][(movimientoFantasma3.x)] != 'R' && incrementoFantasma3X == 0 && movimientoFantasma3.y != 0)
               {          
                    incX = 0;
                    incY = (incrementoFantasma3Y/16);
                    if(incY == -1)
                            Mapa[(movimientoFantasma3.y)][(movimientoFantasma3.x)] = almacenarFantasma3;
                            //almacenarFantasma3 = Mapa[(movimientoFantasma3.y)-1][(movimientoFantasma3.x)]; 
                    Inicializar_Fantasma3(movimientoFantasma3.x + incX, movimientoFantasma3.y+incY, sprite, direccion);
                    Colision_Fantasma3(incrementoFantasma3X, incrementoFantasma3Y, direccion, incX);
                     if(incrementoFantasma3Y > -16)
                          incrementoFantasma3Y--;
                     else
                          incrementoFantasma3Y = 0;                    
                     return 0;
               }
               else if((movimientoFantasma3.y) == 0)
               {
                    Mapa[movimientoFantasma3.y][movimientoFantasma3.x] = ' ';
                    movimientoFantasma3.y = FILAS-1;
                    if(Mapa[movimientoFantasma3.y][movimientoFantasma3.x] == 'c')
                    {
                    }
                    Mapa[movimientoFantasma3.y][movimientoFantasma3.x] = 'M';
               }                              
               break;
          case 3: /* Abajo */
               if(Mapa[(movimientoFantasma3.y)+1][movimientoFantasma3.x] != 'X' && Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)-1] != 'B' && Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)-1] != 'Y' && Mapa[(movimientoFantasma3.y)+1][(movimientoFantasma3.x)] != 'R' && incrementoFantasma3X == 0 && (movimientoFantasma3.y)+1 < FILAS-1)
               {              
                    incX = 0;
                    incY = incrementoFantasma3Y/16;
                    if(incY == 1)
                            Mapa[(movimientoFantasma3.y)][(movimientoFantasma3.x)] = almacenarFantasma3;                    
                            //almacenarFantasma3 = Mapa[(movimientoFantasma3.y)+1][(movimientoFantasma3.x)];                     
                    Inicializar_Fantasma3(movimientoFantasma3.x + incX, movimientoFantasma3.y+incY, sprite, direccion);
                    Colision_Fantasma3(incrementoFantasma3X, incrementoFantasma3Y, direccion, incX);
                     if(incrementoFantasma3Y < 16)
                          incrementoFantasma3Y++;
                     else
                          incrementoFantasma3Y = 0;
                     return 0;
               }
               else if((movimientoFantasma3.y)+1 >= (FILAS-1))
               {
                    Mapa[movimientoFantasma3.y][movimientoFantasma3.x] = ' ';
                    movimientoFantasma3.y = 0;
                    if(Mapa[movimientoFantasma3.y][movimientoFantasma3.x] == 'c')
                    {
                    }
                    Mapa[movimientoFantasma3.y][movimientoFantasma3.x] = 'M';                                        
               }               
               break;
     }
     
     return 1;
}

int Mover_Fantasma4(int direccion, int sprite)
{
     int incX = 0, incY = 0;
     
     switch(direccion)
     {
          case 0: /* Derecha */
               if(Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)+1] != 'X' && Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)+1] != 'Y' && Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)+1] != 'M' && Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)+1] != 'R' && incrementoFantasma4Y == 0 && ((movimientoFantasma4.x)+1 < (COLUMNAS-1)))
               {
                    incX = incrementoFantasma4X/16;
                    incY = 0;
                    if(incX == 1)
                            Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)] = almacenarFantasma4;
                            //almacenarFantasma4 = Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)+1];

                    Inicializar_Fantasma4(movimientoFantasma4.x + incX, movimientoFantasma4.y+incY, sprite, direccion);
                    Colision_Fantasma4(incrementoFantasma4X, incrementoFantasma4Y, direccion, incX);
                     if(incrementoFantasma4X < 16)
                          incrementoFantasma4X++;
                     else
                          incrementoFantasma4X = 0;
                     return 0;
               }
               else if((movimientoFantasma4.x)+1 >= (COLUMNAS-1))
               {
                    Mapa[movimientoFantasma4.y][movimientoFantasma4.x] = ' ';
                    movimientoFantasma4.x = 0;
                    if(Mapa[movimientoFantasma4.y][movimientoFantasma4.x] == 'c')
                    {
                    }
                    Mapa[movimientoFantasma4.y][movimientoFantasma4.x] = 'B'; 
               }
               break;
          case 1: /* Izquierda */
               if(Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)-1] != 'X' && Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)-1] != 'Y' && Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)-1] != 'M' && Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)-1] != 'R' && incrementoFantasma4Y == 0 && (movimientoFantasma4.x) != 0)
               {          
                    incX = (incrementoFantasma4X/16);
                    incY = 0;
                    if(incX == -1)
                            Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)] = almacenarFantasma4;
                            //almacenarFantasma4 = Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)-1];                    
                    Inicializar_Fantasma4(movimientoFantasma4.x + incX, movimientoFantasma4.y+incY, sprite, direccion);
                    Colision_Fantasma4(incrementoFantasma4X, incrementoFantasma4Y, direccion, incX);
                     if(incrementoFantasma4X > -16)
                          incrementoFantasma4X--;
                     else
                          incrementoFantasma4X = 0;
                     return 0;
               }
               else if((movimientoFantasma4.x) == 0)
               {
                    Mapa[movimientoFantasma4.y][movimientoFantasma4.x] = ' ';
                    movimientoFantasma4.x = COLUMNAS-1;
                    if(Mapa[movimientoFantasma4.y][movimientoFantasma4.x] == 'c')
                    {
                    }
                    Mapa[movimientoFantasma4.y][movimientoFantasma4.x] = 'B';                                        
               }                            
               break;
          case 2: /* Arriba */
               if(Mapa[(movimientoFantasma4.y)-1][movimientoFantasma4.x] != 'X' && Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)-1] != 'Y' && Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)-1] != 'M' && Mapa[(movimientoFantasma4.y)-1][(movimientoFantasma4.x)] != 'R' && incrementoFantasma4X == 0 && movimientoFantasma4.y != 0)
               {          
                    incX = 0;
                    incY = (incrementoFantasma4Y/16);
                    if(incY == -1)
                            Mapa[(movimientoFantasma4.y)][(movimientoFantasma4.x)] = almacenarFantasma4;
                            //almacenarFantasma4 = Mapa[(movimientoFantasma4.y)-1][(movimientoFantasma4.x)]; 
                    Inicializar_Fantasma4(movimientoFantasma4.x + incX, movimientoFantasma4.y+incY, sprite, direccion);
                    Colision_Fantasma4(incrementoFantasma4X, incrementoFantasma4Y, direccion, incX);
                     if(incrementoFantasma4Y > -16)
                          incrementoFantasma4Y--;
                     else
                          incrementoFantasma4Y = 0;                    
                     return 0;
               }
               else if((movimientoFantasma4.y) == 0)
               {
                    Mapa[movimientoFantasma4.y][movimientoFantasma4.x] = ' ';
                    movimientoFantasma4.y = FILAS-1;
                    if(Mapa[movimientoFantasma4.y][movimientoFantasma4.x] == 'c')
                    {
                    }
                    Mapa[movimientoFantasma4.y][movimientoFantasma4.x] = 'B';
               }                              
               break;
          case 3: /* Abajo */
               if(Mapa[(movimientoFantasma4.y)+1][movimientoFantasma4.x] != 'X' && Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)-1] != 'Y' && Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)-1] != 'M' && Mapa[(movimientoFantasma4.y)+1][(movimientoFantasma4.x)] != 'R' && incrementoFantasma4X == 0 && (movimientoFantasma4.y)+1 < FILAS-1)
               {              
                    incX = 0;
                    incY = incrementoFantasma4Y/16;
                    if(incY == 1)
                            Mapa[(movimientoFantasma4.y)][(movimientoFantasma4.x)] = almacenarFantasma4;                    
                            //almacenarFantasma4 = Mapa[(movimientoFantasma4.y)+1][(movimientoFantasma4.x)];                     
                    Inicializar_Fantasma4(movimientoFantasma4.x + incX, movimientoFantasma4.y+incY, sprite, direccion);
                    Colision_Fantasma4(incrementoFantasma4X, incrementoFantasma4Y, direccion, incX);
                     if(incrementoFantasma4Y < 16)
                          incrementoFantasma4Y++;
                     else
                          incrementoFantasma4Y = 0;
                     return 0;
               }
               else if((movimientoFantasma4.y)+1 >= (FILAS-1))
               {
                    Mapa[movimientoFantasma4.y][movimientoFantasma4.x] = ' ';
                    movimientoFantasma4.y = 0;
                    if(Mapa[movimientoFantasma4.y][movimientoFantasma4.x] == 'c')
                    {
                    }
                    Mapa[movimientoFantasma4.y][movimientoFantasma4.x] = 'B';                                        
               }               
               break;
     }
     
     return 1;
}

void Dibujar_Mapa(int x, int y, int numeroMapa)
{
     int i, j;
     int auxX = x, auxY = y, auxI, auxJ;
     int auxFantasma1I, auxFantasma1J, auxFantasma2I, auxFantasma2J, auxFantasma3I, auxFantasma3J, auxFantasma4I, auxFantasma4J;
                 rectfill(screen, 0, 0, 600, 600, palette_color[1]);                                                                           
                 for(i = 0; i < FILAS; i++)
                 {
                       for(j = 0; j < COLUMNAS-1; j++)
                       {  
                             if(Mapa[i][j] == 'X')                          
                                   draw_sprite(screen, spriteColumnaMapa[numeroMapa].spriteColumnaMapa, x, y);
                             else if(Mapa[i][j] == 'R')
                             {        
                                   auxFantasma1I = i;
                                   auxFantasma1J = j;
                                   //Inicializar_Fantasma1(x, y, 0, 0);
                             }
                             else if(Mapa[i][j] == 'Y')
                             {
                                   auxFantasma2I = i;
                                   auxFantasma2J = j;
                                   //draw_sprite(screen, spriteFantasma2[0].spriteFantasma2[0].fantasma, x, y);
                                   //Inicializar_Fantasma2(x, y, 0, 0);
                             }
                             else if(Mapa[i][j] == 'M')
                             {
                                   //draw_sprite(screen, spriteFantasma3[0].spriteFantasma3[0].fantasma, x, y);                                    
                                   auxFantasma3I = i;
                                   auxFantasma3J = j;                                   
                                   //Inicializar_Fantasma3(x, y, 0, 0);
                             }
                             else if(Mapa[i][j] == 'B')
                             {
                                   //draw_sprite(screen, spriteFantasma4[0].spriteFantasma4[0].fantasma, x, y);                                   
                                   //Inicializar_Fantasma4(x, y, 0, 0);
                                   auxFantasma4I = i;
                                   auxFantasma4J = j;                                   
                             }
                             else if(Mapa[i][j] == 'Z')
                                   draw_sprite(screen, spriteComida[0].spriteComida, x, y);                                                                
                             else if(Mapa[i][j] == 'W')
                                   draw_sprite(screen, spriteComida[2].spriteComida, x, y);
                             else if(Mapa[i][j] == 'c')
                                   draw_sprite(screen, spriteComida[1].spriteComida, x, y);                                                                
                             else if(Mapa[i][j] == 'P')
                             {              
                                    auxI = i;
                                    auxJ = j;                     
                                   //textprintf(screen, font, 200, 10, palette_color[15], "%d - %c", ((j*16)+incrementoPacmanX)+auxX, Mapa[13][14]);
                                   //Inicializar_Pacman(x, y, 0, 0);
                             }
                             x+=SPRITEX;
                       }
                       x = auxX;
                       y+=SPRITEY;
                 }            
draw_sprite(screen, spriteFantasma1[movimientoFantasma1.direccion].spriteFantasma1[movimientoFantasma1.sprite].fantasma, ((auxFantasma1J*16)+incrementoFantasma1X)+auxX, ((auxFantasma1I*16)+incrementoFantasma1Y)+auxY);                 
draw_sprite(screen, spriteFantasma2[movimientoFantasma2.direccion].spriteFantasma2[movimientoFantasma2.sprite].fantasma, ((auxFantasma2J*16)+incrementoFantasma2X)+auxX, ((auxFantasma2I*16)+incrementoFantasma2Y)+auxY);
draw_sprite(screen, spriteFantasma3[movimientoFantasma3.direccion].spriteFantasma3[movimientoFantasma3.sprite].fantasma, ((auxFantasma3J*16)+incrementoFantasma3X)+auxX, ((auxFantasma3I*16)+incrementoFantasma3Y)+auxY);
draw_sprite(screen, spriteFantasma4[movimientoFantasma4.direccion].spriteFantasma4[movimientoFantasma4.sprite].fantasma, ((auxFantasma4J*16)+incrementoFantasma4X)+auxX, ((auxFantasma4I*16)+incrementoFantasma4Y)+auxY);
draw_sprite(screen, spritePacman[movimientoPacman.direccion].spritePacman[movimientoPacman.sprite].pacman, ((auxJ*16)+incrementoPacmanX)+auxX, ((auxI*16)+incrementoPacmanY)+auxY);                         
}

void Colision_Fantasma1(int incrementoFantasmaX, int incrementoFantasmaY, int direccion, int incX)
{
     //textprintf(screen, font, 10, 10, palette_color[15], "%d,%d - %d - %d - %c",movimientoFantasma1.x,movimientoFantasma1.y, incrementoFantasma1X, incX, Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)]);
     //readkey();     
     if((Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)] == 'Z' || Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)] == 'W' || Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)] == 'c' || Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)] == ' ') && incrementoFantasma1X >= 16 && direccion == 0)
     {
          almacenarFantasma1 = Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)];                                                              
          Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)] = 'R';
          //Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)-1] = almacenarFantasma1;
     }  
     else if((Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)] == 'Z' || Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)] == 'W' || Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)] == 'c' || Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)] == ' ') && incrementoFantasma1X <= -16 && direccion == 1)
     {
          //Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)+1] = almacenarFantasma1;
          almacenarFantasma1 = Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)];
          Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)] = 'R';                    
     } 
     else if((Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)] == 'Z' || Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)] == 'W' || Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)] == 'c' || Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)] == ' ') && incrementoFantasma1Y <= -16 && direccion == 2) //Arriba
     {
          //Mapa[(movimientoFantasma1.y)+1][(movimientoFantasma1.x)] = almacenarFantasma1;
          almacenarFantasma1 = Mapa[(movimientoFantasma1.y)][(movimientoFantasma1.x)];           
          Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)] = 'R';                    
     } 
     else if((Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)] == 'Z' || Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)] == 'W' || Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)] == 'c' || Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)] == ' ') && incrementoFantasma1Y >= 16 && direccion == 3)
     {
          almacenarFantasma1 = Mapa[(movimientoFantasma1.y)][(movimientoFantasma1.x)];                               
          Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)] = 'R';
          //Mapa[(movimientoFantasma1.y)-1][(movimientoFantasma1.x)] = almacenarFantasma1;
     }             
     
     //textprintf(screen, font, 300, 10, palette_color[15], "%c - %c - %c",Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)], Mapa[movimientoFantasma1.y][(movimientoFantasma1.x)+1], Mapa[13][13]);
     //readkey();                
}

void Colision_Fantasma2(int incrementoFantasmaX, int incrementoFantasmaY, int direccion, int incX)
{
     //textprintf(screen, font, 10, 10, palette_color[15], "%d,%d - %d - %d - %c",movimientoFantasma2.x,movimientoFantasma2.y, incrementoFantasma2X, incX, Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)]);
     //readkey();     
     if((Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)] == 'Z' || Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)] == 'W' || Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)] == 'c' || Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)] == ' ') && incrementoFantasma2X >= 16 && direccion == 0)
     {
          almacenarFantasma2 = Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)];                                                              
          Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)] = 'Y';
          //Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)-1] = almacenarFantasma2;
     }  
     else if((Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)] == 'Z' || Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)] == 'W' || Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)] == 'c' || Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)] == ' ') && incrementoFantasma2X <= -16 && direccion == 1)
     {
          //Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)+1] = almacenarFantasma2;
          almacenarFantasma2 = Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)];
          Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)] = 'Y';                    
     } 
     else if((Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)] == 'Z' || Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)] == 'W' || Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)] == 'c' || Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)] == ' ') && incrementoFantasma2Y <= -16 && direccion == 2) //Arriba
     {
          //Mapa[(movimientoFantasma2.y)+1][(movimientoFantasma2.x)] = almacenarFantasma2;
          almacenarFantasma2 = Mapa[(movimientoFantasma2.y)][(movimientoFantasma2.x)];           
          Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)] = 'Y';                    
     } 
     else if((Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)] == 'Z' || Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)] == 'W' || Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)] == 'c' || Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)] == ' ') && incrementoFantasma2Y >= 16 && direccion == 3)
     {
          almacenarFantasma2 = Mapa[(movimientoFantasma2.y)][(movimientoFantasma2.x)];                               
          Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)] = 'Y';
          //Mapa[(movimientoFantasma2.y)-1][(movimientoFantasma2.x)] = almacenarFantasma2;
     }             
     
     //textprintf(screen, font, 300, 10, palette_color[15], "%c - %c - %c",Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)], Mapa[movimientoFantasma2.y][(movimientoFantasma2.x)+1], Mapa[13][13]);
     //readkey();                
}

void Colision_Fantasma3(int incrementoFantasmaX, int incrementoFantasmaY, int direccion, int incX)
{
     //textprintf(screen, font, 10, 10, palette_color[15], "%d,%d - %d - %d - %c",movimientoFantasma3.x,movimientoFantasma3.y, incrementoFantasma3X, incX, Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)]);
     //readkey();     
     if((Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)] == 'Z' || Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)] == 'W' || Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)] == 'c' || Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)] == ' ') && incrementoFantasma3X >= 16 && direccion == 0)
     {
          almacenarFantasma3 = Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)];                                                              
          Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)] = 'M';
          //Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)-1] = almacenarFantasma3;
     }  
     else if((Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)] == 'Z' || Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)] == 'W' || Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)] == 'c' || Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)] == ' ') && incrementoFantasma3X <= -16 && direccion == 1)
     {
          //Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)+1] = almacenarFantasma3;
          almacenarFantasma3 = Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)];
          Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)] = 'M';                    
     } 
     else if((Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)] == 'Z' || Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)] == 'W' || Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)] == 'c' || Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)] == ' ') && incrementoFantasma3Y <= -16 && direccion == 2) //Arriba
     {
          //Mapa[(movimientoFantasma3.y)+1][(movimientoFantasma3.x)] = almacenarFantasma3;
          almacenarFantasma3 = Mapa[(movimientoFantasma3.y)][(movimientoFantasma3.x)];           
          Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)] = 'M';                    
     } 
     else if((Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)] == 'Z' || Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)] == 'W' || Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)] == 'c' || Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)] == ' ') && incrementoFantasma3Y >= 16 && direccion == 3)
     {
          almacenarFantasma3 = Mapa[(movimientoFantasma3.y)][(movimientoFantasma3.x)];                               
          Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)] = 'M';
          //Mapa[(movimientoFantasma3.y)-1][(movimientoFantasma3.x)] = almacenarFantasma3;
     }             
     
     //textprintf(screen, font, 300, 10, palette_color[15], "%c - %c - %c",Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)], Mapa[movimientoFantasma3.y][(movimientoFantasma3.x)+1], Mapa[13][13]);
     //readkey();                
}

void Colision_Fantasma4(int incrementoFantasmaX, int incrementoFantasmaY, int direccion, int incX)
{
     //textprintf(screen, font, 10, 10, palette_color[15], "%d,%d - %d - %d - %c",movimientoFantasma4.x,movimientoFantasma4.y, incrementoFantasma4X, incX, Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)]);
     //readkey();     
     if((Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)] == 'Z' || Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)] == 'W' || Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)] == 'c' || Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)] == ' ') && incrementoFantasma4X >= 16 && direccion == 0)
     {
          almacenarFantasma4 = Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)];                                                              
          Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)] = 'B';
          //Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)-1] = almacenarFantasma4;
     }  
     else if((Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)] == 'Z' || Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)] == 'W' || Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)] == 'c' || Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)] == ' ') && incrementoFantasma4X <= -16 && direccion == 1)
     {
          //Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)+1] = almacenarFantasma4;
          almacenarFantasma4 = Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)];
          Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)] = 'B';                    
     } 
     else if((Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)] == 'Z' || Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)] == 'W' || Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)] == 'c' || Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)] == ' ') && incrementoFantasma4Y <= -16 && direccion == 2) //Arriba
     {
          //Mapa[(movimientoFantasma4.y)+1][(movimientoFantasma4.x)] = almacenarFantasma4;
          almacenarFantasma4 = Mapa[(movimientoFantasma4.y)][(movimientoFantasma4.x)];           
          Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)] = 'B';                    
     } 
     else if((Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)] == 'Z' || Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)] == 'W' || Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)] == 'c' || Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)] == ' ') && incrementoFantasma4Y >= 16 && direccion == 3)
     {
          almacenarFantasma4 = Mapa[(movimientoFantasma4.y)][(movimientoFantasma4.x)];                               
          Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)] = 'B';
          //Mapa[(movimientoFantasma4.y)-1][(movimientoFantasma4.x)] = almacenarFantasma4;
     }             
     
     //textprintf(screen, font, 300, 10, palette_color[15], "%c - %c - %c",Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)], Mapa[movimientoFantasma4.y][(movimientoFantasma4.x)+1], Mapa[13][13]);
     //readkey();                
}

void Colision(int incrementoPacmanX, int incrementoPacmanY, int direccion, int incX)
{
     //textprintf(screen, font, 10, 10, palette_color[15], "%d,%d - %d - %d - %c",movimientoPacman.x,movimientoPacman.y, incrementoPacmanX, incX, Mapa[movimientoPacman.y][(movimientoPacman.x)]);
     //readkey();     
     
     /* Colisiones de pacman con la comida o la comida super */
     if((Mapa[movimientoPacman.y][(movimientoPacman.x)] == 'Z' || Mapa[movimientoPacman.y][(movimientoPacman.x)] == 'W' || Mapa[movimientoPacman.y][(movimientoPacman.x)] == 'c' || Mapa[movimientoPacman.y][(movimientoPacman.x)] == ' ') && incrementoPacmanX >= 16 && direccion == 0)
     {
          Mapa[movimientoPacman.y][(movimientoPacman.x)] = 'P';                    
          Mapa[movimientoPacman.y][(movimientoPacman.x)-1] = ' ';
     }  
     else if((Mapa[movimientoPacman.y][(movimientoPacman.x)] == 'Z' || Mapa[movimientoPacman.y][(movimientoPacman.x)] == 'W' || Mapa[movimientoPacman.y][(movimientoPacman.x)] == 'c' || Mapa[movimientoPacman.y][(movimientoPacman.x)] == ' ') && incrementoPacmanX <= -16 && direccion == 1)
     {
          Mapa[movimientoPacman.y][(movimientoPacman.x)+1] = ' ';
          Mapa[movimientoPacman.y][(movimientoPacman.x)] = 'P';
     } 
     else if((Mapa[movimientoPacman.y][(movimientoPacman.x)] == 'Z' || Mapa[movimientoPacman.y][(movimientoPacman.x)] == 'W' || Mapa[movimientoPacman.y][(movimientoPacman.x)] == 'c' || Mapa[movimientoPacman.y][(movimientoPacman.x)] == ' ') && incrementoPacmanY <= -16 && direccion == 2) //Arriba
     {
          Mapa[(movimientoPacman.y)+1][(movimientoPacman.x)] = ' ';
          Mapa[movimientoPacman.y][(movimientoPacman.x)] = 'P';
     } 
     else if((Mapa[movimientoPacman.y][(movimientoPacman.x)] == 'Z' || Mapa[movimientoPacman.y][(movimientoPacman.x)] == 'W' || Mapa[movimientoPacman.y][(movimientoPacman.x)] == 'c' || Mapa[movimientoPacman.y][(movimientoPacman.x)] == ' ') && incrementoPacmanY >= 16 && direccion == 3)
     {
          Mapa[movimientoPacman.y][(movimientoPacman.x)] = 'P';
          Mapa[(movimientoPacman.y)-1][(movimientoPacman.x)] = ' ';
     }
     /* Colisiones de pacman con los fantasmas */
     else if((Mapa[movimientoPacman.y][(movimientoPacman.x)+1] == 'R' || Mapa[movimientoPacman.y][(movimientoPacman.x)+1] == 'M' || Mapa[movimientoPacman.y][(movimientoPacman.x)+1] == 'B' || Mapa[movimientoPacman.y][(movimientoPacman.x)+1] == 'Y') && (incrementoPacmanX == 2) && direccion == 0)
     {
          readkey();
          exit(1);
     }
     else if((Mapa[movimientoPacman.y][(movimientoPacman.x)-1] == 'R' || Mapa[movimientoPacman.y][(movimientoPacman.x)-1] == 'M' || Mapa[movimientoPacman.y][(movimientoPacman.x)-1] == 'B' || Mapa[movimientoPacman.y][(movimientoPacman.x)-1] == 'Y') && (incrementoPacmanX == -2) && direccion == 1)
     {
          readkey();
          exit(1);
     }     
     else if((Mapa[(movimientoPacman.y)-1][(movimientoPacman.x)] == 'R' || Mapa[(movimientoPacman.y)-1][(movimientoPacman.x)] == 'M' || Mapa[(movimientoPacman.y)-1][(movimientoPacman.x)] == 'B' || Mapa[(movimientoPacman.y)-1][(movimientoPacman.x)] == 'Y') && (incrementoPacmanY == -2) && direccion == 2)
     {
          readkey();
          exit(1);
     }     
     else if((Mapa[(movimientoPacman.y)+1][(movimientoPacman.x)] == 'R' || Mapa[(movimientoPacman.y)+1][(movimientoPacman.x)+1] == 'M' || Mapa[(movimientoPacman.y)+1][(movimientoPacman.x)+1] == 'B' || Mapa[(movimientoPacman.y)+1][(movimientoPacman.x)+1] == 'Y') && (incrementoPacmanY == 2) && direccion == 3)
     {
          readkey();
          exit(1);
     }          
     //textprintf(screen, font, 300, 10, palette_color[15], "%c - %c - %c",Mapa[movimientoPacman.y][(movimientoPacman.x)], Mapa[movimientoPacman.y][(movimientoPacman.x)+1], Mapa[13][13]);
     //readkey();               
}

void Cambiar_Color_Super_Comida()
{
     int i, j;
     
     for(i = 0; i < FILAS; i++)     
     {
           for(j = 0; j < COLUMNAS; j++)           
           {
                 if(Mapa[i][j] == 'Z')                 
                      Mapa[i][j] = 'W';     
                 else if(Mapa[i][j] == 'W')
                      Mapa[i][j] = 'Z';
           }
     }
}

int main()
{             
      BITMAP *imagen;
      PALETTE palette;
      int generar = 0, x = 20, y = 50, numeroMapa = 1, direccion = 0, direccionAnterior = 0, nuevaTecla = 0, salida = 0;
      int direccionAnteriorFantasma1 = 0, direccionFantasma1 = 0, direccionAnteriorFantasma2 = 0, direccionFantasma2 = 0;
      int direccionAnteriorFantasma3 = 0, direccionFantasma3 = 0, direccionAnteriorFantasma4 = 0, direccionFantasma4 = 0;      
      char tecla;
              
      Inicializar_Variables();
      Inicializar_Funciones();
      Inicializar_Graficos(600, 600);
      
      /* Creamos los sprites del juego */
      if(Crear_Sprites(imagen, palette, "spritePacman.bmp") != 0)
          exit(1);
          
      Cargar_Mapa(numeroMapa, x, y);
      /* Inicializamos las coordenadas de pacman */
      //Inicializar_Pacman(50, 50, 0, 0);
      
      /* Inicializamos las coordenadas de los fantasmas */
      //Inicializar_Fantasmas(50, 50, 0, 3);
          
      
      do
      {
           if(keypressed())
           {
                tecla = readkey()>>8;                
                direccionAnterior = direccion;
                if(tecla == KEY_RIGHT)
                     direccion = 0;
                else if(tecla == KEY_LEFT)      
                     direccion = 1;
                else if(tecla == KEY_UP)
                     direccion = 2;
                else if(tecla == KEY_DOWN)
                     direccion = 3;
                else if(tecla == KEY_ESC)
                     salida = 1;
                clear_keybuf();
           }
           
           if(contadorPacmanSprite >= CAMBIOPACMANSPRITE)
           {
                if(movimientoPacman.sprite < 2)
                     movimientoPacman.sprite = movimientoPacman.sprite + 1;
                else
                     movimientoPacman.sprite = 0;
                contadorPacmanSprite = 0;
           }
           if(contadorFantasma1Sprite >= CAMBIOFANTASMASPRITE)
           {
                if(movimientoFantasma1.sprite < 1)
                     movimientoFantasma1.sprite = movimientoFantasma1.sprite + 1;
                else
                     movimientoFantasma1.sprite = 0;
                     
                if(movimientoFantasma2.sprite < 1)
                     movimientoFantasma2.sprite = movimientoFantasma2.sprite + 1;
                else
                     movimientoFantasma2.sprite = 0;
                     
                if(movimientoFantasma3.sprite < 1)
                     movimientoFantasma3.sprite = movimientoFantasma3.sprite + 1;
                else
                     movimientoFantasma3.sprite = 0;
                     
                if(movimientoFantasma4.sprite < 1)
                     movimientoFantasma4.sprite = movimientoFantasma4.sprite + 1;
                else
                     movimientoFantasma4.sprite = 0;
                                                               
                contadorFantasma1Sprite = 0;
           }           
           if(contadorPacmanMovimiento >= CAMBIOPACMANMOVIMIENTO)
           {
                if(Mover_Pacman(direccion, movimientoPacman.sprite) == 1)
                     Mover_Pacman(direccionAnterior, movimientoPacman.sprite);
                else
                     direccionAnterior = direccion;
                contadorPacmanMovimiento = 0;
           }
           if(contadorFantasma1Movimiento >= CAMBIOFANTASMAMOVIMIENTO)
           {
                if(Mover_Fantasma1(direccionFantasma1, movimientoFantasma1.sprite) == 1)
                {
                     Mover_Fantasma1(direccionAnteriorFantasma1, movimientoFantasma1.sprite);
                     generar = 1;
                }
                else
                     direccionAnteriorFantasma1 = direccionFantasma1;
                     
                if(Mover_Fantasma2(direccionFantasma2, movimientoFantasma2.sprite) == 1)
                {
                     Mover_Fantasma1(direccionAnteriorFantasma2, movimientoFantasma2.sprite);
                     generar = 1;
                }
                else
                     direccionAnteriorFantasma2 = direccionFantasma2;                     

                if(Mover_Fantasma3(direccionFantasma3, movimientoFantasma3.sprite) == 1)
                {
                     Mover_Fantasma3(direccionAnteriorFantasma3, movimientoFantasma3.sprite);
                     generar = 1;
                }
                else
                     direccionAnteriorFantasma3 = direccionFantasma3;
                     
                if(Mover_Fantasma4(direccionFantasma4, movimientoFantasma4.sprite) == 1)
                {
                     Mover_Fantasma4(direccionAnteriorFantasma4, movimientoFantasma4.sprite);
                     generar = 1;
                }
                else
                     direccionAnteriorFantasma4 = direccionFantasma4;                      
                contadorFantasma1Movimiento = 0;
           }
           if(contadorComidaCambio >= CAMBIOCOMIDA)
           {
                Cambiar_Color_Super_Comida();
                contadorComidaCambio = 0;
           }
           
           if(contadorCambioDireccionFantasma1 >= CAMBIODIRECCION && generar == 1)
           {
                direccionAnteriorFantasma1 = direccionFantasma1;
                direccionFantasma1 = rand()%4;                
                direccionAnteriorFantasma2 = direccionFantasma2;
                direccionFantasma2 = rand()%4;    
                direccionAnteriorFantasma3 = direccionFantasma3;
                direccionFantasma3 = rand()%4;                
                direccionAnteriorFantasma4 = direccionFantasma4;
                direccionFantasma4 = rand()%4;                                              
                contadorCambioDireccionFantasma1 = 0;              
                generar = 0;  
           }
           
           if(abrirPuerta >= TIEMPOABRIR)
           {
                Mapa[10][12] = ' ';
                Mapa[10][13] = ' ';
                Mapa[10][14] = ' ';
                Mapa[10][15] = ' ';                                
                abrirPuerta = 0;
           }
           vsync();
           clear_bitmap(screen);
           Dibujar_Mapa(x, y, numeroMapa);
                //draw_sprite(screen, spritePacman[movimientoPacman.direccion].spritePacman[movimientoPacman.sprite].pacman, movimientoPacman.x, movimientoPacman.y);                      
      }while(salida == 0);
      
      readkey();
      Destruir_Sprites(imagen);
      return 1;
}
END_OF_MAIN();
