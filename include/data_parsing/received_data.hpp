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

    struct drivingData_
    {
        int speed;
        int heading;
    };

    struct imu_
    {
        int pitch;
        int roll;
        int yaw;
    };

    distSensor_     distSensor;
    drivingData_    drivingdata;
    imu_            imu;
};
#endif //LONK_RECEIVED_DATA_HPP
