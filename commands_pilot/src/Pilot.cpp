//
// Created by alberto on 4/09/19.
//

#include <tf/LinearMath/Quaternion.h>
#include "Pilot.h"
#include <geometry_msgs/Pose2D.h>
#include <tf/LinearMath/Matrix3x3.h>
#include <fstream>

Pilot::Pilot(int argc, char **argv) {
    Pilot::posX, Pilot::posY, Pilot::turnZ= 0;
    Pilot::currentCommand = 0;
    Pilot::flag_init = 1;
    Pilot::heading = Pilot::Dir::UP;

    Pilot::readCommands();

    ros::init(argc, argv, "pilot");

    ros::NodeHandle nh;

    Pilot::cmdVelPublisher = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    Pilot::odomSubscriber = nh.subscribe("/odom", 1, &Pilot::odomCallback, this);

    ros::spin();
}

int Pilot::getYaw(const nav_msgs::Odometry::ConstPtr &msg) {
    tf::Quaternion q(msg->pose.pose.orientation.x, msg->pose.pose.orientation.y, msg->pose.pose.orientation.z, msg->pose.pose.orientation.w);
    tf::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);
    return (int)((yaw + M_PI)*180/M_PI);
}

void Pilot::goForward() {
    geometry_msgs::Twist cmd;
    cmd.linear.x = 0.2;
    cmd.angular.z = 0.0;
    Pilot::cmdVelPublisher.publish(cmd);
}

void Pilot::turnLeft() {
    geometry_msgs::Twist cmd;
    cmd.linear.x = 0.0;
    cmd.angular.z = 0.3;
    Pilot::cmdVelPublisher.publish(cmd);
}

void Pilot::turnRight() {
    geometry_msgs::Twist cmd;
    cmd.linear.x = 0.0;
    cmd.angular.z = -0.3;
    Pilot::cmdVelPublisher.publish(cmd);
}

void Pilot::stop() {
    geometry_msgs::Twist cmd;
    cmd.linear.x = 0.0;
    cmd.angular.z = 0.0;
    Pilot::cmdVelPublisher.publish(cmd);
}

void Pilot::odomCallback(const nav_msgs::Odometry::ConstPtr &msg) {
    if(!Pilot::commands.empty()) {
        // Go forward command.
        if (Pilot::commands.front() == "FORWARD") {
            if (Pilot::flag_init == 1) {
                ROS_INFO("FORWARD");
                Pilot::posX = msg->pose.pose.position.x;
                Pilot::posY = msg->pose.pose.position.y;
                Pilot::flag_init = 0;
            }

            Pilot::goForward();

            // If advanced one unit...
            if (fabs(posX - msg->pose.pose.position.x) > 0.9 || fabs(posY - msg->pose.pose.position.y) > 0.9) {
                Pilot::flag_init = 1;
                Pilot::commands.pop();
            }
        }

        //Turn left command.
        if (Pilot::commands.front() == "LEFT") {
            if (Pilot::flag_init == 1) {
                ROS_INFO("LEFT");
                Pilot::updateHeading(true);
                Pilot::flag_init = 0;
            }

            int yaw = Pilot::getYaw(msg);
            Pilot::turnLeft();

            // If end turning...
            if ((int) yaw == (int) Pilot::turnZ) {
                Pilot::flag_init = 1;
                Pilot::commands.pop();
            }
        }

        //Turn right command.
        if (Pilot::commands.front() == "RIGHT") {
            if (Pilot::flag_init == 1) {
                ROS_INFO("RIGHT");
                Pilot::updateHeading(false);
                Pilot::flag_init = 0;
            }

            int yaw = Pilot::getYaw(msg);
            Pilot::turnRight();

            // If end turning...
            if ((int) yaw == (int) Pilot::turnZ) {
                Pilot::flag_init = 1;
                Pilot::commands.pop();
            }
        }
    } else {
        Pilot::stop();
    }
}

void Pilot::readCommands() {
    std::ifstream file("commands.txt");
    std::string line;
    if (file.is_open()){
        while ( std::getline (file,line) )
        {
            Pilot::commands.push(line);
        }
        file.close();
    }
}

void Pilot::updateHeading(bool left){
    if(left){
        switch(Pilot::heading){
            case Pilot::Dir::UP:
                Pilot::turnZ = 270;
                Pilot::heading = Pilot::Dir::LEFT;
                break;
            case Pilot::Dir::LEFT:
                Pilot::turnZ = 0;
                Pilot::heading = Pilot::Dir::DOWN;
                break;
            case Pilot::Dir::DOWN:
                Pilot::turnZ = 90;
                Pilot::heading = Pilot::Dir::RIGHT;
                break;
            case Pilot::Dir::RIGHT:
                Pilot::turnZ = 180;
                Pilot::heading = Pilot::Dir::UP;
                break;
        }
    } else {
        switch(Pilot::heading){
            case Pilot::Dir::UP:
                Pilot::turnZ = 90;
                Pilot::heading = Pilot::Dir::RIGHT;
                break;
            case Pilot::Dir::LEFT:
                Pilot::turnZ = 180;
                Pilot::heading = Pilot::Dir::UP;
                break;
            case Pilot::Dir::DOWN:
                Pilot::turnZ = 270;
                Pilot::heading = Pilot::Dir::LEFT;
                break;
            case Pilot::Dir::RIGHT:
                Pilot::turnZ = 0;
                Pilot::heading = Pilot::Dir::DOWN;
                break;
        }

    }
}
