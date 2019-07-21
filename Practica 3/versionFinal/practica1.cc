//**************************************************************************
// Práctica 1
//
// Domingo Martin Perandres 2013
//
// GPL
//**************************************************************************

#include "stdlib.h"
#include "stdio.h"
#include <GL/glut.h>
#include <ctype.h>
#include "user_code.h"
#include "grua.h"
#include "file_ply_stl.h"
#include <iostream>



using namespace std;

solido *cubo;
solido *piramide;
solido *esfera;
solido *ply;
solido *revolucion;
solido *general;

base *baseG;
brazo *brazoG;
gancho *ganchoG;
grua *gruaG;

//para eventos
GLenum modo=GL_FILL;
bool ajedrez=0, borde=1, normal=0,draw_grua=1;
float width=3.0;
float TAM_CUBO=0.4;
vector<color> paleta;
color c_actual;
int p;

//grados de libertad
int g1=0;
float g2=0,g3=0;

// tamaño de los ejes
const int AXIS_SIZE=5000;

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Window_width,Window_height,Front_plane,Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int UI_window_pos_x=50,UI_window_pos_y=50,UI_window_width=500,UI_window_height=500;

//**************************************************************************
//
//***************************************************************************

void clear_window()
{

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

void change_projection()
{

glMatrixMode(GL_PROJECTION);
glLoadIdentity();

// formato(x_minimo,x_maximo, y_minimo, y_maximo,Front_plane, plano_traser)
//  Front_plane>0  Back_plane>PlanoDelantero)
glFrustum(-Window_width,Window_width,-Window_height,Window_height,Front_plane,Back_plane);
}

//**************************************************************************
// Funcion para definir la transformación de vista (posicionar la camara)
//***************************************************************************

void change_observer()
{

// posicion del observador
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glTranslatef(0,0,-Observer_distance);
glRotatef(Observer_angle_x,1,0,0);
glRotatef(Observer_angle_y,0,1,0);
}

//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis()
{
glBegin(GL_LINES);
// eje X, color rojo
glColor3f(1,0,0);
glVertex3f(-AXIS_SIZE,0,0);
glVertex3f(AXIS_SIZE,0,0);
// eje Y, color verde
glColor3f(0,1,0);
glVertex3f(0,-AXIS_SIZE,0);
glVertex3f(0,AXIS_SIZE,0);
// eje Z, color azul
glColor3f(0,0,1);
glVertex3f(0,0,-AXIS_SIZE);
glVertex3f(0,0,AXIS_SIZE);
glEnd();
}


//**************************************************************************
// Funcion que dibuja los objetos
//***************************************************************************

void draw_objects(){
  
  if(!draw_grua){
    general->draw(modo,c_actual,ajedrez,borde,color(0,1,1),width);
    if(normal)
      general->drawNormalVertice(0.3,color(0,0,0));
  }else{
    //brazoG->draw(g2,g3,modo,c_actual,ajedrez,borde,width);
    //ganchoG->draw(g3,modo,ajedrez,borde);
    //baseG->draw();,GLenum modo,color c_brazo,bool ajedrez, int borde, float ancho
    gruaG->draw(g1,g2,g3,modo,c_actual,ajedrez,borde,width);
  }
}



//**************************************************************************
//
//***************************************************************************

void draw_scene(void)
{

clear_window();
change_observer();
draw_axis();
draw_objects();
glutSwapBuffers();
}



//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion:
// nuevo ancho
// nuevo alto
//***************************************************************************

void change_window_size(int Ancho1,int Alto1)
{
change_projection();
glViewport(0,0,Ancho1,Alto1);
glutPostRedisplay();
}


//***************************************************************************
// Funcion llamada cuando se produce aprieta una tecla normal
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton
//***************************************************************************

void normal_keys(unsigned char Tecla1,int x,int y)
{

if (toupper(Tecla1)=='Q') exit(0);
if (Tecla1=='p' || Tecla1=='P') {modo = GL_POINT;/*cout<<"BREAK 1"<<endl;//*/}
if (Tecla1=='l' || Tecla1=='L') {modo = GL_LINE;}
if (Tecla1=='s' || Tecla1=='S') {modo = GL_FILL;}
if (Tecla1=='a' || Tecla1=='A') {ajedrez = (ajedrez+1)%2;/*cout<<"BREAK 2"<<endl;//*/}
if (Tecla1=='b' || Tecla1=='B') {borde = (borde+1)%2;/*cout<<"BREAK 2"<<endl//*/;}
if (Tecla1=='n') {normal = (normal+1)%2;}
if (Tecla1=='N') {general->crearNormales();}
if (Tecla1=='o') {p = (p+1)%paleta.size(); c_actual=paleta[p];}
if (Tecla1=='x') {g2+=0.05;}
if (Tecla1=='X') {g2-=0.05;}
if (Tecla1=='z') {g1=(g1+2)%360;}
if (Tecla1=='Z') {g1=(g1-2)%360;}
if (Tecla1=='c') {g3+=0.05;}
if (Tecla1=='C') {g3-=0.05;}
if (Tecla1=='w') {width++;}
if (Tecla1=='W') {width--;}
if (Tecla1=='1') {general = ply;draw_grua=0;}
if (Tecla1=='2') {general = revolucion;draw_grua=0;}
if (Tecla1=='3') {draw_grua=1;}
if (Tecla1=='4') {general = esfera;draw_grua=0;}
if (Tecla1=='5') {general = cubo;draw_grua=0;}
if (Tecla1=='6') {general = piramide;draw_grua=0;}
glutPostRedisplay();
}

//***************************************************************************
// Funcion llamada cuando se produce aprieta una tecla especial
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton

//***************************************************************************

void special_keys(int Tecla1,int x,int y)
{

switch (Tecla1){
	case GLUT_KEY_LEFT:Observer_angle_y--;break;
	case GLUT_KEY_RIGHT:Observer_angle_y++;break;
	case GLUT_KEY_UP:Observer_angle_x--;break;
	case GLUT_KEY_DOWN:Observer_angle_x++;break;
	case GLUT_KEY_PAGE_UP:Observer_distance*=1.2;break;
	case GLUT_KEY_PAGE_DOWN:Observer_distance/=1.2;break;
	}
glutPostRedisplay();
}



//***************************************************************************
// Funcion de incializacion
//***************************************************************************

void initialize(void)
{
// se inicalizan la ventana y los planos de corte
Window_width=.5;
Window_height=.5;
Front_plane=1;
Back_plane=1000;

// se inicia la posicion del observador, en el eje z
Observer_distance=3*Front_plane;
Observer_angle_x=0;
Observer_angle_y=0;

// se indica cual sera el color para limpiar la ventana	(r,v,a,al)
// blanco=(1,1,1,1) rojo=(1,0,0,1), ...
glClearColor(1,1,1,1);

// se habilita el z-bufer
glEnable(GL_DEPTH_TEST);
change_projection();
glViewport(0,0,UI_window_width,UI_window_height);
}


//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************
int main(int argc, char **argv)
{
    cubo=new solido();
    piramide=new solido();
    esfera=new solido();
    
    baseG=new base(ANCHO_BASE,ALTO_BASE,PROFUNDIDAD_BASE);
    brazoG=new brazo(ANCHO_BRAZO,ALTO_BRAZO,PROFUNDIDAD_BRAZO);
    ganchoG=new gancho(ANCHO_GANCHO,ALTO_GANCHO,PROFUNDIDAD_GANCHO);
    gruaG=new grua(ANCHO,ALTO,PROFUNDIDAD);
    
    construir_cubo(1,1,1/*TAM_CUBO,TAM_CUBO,TAM_CUBO*/,cubo);
    construir_piramide(1/*TAM_CUBO*/,piramide);
    construir_esfera(1/*0.6*/,20,esfera);
    
    
    
    paleta.push_back(color(0,0,255));
    paleta.push_back(color(0,0,0));
    paleta.push_back(color(255,0,0));
    paleta.push_back(color(0,255,0));  
    paleta.push_back(color(255,255,0));
    paleta.push_back(color(0,255,255));
    paleta.push_back(color(255,0,255));
    paleta.push_back(color(255,255,255));
    
    c_actual=paleta[0];
    
    _file_ply fichero;
    fichero.open("perfil.ply");
    
    vector<float> vertices;
    vector<int> caras;
    
    fichero.read(vertices,caras);
    
    revolucion = new solido(); // Para revolucion
    
    fichero.close();
    
    vector<vertice> vv= toVectorVertices(vertices);
    revolucion->crearRevolucion(12,vv);
    
    fichero.open("beethoven.ply");
    
    fichero.read(vertices,caras);
    
    ply = new solido(vertices,caras); // Sin revolucion
    
    fichero.close();
    
    general=revolucion;    
    
    // se llama a la inicialización de glut
    glutInit(&argc, argv);

    // se indica las caracteristicas que se desean para la visualización con OpenGL
    // Las posibilidades son:
    // GLUT_SIMPLE -> memoria de imagen simple
    // GLUT_DOUBLE -> memoria de imagen doble
    // GLUT_INDEX -> memoria de imagen con color indizado
    // GLUT_RGB -> memoria de imagen con componentes rojo, verde y azul para cada pixel
    // GLUT_RGBA -> memoria de imagen con componentes rojo, verde, azul y alfa para cada pixel
    // GLUT_DEPTH -> memoria de profundidad o z-bufer
    // GLUT_STENCIL -> memoria de estarcido
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    // posicion de la esquina inferior izquierdad de la ventana
    glutInitWindowPosition(UI_window_pos_x,UI_window_pos_y);

    // tamaño de la ventana (ancho y alto)
    glutInitWindowSize(UI_window_width,UI_window_height);

    // llamada para crear la ventana, indicando el titulo (no se visualiza hasta que se llama
    // al bucle de eventos)
    glutCreateWindow("Práctica 1");
     // asignación de la funcion llamada "dibujar" al evento de dibujo
    glutDisplayFunc(draw_scene);
    // asignación de la funcion llamada "cambiar_tamanio_ventana" al evento correspondiente
    glutReshapeFunc(change_window_size);
    // asignación de la funcion llamada "tecla_normal" al evento correspondiente
    glutKeyboardFunc(normal_keys);
    // asignación de la funcion llamada "tecla_Especial" al evento correspondiente
    glutSpecialFunc(special_keys);

    // funcion de inicialización
    initialize();

    // inicio del bucle de eventos
    glutMainLoop();

    return 0;
}
