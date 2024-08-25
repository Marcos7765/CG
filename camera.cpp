#include "camera.h"

void normalize(double target[3]){
   double norm = std::sqrt(target[0]*target[0] + target[1]*target[1] + target[2]*target[2]);
   target[0] /= norm;
   target[1] /= norm;
   target[2] /= norm;
}

void cross(double a[3], double b[3], double dest[3]){
   dest[0] = a[1]*b[2] - a[2]*b[1];
   dest[1] = a[2]*b[0] - a[0]*b[2];
   dest[2] = a[0]*b[1] - a[1]*b[0];
}

void camera::updateDirections(){
   std::memcpy(front, pos, sizeof(double)*3);
   normalize(front);
   cross(up, front, right);
   normalize(right);
}

void camera::updateUp(){
   cross(front, right, up);
   //front e right são ortonormais entre si ent nn preciso normalizar
}

void camera::moveRight(double velocity){
   pos[0] += right[0]*velocity;
   pos[1] += right[1]*velocity;
   pos[2] += right[2]*velocity;
   updateDirections();
}

void camera::moveFront(double velocity){
   pos[0] += front[0]*velocity;
   pos[1] += front[1]*velocity;
   pos[2] += front[2]*velocity;
   updateDirections();
}

void camera::moveUp(double velocity){
   pos[0] += up[0]*velocity;
   pos[1] += up[1]*velocity;
   pos[2] += up[2]*velocity;
   updateDirections();
   updateUp();
}