#include"planetas.h"
#include"camera.h"

camera cam = {
   {520.,75.,0.},
   {0.,0.,0.},
   {0.,0.,0.},
   {0.,0.,0.}
};

double renderPos[3] = {Sun.pos.x, Sun.pos.y, Sun.pos.z};

#define STARTVEL 2.0
#define CAMERA_ACCEL 5
//clarificando, cada tick equivale a 2^HOUR_TICK_2EXP horas
#define HOUR_TICK_2EXP 2
int hour_tick_2exp = HOUR_TICK_2EXP;
float cameraVel = STARTVEL;

double globalUp[3] = {0.,0.,1.};
bool simulate = true;
const double tickPeriod = 1/30;
time_t lastTickTime;
unsigned long int tick = 0;
unsigned int tickMultiplier = 1;

void countTick(){
   time_t temp_time = std::time(NULL);
   if (std::difftime(temp_time, lastTickTime) >= tickPeriod){
      if (simulate) {
         tick += (((unsigned int) (std::difftime(temp_time, lastTickTime)/tickPeriod))+1)<<hour_tick_2exp;
         for (auto body : bodies){
            (*body).updateVars(tick);
         }
      }
      lastTickTime = temp_time;
      glutPostRedisplay();
   }
}

void init(void) {
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
   cam.up[0] = 0.;
   cam.up[1] = 0.;
   cam.up[2] = 1.;
   cam.updateDirections();
}

bool sphereCollision(double sphereCenter[3], double radius, double camPos[3]){
   return std::pow((camPos[0] - sphereCenter[0]), 2.)+std::pow((camPos[1] - sphereCenter[1]), 2.)+std::pow((camPos[2] - sphereCenter[2]), 2.) <= radius*radius;
}

void CelestialBody::render(){
   /*
      usando coords polares pra translação: theta, r
      angulo de rotação = phi
      funcao:
         1. pushMatrix
         2. roda theta
         3. translada (r,0,0)
         4. para cada orbitante chama render
         5. roda phi
         6. desenha corpo
         7. popMatrix
      inicialmente pensei em usar a eq parametrica da elipse,
      supostamente dessa forma aqui a gente só precisa calcular
      3 variáveis ao invés das 4 lá, mas o r até usa mais funções
      custosas que x e y
      
      usando coords cartesianas: x, y
      angulo de rotação = phi
      funcao:
         1. pushMatrix
         2. translada (x,y, 0)
         3. para cada orbitante chama render
         4. roda phi
         5. desenha corpo
         5.1 corrige colisão câmera-corpo se houver
         6. popMatrix
      é melhor usar a cartesiana mesmo
      vale notar que (x,y) é prum sistema onde a origem é o corpo central
   */
   glPushMatrix();
   glTranslated(pos.x, pos.y, pos.z);
   renderPos[0] += pos.x;
   renderPos[1] += pos.y;
   renderPos[2] += pos.z;
   for (auto body : orbiters){
      (*body).render();
   }
   int pedacos = 10 + (int) std::sqrt(radius)*3;
   glRotated(rotationAngle, globalUp[0], globalUp[1], globalUp[2]);
   glutWireSphere(radius, pedacos, pedacos);  
   
   if (sphereCollision(renderPos, radius, cam.pos)){ //minimal effort pra evitar entrar nos planetas
      cam.pos[2] += radius;
      cam.updateUp();
   }
   renderPos[0] -= pos.x;
   renderPos[1] -= pos.y;
   renderPos[2] -= pos.z;
   glPopMatrix();
}

void CelestialBody::updateVars(unsigned long int tick){
   //printf("antes tA = %f rA = %f\n", this->translationAngle, rotationAngle);
   translationAngle = transFreqAng*std::fmod((double) tick, translationPeriod);
   rotationAngle = rotFreqAng*std::fmod((double) tick, rotationPeriod);
   //printf("depois tA = %f rA = %f\n", translationAngle, rotationAngle);
   pos = {
      a*std::cos(translationAngle),
      b*std::sin(translationAngle),
      pos.z
   };
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(1.0, 1.0, 1.0);
   glPushMatrix();
   renderPos[0] = Sun.pos.x;
   renderPos[1] = Sun.pos.y;
   renderPos[2] = Sun.pos.z;
   Sun.render();
   glPopMatrix();
   glLoadIdentity();
   gluLookAt(cam.pos[0], cam.pos[1], cam.pos[2], 0.0, 0.0, 0.0, cam.up[0], cam.up[1], cam.up[2]);
   glutSwapBuffers();
}

void reshape (int w, int h){
   glViewport(0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 100000.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(cam.pos[0], cam.pos[1], cam.pos[2], 0.0, 0.0, 0.0, cam.up[0], cam.up[1], cam.up[2]);
}

void keyboard (unsigned char key, int x, int y){
   switch (key) {
      case 'a':
         cam.moveRight(-cameraVel);
         break;
      case 'd':
         cam.moveRight(cameraVel);
         break;
      case 'w':
         cam.moveFront(-cameraVel);
         break;
      case 's':
         cam.moveFront(cameraVel);
         break;
      case 'q':
         cam.moveUp(-cameraVel);
         break;
      case 'e':
         cam.moveUp(cameraVel);
         break;
      case 'r':
         cameraVel += CAMERA_ACCEL;
         break;
      case 't':
         if (cameraVel > CAMERA_ACCEL){
            cameraVel -= CAMERA_ACCEL;
         }
         break;
      case 'o':
         hour_tick_2exp++;
         break;
      case 'p':
         hour_tick_2exp = (1> hour_tick_2exp-1) ? 1 : hour_tick_2exp-1;
         break;
      case 'g':
         printf("pos atual: %f %f %f\n", cam.pos[0], cam.pos[1], cam.pos[2]);
         break;
      case 8: //backspace
         cameraVel = STARTVEL;
         break;
      case 32: //space
         simulate = !simulate;
         break;
      case 10: //enter
         hour_tick_2exp = HOUR_TICK_2EXP;
         break;
      default:
         break;
   }
}

int main(int argc, char** argv){
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); //placeholder
   glutInitWindowSize(500, 500); 
   glutInitWindowPosition(100, 100);
   glutCreateWindow(argv[0]);
   init();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutIdleFunc(countTick);
   glutMainLoop();
   return 0;
}