//
// Created by zaime on 12/3/2022.
//

#include "controller/system_timer.hpp"
#include "controller/controller.hpp"
#include <iostream>

int main(){
    controller controller;
    system_timer timer;

    int a = 5;
    int b = 3;

    controller.floor_mod(a,b);
    timer.start();

    while(true){
        double t = timer.elapsedSeconds();
        std::cout << t << std::endl;
    }


}



