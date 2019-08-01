//
// Created by mtzschoppe on 22/05/19.
//

#ifndef RAWTOCV_FACTORY_H
#define RAWTOCV_FACTORY_H

//#pragma once

#include <vector>
#include <structures.h>

class PacketFactory {
private:
    PacketFactory();

public:
    static PacketFactory* getInstance();

    std::vector<Packet*> packets;
    Packet* createPacket();
    void deletePacket(Packet* p);
};

#endif //RAWTOCV_FACTORY_H
