//
// Created by mtzschoppe on 22/05/19.
//

#ifndef RAWTOCV_STRUCTURES_H
#define RAWTOCV_STRUCTURES_H

#include <string>
#include <iosfwd>
#include <ostream>
#include <queue>
#include <set>
#include <vector>

struct nt_packet_t {
    typedef int addr_t;
    typedef int node_t;
    typedef int data_t;
    typedef long long id_t;

    static id_t id;
    static id_t idcnt;
    long long cyck;
    data_t Datatyp;
    addr_t addr_src;
    addr_t dst;
    node_t node_type;
    int num_deps;
    std::vector<id_t> deps;

    nt_packet_t(id_t& id);

    friend std::ostream & operator <<(std::ostream & os, const nt_packet_t& p);


};

#endif //RAWTOCV_STRUCTURES_H
