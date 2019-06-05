//
// Created by jacob on 05/06/19.
//

#include "structures.h"

long long nt_packet_t::idcnt = 0;

nt_packet_t::nt_packet_t(id_t& id){
    this->id = idcnt;
    ++idcnt;
}

std::ostream& operator<<(std::ostream& os, const nt_packet_t& p)
{
    os << "ID: " << p.id << ", SRC: " << p.addr_src << ", DST: " << p.dst<< std::endl;
    return os;
}

bool operator==(const nt_packet_t& p1, const nt_packet_t& p2)
{
    return (p1.id == p2.id);
}

bool operator!=(const nt_packet_t& p3, const nt_packet_t& p4)
{
    return !(p3.id == p4.id);
}
