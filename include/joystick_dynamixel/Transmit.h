#ifndef TRANSMIT_H
#define TRANSMIT_H

#include <iostream>
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Joy.h"
#include <vector>
#include <qt5/QtCore/QObject>
#include <qt5/QtNetwork/QUdpSocket>
#include <qt5/QtCore/QCoreApplication>
#include <cstdlib>
#include <cmath>

using namespace std;

class Transmit
{

private:
    QUdpSocket sender_;
    ros::NodeHandle *n;
    ros::Subscriber listener;
    int mode;
    int counter;
    int speed;

    struct Joystick
    {
        float axes_1;
        float axes_2;
        float axes_5;
        float axes_7;
        int button_A;
        int button_Y;
        int power;
        int button_Home;
        bool direction;
    };

    Joystick js;

public:
    explicit Transmit();
    void joyCallBack(const sensor_msgs::Joy::ConstPtr &joy_msg);
    void switchMode();

    void setMode(int value);
    int getMode();

    void setCounter(int value);
    int getCounter();

    void setSpeed(int value);
    int getSpeed();


signals:
public slots:

    void socketWriter();
};

#endif // TRANSMIT_H
