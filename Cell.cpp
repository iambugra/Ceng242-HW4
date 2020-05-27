#include "Cell.h"
#include "Tissue.h"

/*********************************************************************
*                                    Constructor
*
* The first param is the id of the cell
* The second param is the cell wall which defines the shape of the cell
* The third param is the Tissue that cell belongs to
*/
Cell::Cell(int pid, const vector<Wall*>& pwalls, Tissue* ptissue){
    unsigned long size;
    this->id_no = pid;
    
    size = pwalls.size();
    for(int i=0; i<size; i++){
        walls.push_back(pwalls[i]->Clone());
    }
    this->tissue = ptissue;
    this->tissue->AddCell(*this);
}
/*********************************************************************
*                                    Copy Constructor
*
* Deep copy
*/
Cell::Cell(const Cell& rhs){
    unsigned long size;
    this->id_no = rhs.id_no;
    
    size = rhs.walls.size();
    for(int i=0; i<size; i++)
        walls.push_back(rhs.walls[i]->Clone());
    
    tissue = rhs.tissue;
}
/*********************************************************************
*                                    GetCellID
*
* @return the cell id
*/
int Cell::GetCellID() const{
    return id_no;
}
/*********************************************************************
*                                    GetCellID
*
* @return pointer for the tissue that this cell belongs to.
*/
Tissue* Cell::GetTissue() const{
    return tissue;
}
/*********************************************************************
*                                    GetCellWall
*
* @return the cell wall
*/
const vector<Wall*>& Cell::GetCellWall() const{
    return walls;
}
/*********************************************************************
*                                    RenewCellWall
*
* Replaces the current cell wall with the one given in the argument
*/
void Cell::RenewCellWall(vector<Wall*>& rhs){
    unsigned long msize = walls.size();
    
    for(int i=0; i<msize; i++)
        if(walls[i] != NULL)
            delete walls[i];
    
    walls = rhs;
    
}
/*********************************************************************
*                                    StrengthenCellWall
*
* Creates a new cell wall by connecting the partial walls which can
  be added with + operator
* Replaces the old cell wall with the new one
*/
void Cell::StrengthenCellWall(){
//    IF CASE KOY CURVYYE DUZ EKLEME DURUMU ICIN
    for(vector<Wall*>::iterator iter = walls.begin(); iter != walls.end(); ++iter){
        
        if(std::next(iter) != walls.end()){
            float this_len = sqrt(pow((*iter)->GetInitialParticle().X()-(*iter)->GetFinalParticle().X(), 2) + pow((*iter)->GetInitialParticle().Y()-(*iter)->GetFinalParticle().Y(), 2));
            float next_len = sqrt(pow((*std::next(iter))->GetInitialParticle().X()-(*std::next(iter))->GetFinalParticle().X(), 2) + pow((*std::next(iter))->GetInitialParticle().Y()-(*std::next(iter))->GetFinalParticle().Y(), 2));
            
            if(((*iter)->GetLength() == this_len && (*std::next(iter))->GetLength() == next_len) || ((*iter)->GetLength() != this_len && (*std::next(iter))->GetLength() != next_len)){
                try {
    //                cout << "Strengthening " << (*iter)->GetInitialParticle() << "-" << (*iter)->GetFinalParticle();
    //                cout << " and " << (*(std::next(iter)))->GetInitialParticle() << "-" << (*(std::next(iter)))->GetFinalParticle() << "\n";
                    Wall *old1, *old2, *neww;
                    old1 = *iter;
                    old2 = *(std::next(iter));
                    neww = (*old1 + *old2).Clone();
                    delete old1; delete old2;
                    *iter = neww;
                    walls.erase(std::next(iter));
                    --iter;
                    
                } catch( ApplePearException e) {
                    
                    continue;
                }
            }
        }
        else {
            float zero_len = sqrt(pow(walls[0]->GetInitialParticle().X()-walls[0]->GetFinalParticle().X(), 2) + pow(walls[0]->GetInitialParticle().Y()-walls[0]->GetFinalParticle().Y(), 2));
            float this_len = sqrt(pow((*iter)->GetInitialParticle().X()-(*iter)->GetFinalParticle().X(), 2) + pow((*iter)->GetInitialParticle().Y()-(*iter)->GetFinalParticle().Y(), 2));
            if(walls.size() > 1){
                if(((*iter)->GetLength() == this_len && walls[0]->GetLength() == zero_len) || ((*iter)->GetLength() != this_len && walls[0]->GetLength() != zero_len)){
                    try{
                        Wall *neww, *old1, *old2;
                        old1 = *iter;
                        old2 = *(walls.begin());
                        neww = (*old1 + *old2).Clone();
                        delete old1; delete old2;
                        *(walls.begin()) = neww;
                        walls.erase(iter);
                        break;

                    } catch (ApplePearException e) {
                        break;
                    }
                }
            }
        }
    
    }
}
/*********************************************************************
*                                    Destructor
*
*/
Cell::~Cell(){
    unsigned long size;
    this->id_no = 0;
    size = walls.size();
    for(int i=0; i<size; i++)
        if(walls[i] != NULL)
            delete walls[i];
    tissue->RemoveCell(*this);
    
}
/*********************************************************************
*                                    Stream Operator
*
* Prints cell wall
* The format is: (p1.x,p1.y) - (p2.x,p2.y) - ... - (pn.x,pn.y)
  where p1, p2, ..., pn are the the particles on the cell wall.
* Do NOT add new line character "\n" to the end of the stream.
*/
std::ostream& operator<<(std::ostream& os, const Cell& cell){
    unsigned long size = cell.walls.size();

    for(int i=0; i<size-1; i++)
        os << "(" << cell.walls[i]->GetInitialParticle().X() << "," << cell.walls[i]->GetInitialParticle().Y() << ")" << "-";
    
    os << "(" << cell.walls[size-1]->GetInitialParticle().X() << "," << cell.walls[size-1]->GetInitialParticle().Y() << ")";
    
    return os;
}
/********************************************************************/
