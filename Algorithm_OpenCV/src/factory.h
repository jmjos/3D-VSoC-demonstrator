//
// Created by mtzschoppe on 22/05/19.
//

#ifndef RAWTOCV_FACTORY_H
#define RAWTOCV_FACTORY_H

//#pragma once

#include <vector>
#include <structures.h>

class PacketFactory {
public:
    std::vector<nt_packet_t*> packets;
    static PacketFactory& getInstance();
    nt_packet_t* createPacket(id_t id);
    void deletePacket(nt_packet_t* p);
};

#endif //RAWTOCV_FACTORY_H
