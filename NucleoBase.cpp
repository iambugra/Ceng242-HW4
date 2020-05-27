#include "NucleoBase.h"


/*********************************************************************
*                                    Constructor
*
* The first param is the whole RNA sequence
* The second parameter is a NucleoBase pointer addressing the previous
  NucleoBase object. For the initial nucleobase this is NULL.
*/
NucleoBase::NucleoBase(string w_rna, NucleoBase* p_prev){
    if(w_rna.length() > 1){
        string rest = w_rna.substr(1,w_rna.length()-1);
        this->base = w_rna[0];
        this->prev = p_prev;
        this->next = new NucleoBase(rest, this);
        
    }else{
        this->base = w_rna[0];
        this->prev = p_prev;
        this->next = NULL;
        
    }
    
}
/*********************************************************************
*                                Copy Constructor
*
* Deep copy
*/


NucleoBase::NucleoBase(const NucleoBase& rhs){
    this->base = rhs.base;
    if(rhs.next != NULL){
        this->prev = rhs.prev;
        this->next = new NucleoBase(*(rhs.next));
    }else{
        this->prev = rhs.prev;
        this->next = NULL;
    }
    
}
/*********************************************************************
*                                    Destructor
*
*/


NucleoBase::~NucleoBase(){
    delete next;
}
/*********************************************************************
*                                    GetLast
*
* @return pointer to the last element of NucleoBase chain.
*/
NucleoBase* NucleoBase::GetLast(){
    NucleoBase* cur = this;
    while(cur->next != NULL)
        cur = cur->next;
    return cur;
}
NucleoBase* NucleoBase::GetInitial(){
    NucleoBase* cur = this;
    while(cur->prev != NULL)
        cur = cur->prev;
    return cur;
}
/*********************************************************************
*                                    Stream Operator
* Prints the data in NucleoBase chain.
* Prints the char data of each NucleoBase object starting from the
  given NucleoBase in the param upto the last element of the chain.
* Do NOT add new line character "\n" to the end of the stream.
*/
ostream& operator<<(ostream& os, const NucleoBase& _base){
    
    if(_base.next != NULL){
        os << _base.base;
        os << *(_base.next);
    }else{
        os << _base.base;
    }
        
    return os;
    
}
