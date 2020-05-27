#include "CurvyWall.h"

/*********************************************************************
*                                    Constructor
*
* The first param is the length of the CurvyWall
* The second param is the radius of the CurvyWall
*/
CurvyWall::CurvyWall(float l, float r) : Wall(l){
    this->length = l;
    this->radius = r;
    this->start_pt = NULL;
    this->end_pt = NULL;
    this->center_pt = NULL;
}
/*********************************************************************
*                                    Constructor
*
* The first param is the start point of CurvyWall
* The second param is the end point of CurvyWall
* The third param is the center point of CurvyWall
* The CurvyWall object is defined in counterclockwise direction.
*/
CurvyWall::CurvyWall(const Particle& st_pt, const Particle& e_pt, const Particle& cr_pt) : Wall(st_pt, e_pt){
    this->length = -1;
    this->radius = -1;
    this->start_pt = new Particle(st_pt.X(), st_pt.Y());
    this->end_pt = new Particle(e_pt.X(), e_pt.Y());
    this->center_pt = new Particle(cr_pt.X(), cr_pt.Y());
}
/*********************************************************************
*                                    Copy Constructor
*
* Deep copy
*/
CurvyWall::CurvyWall(const CurvyWall& rhs) : Wall(rhs){
    length = rhs.length;
    radius = rhs.radius;
    
    if(rhs.start_pt == NULL) start_pt = NULL;
    else start_pt = new Particle(rhs.start_pt->X(), rhs.start_pt->Y());
    
    if(rhs.end_pt == NULL) end_pt = NULL;
    else end_pt = new Particle(rhs.end_pt->X(), rhs.end_pt->Y());
    
    if(rhs.center_pt == NULL) center_pt = NULL;
    else center_pt = new Particle(rhs.center_pt->X(), rhs.center_pt->Y());
}
/*********************************************************************
*                                    GetRadius
*
* @return the radius of CurvyWall
*/
float CurvyWall::GetRadius() const{
    return radius;
}
/*********************************************************************
*                                    GetCenter
*
* @return the center point of CurvyWall
*/
const Particle& CurvyWall::GetCenter() const{
    return *center_pt;
}
/*********************************************************************
*                                    ComputeLength
*
* Computes the length of the CurvyWall object
* Use PI defined in the macros in case that you need
*/
void CurvyWall::ComputeLength(){
    if(length == -1){
        float angle;
        Wall tempWall1(*start_pt, *center_pt);
        Wall tempWall2(*end_pt, *center_pt);
        tempWall1.ComputeLength();
        tempWall2.ComputeLength();
        radius = tempWall1.length;
        angle = tempWall1.FindAngleBetween(tempWall2);
        length = 2 * PI * radius * angle;
        
    }
}
/*********************************************************************
*                                    Clone
*
* @return pointer to a copy of this Wall object
*/
Wall* CurvyWall::Clone() const{
    CurvyWall* obj;
    if(start_pt != NULL && end_pt != NULL && center_pt != NULL){
        obj = new CurvyWall(*start_pt, *end_pt, *center_pt);
        obj->length = length;
        obj->radius = radius;
    }
    else
        obj = new CurvyWall(length, radius);
        
    return (Wall *)obj;
}
/*********************************************************************
*                                    IsContinuousLinear
*
* @return false always
*/
bool CurvyWall::IsContinuousLinear(const Wall&) const{
    return false;
}
/*********************************************************************
*                                    Operator overload
*
* Combines this CurvyWall object with the Wall given in the param
* @return the combined wall
*    - The combination can be done only if the wall given in the argument
    is also a CurvyWall such that one wall starts at the point where
      the other ends and their center is common (coordinates of their
      center points are the same)
    - On the other hand, the fact that which one of the two walls starts
      first is not important. Similar to the fact that 3+5 = 5+3.
* For the cases which do not satisfy above, the method should throw
  ApplePearException which was defined in Exception.h.
* The walls to be combined will always be the ones constructed with
  their start and end points (anomalous examples will not be tested.)
*/
const Wall& CurvyWall::operator+(const Wall& rhs) const{
    Wall temp(rhs.GetFinalParticle(), rhs.GetInitialParticle());
    Wall* ticc = rhs.Clone();
    Wall* combined = NULL;
    if(rhs.IsContinuousLinear(temp) == true) throw ApplePearException();
    Particle rct = ((CurvyWall*)ticc)->GetCenter();
    Particle lct = this->GetCenter();
    float rrad = ((CurvyWall*)(ticc))->GetRadius();
    float lrad = this->GetRadius();
    
    if(!(rct==lct) || (fabs(rrad-lrad) > EPSILON)){
        throw ApplePearException();
        
    }else{
        if(GetFinalParticle() == ticc->GetInitialParticle()){
            combined = new CurvyWall(GetInitialParticle(), ticc->GetFinalParticle(), rct);
            delete ticc;
            return *combined;
        }else if(GetInitialParticle() == ticc->GetFinalParticle()){
            combined = new CurvyWall(ticc->GetInitialParticle(), GetFinalParticle(), lct);
            delete ticc;
            return *combined;
        }else{
            delete ticc;
            throw ApplePearException();
        }
        
    }
    
}
/*********************************************************************
*                                    Destructor
*
*/
CurvyWall::~CurvyWall(){
    length = -1;
    radius = -1;
    if(start_pt != NULL) delete start_pt;
    if(end_pt != NULL) delete end_pt;
    if(center_pt != NULL) delete center_pt;
}
/********************************************************************/
