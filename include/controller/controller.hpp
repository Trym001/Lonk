//
// Created by Bruker on 02.12.2022.
//

#ifndef LONK_02_CONTROLLER_HPP
#define LONK_02_CONTROLLER_HPP


class controller {
public:


    // src - https://www.geeksforgeeks.org/modulo-operations-in-programming-with-negative-results/
    // floorMod declaration
    static int floor_mod(int a, int n);
    // p_controller declaration
    [[nodiscard]] int p_controller(const int* leftSens, const int* rightSens, int heading);
    // pi_controller declaration
    [[nodiscard]] int pi_controller(const int* leftSens, const int* rightSens, int heading);

    void diff_converting(int leftSens, int rightSens);

private:
    float p_ = 6.42f;
    float i_ = 0.1f;
    //double d_ = 0.1;

    float diff_{};
};


#endif //LONK_02_CONTROLLER_HPP
