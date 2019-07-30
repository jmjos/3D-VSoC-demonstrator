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
#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

typedef int addr_t;
typedef int node_t;
typedef int data_t;
typedef long long id_type;

class Packet {
private:
    static id_type idcnt;
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & id;
        ar & cyck;
        ar & datatyp;
        ar & addr_src;
        ar & dst;
        ar & node_type;
        ar & num_deps;
        ar & deps;
        ar & data;
    }

public:
    id_type id = 0;
    long long cyck = 0;
    data_t datatyp = 0;
    addr_t addr_src = 0;
    addr_t dst = 0;
    node_t node_type = 0;
    int num_deps = 0;
    std::vector<id_t> deps;
    std::vector<char> data;

    Packet();

    friend std::ostream & operator <<(std::ostream & os, const Packet& p);
    friend bool operator==(const Packet& p1, const Packet& p2);
    friend bool operator!=(const Packet& p1, const Packet& p2);
};

// How to serialize:
//https://isocpp.org/wiki/faq/serialization#serialize-no-inherit-no-ptrs
//How do I serialize objects that contain pointers to other objects, but those pointers form a tree with no cycles and no joins?


#endif //RAWTOCV_STRUCTURES_H
