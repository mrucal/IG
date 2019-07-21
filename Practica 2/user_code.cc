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

void solido::draw(GLenum modo, bool ajedrez, int borde, color c_borde)
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
  
  if(borde!=2)
    glColor3f(c.getR(),c.getG(),c.getB());
  else{
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
  
  if(borde==1){ 
    draw(GL_LINE,0,2,c_borde);
  }
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
    }
  }
  
  for(int i=0;i<ndiv;i++)
    insertarCara((i+1)%ndiv,getNumVertices()-2,i);
  
  for(int i=0;i<ndiv;i++)
    insertarCara(i+(naux)*ndiv,getNumVertices()-1,(i+1)%ndiv+(naux)*ndiv);
  
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

void construir_cubo(float tam, solido *cubo){
  
  cubo->insertarVertice(-tam,-tam,tam);
  cubo->insertarVertice(tam,-tam,tam);
  cubo->insertarVertice(tam,tam,tam);
  cubo->insertarVertice(-tam,tam,tam);
  
  cubo->insertarVertice(-tam,-tam,-tam);
  cubo->insertarVertice(tam,-tam,-tam);
  cubo->insertarVertice(tam,tam,-tam);
  cubo->insertarVertice(-tam,tam,-tam);
  
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
  
}

void construir_piramide(float tam, solido *piramide){
  
  piramide->insertarVertice(-tam,-tam,tam);
  piramide->insertarVertice(tam,-tam,tam);
  piramide->insertarVertice(tam,-tam,-tam);
  piramide->insertarVertice(-tam,-tam,-tam);
  
  piramide->insertarVertice(0,tam,0);
  
  piramide->insertarCara(0,1,4);
  piramide->insertarCara(1,2,4);
  piramide->insertarCara(2,3,4);
  piramide->insertarCara(3,0,4);
  piramide->insertarCara(3,2,0);
  piramide->insertarCara(2,1,0);

}

void construir_esfera(float r,int nPerfil, solido *esfera){
  
  vector<vertice> aux;
  float angulo;
  
  for(int i=0;i<=nPerfil;i++){
    angulo=(M_PI*i/nPerfil)-M_PI/2; //esfera
    //angulo=((M_PI/2)*i/nPerfil); //semiesfera superior
    //angulo=((M_PI/2)*i/nPerfil)-M_PI/2; //semiesfera inferior
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