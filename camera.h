#ifndef CAMERA_H
#define CAMERA_H
#include<cstring>
#include<cmath>

//principal ref
//https://learnopengl.com/Getting-started/Camera

typedef struct camera {
   double pos[3]; //posicao
   
   //vetores de direção normalizados
   double front[3]; 
   double right[3];
   double up[3];

   //atualizacao dos vetores de direção
   void updateDirections(); //para deslocamento frente/trás e esquerda/direita
   void updateUp(); //para deslocamento cima/baixo
   
   //deslocamento em relação aos vetores de direção
   void moveRight(double velocity);
   void moveFront(double velocity);
   void moveUp(double velocity);
} camera;

//funcoes pra garantir que vai ter no opengl de lá (e sem usar o glm)
void normalize(double target[3]);
void cross(double a[3], double b[3], double dest[3]);
#endif