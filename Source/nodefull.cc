#include "../Headers/nodefull.h"

Utilities::NodeFull::NodeFull(int x, int y, int status) : Node(x, y){
    this->status = status;
}

int Utilities::NodeFull::get_status(){
    return this->status;
}

void Utilities::NodeFull::set_status(int status){
    this->status = status;
}