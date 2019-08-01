//
// Created by jacob on 05/06/19.
//

#include "structures.h"
#include "factory.h"

id_type Packet::idcnt = 0;

Packet::Packet(){
    this->id = idcnt;
    ++idcnt;
}

std::ostream& operator<<(std::ostream& os, const Packet* p)
{
    os << "\tID: " << p->id << "\tSRC: " << p->addr_src << "\tDST: " << p->dst;
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
