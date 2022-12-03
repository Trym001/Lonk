//
// Created by Bruker on 02.12.2022.
//

#include "../include/controller/controller.hpp"
#include "controller/system_timer.hpp"

// floorMod constructor
int controller::floor_mod(int a, int n)
{
    // Type casting is necessary
    // as (int) / (int) will give
    // int result, i.e. -3 / 2
    // will give -1 and not -1.5
    int q = (int) floor((double) a / n);
    // Return the resultant remainder
    return a - n * q;
}


// p_controller constructor
int controller::p_controller(const int* leftSens, const int* rightSens, int heading)
{
    diff_converting(*leftSens, *rightSens);

    float u = -p_ * diff_;

    int newHeading = floor_mod((heading + int(u)), 360);

    return newHeading;
}


//pi_controller constructor
int controller::pi_controller(const int* leftSens, const int* rightSens, int heading)
{
    diff_converting(*leftSens, *rightSens);

    float u = -p_ * diff_;
    u = u + i_*diff_; //*fokkelg::elapsedMilliseconds();

    int newHeading = floor_mod((heading + int(u)), 360);

    return newHeading;
}

void controller::diff_converting(int leftSens, int rightSens) {
    leftSens = leftSens / 10;
    rightSens = rightSens / 10;

    diff_ = float(leftSens) - float(rightSens);
}
