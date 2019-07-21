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
//     cout<<c_borde.getR()<<" "<<c_borde.getG()<<" "<<c_borde.getB()<<endl;
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
//**************************************************************************
// Funcion para dibujar vertices
//**************************************************************************

void draw_vertices(vector<float> &Vertices, color c, int ps)
{
  /*
  glColor3f(c.getR(),c.getG(),c.getB());
  glPointSize(ps);
  glBegin(GL_POINTS);
    for(int i=0;i<Vertices.size();i+=3)
      glVertex3fv(&Vertices[i]);
  glEnd();*/
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3,GL_FLOAT,0,&Vertices[0]);
  glColor3f(c.getR(),c.getG(),c.getB());
  glPointSize(ps);
  glDrawArrays(GL_POINTS,0,Vertices.size());
  glDisableClientState (GL_VERTEX_ARRAY);
  
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
  cubo->insertarCara(3,2,6);
  cubo->insertarCara(2,1,6);
  cubo->insertarCara(6,1,5);
  cubo->insertarCara(0,1,5);
  cubo->insertarCara(0,4,5);
  cubo->insertarCara(0,4,3);
  cubo->insertarCara(4,3,7);
  cubo->insertarCara(6,7,3);
  cubo->insertarCara(4,6,7);
  cubo->insertarCara(4,5,6);

}

void construir_piramide(float tam, solido *piramide){
  
  piramide->insertarVertice(-tam,-tam,tam);
  piramide->insertarVertice(tam,-tam,tam);
  piramide->insertarVertice(tam,-tam,-tam);
  piramide->insertarVertice(-tam,-tam,-tam);
  
  piramide->insertarVertice(0,tam,0);
  
  piramide->insertarCara(0,1,4);
  piramide->insertarCara(1,2,4);
  piramide->insertarCara(3,2,4);
  piramide->insertarCara(0,4,3);
  piramide->insertarCara(0,1,3);
  piramide->insertarCara(1,2,3);

}