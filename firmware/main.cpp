#include "mbed.h"
#include "mDot.h"
#include "select_project.h"

#if PROJECT == 1
    mDot* dot = NULL;
    #include "1_blinky/main.h"
#elif PROJECT == 2
    mDot* dot = NULL;
    #include "2_dust_sensor/main.h"
#elif PROJECT == 3
    #include "3_over_lora/main.h"
#else
    #error "No project selected, see select_project.h"
#endif
