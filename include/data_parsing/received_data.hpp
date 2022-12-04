//
// Created by Bruker on 04.12.2022.
//

#ifndef LONK_RECEIVED_DATA_HPP
#define LONK_RECEIVED_DATA_HPP
struct received_data
{
    struct distSensor_
    {
        int front;
        int left;
        int right;
    };

    distSensor_ distSensor;
};
#endif //LONK_RECEIVED_DATA_HPP
