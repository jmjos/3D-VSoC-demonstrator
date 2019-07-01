//
// Created by jacob on 05/06/19.
//

#include "structures.h"

id_type Packet::idcnt = 0;

Packet::Packet(){
    this->id = idcnt;
    ++idcnt;
}

std::ostream& operator<<(std::ostream& os, const Packet& p)
{
    os << "ID: " << p.id << ", SRC: " << p.addr_src << ", DST: " << p.dst<< std::endl;
    return os;
}

bool operator==(const Packet& p1, const Packet& p2)
{
    return (p1.id == p2.id);
}

bool operator!=(const Packet& p1, const Packet& p2)
{
    return !(p1.id == p2.id);
}
