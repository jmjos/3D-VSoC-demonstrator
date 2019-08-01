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
        Packet* createPacket(data_t addr_source, addr_t destination);
        void deletePacket(Packet* p);
};

#endif //RAWTOCV_FACTORY_H
