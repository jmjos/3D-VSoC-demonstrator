//
// Created by joseph on 14/12/18.
//

#ifndef SIM_TOP_H
#define SIM_TOP_H


#include "Initiator.h"
#include "Target.h"

SC_MODULE(Top)
        {
                Initiator *initiator;
                Memory    *memory;

                SC_CTOR(Top)
                {
                    // Instantiate components
                    initiator = new Initiator("initiator");
                    memory    = new Memory   ("memory");

                    // One initiator is bound directly to one target with no intervening bus

                    // Bind initiator socket to target socket
                    initiator->socket.bind( memory->socket );
                }
        };

#endif //SIM_TOP_H
