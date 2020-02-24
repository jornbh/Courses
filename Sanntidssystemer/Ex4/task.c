#include "stdlib.h"
#include "stdio.h"
struct responseTaskArgs {
struct {
    uint32_t test;
    uint32_t response;
} pin;
// other args ...
};
static void responseTask(void* args){
struct responseTaskArgs a = *(struct responseTaskArgs*)args;
    while(1){        
        if(gpio_pin_is_low(a.pin.test)){
            // ...
        }        
    }
}
int main(){
...
xTaskCreate(responseTask, "", 1024, 
(&(struct responseTaskArgs){{TEST_A, RESPONSE_A}, other args...}), 
tskIDLE_PRIORITY + 1, NULL);
}