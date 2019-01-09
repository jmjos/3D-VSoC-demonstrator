#include <systemc.h>

#include "Top.h"
#include "VSoC.h"

int sc_main(int arg_num, char *arg_vec[]) {

    VSoC vsoc("VSoC");

    sc_start(10, SC_SEC);
    return 0;
}