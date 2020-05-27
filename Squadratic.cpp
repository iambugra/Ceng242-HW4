#include "Squadratic.h"


/*********************************************************************
*                                    Constructor
*
* The first param is the id of the cell
* The second param is the edge length in the boundary
*/
Squadratic::Squadratic(int id, float edgeLength) : MicroOrganism(id, walls){
    this->id_no = id;
    this->edge_length = edgeLength;
    one_boundary = new Wall(edgeLength);
    walls.push_back(one_boundary); walls.push_back(one_boundary);
    walls.push_back(one_boundary); walls.push_back(one_boundary);

}
/*********************************************************************
*                                    Copy Constructor
*
* Deep copy
*/
Squadratic::Squadratic(const Squadratic& rhs) : MicroOrganism(rhs.id_no, rhs.walls){
    this->id_no = rhs.id_no;
    this->edge_length = rhs.edge_length;
    this->one_boundary = new Wall(edge_length);
    walls.push_back(one_boundary); walls.push_back(one_boundary);
    walls.push_back(one_boundary); walls.push_back(one_boundary);
}
/*********************************************************************
*                                    Destructor
*
*/
Squadratic::~Squadratic(){
    if(one_boundary != NULL) delete one_boundary;
    for(int i=0; i<walls.size(); i++)
        if(walls[i] != NULL)
            delete walls[i];
    for(int i=0; i<babies.size(); i++)
        if(babies[i] != NULL)
            delete babies[i];
}
/*********************************************************************
*                                    ConnectToCell
*
* See the base class explanation.
*/
void Squadratic::ConnectToCell(Cell* const toBeConn){
//    if(cell_cnd == NULL){
        Cell* temp_cell = new Cell(*toBeConn);
        temp_cell->StrengthenCellWall();
        if(DoesFitInto(*temp_cell)){
            cell_cnd = toBeConn ;
            cell_cnd->StrengthenCellWall();
            position.push_back(cell_cnd->GetCellWall()[0]->GetInitialParticle());
            position.push_back(cell_cnd->GetCellWall()[1]->GetInitialParticle());
            position.push_back(cell_cnd->GetCellWall()[2]->GetInitialParticle());
            position.push_back(cell_cnd->GetCellWall()[3]->GetInitialParticle());
        }
        delete temp_cell;
//    }
}
/*********************************************************************
*                                    DoesFitInto
*
* See the base class explanation.
*/
bool Squadratic::DoesFitInto(const Cell& cell) const{
//    cout << "girdi\n";
    Cell* temp_cell = new Cell(cell);
    temp_cell->StrengthenCellWall();
    if(temp_cell->GetCellWall().size() == 4){
        if((temp_cell->GetCellWall()[0]->GetLength() - one_boundary->GetLength()) <= EPSILON && (temp_cell->GetCellWall()[1]->GetLength() - one_boundary->GetLength()) <= EPSILON && (temp_cell->GetCellWall()[2]->GetLength() - one_boundary->GetLength()) <= EPSILON && (temp_cell->GetCellWall()[3]->GetLength() - one_boundary->GetLength()) <= EPSILON){
//            cout << *temp_cell << endl;
            
            if( (temp_cell->GetCellWall()[0]->FindAngleBetween(*(temp_cell->GetCellWall()[1])) - PI/2) <= EPSILON && (temp_cell->GetCellWall()[1]->FindAngleBetween(*(temp_cell->GetCellWall()[2])) - PI/2) <= EPSILON && (temp_cell->GetCellWall()[2]->FindAngleBetween(*(temp_cell->GetCellWall()[3])) - PI/2) <= EPSILON && (temp_cell->GetCellWall()[3]->FindAngleBetween(*(temp_cell->GetCellWall()[0])) - PI/2) <= EPSILON ){
//                cout << "simd\n";
                delete temp_cell;
                return true;
            }
        }
    }
    delete temp_cell;
    return false;
    
}
/*********************************************************************
*                                    React
*
* This type of microorganism proceeds into meiosis division as follows:
  The current squadratic microorganism becomes a parent of 4 children
  such that each child is a new squadratic microorganism whose edge
  length is half of the parent organism's edge length.
* Id of the child microorganisms does not matter. Give anything you
  want different than the previous microorganism ids.
* Each child squadratic positions into a different corner of the cell
  such that one of their top left, bottom left, top right and bottom
  right corners match with one of the corresponding corner of the cell.
* React() method can also be called for child microorganisms.
*/
void Squadratic::React(){
    vector<Particle> new_position;
    Squadratic *baby0, *baby1, *baby2, *baby3;
    
    new_position.push_back(Particle((position[0].X()+position[1].X())/2, (position[0].Y()+position[1].Y())/2));
    new_position.push_back(Particle((position[1].X()+position[2].X())/2, (position[1].Y()+position[2].Y())/2));
    new_position.push_back(Particle((position[2].X()+position[3].X())/2, (position[2].Y()+position[3].Y())/2));
    new_position.push_back(Particle((position[3].X()+position[0].X())/2, (position[3].Y()+position[0].Y())/2));
    new_position.push_back(Particle((position[0].X()+position[2].X())/2, (position[0].Y()+position[2].Y())/2));
    
    baby0 = new Squadratic(one_boundary->GetLength() * 7, (one_boundary->GetLength()) /2);
    baby1 = new Squadratic(one_boundary->GetLength() * 10, (one_boundary->GetLength()) /2);
    baby2 = new Squadratic(one_boundary->GetLength() * 11, (one_boundary->GetLength()) /2);
    baby3 = new Squadratic(one_boundary->GetLength() * 23, (one_boundary->GetLength()) /2);
    
    baby0->position.push_back(position[0]); baby0->position.push_back(new_position[0]); baby0->position.push_back(new_position[4]); baby0->position.push_back(new_position[3]);
    
    baby1->position.push_back(new_position[0]); baby1->position.push_back(position[1]); baby1->position.push_back(new_position[1]); baby1->position.push_back(new_position[4]);
    
    baby2->position.push_back(new_position[3]); baby2->position.push_back(new_position[4]); baby2->position.push_back(new_position[2]); baby2->position.push_back(position[3]);
    
    baby3->position.push_back(new_position[4]); baby3->position.push_back(new_position[1]); baby3->position.push_back(position[2]); baby3->position.push_back(new_position[2]);
    
    babies.push_back(baby0);
    babies.push_back(baby1);
    babies.push_back(baby2);
    babies.push_back(baby3);
    
}
/*********************************************************************
*                                    GetChild
*
* Special to this type of microorganism
* @return the child microorganism which is located between the minimum
  and maximum x and y coordinates given in the param.
* The first param is the minimum x border.
* The second param is the maximum x border.
* The third param is the minimum y border.
* The fourth param is the maximum y border.
* This method either directly returns the child whose border coordinates
  exactly equal to the parameters, or throws NotBornChildException in
    case that the asked child was created yet (the microorganism may
  have not been divided upto that level.)
*/
float get_min_x(vector<Particle> points){
    float sonuc = 999999;
    for(int i=0; i<points.size(); i++){
        if(points[i].X() < sonuc)
            sonuc = points[i].X();
    }
    return sonuc;
}
float get_max_x(vector<Particle> points){
    float sonuc = -999999;
    for(int i=0; i<points.size(); i++){
        if(points[i].X() > sonuc)
            sonuc = points[i].X();
    }
    return sonuc;
}
float get_min_y(vector<Particle> points){
    float sonuc = 999999;
    for(int i=0; i<points.size(); i++){
        if(points[i].Y() < sonuc)
            sonuc = points[i].Y();
    }
    return sonuc;
}
float get_max_y(vector<Particle> points){
    float sonuc = -999999;
    for(int i=0; i<points.size(); i++){
        if(points[i].Y() > sonuc)
            sonuc = points[i].Y();
    }
    return sonuc;
}
Squadratic& Squadratic::GetChild(float minX, float maxX, float minY, float maxY) const{
    for(int i=0; i<babies.size(); i++){
        if(fabs(get_max_x(babies[i]->position) - maxX) <= EPSILON && fabs(get_max_y(babies[i]->position) - maxY) <= EPSILON && fabs(get_min_x(babies[i]->position) - minX) <= EPSILON && fabs(get_min_y(babies[i]->position) - minY) <= EPSILON){
            return *babies[i];
        }
    }
    throw NotBornChildException();
}

