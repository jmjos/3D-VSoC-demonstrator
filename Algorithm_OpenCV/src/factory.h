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
    std::vector<Packet*> packets;
    static PacketFactory& getInstance();
    Packet* createPacket(id_t id);
    void deletePacket(Packet* p);
};

#endif //RAWTOCV_FACTORY_H
