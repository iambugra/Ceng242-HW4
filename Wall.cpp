#include "Wall.h"
#

/*********************************************************************
*                                    Constructor
*
* param is the length of the Wall
*/
Wall::Wall(float length){
    this->length = length;
    this->fst = NULL;
    this->lst = NULL;
    
}
/*********************************************************************
*                                    Constructor
*
* The first param is the start point of Wall
* The second param is the end point of Wall
* The Wall object is defined in counterclockwise direction.
*/
Wall::Wall(const Particle& start, const Particle& end){
    length = -1.0;
    fst = new Particle(start.X(), start.Y());
    lst = new Particle(end.X(), end.Y());
    
}
/*********************************************************************
*                                    Copy constructor
*
* Deep copy
*/
Wall::Wall(const Wall& rhs){
    length = rhs.length;
    
    if(rhs.fst == NULL) fst = NULL;
    else fst = new Particle(rhs.fst->X(), rhs.fst->Y());
    
    if(rhs.lst == NULL) lst = NULL;
    else lst = new Particle(rhs.lst->X(), rhs.lst->Y());
        
}
/*********************************************************************
*                                    GetInitialParticle
*
* @return the start point of Wall
* This method will be called only for the walls construted with their start
  and end points
*/
const Particle& Wall::GetInitialParticle() const{
    return *fst;
}
/*********************************************************************
*                                    GetFinalParticle
*
* @return the end point of Wall
* This method will be called only for the walls construted with their start
  and end points
*/
const Particle& Wall::GetFinalParticle() const{
    return *lst;
}
/*********************************************************************
*                                    FindAngleBetween
*
* @return the angle between this Wall object and the one given in the param
*/
float Wall::FindAngleBetween(const Wall& wall) const{
    float ang1 = atan2((lst->X() - fst->X()), (lst->Y() - fst->Y()));
    float ang2 = atan2((wall.lst->X() - wall.fst->X()), (wall.lst->Y() - wall.fst->Y()));
    
    return ang1-ang2;
}
/*********************************************************************
*                                    GetLength
*
* @return the length of Wall
*/
float Wall::GetLength() const{
    return length;
}
/*********************************************************************
*                                    ComputeLength
*
* Computes the length of the Wall object
*/
void Wall::ComputeLength(){
    if(fabs(length+1) <= EPSILON) length = sqrt(pow(fst->X()-lst->X(),2) + pow(fst->Y()-lst->Y(),2));
}
/*********************************************************************
*                                    Clone
*
* @return pointer to a copy of this Wall object
*/
Wall* Wall::Clone() const{
    Wall * obj;
    if(fst != NULL && lst != NULL){
        obj = new Wall(*fst, *lst);
        
    }else
        obj = new Wall(length);
    obj->ComputeLength();
    
    return obj;
}
/*********************************************************************
*                                    IsContinuousLinear
*
* Checks whether the end points of this Wall object and the end points
  of the Wall given in the param are linear and the walls are continuous
* @return true if they are linear and continuous, false otherwise
* The method returns always false in case that the wall given in the
  argument is a curvy wall
* Two walls are linear only if they are straight walls and their slopes
  are equal (Be cautious while calculating slope of the vertical lines!)
* Two walls are continuous only if one starts at the point where the
  other ends (the fact that which one of the two walls starts first is
  not important.)
* The walls to be combined will always be the ones constructed with
  their start and end points (anomalous examples will not be tested.)
*/
bool Wall::IsContinuousLinear(const Wall& rhs) const{
    
    if((*fst == *(rhs.lst)) && (*lst == *(rhs.fst))){ return true;}
    
    else if((*fst == *(rhs.lst)) && !(*lst == *(rhs.fst))){
        
        if((fabs(lst->X()-rhs.lst->X()) <= EPSILON) && (fabs(rhs.fst->X()-rhs.lst->X()) <= EPSILON)){ return true;}
        
        /*   egim sonsuz */
        
        float mySlope = (lst->Y() - fst->Y()) / (lst->X() - fst->X());
        float rhsSlope = (rhs.lst->Y() - rhs.fst->Y()) / (rhs.lst->X() - rhs.fst->X());

        if(fabs(mySlope-rhsSlope) <= EPSILON) return true;
        else{
            
            return false;
        }
    }
    
    else if(!(*fst == *(rhs.lst)) && (*lst == *(rhs.fst))){
       
        if((fabs(rhs.lst->X()-lst->X()) <= EPSILON) && (fabs(lst->X()-fst->X()) <= EPSILON)){return true;}  /*   egim sonsuz */
        
        
        float mySlope = (lst->Y() - fst->Y()) / (lst->X() - fst->X());
        float rhsSlope = (rhs.lst->Y() - rhs.fst->Y()) / (rhs.lst->X() - rhs.fst->X());
        
        if(fabs(mySlope-rhsSlope) <= EPSILON) return true;
        else{
            
            return false;
        }
    }else{

        return false;
    }
}
/*********************************************************************
*                                    Operator overload
*
* Combines this Wall object with the Wall given in the param
* @return the combined wall
* Two walls can be combined only if one of the two cases below are
  satisfied:
    - They are both straight walls such that one starts at the point
      where the other ends
    - They are both curvy walls such that one starts at the point where
    the other ends and their center is common (coordinates of their
      center points are the same)
    - On the other hand, the fact that which one of the two walls starts
      first is not important. Similar to the fact that 3+5 = 5+3.
* For the cases which do not satisfy above, the method should throw
  ApplePearException which was defined in Exception.h.
* The walls to be combined will always be the ones constructed with
  their start and end points (anomalous examples will not be tested.)
*/
const Wall& Wall::operator+(const Wall& rhs) const{
    if(IsContinuousLinear(rhs) == false) throw ApplePearException();
    
    Wall* combined = NULL;
    
    if((*fst == *(rhs.lst)) && (*lst == *(rhs.fst))){
        combined = new Wall(*fst, *lst);
        
    }else if((*fst == *(rhs.lst)) && !(*lst == *(rhs.fst))){
        combined = new Wall(*(rhs.fst), *lst);
        
    }else if(!(*fst == *(rhs.lst)) && (*lst == *(rhs.fst))){
        combined = new Wall(*fst, *(rhs.lst));
        
    }
    combined->ComputeLength();
    return *combined;
}
/*********************************************************************
*                                    Destructor
*
*/
Wall::~Wall(){
    length = -1;
    if(fst != NULL) delete fst;
    if(lst != NULL) delete lst;
}
/********************************************************************/
