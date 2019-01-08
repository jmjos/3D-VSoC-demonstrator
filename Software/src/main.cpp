#include <systemc.h>

#include "Top.h"
#include "VSoC.h"

int sc_main(int arg_num, char *arg_vec[]) {

    //Top top("top");
    VSoC vsoc("VSoC");

    sc_start();
    return 0;
}