#include "Circular.h"

/*********************************************************************
*                                    Constructor
*
* The first param is the id of the cell
* The second param is the radius of the boundary
*/
Circular::Circular(int pid, float prad) : MicroOrganism(pid, wall){
    this->id_no = pid;
    this->radius = prad;
    Wall* my_wall = new CurvyWall(2 * PI * prad ,prad);
    wall.push_back(my_wall);
    cent_loc = NULL;
}
/*********************************************************************
*                                    Destructor
*
*/
Circular::~Circular(){
    for(int i=0; i<wall.size(); i++)
        delete wall[i];
    if(cent_loc != NULL) delete cent_loc;
}
/*********************************************************************
*                                    ConnectToCell
*
* See the base class explanation.
*/
void Circular::ConnectToCell(Cell* const cell){
//    if(cell_cnd == NULL){
        Cell* temp_cell = new Cell(*cell);
        float x, y;
        temp_cell->StrengthenCellWall();
        if(DoesFitInto(*temp_cell)){
            cell_cnd = cell;
            cell_cnd->StrengthenCellWall();
            x = ((CurvyWall*)(temp_cell->GetCellWall()[0]))->GetCenter().X();
            y =((CurvyWall*)(temp_cell->GetCellWall()[0]))->GetCenter().Y();
            cent_loc = new Particle(x,y);
        }
        
        delete temp_cell;
//    }
}
/*********************************************************************
*                                    DoesFitInto
*
* See the base class explanation.
*/
bool Circular::DoesFitInto(const Cell& cell) const{
//    cout << "girdi\n";
    Cell* temp_cell = new Cell(cell);
    temp_cell->StrengthenCellWall();
    if(temp_cell->GetCellWall().size() == 1){
//        cout << "sdn\n";
        if(abs(((CurvyWall*)(temp_cell->GetCellWall()[0]))->GetRadius() - this->radius) <= EPSILON){
            delete temp_cell;
            return true;
        }
    }
    delete temp_cell;
    return false;
}
/*********************************************************************
*                                        React
*
* This type of microorganism makes the size of the cell get two times
  larger without changing the center of the cell.
* As an effect of the enlarging, you should renew the cell walls.
* The microorganism size does not change.
*/
void Circular::React(){
    vector<Wall*> new_wall_vec;
    cell_cnd->StrengthenCellWall();
    Particle center = ((CurvyWall*)(cell_cnd->GetCellWall()[0]))->GetCenter();
    Particle start = Particle(center.X(), center.Y()+2*radius);
    CurvyWall* new_wall = new CurvyWall(start, start, center);
    new_wall_vec.push_back(new_wall);
    cell_cnd->RenewCellWall(new_wall_vec);
    ((CurvyWall*)(cell_cnd->GetCellWall()[0]))->ComputeLength();
    
}
/*********************************************************************
*                                    DoesContain
*
* Special to this type of microorganism
* Checks whether the given Particle in the param is inside the
  this microorganism, or not.
* @return true if the particle is inside, false otherwise.
*/
bool Circular::DoesContain(const Particle& part) const{
    float distance = cent_loc->FindDistance(part);
    if(fabs(distance-radius) <= EPSILON) return true;
    return false;
}
