#include "../include/rtt-jr3-ft-sensor-component.hpp"
#include <rtt/Component.hpp>
#include <iostream>

#include <fcntl.h>
#include <sys/ioctl.h>

#define LOG_ERROR(msg) RTT::log(RTT::Error) << msg << RTT::endlog()
#define LOG_INFO(msg)  RTT::log(RTT::Info)  << msg << RTT::endlog()

#define FORCE 0
#define TORQUE 1

RTT_JR3::RTT_JR3(std::string const& name) : TaskContext(name)
{

    // output port
    wrench_output_port.setName("ForceTorqueOutputPort");
    wrench_output_port.setDataSample(wrench_message);
    ports()->addPort(wrench_output_port).doc("Output port for sending force and torque values.");

    //path to sensor file
    path_to_file = "/dev/jr3";
    addProperty("path_to_sensor_file",path_to_file).doc("Name of the file that sensor creates.");

    addOperation("print", &RTT_JR3::print, this).doc("Prints the last sensor data to info log.");
    addOperation("calibrate", &RTT_JR3::calibrate, this).doc("Calibrates the sensor.");
}

bool RTT_JR3::configureHook()
{
    // open sensor
    fd = open(path_to_file.c_str(), O_RDWR);
    if (fd < 0)
    {
        LOG_ERROR("Couldn't open sensor file!");
        return false;
    }

    // get full scale
    ret = ioctl(fd, IOCTL0_JR3_GET_FULL_SCALES, &full_scale);
    if (ret < 0)
    {
        LOG_ERROR("Couldn't get full scales!");
        close(fd);
        return false;
    }

    // calibrate
    ioctl(fd, IOCTL0_JR3_ZEROOFFS);

    LOG_INFO("Force/Torque sensor configured!");
    return true;
}

bool RTT_JR3::startHook()
{
    return true;
}

void RTT_JR3::updateHook()
{
    // get sensor data
    ret = ioctl(fd, IOCTL0_JR3_FILTER0, &force_torque);

    if (ret < 0)
    {
        LOG_ERROR("Couldn't get data!");
    }
    else
    {
        wrench_message.forces[0]  = getSensorValue(FORCE, 0);
        wrench_message.forces[1]  = getSensorValue(FORCE, 1);
        wrench_message.forces[2]  = getSensorValue(FORCE, 2);
        wrench_message.torques[0] = getSensorValue(TORQUE, 0);
        wrench_message.torques[1] = getSensorValue(TORQUE, 1);
        wrench_message.torques[2] = getSensorValue(TORQUE, 2);

        // send data
        wrench_output_port.write(wrench_message);
    }
}

void RTT_JR3::stopHook()
{
}

void RTT_JR3::cleanupHook()
{
    if (fd > 0) close(fd);
}

bool RTT_JR3::calibrate()
{
    ret = ioctl(fd, IOCTL0_JR3_ZEROOFFS);

    if (ret < 0)
    {
        LOG_ERROR("Couldn't calibrate the force/torque sensor!");
        return false;
    }
    else
    {
        LOG_INFO("Succesfully calibrated force/torque sensor.");
        return true;
    }
}

void RTT_JR3::print()
{
    LOG_INFO(" Force: ");
    LOG_INFO("     x: " << getSensorValue(FORCE, 0));
    LOG_INFO("     y: " << getSensorValue(FORCE, 1));
    LOG_INFO("     z: " << getSensorValue(FORCE, 2));
    LOG_INFO("Torque:");
    LOG_INFO("     x: " << getSensorValue(TORQUE, 0));
    LOG_INFO("     y: " << getSensorValue(TORQUE, 1));
    LOG_INFO("     z: " << getSensorValue(TORQUE, 2));
}

float RTT_JR3::getSensorValue(int type, int index, int magic)
{
    switch(type)
    {
    case FORCE:
        return ((float)force_torque.f[index] * (float)full_scale.f[index] / (float)magic);

    case TORQUE:
        return ((float)force_torque.m[index] * (float)full_scale.m[index] / (float)magic);

    default:
        return 0;
    }
}

/*
 * Using this macro, only one component may live
 * in one library *and* you may *not* link this library
 * with another component library. Use
 * ORO_CREATE_COMPONENT_TYPE()
 * ORO_LIST_COMPONENT_TYPE(Mytest)
 * In case you want to link with another library that
 * already contains components.
 *
 * If you have put your component class
 * in a namespace, don't forget to add it here too:
 */
ORO_CREATE_COMPONENT(RTT_JR3)
