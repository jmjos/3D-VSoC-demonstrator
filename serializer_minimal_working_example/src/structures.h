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
#include <iostream>

typedef int addr_t;
typedef int node_t;
typedef int data_t;
typedef long long id_type;

class Packet {
private:
    static id_type idcnt;
public:
    id_type id = 0;
    long long cyck = 0;
    data_t Datatyp = 0;
    addr_t addr_src = 0;
    addr_t dst = 0;
    node_t node_type = 0;
    int num_deps = 0;
    //std::vector<id_t> deps;

    Packet();

    friend std::ostream & operator <<(std::ostream & os, const Packet& p);
    friend bool operator==(const Packet& p1, const Packet& p2);
    friend bool operator!=(const Packet& p1, const Packet& p2);
};

#endif //RAWTOCV_STRUCTURES_H
