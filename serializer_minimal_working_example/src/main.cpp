//
// Created by joseph on 01.07.19.
//

#include "structures.h"

#include <iostream>

int main (int argc, char *argv[]) {
    Packet p2;
    Packet p3;
    Packet p4;
    Packet p1;
    p1.addr_src = 100;
    p1.dst = 99;
    p1.data.emplace_back(0b11);
    std::cout << p1 << std::endl;
    for (auto i : p1.data){
        std::cout << i << std::endl;
    }

    std::ofstream ofs("filename");
    {
        boost::archive::text_oarchive oa(ofs);
        oa << p1;
    }

    Packet newP;
    {
        std::ifstream ifs("filename");
        boost::archive::text_iarchive ia(ifs);
        ia >> newP;
    }

    std::cout << newP << std::endl;

    for (auto i : newP.data){
        std::cout << i << std::endl;
    }
}