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

std::ostream& operator==(std::ostream& os, const nt_packet_t& p)
{
    os << "ID: " << p.id << ", SRC: " << p.addr_src << ", DST: " << p.dst<< std::endl;
    return os;
}

std::ostream& operator!=(std::ostream& os, const nt_packet_t& p)
{
    os << "ID: " << p.id << ", SRC: " << p.addr_src << ", DST: " << p.dst<< std::endl;
    return os;
}
