#ifndef PLANETAS_H
#define PLANETAS_H
#include<vector>
#include<ctime>
#include<cstring>
#include<cmath>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<stdlib.h>
#include<stdio.h>

/* sobre a representação do sistema solar
Inicialmente renderizei em escala os corpos, desde raios a trajetórias, e o resultado foi bem desagradável em questão de
visualização. O sol realmente é grande pra bexiga e os planetas são muito espaçados em relação ao seu tamanho. Para
tratar disso, foi adicionado o PROXIMITY_WEIGHT. As distâncias relativas entre os centros de cada orbitante são mantidos,
mas como não se escala o tamanho dos planetas, os planetas aparentaram-se mais próximos que antes.

Outros pontos importantes:
    Saturno não tem anéis aqui;
*/

//só pra acessar por x, y e z
typedef struct position {
    double x, y, z;
} position;

//classe pra td corpo celeste
class CelestialBody {
    public:
        double radius;
        position pos; //posicao cartesiana num sistema com o corpo central como origem
        std::vector<CelestialBody*> orbiters; //corpos que orbitam este corpo
        double aphelion, perihelion; //maior e menor distância ao corpo central respectivamente
        double rotationPeriod, translationPeriod; //periodos de rotação e translação, em horas
        CelestialBody(double radius, position pos) : radius{radius}, pos{pos} {}; //inicializador de um corpo estático (pos da cena)
        CelestialBody(double radius, double aphelion, double perihelion, 
            double rotationPeriod, double translationPeriod, CelestialBody& orbit); //inicializador de orbitante
        void render(); //func recursiva de render do sistema, você só deve chamá-la para os corpos principais e/ou estacionários
        void updateVars(unsigned long int tick); //cálculo de posição e ângulos
    private:
        //valores internos usados pra poupar ops
        double translationAngle;
        double rotationAngle;
        double a, b; //semieixo principal e secundário respectivamente
        double transFreqAng=0, rotFreqAng=0; //valor pra
};

//divisor da distância de um orbitante ao corpo principal
#define PROXIMITY_WEIGHT 1000.
//2pi, alterar isso aqui deve gerar umas trajetórias engraçadas
#define TAU 6.28318530718
//ainda tem um vector de CelestialBody depois da inicialização dos planetas,
//ele tem o sol e todos eles

//dados planetas: https://nssdc.gsfc.nasa.gov/planetary/factsheet/
//sol foi do wikipedia

CelestialBody::CelestialBody(double radius, double _aphelion, double _perihelion, double _rotationPeriod,
double _translationPeriod, CelestialBody& orbit) : radius{radius}, aphelion{_aphelion}, perihelion{_perihelion},
rotationPeriod{_rotationPeriod}, translationPeriod{_translationPeriod}{    
    orbit.orbiters.push_back(this);
    pos = {orbit.pos.x + orbit.radius + _aphelion/PROXIMITY_WEIGHT, 0., 0.};
    a = orbit.radius + _aphelion/PROXIMITY_WEIGHT;
    b = orbit.radius + _perihelion/PROXIMITY_WEIGHT;
    transFreqAng = 2*TAU/_translationPeriod;
    rotFreqAng = 2*TAU/_rotationPeriod;
    //ab_2 = a*a*b*b;
}

//daqui pra frente é inicialização dos corpos e um vector dos orbitantes
CelestialBody Sun = CelestialBody(std::sqrt(69.57), //radius (10^4km)
    {0,0,0} //pos (10^4km)
);

CelestialBody Mercury = CelestialBody(0.24395, //radius (10^4km)
    6980., //aphelion (10^4km)
    4600., //perihelion (10^4km)
    1407.6, //rotationPeriod   (hours)
    (24.*88.), //translationPeriod (hours)
    Sun //orbit (corpo celeste o qual orbita, não soube escrever essa em inglês)
);

CelestialBody Venus = CelestialBody(0.6052,
    10890., 
    10750., 
    -5832.5, 
    (24.*224.7), 
    Sun
);

CelestialBody Earth = CelestialBody(0.6378,
    15210., 
    14710., 
    23.9, 
    (24.*365.2), 
    Sun
);

CelestialBody Mars = CelestialBody(0.3396,
    24930., 
    20670., 
    24.6, 
    (24.*687.0), 
    Sun
);

CelestialBody Jupiter = CelestialBody(7.1492,
    81640., 
    74060., 
    9.9, 
    (24.*4331.), 
    Sun
);

CelestialBody Saturn = CelestialBody(6.0268,
    150650., 
    135760., 
    10.7, 
    (24.*10747.), 
    Sun
);

CelestialBody Uranus = CelestialBody(2.5559,
    300140., 
    273270., 
    -17.2, 
    (24.*30589.), 
    Sun
);

CelestialBody Neptune = CelestialBody(2.4764,
    455890., 
    447110., 
    16.1, 
    (24.*59800.), 
    Sun
);

std::vector<CelestialBody*> bodies = {
    &Mercury,
    &Venus,
    &Earth,
    &Mars,
    &Jupiter,
    &Saturn,
    &Uranus,
    &Neptune
};
#endif