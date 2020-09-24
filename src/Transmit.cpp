#include "joystick_dynamixel/Transmit.h"

Transmit::Transmit()
{
    n = new ros::NodeHandle("~");

    sender_.bind(QHostAddress("192.168.10.115"), 1001);

    setMode(1);

    listener = n->subscribe("/joy", 10000, &Transmit::joyCallBack, this);

    setCounter(0);
}

void Transmit::setCounter(int value)
{
    counter = value;
}

int Transmit::getCounter()
{
    return counter;
}

void Transmit::setMode(int value)
{
    mode = value;
}

int Transmit::getMode()
{
    return mode;
}

void Transmit::setSpeed(int value)
{
    speed = value;
}

int Transmit::getSpeed()
{
    return speed;
}

void Transmit::joyCallBack(const sensor_msgs::Joy::ConstPtr &joy_msg)
{
    if (joy_msg->buttons[7])
    {
        setSpeed(0);
        socketWriter();
        ros::shutdown();
        return;
    }
    js.button_Home = joy_msg->buttons[8];

    switch (getMode())
    {
    //----------------------------------------------------
    case 1:

        ROS_WARN("Now you are controling dynamixel with left joystick.");
        js.axes_1 = joy_msg->axes[1];

        if (js.axes_1 < 0)
        {
            js.direction = false; //Counter Clockwise
            setSpeed((js.axes_1) * (-1023));
        }
        else
        {
            js.direction = true; //Round Clock
            setSpeed((js.axes_1) * (1023));
        }
        socketWriter();
        switchMode();
        break;
    //----------------------------------------------------
    case 2:
        ROS_WARN("Now you are controling dynamixel with RT & LT.");
        js.axes_5 = (((joy_msg->axes[5]) - 1) * -1023) / 2;
        js.axes_2 = (((joy_msg->axes[2]) - 1) * -1023) / 2;

        if ((floor(js.axes_2) > 0))
        {
            js.direction = false;
            setSpeed(js.axes_2);
        }
        else if (floor(js.axes_5) > 0)
        {
            js.direction = true;
            setSpeed(js.axes_5);
        }

        else if ((js.axes_5 != 0) && (js.axes_2 != 0))
        {
            if (js.axes_2 > js.axes_5)
            {
                js.direction = false;
                setSpeed((js.axes_2) - (js.axes_5));
            }
            else
            {
                js.direction = true;
                setSpeed((js.axes_5) - (js.axes_2));
            }
        }
        else
        {
            setSpeed(0);
        }
        socketWriter();
        switchMode();
        break;
    //----------------------------------------------------
    case 3:
        ROS_WARN("Now you are controling dynamixel with A & Y , You can speed up by pressing up and down buttons.");
        js.button_A = joy_msg->buttons[0];
        js.button_Y = joy_msg->buttons[3];
        js.axes_7 = joy_msg->axes[7];

        if ((js.button_A = 1))
        {
            js.direction = false;
        }
        else if ((js.button_Y = 1))
        {
            js.direction = true;
        }

        if (js.axes_7 == 1)
        {
            setCounter(getCounter() + 1);
            setSpeed(getCounter());
            if (getSpeed() >= 1023)
            {
                setSpeed(1023);
                cout << "You are at Maximum speed." << endl;
            }
        }
        else if (js.axes_7 == -1)
        {
            setCounter(getCounter() - 1);
            setSpeed(getCounter());
            if (getSpeed() <= 0)
            {
                setSpeed(0);
                cout << "Motor is stopped." << endl;
            }
        }

        socketWriter();
        switchMode();
        break;
    }

}

void Transmit::switchMode()
{
    if (js.button_Home == 1 && getMode() == 1)
    {
        setMode(2);
    }
    else if (js.button_Home == 1 && getMode() == 2)
    {
        setMode(3);
    }
    else if (js.button_Home == 1 && getMode() == 3)
    {
        setMode(1);
    }
}

void Transmit::socketWriter()
{

    QByteArray Data;

    qDebug() << "Sending joystick data to Dynamixel RX-64 ..." << endl;

    if (js.direction == true)
    {
        Data[0] = (2);
    }
    else
    {
        Data[0] = (1);
    }

    Data[1] = (getSpeed());
    Data[2] = (getSpeed() >> 8);

    sender_.writeDatagram(Data, QHostAddress("192.168.10.115"), 1001);

    qDebug() << "Now dynamixel is moving with " << floor((getSpeed() * (0.0978))) << "% of its power." << endl;
}
