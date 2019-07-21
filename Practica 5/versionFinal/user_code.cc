//**************************************************************************
// Codigo del usuario
//
// Domingo Martin Perandres 2013
//
// GPL
//**************************************************************************


#include "user_code.h"
#include <iostream>

using namespace std;

//**************************************************************************
// Funcion para dibujar los vertices de un cubo unidad
//**************************************************************************

void draw_cube()
{
    GLfloat Vertices[8][3]= {{-0.5,-0.5,0.5},{0.5,-0.5,0.5},{0.5,0.5,0.5},{-0.5,0.5,0.5},{-0.5,-0.5,-0.5},{0.5,-0.5,-0.5},{0.5,0.5,-0.5},{-0.5,0.5,-0.5}};

    glColor3f(0,1,0);
    glPointSize(4);

    glBegin(GL_POINTS);
    for (unsigned i=0;i<8;i++){
        glVertex3fv((GLfloat *) &Vertices[i]);
        }
    glEnd();
}

void solido::draw(GLenum modo, color c_fondo, bool ajedrez, int borde, color c_borde, float ancho)
{
  if(!ajedrez)
    switch(modo){
      case GL_LINE:
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	break;
      case GL_FILL:
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	break;
      case GL_POINT:
	glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
	break;
    }
  else
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  
  if(c_fondo.getR()==-1 && c_fondo.getG()==-1 && c_fondo.getB()==-1)
    c_fondo=this->c;
  
  if(borde!=2)
    glColor3f(c_fondo.getR(),c_fondo.getG(),c_fondo.getB());
  else{
    glLineWidth(ancho);
    glColor3f(c_borde.getR(),c_borde.getG(),c_borde.getB());
  }

  glPointSize(ps);
      
  glBegin(GL_TRIANGLES);
    for(int i=0;i<this->getNumCaras();i++){
      if(ajedrez)
	if(i%2)
	  glColor3f(aj.first.getR(),aj.first.getG(),aj.first.getB());
	else
	  glColor3f(aj.second.getR(),aj.second.getG(),aj.second.getB());
	
      glVertex3fv(this->getVertice(this->getCara(i).getIndice(0)).getCoord());
      glVertex3fv(this->getVertice(this->getCara(i).getIndice(1)).getCoord());
      glVertex3fv(this->getVertice(this->getCara(i).getIndice(2)).getCoord());
    }
  glEnd();
  
  if(borde==1 && modo!=GL_LINE)
    if(!getAristas().empty())
      drawAristas(c_borde,ancho);
    else
      draw(GL_LINE,c_fondo,0,2,c_borde,ancho);
  glLineWidth(1.0);
}

void solido::drawAristas(color c_borde, float ancho){
  
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  
  glColor3f(c_borde.getR(),c_borde.getG(),c_borde.getB());
  
  glLineWidth(ancho);
  
  glBegin(GL_LINES);
    for(int i=0;i<this->getNumAristas();i++){
      glVertex3fv(this->getVertice(this->getArista(i).getIndice(0)).getCoord());
      glVertex3fv(this->getVertice(this->getArista(i).getIndice(1)).getCoord());
    }
  glEnd();
  
  glLineWidth(1.0);
  
}

void solido::drawNormalVertice(float k,color col)
{
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  
  glColor3f(col.getR(),col.getG(),col.getB());
  
  glBegin(GL_LINES);
    for(int i=0;i<this->getNumVertices();i++){
      glVertex3fv(this->getVertice(i).getCoord());
      vertice aux = vertice((this->getVertice(i).getCoord(0))+(k*vnormales[i].getCoord(0)),	(this->getVertice(i).getCoord(1))+(k*vnormales[i].getCoord(1)),
	(this->getVertice(i).getCoord(2))+(k*vnormales[i].getCoord(2)));
      glVertex3fv(aux.getCoord());
    }
  glEnd();
}

void solido::crearRevolucion(int ndiv, vector<vertice> v){

  float angulo,x,y,z;
  int nPerfil= v.size(),s=0,nivel=nPerfil;
  bool primeroCero = v[0].getCoord(0)>=-0.0001 && v[0].getCoord(0)<=0.0001 && v[0].getCoord(2)>=-0.0001 && v[0].getCoord(2)<=0.0001, 
       ultimoCero = v[nPerfil-1].getCoord(0)>=-0.0001 && v[nPerfil-1].getCoord(0)<=0.0001 && v[nPerfil-1].getCoord(2)>=-0.0001 && v[nPerfil-1].getCoord(2)<=0.0001;
  
  if(primeroCero)
    s=1;
  if(ultimoCero)
    nivel=nPerfil-1;
  
  for(int i=s; i<nivel;i++){
    //for(int j=ndiv;j>0;j--){
      for(int j=0;j<ndiv;j++){
      
      angulo=2*M_PI*j/ndiv;
      
      x=sqrt(v[i].getCoord(0)*v[i].getCoord(0)+v[i].getCoord(2)*v[i].getCoord(2))*cos(angulo);
      y=v[i].getCoord(1);
      z=sqrt(v[i].getCoord(0)*v[i].getCoord(0)+v[i].getCoord(2)*v[i].getCoord(2))*sin(angulo);
      
      insertarVertice(x,y,z);
      
    }
  }
  
  insertarVertice(0,v[0].getCoord(1),0);
  insertarVertice(0,v[nPerfil-1].getCoord(1),0);
  
  int naux=nPerfil-1;
  if(primeroCero)
    naux--;
  if(ultimoCero)
    naux--;
  
  for(int i=0;i<naux;i++){
    for(int j=0;j<ndiv;j++){
      insertarCara(	((j+1)%ndiv)+(i*ndiv),         j+(i*ndiv),	  ((j+1)%ndiv)+((i+1)*ndiv));
      insertarCara(	((j+1)%ndiv)+((i+1)*ndiv),     j+(i*ndiv),       j+((i+1)*ndiv));
      insertarArista(	((j+1)%ndiv)+(i*ndiv),         j+(i*ndiv));
      insertarArista(	(j+(i*ndiv)),j+((i+1)*ndiv));
    }
  }
  
  for(int i=0;i<ndiv;i++){
    insertarCara((i+1)%ndiv,getNumVertices()-2,i);
    insertarArista(i,(i+1)%ndiv);
    if(primeroCero)
      insertarArista(i,getNumVertices()-2);
  }
  
  for(int i=0;i<ndiv;i++){
    insertarCara(i+(naux)*ndiv,getNumVertices()-1,(i+1)%ndiv+(naux)*ndiv);
    insertarArista(i+(naux)*ndiv,(i+1)%ndiv+(naux)*ndiv);
    if(ultimoCero)
      insertarArista(i+(naux)*ndiv,getNumVertices()-1);
  }
  
//   crearNormales();
}

vector<int> solido::carasPorVertice(vertice v){
    
  vector<int> indicesCaras;
    
  for(int i=0;i<getNumCaras();i++){
      
    if((getVertice(getCara(i).getIndice(0))==v && 1) || getVertice(getCara(i).getIndice(1))==v || getVertice(getCara(i).getIndice(2))==v)
      indicesCaras.push_back(i);
      
    }
    
  return indicesCaras;
    
}

void solido::crearNormales(){
    
  vector<float> ab(3,0),bc(3,0);
  float cx,cy,cz,moduloCara,moduloVertice;
  /*
  if(!cnormales.empty())
    cnormales.clear();
  if(!vnormales.empty())
    vnormales.clear();
  */
  // Crear normales de las caras
  for(int i=0;i<getNumCaras();i++){
      
    ab[0]=getVertice(getCara(i).getIndice(1)).getCoord(0)-getVertice(getCara(i).getIndice(0)).getCoord(0);
    ab[1]=getVertice(getCara(i).getIndice(1)).getCoord(1)-getVertice(getCara(i).getIndice(0)).getCoord(1);
    ab[2]=getVertice(getCara(i).getIndice(1)).getCoord(2)-getVertice(getCara(i).getIndice(0)).getCoord(2);
      
    bc[0]=getVertice(getCara(i).getIndice(2)).getCoord(0)-getVertice(getCara(i).getIndice(0)).getCoord(0);
    bc[1]=getVertice(getCara(i).getIndice(2)).getCoord(1)-getVertice(getCara(i).getIndice(0)).getCoord(1);
    bc[2]=getVertice(getCara(i).getIndice(2)).getCoord(2)-getVertice(getCara(i).getIndice(0)).getCoord(2);
      
    moduloCara=sqrt(pow(ab[1]*bc[2]-ab[2]*bc[1],2)+pow(ab[2]*bc[0]-ab[0]*bc[2],2)+pow(ab[0]*bc[1]-ab[1]*bc[0],2));
      
    cx=((ab[1]*bc[2])-(ab[2]*bc[1]))/moduloCara;
    cy=((ab[2]*bc[0])-(ab[0]*bc[2]))/moduloCara;
    cz=((ab[0]*bc[1])-(ab[1]*bc[0]))/moduloCara;
      
    cnormales[i]=normal(cx, cy, cz);
  }

  // Crear normales de los vertices
  for(int i=0;i<getNumVertices();i++){
      
    vector<int> indicesCaras;
    normal cara_j;
    float vx=0,vy=0,vz=0;
      
    indicesCaras=carasPorVertice(getVertice(i));
    vector<normal> normalesCarasRepetidas;
      
    //Sumar normales de las caras que comparten el vertice
    for(int j=0;j<indicesCaras.size();j++){
	
      cara_j=normal(cnormales[indicesCaras[j]].getCoord(0),cnormales[indicesCaras[j]].getCoord(1),cnormales[indicesCaras[j]].getCoord(2));
      bool repeitdo=false;
      
      for(int k=0;k<normalesCarasRepetidas.size();k++){
	if(normalesCarasRepetidas[k]==cara_j){
	  repeitdo=true;
	  break;
	}
      }
      
      if(!repeitdo){
	normalesCarasRepetidas.push_back(cara_j);
	vx+=cnormales[indicesCaras[j]].getCoord(0);
	vy+=cnormales[indicesCaras[j]].getCoord(1);
	vz+=cnormales[indicesCaras[j]].getCoord(2);
      }
    }
    
    moduloVertice=sqrt(pow(vx,2)+pow(vy,2)+pow(vz,2));
      
    vnormales[i]=normal(vx/moduloVertice,vy/moduloVertice,vz/moduloVertice);
  }
}

//**************************************************************************
// Funcion para dibujar vertices
//**************************************************************************

void draw_vertices(vector<float> &Vertices, color c, int ps)
{
  
  glColor3f(c.getR(),c.getG(),c.getB());
  glPointSize(ps);
  glBegin(GL_POINTS);
    for(int i=0;i<Vertices.size();i+=3)
      glVertex3fv(&Vertices[i]);
  glEnd();
  /*
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3,GL_FLOAT,0,&Vertices[0]);
  glColor3f(c.getR(),c.getG(),c.getB());
  glPointSize(ps);
  glDrawArrays(GL_POINTS,0,Vertices.size());
  glDisableClientState (GL_VERTEX_ARRAY);*/
  
}

//**************************************************************************
// Funcion para construir modelos solidos de poligonos
//**************************************************************************

void construir_cubo(float ancho, float alto, float profundidad, solido *cubo){
  
  cubo->insertarVertice(-(ancho)/2,-(alto)/2,(profundidad)/2);
  cubo->insertarVertice((ancho)/2,-(alto)/2,(profundidad)/2);
  cubo->insertarVertice((ancho)/2,(alto)/2,(profundidad)/2);
  cubo->insertarVertice(-(ancho)/2,(alto)/2,(profundidad)/2);
  
  cubo->insertarVertice(-(ancho)/2,-(alto)/2,-(profundidad)/2);
  cubo->insertarVertice((ancho)/2,-(alto)/2,-(profundidad)/2);
  cubo->insertarVertice((ancho)/2,(alto)/2,-(profundidad)/2);
  cubo->insertarVertice(-(ancho)/2,(alto)/2,-(profundidad)/2);
  
  cubo->insertarCara(0,1,3);
  cubo->insertarCara(3,1,2);
  cubo->insertarCara(1,5,2);
  cubo->insertarCara(5,6,2);
  cubo->insertarCara(5,4,6);
  cubo->insertarCara(4,7,6);
  cubo->insertarCara(4,0,7);
  cubo->insertarCara(0,3,7);
  cubo->insertarCara(4,5,0);
  cubo->insertarCara(5,1,0);
  cubo->insertarCara(3,2,7);
  cubo->insertarCara(2,6,7);
  
  cubo->insertarArista(0,1);
  cubo->insertarArista(1,2);
  cubo->insertarArista(2,3);
  cubo->insertarArista(3,0);
  
  cubo->insertarArista(4,5);
  cubo->insertarArista(5,6);
  cubo->insertarArista(6,7);
  cubo->insertarArista(7,4); 
  
  cubo->insertarArista(0,4);
  cubo->insertarArista(1,5);
  cubo->insertarArista(2,6);
  cubo->insertarArista(3,7);  
  
//   cubo->crearNormales();
}

void construir_piramide(float tam, solido *piramide,bool estado){
  
  piramide->insertarVertice(-tam,-tam,tam);
  piramide->insertarVertice(tam,-tam,tam);
  piramide->insertarVertice(tam,-tam,-tam);
  piramide->insertarVertice(-tam,-tam,-tam);
  
  if(!estado)
    piramide->insertarVertice(0,tam,0); // piramide normal
  else
    piramide->insertarVertice(-tam,tam,0); // piramide "tipo 2"
  
  piramide->insertarCara(0,1,4);
  piramide->insertarCara(1,2,4);
  piramide->insertarCara(2,3,4);
  piramide->insertarCara(3,0,4);
  piramide->insertarCara(3,2,0);
  piramide->insertarCara(2,1,0);
  
  piramide->insertarArista(0,1);
  piramide->insertarArista(1,2);
  piramide->insertarArista(2,3);
  piramide->insertarArista(3,0);
  
  piramide->insertarArista(0,4);
  piramide->insertarArista(1,4);
  piramide->insertarArista(2,4);
  piramide->insertarArista(3,4);
  
//   piramide->crearNormales();

}

void construir_esfera(float r,int nPerfil, solido *esfera, int estado){
  
  vector<vertice> aux;
  float angulo;
  
  for(int i=0;i<=nPerfil;i++){
    switch(estado){
      case 0:
	angulo=(M_PI*i/nPerfil)-M_PI/2; //esfera
	break;
      case 1:
	angulo=((M_PI/2)*i/nPerfil); //semiesfera superior
	break;
      case 2:
	angulo=((M_PI/2)*i/nPerfil)-M_PI/2; //semiesfera inferior
	break;
    }
    
    aux.push_back(vertice(r*cos(angulo),r*sin(angulo),0));
  }
  
  esfera->crearRevolucion(nPerfil,aux);
  
}

vector<vertice> toVectorVertices(vector<float> ver){
  
  vector<vertice> aux;
  for(int i=0;i<ver.size();i+=3)
      aux.push_back(vertice(ver[i],ver[i+1],ver[i+2]));
  return aux;
}
/*
void CargarCamaras(float x_max, float y_max, float x_min, float y_min, float front, float back){
  
  camara normal(1, x_max,  y_max,  x_min,  y_min,  front,  back), 
	 perfil, planta, alzado;
}*/