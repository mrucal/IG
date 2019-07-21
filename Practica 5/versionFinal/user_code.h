//**************************************************************************
// Práctica 1
//
// Domingo Martin Perandres 2013
//
// GPL
//**************************************************************************

#ifndef USER_CODE_
#define USER_CODE_

#include <GL/gl.h>
#include <vector>
#include <cmath>
#include <assert.h>
#include <iostream>

using namespace std;

class camara{
private:
  bool tipo;
  GLfloat x_max,y_max,x_min,y_min,front,back,Observer_angle_x,Observer_angle_y,Observer_distance;
  
//   static int n;
  
public:
  camara(){
    tipo=0;//Perspectiva
    x_max,y_max,x_min,y_min,front,back,Observer_angle_x,Observer_angle_y,Observer_distance=0.0;
    Observer_distance=1.0;
//     n++;
  }
  
  camara(bool tipo, GLfloat x_max, GLfloat y_max, GLfloat x_min, GLfloat y_min, GLfloat front, GLfloat back){
    this->tipo=tipo;
    this->x_max=x_max;
    this->y_max=y_max;
    this->x_min=x_min;
    this->y_min=y_min;
    this->front=front;
    this->back=back;
    Observer_angle_y,Observer_angle_x=0.0;
    Observer_distance=1.0;
//     n++;
  }
  
  void Proyeccion(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(!tipo){
      glOrtho(x_max,y_max,x_min,y_min,front,back);
    }else{
      glFrustum(x_max,y_max,x_min,y_min,front,back);
    }
    
  }
  
  void change_observer(bool x, bool y, bool d){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // posicion del observador
    if(d)
      glTranslatef(0,0,-Observer_distance);
    if(x)
      glRotatef(Observer_angle_x,1,0,0);
    if(y)
      glRotatef(Observer_angle_y,0,1,0);
  }
  
  void setObserver_distance(GLfloat Observer_distance){
    this->Observer_distance=Observer_distance;
  }
  
  void setObserver_angle_y(GLfloat Observer_angle_y){
    this->Observer_angle_y=Observer_angle_y;
  }
  
  void setObserver_angle_x(GLfloat Observer_angle_x){
    this->Observer_angle_x=Observer_angle_x;
  }
  
  GLfloat getObserver_distance(){
    return Observer_distance;
  }
  
  GLfloat getObserver_angle_x(){
    return Observer_angle_x;
  }
  
  GLfloat getObserver_angle_y(){
    return Observer_angle_y;
  }

  /*
  int getNumCamaras(){
    return n;
  }*/
};

/*********************************************************************************/

class color{
private:
  
  float r,g,b;
  
public:
  
  color(){
    r,g,b=0;
  }
  
  color(float r, float g, float b){
    this->r=r;
    this->g=g;
    this->b=b;
  }
  
  ~color(){}
  
  float getR(){
    return r;
  }
  
  float getG(){
    return g;
  }
  
  float getB(){
    return b;
  }
  
  void setR(float r){
    this->r=r;
  }
  
  void setG(float g){
    this->g=g;
  }
  
  void setB(float b){
    this->b=b;
  }
  
  void setColor(float r, float g, float b){
    this->r=r;
    this->g=g;
    this->b=b;
  }
  
};

/*********************************************************************************/

class vertice{
  
private:
  
  vector<float> coord;
  color c;
  int ps;
  
public:
  
  vertice(){
    vector<float> aux(3,0);
    this->coord=aux;
    c.setColor(0,0,0);
    ps=5;
  }
  
  vertice(float x, float y, float z){
    coord.push_back(x);
    coord.push_back(y);
    coord.push_back(z);
    c.setColor(0,0,0);
    ps=5;
  }
  
  bool operator==(vertice v){
    return v.getCoord(0)==this->getCoord(0) && v.getCoord(1)==this->getCoord(1) && v.getCoord(2)==this->getCoord(2);
  }
  
  ~vertice(){
    coord.clear();
  }
  
  /*vector<float> getCoord(){
    return coord;
  }*/
  
  float * getCoord(){
    return &coord[0];
  }
  
  float getCoord(int i) const{
    assert(i<3 && i>=0);
    return coord[i];
  }
  
  color getColor(){
    return c;
  }
  
  int getPS(){
    return ps;
  }
  
  void setPS(int ps){
    this->ps=ps;
  }
  
  void setColor(float r, float g, float b){
    c.setColor(r,g,b);
  }
  
  void setCoord(float x, float y, float z){
    coord[0]=x; coord[1]=y; coord[2]=z;
  }
  
  void setCoord(int i, float x){
    assert(i<3 && i>=0);
    coord[i]=x;
  }
  
  void draw(){
    
    glColor3f(c.getR(),c.getG(),c.getB());
    glPointSize(ps);

    glBegin(GL_POINTS);
      glVertex3fv(&coord[0]);
    glEnd();
  }
  
};

/*********************************************************************************/

class cara{
  
private:
  
  vector<int> indices;
  
public:
  
  cara(){
    vector<int> aux(3,0);
    this->indices=aux;
  }
  
  cara(int i1, int i2, int i3){
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
  }
  
  ~cara(){
    indices.clear();
  }
  
  vector<int> getIndices(){
    return indices;
  }
  
  int getIndice(int i){
    assert(i<3 && i>=0);
    return indices[i];
  }
  
  void setIndices(int i1, int i2, int i3){
    indices[0]=i1; indices[1]=i2; indices[2]=i3;
  }
  
  void setIndice(int i, int v){
    assert(i<3 && i>=0);
    indices[i]=v;
  }
  
};

/*********************************************************************************/

class arista{
  
private:
  
  vector<int> indices;
  
public:
  
  arista(){
    vector<int> aux(2,0);
    this->indices=aux;
  }
  
  arista(int i1, int i2){
    indices.push_back(i1);
    indices.push_back(i2);
  }
  
  ~arista(){
    indices.clear();
  }
  
  vector<int> getIndices(){
    return indices;
  }
  
  int getIndice(int i){
    assert(i<2 && i>=0);
    return indices[i];
  }
  
  void setIndices(int i1, int i2){
    indices[0]=i1; indices[1]=i2; 
  }
  
  void setIndice(int i, int v){
    assert(i<2 && i>=0);
    indices[i]=v;
  }
  
};

/*********************************************************************************/

class normal{
  
private:
  vector<float> coord;

public:
  normal(){
    vector<float> aux(3,0);
    this->coord=aux;
  }
  
  normal(float nx, float ny, float nz){
    coord.push_back(nx);
    coord.push_back(ny);
    coord.push_back(nz);   
  }
  
  ~normal(){
    coord.clear();
  }
  
  bool operator==(normal c){
    return c.getCoord(0)==this->getCoord(0) && c.getCoord(1)==this->getCoord(1) && c.getCoord(2)==this->getCoord(2);
  }
  
  float * getCoord(){
    return &coord[0];
  }
  
  float getCoord(int i){
    assert(i<3 && i>=0);
    return coord[i];
  } 
  
  float getModulo(){
    return sqrt((getCoord(0)*getCoord(0))+(getCoord(1)*getCoord(1))+(getCoord(2)*getCoord(2)));
  }
  
  void setCoord(float x, float y, float z){
    coord[0]=x; coord[1]=y; coord[2]=z;
  }
  
  void setCoord(int i, float x){
    assert(i<3 && i>=0);
    coord[i]=x;
  }
  
};

/*********************************************************************************/

class solido{
  
private:
  
  vector<vertice> vertices;
  vector<cara> caras;
  vector<arista> aristas;
  vector<normal> vnormales;
  vector<normal> cnormales;
  color c;
  pair<color,color> aj;
  int ps;
  
  
public:
  
  solido() {
    c.setColor(0,0,0);
    aj.first.setColor(0,0,0);
    aj.second.setColor(0,255,0);
    ps = 5;
  }
  
  solido(vector<float> ver, vector<int> car){
    
    assert(ver.size()%3==0 && car.size()%3==0);
    
    c.setColor(0,0,0);
    aj.first.setColor(0,0,0);
    aj.second.setColor(0,255,0);
    ps = 5;
    
    for(int i=0;i<ver.size();i+=3)
      this->insertarVertice(ver[i],ver[i+1],ver[i+2]);
    
    for(int i=0;i<car.size();i+=3)
      this->insertarCara(car[i],car[i+1],car[i+2]);
    
  }
  
  ~solido(){
    vertices.clear();
    caras.clear();
    aristas.clear();
  }
  
  
  vector<vertice> getVerticesVec(){
    return vertices;
  }
  
  float* getVertices(){
    return getVertice(0).getCoord();
  }
  
  vector<cara> getCaras(){
    return caras;
  }
  
  vector<arista> getAristas(){
    return aristas;
  }
  
  vertice getVertice(int i){
    return vertices[i];
  }
  
  cara getCara(int i){
    return caras[i];
  }
  
  arista getArista(int i){
    return aristas[i];
  }
  
  int getNumVertices(){
    return vertices.size();
  }
  
  int getNumCaras(){
    return caras.size();
  }
  
  int getNumAristas(){
    return aristas.size();
  }
  
  color getColor(){
    return c;
  }
  
  color getColorAjFirst(){
    return aj.first;
  }
  
  color getColorAjSecond(){
    return aj.second;
  }
  
  int getPS(){
    return ps;
  }
  
  void setVertices(vector<float> ver){
    for(int i=0;i<ver.size();i+=3)
      this->insertarVertice(ver[i],ver[i+1],ver[i+2]);
  }
  
  void setPS(int ps){
    this->ps=ps;
  }
  
  void setColor(float r, float g, float b){
    c.setColor(r,g,b);
  }
  
  void setColoresAj(color aj1, color aj2){
    aj.first = aj1;
    aj.second= aj2;
  }
  
  void setColorAjFirst(color aj1){
    aj.first = aj1;
  }

  void setColorAjSecond(color aj2){
    aj.second= aj2;
  }
  
  void insertarVertice(float x, float y, float z){
    vertices.push_back(vertice(x,y,z));
    vnormales.push_back(normal());
  }
  
  void insertarCara(int i1, int i2, int i3){
    caras.push_back(cara(i1,i2,i3));
    cnormales.push_back(normal());
  }
  
  void insertarArista(int i1, int i2){
    aristas.push_back(arista(i1,i2));
  }
  
  void draw(GLenum modo, color c_fondo, bool ajedrez, int borde, color c_borde, float ancho);
  
  void drawAristas(color c_borde, float ancho);
  
  void draw2(){
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3,GL_FLOAT,0,getVertices());
    glDrawArrays(GL_TRIANGLES,0,getNumVertices());
    glDisableClientState (GL_VERTEX_ARRAY);
  }
  
  void drawNormalVertice(float k,color col);
  
  void crearRevolucion(int ndiv, vector<vertice> v);
  
  vector<int> carasPorVertice(vertice v);
  
  void crearNormales();
  
};

/*********************************************************************************/

void draw_cube();
void draw_vertices(vector<float> &Vertices, color c, int ps);
void construir_cubo(float ancho, float alto, float profundidad, solido *cubo);
void construir_piramide(float tam, solido *piramide, bool estado=0);
void construir_esfera(float r,int nPerfil, solido *esfera, int estado=0);
vector<vertice> toVectorVertices(vector<float> ver);

#endif