#include "main.h"
#include "Task/Init.h"

#include <library.h>
#include <parallel/RTOS.h>

int main() {
    initHAL();
    library::init();

    initAMS_Shield();

    RTOS::start();
}
