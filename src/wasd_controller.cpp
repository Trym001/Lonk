//
// Created by Dawid_Win on 05-Dec-22.
//

#include "other/wasd_controller.hpp"
#include <iostream>

void controller::calculate(char &input) {
    if (input == 'w') {
        speed_ = speed_ + 20;
    }
    else if (input == 's') {
        speed_ = 0;
        speed_ = std::max(0,speed_);
    }
    else if (input == 'a') {
        heading_ = heading_ - 90;
        heading_ = heading_ % 360;
    }
    else if (input == 'd') {
        heading_ = heading_ + 90;
        heading_ = heading_ % 360;
    }
};

int controller::speed(){
        return speed_;
    }

int controller::heading(){
    return heading_;
}

