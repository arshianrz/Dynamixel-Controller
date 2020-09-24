#include "joystick_dynamixel/Transmit.h"

using namespace std;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "joystick_dynamixel");
    Transmit object;

    ros::spin();

    return 0;
}
