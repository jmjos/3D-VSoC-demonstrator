//
// Created by mtzschoppe on 01/08/19.
//

#include "factory.h"

PacketFactory* PacketFactory::getInstance() //only one instance of PacketFactory
{
    static PacketFactory instance;
    return &instance;
}

PacketFactory::PacketFactory() {}

Packet* PacketFactory::createPacket()
{
    auto p = new Packet;
    packets.push_back(p);
    return p;
}

void PacketFactory::deletePacket(Packet* p)
{
    auto it = std::find(packets.begin(), packets.end(), p);
    delete(*it);
    packets.erase(it);
}

