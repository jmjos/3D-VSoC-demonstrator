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

struct Packet {
    static id_t id;

    Packet(id_t id);

    //~Packet();
};

#endif //RAWTOCV_STRUCTURES_H
