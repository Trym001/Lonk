//
// Created by Dawid_Win on 05-Dec-22.
//

#ifndef WASD_CONTROLLER_HPP
#define WASD_CONTROLLER_HPP

struct controller{
public:
    controller() : speed_(0), heading_(0){}
    void calculate(char &input);
    int speed();
    int heading();
private:
    int speed_;
    int heading_;
};

#endif //WASD_CONTROLLER_HPP
