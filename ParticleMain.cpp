#include "Particle.h"

using namespace std;

int main(){
  Particle hid(0.3,0.4);
  Particle neon(0.3,0.4);
  Particle hel(0.5,1.2);
  Particle bug(0,0);
  Particle alp(0,1);
  Particle p0 =  Particle(0,0);
  Particle p1 =  Particle(0,3);
  Particle q0 =  Particle(0,3);
  Particle q1 =  Particle(0,5);
  cout << "--direk bas cout deneme" << endl;
  cout << hid << "and" << hel << "and" << bug << endl;

  cout << "--xini ysini donuyor mu" << endl;
  cout << hid.X() << endl;
  cout << hel.Y() << endl;

  cout << "--distance" << endl;
  cout << hel.FindDistance(bug) << endl;

  cout << "--bool deneme" << endl;
  if(!(bug == alp)) cout << "dogru" << endl;
  else cout << "yanlis" << endl;
  
    
  

  return 0;
}

