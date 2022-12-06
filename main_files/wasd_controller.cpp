#include <iostream>
#include "other/wasd_controller.hpp"

int main(){
    int speed = 0;
    int heading = 0;
    controller ctrl;
    while(true){
        char input;
        std::cin>>input;

        ctrl.calculate(input);

        std::cout << "speed:"<< ctrl.speed() << " heading:" << ctrl.heading() << std::endl;
    }

    return 0;
}