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
#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <assert.h>
#include <iostream>

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

class tabla_textura{
  
private:
  
  vector<float> coord;
  
public:
  
  tabla_textura(){
    vector<float> aux(2,0);
    this->coord=aux;
  }
  
  tabla_textura(float c1, float c2){
    coord.push_back(c1);
    coord.push_back(c2);
  }
  
  ~tabla_textura(){
    coord.clear();
  }
  
  vector<float> getIndices(){
    return coord;
  }
  
  float getIndice(int i){
    assert(i<2 && i>=0);
    return coord[i];
  }
  
  void setIndices(float c1, float c2){
    coord[0]=c1; coord[1]=c2; 
  }
  
  void setIndice(int i, float v){
    assert(i<2 && i>=0);
    coord[i]=v;
  }
  
};

/*********************************************************************************/

class textura{
private:
  unsigned char *pixels;
  unsigned long anchura,altura;
  GLuint id;
  bool autom;
  tabla_textura *t_tex;
  float plano_s[4],plano_t[4];
  
public:
  textura(){
    pixels=NULL;
    anchura=0;
    altura=0;
    id=0;
    autom=1;
    t_tex=NULL;
    float s[]={1.0, 0.0, 0.0, 0.0},t[]={0.0,-1.0, 0.0, 0.0};
    for(int i=0;i<4;i++) {
      plano_s[i]=s[i];
      plano_t[i]=t[i];
    }
  }
  
  textura(unsigned char *pixels,unsigned long anchura, unsigned long altura, GLuint id, bool autom, tabla_textura *t_tex,float *s,float *t){
    this->pixels=pixels;
    this->anchura=anchura;
    this->altura=altura;
    this->id=id;
    this->autom=autom;
    this->t_tex=t_tex;
    for(int i=0;i<4;i++) {
	  plano_s[i]=s[i];
	  plano_t[i]=t[i];
	}
  }
  
  void activar(){
    
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,anchura,altura,0,GL_RGB,GL_UNSIGNED_BYTE,pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   /* glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
    glEnable(GL_TEXTURE_2D);
    
    if(autom){

      glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
      glTexGenfv(GL_S,GL_OBJECT_PLANE,plano_s);
      glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
      glTexGenfv(GL_T,GL_OBJECT_PLANE,plano_t);
      glEnable(GL_TEXTURE_GEN_S);
      glEnable(GL_TEXTURE_GEN_T);
    }else{
      glBindTexture(GL_TEXTURE_2D, id);
    }
    
  }
  
  void desactivar(){
    glDisable(GL_TEXTURE_2D);
  }
  
  GLuint getId(){
    return id;
  }
};

/*********************************************************************************/

class fuente_luz{
  
private:
  
  color c;
  bool tipo;
  float *pos,a,b;
  int id;
  vector<float> esf;
  
public:
  
  fuente_luz(){
    c=color();
    tipo=0;
    pos=NULL;
    id=GL_LIGHT0;
    a=b=0;
  }
  
  fuente_luz(color col,bool t, float *f, float a, float b,int id=GL_LIGHT1){
    c=col;
    tipo=t;
    pos=f;
    this->id=id;
    this->a=a;
    this->b=b;
  }
  
  float* getPos(){
    return pos;
  }
  
  void activar(){
    
    GLfloat l[]={c.getR(),c.getG(),c.getB(),1.0};
    
    glLightfv(GL_LIGHT1,GL_AMBIENT,l);
    glLightfv(id,GL_DIFFUSE,l);
    glLightfv(id,GL_SPECULAR,l);
    if(tipo)
      glLightfv(id,GL_POSITION,getPos());
    else{
      GLfloat dir[]={cos(a)*sin(b),sin(a)*sin(b),cos(b),0.0};
      glLightfv(id,GL_POSITION,dir);
    }
    
  //   GLfloat p1[]={-5,-5,-5,1.0};
  //   glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,p);
  //   glPushMatrix();
  //   glRotatef(45,1,1,1);
  //   glLightfv(GL_LIGHT0,GL_POSITION,p);
  //   glPopMatrix();
  //   glLightfv(GL_LIGHT1,GL_POSITION,p);

//     glEnable(GL_NORMALIZE);
    
    
    glEnable(GL_LIGHTING);
    glDisable (GL_LIGHT0);
    glEnable(id);
    
  }
  
  void desactivar(){
    
    glDisable(GL_NORMALIZE);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(id);
    
  }
  
};

/*********************************************************************************/

class material{
  
private:
  
  float *emision;
  float *ambiente;
  float *difuso;
  float *especular;
  float brillo;
  
  textura *tex;
  
public:
  
  material(){
    emision=NULL;
    ambiente=NULL;
    difuso=NULL;
    especular=NULL;
    brillo=0;
    tex=NULL;
  }
  
  material(float *emision,float *ambiente,float *difuso,float *especular,float brillo, textura *tex=NULL){
    this->emision=emision;
    this->ambiente=ambiente;
    this->difuso=difuso;
    this->especular=especular;
    this->brillo=brillo;
    this->tex=tex;
  }
    
  void activar(){
    float aux[]={0.0,0.0,0.0,1.0};
    if(emision!=NULL)
      glMaterialfv(GL_FRONT, GL_EMISSION,emision);
    else
      glMaterialfv(GL_FRONT, GL_EMISSION,aux);
    glMaterialfv(GL_FRONT, GL_AMBIENT,ambiente);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,difuso);
    glMaterialfv(GL_FRONT, GL_SPECULAR,especular);
    glMaterialf(GL_FRONT, GL_SHININESS,brillo);

    glEnable(GL_LIGHTING);
    
    if(tex!=NULL)
      tex->activar();
    
  }
  
  void desactivar(){
    
    glDisable(GL_LIGHTING);
    if(tex!=NULL)
      tex->desactivar();
    
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
  vector<tabla_textura> t_texturas;
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
  
  tabla_textura* getTablaTexturas(){
    return &t_texturas[0];
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
  
  normal getNormalCara(int i){
    return cnormales[i];
  }
  
  normal getNormalVertice(int i){
    return vnormales[i];
  }
  
  tabla_textura getTextura(int i){
    return t_texturas[i];
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
  
  void setNormalCara(int i, float x,float y, float z){
    cnormales[i]=normal(x,y,z);
  }
  
  void setNormalVertice(int i, float x,float y, float z){
    vnormales[i]=normal(x,y,z);
  }
  
  void insertarVertice(float x, float y, float z,tabla_textura t=tabla_textura()){
    vertices.push_back(vertice(x,y,z));
    vnormales.push_back(normal());
    t_texturas.push_back(t);
  }
  
  void insertarCara(int i1, int i2, int i3){
    caras.push_back(cara(i1,i2,i3));
    cnormales.push_back(normal());
  }
  
  void insertarArista(int i1, int i2){
    aristas.push_back(arista(i1,i2));
  }
  
  void draw(GLenum modo, color c_fondo, bool ajedrez, int borde, color c_borde, float ancho,bool textura=0,GLuint id=-1);
  
  void drawAristas(color c_borde, float ancho);
  
  void draw2(){
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3,GL_FLOAT,0,getVertices());
    glDrawArrays(GL_TRIANGLES,0,getNumVertices());
    glDisableClientState (GL_VERTEX_ARRAY);
  }
  
  void drawNormalVertice(float k,color col);
  
  void crearRevolucion(int ndiv, vector<vertice> v,bool textura=false);
  
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
void luces();
void objetos(bool mio);
void escenaP4(float a, float b, solido * peon,textura *tex, solido *lata, textura *tex_lata, solido *lata_inf, solido * lata_sup);

#endif