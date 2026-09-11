#include "battery.h"

uint8_t get_battery_percentage(uint16_t voltage){
    
    if(voltage > 4450){
        return 100;
    }else if(voltage > 4300){
        return 90;
    }else if(voltage > 4200){
        return 80;
    }else if(voltage > 4080){
        return 70;
    }else if(voltage > 3980){
        return 60;
    }else if(voltage > 3880){
        return 50;
    }else if(voltage > 3820){
        return 40;
    }else if(voltage > 3780){
        return 30;
    }else if(voltage > 3720){
        return 20;
    }else if(voltage > 3650){
        return 10;
    }else{
        return 0;
    }
}