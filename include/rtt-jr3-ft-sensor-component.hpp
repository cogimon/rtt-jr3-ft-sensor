#ifndef OROCOS_MYTEST_COMPONENT_HPP
#define OROCOS_MYTEST_COMPONENT_HPP

#include <rtt/RTT.hpp>
#include <rst-rt/dynamics/Wrench.hpp>
#include <rtt/Port.hpp>

// part of the jr3 driver. used to access the sensor
#include "jr3pci-ioctl.h"

class RTT_JR3 : public RTT::TaskContext{
public:
    //default methods
    RTT_JR3(std::string const& name);
    bool configureHook();
    bool startHook();
    void updateHook();
    void stopHook();
    void cleanupHook();

private:
    bool calibrate(); // calibrates the sensor
    void print(); // prints last sensor values to info log
    float getSensorValue(int type, int index, int magic = 16384); // gets a single adjusted sensor value. force_torque and full_scale have to be set!

    std::string path_to_file;
    RTT::OutputPort<rstrt::dynamics::Wrench> wrench_output_port;

    rstrt::dynamics::Wrench wrench_message;
    six_axis_array force_torque;      // used for getting force and torque
    force_array full_scale; // used for getting full scales
    int fd, ret;            // file descriptor and return value
};
#endif
