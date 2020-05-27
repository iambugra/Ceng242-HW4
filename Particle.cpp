#include "Particle.h"

Particle::Particle(float xcor, float ycor){
    this->x_cor = xcor;
    this->y_cor = ycor;
}

Particle::Particle(const Particle& rhs){
    this->x_cor = rhs.x_cor;
    this->y_cor = rhs.y_cor;
}

float Particle::X() const{
    return x_cor;
}

float Particle::Y() const{
    return y_cor;
}

float Particle::FindDistance(const Particle& rhs) const{
    return sqrt(pow(x_cor-rhs.x_cor, 2) + pow(y_cor-rhs.y_cor, 2));
}

bool Particle::operator== (const Particle& rhs) const{
    if(fabs(x_cor - rhs.x_cor) <= EPSILON && fabs(y_cor - rhs.y_cor) <= EPSILON) return true;
    return false;
}

std::ostream& operator<<(std::ostream& os, const Particle& rhs){
    os << "(" << rhs.x_cor << "," << rhs.y_cor << ")";
    return os;
}

