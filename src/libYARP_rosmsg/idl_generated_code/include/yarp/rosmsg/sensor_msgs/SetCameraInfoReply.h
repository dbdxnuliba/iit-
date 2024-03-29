/*
 * Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

// This is an automatically generated file.

// Generated from the following "sensor_msgs/SetCameraInfoReply" msg definition:
// Instances of this class can be read and written with YARP ports,
// using a ROS-compatible format.

#ifndef YARP_ROSMSG_sensor_msgs_SetCameraInfoReply_h
#define YARP_ROSMSG_sensor_msgs_SetCameraInfoReply_h

#include <yarp/os/Wire.h>
#include <yarp/os/idl/WireTypes.h>
#include <string>
#include <vector>

namespace yarp {
namespace rosmsg {
namespace sensor_msgs {

class SetCameraInfoReply : public yarp::os::idl::WirePortable
{
public:
    bool success;
    std::string status_message;

    SetCameraInfoReply() :
            success(false),
            status_message("")
    {
    }

    void clear()
    {
        // *** success ***
        success = false;

        // *** status_message ***
        status_message = "";
    }

    bool readBare(yarp::os::ConnectionReader& connection) override
    {
        // *** success ***
        if (!connection.expectBlock((char*)&success, 1)) {
            return false;
        }

        // *** status_message ***
        int len = connection.expectInt();
        status_message.resize(len);
        if (!connection.expectBlock((char*)status_message.c_str(), len)) {
            return false;
        }

        return !connection.isError();
    }

    bool readBottle(yarp::os::ConnectionReader& connection) override
    {
        connection.convertTextMode();
        yarp::os::idl::WireReader reader(connection);
        if (!reader.readListHeader(2)) {
            return false;
        }

        // *** success ***
        success = reader.expectInt();

        // *** status_message ***
        if (!reader.readString(status_message)) {
            return false;
        }

        return !connection.isError();
    }

    using yarp::os::idl::WirePortable::read;
    bool read(yarp::os::ConnectionReader& connection) override
    {
        return (connection.isBareMode() ? readBare(connection)
                                        : readBottle(connection));
    }

    bool writeBare(yarp::os::ConnectionWriter& connection) override
    {
        // *** success ***
        connection.appendBlock((char*)&success, 1);

        // *** status_message ***
        connection.appendInt(status_message.length());
        connection.appendExternalBlock((char*)status_message.c_str(), status_message.length());

        return !connection.isError();
    }

    bool writeBottle(yarp::os::ConnectionWriter& connection) override
    {
        connection.appendInt(BOTTLE_TAG_LIST);
        connection.appendInt(2);

        // *** success ***
        connection.appendInt(BOTTLE_TAG_INT);
        connection.appendInt((int)success);

        // *** status_message ***
        connection.appendInt(BOTTLE_TAG_STRING);
        connection.appendInt(status_message.length());
        connection.appendExternalBlock((char*)status_message.c_str(), status_message.length());

        connection.convertTextMode();
        return !connection.isError();
    }

    using yarp::os::idl::WirePortable::write;
    bool write(yarp::os::ConnectionWriter& connection) override
    {
        return (connection.isBareMode() ? writeBare(connection)
                                        : writeBottle(connection));
    }

    // This class will serialize ROS style or YARP style depending on protocol.
    // If you need to force a serialization style, use one of these classes:
    typedef yarp::os::idl::BareStyle<yarp::rosmsg::sensor_msgs::SetCameraInfoReply> rosStyle;
    typedef yarp::os::idl::BottleStyle<yarp::rosmsg::sensor_msgs::SetCameraInfoReply> bottleStyle;

    // Give source text for class, ROS will need this
    static yarp::os::ConstString typeText()
    {
        return yarp::os::ConstString("\
");
    }

    yarp::os::ConstString getTypeText() const
    {
        return yarp::rosmsg::sensor_msgs::SetCameraInfoReply::typeText();
    }

    // Name the class, ROS will need this
    yarp::os::Type getType() override
    {
        yarp::os::Type typ = yarp::os::Type::byName("sensor_msgs/SetCameraInfoReply", "sensor_msgs/SetCameraInfoReply");
        typ.addProperty("md5sum", yarp::os::Value("2ec6f3eff0161f4257b808b12bc830c2"));
        typ.addProperty("message_definition", yarp::os::Value(getTypeText()));
        return typ;
    }
};

} // namespace sensor_msgs
} // namespace rosmsg
} // namespace yarp

#endif // YARP_ROSMSG_sensor_msgs_SetCameraInfoReply_h
