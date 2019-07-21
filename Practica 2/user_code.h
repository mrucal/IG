//**************************************************************************
// Práctica 1
//
// Domingo Martin Perandres 2013
//
// GPL
//**************************************************************************

#include <GL/gl.h>
#include <vector>
#include <cmath>
#include <assert.h>

using namespace std;

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
  }
  
  /*
  vector<vertice> getVertices(){
    return vertices;
  }*/
  
  float* getVertices(){
    return getVertice(0).getCoord();
  }
  
  vector<cara> getCaras(){
    return caras;
  }
  
  vertice getVertice(int i){
    return vertices[i];
  }
  
  cara getCara(int i){
    return caras[i];
  }
  
  int getNumVertices(){
    return vertices.size();
  }
  
  int getNumCaras(){
    return caras.size();
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
  
  void draw(GLenum modo, bool ajedrez, int borde, color c_borde);
  
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
void construir_cubo(float tam, solido *cubo);
void construir_piramide(float tam, solido *piramide);
void construir_esfera(float r,int nPerfil, solido *esfera);
vector<vertice> toVectorVertices(vector<float> ver);