/*
 * Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
 * Copyright (C) 2006-2010 RobotCub Consortium
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#include <yarp/os/impl/NameserCarrier.h>
#include <yarp/os/ConstString.h>

using namespace yarp::os;
using namespace yarp::os::impl;

// just to keep linkers from complaining about empty archive
bool dummyNameserCarrierMethod() {
    return false;
}

yarp::os::impl::NameserTwoWayStream::NameserTwoWayStream(TwoWayStream *delegate) :
        delegate(delegate),
        pendingRead(""),
        swallowRead("VER ") {
}

yarp::os::impl::NameserTwoWayStream::~NameserTwoWayStream() {
    if (delegate!=nullptr) {
        delete delegate;
        delegate = nullptr;
    }
}

yarp::os::InputStream& yarp::os::impl::NameserTwoWayStream::getInputStream() {
    return *this;
}

yarp::os::OutputStream& yarp::os::impl::NameserTwoWayStream::getOutputStream() {
    return delegate->getOutputStream();
}

const Contact& yarp::os::impl::NameserTwoWayStream::getLocalAddress() {
    return delegate->getLocalAddress();
}

const Contact& yarp::os::impl::NameserTwoWayStream::getRemoteAddress() {
    return delegate->getRemoteAddress();
}

bool yarp::os::impl::NameserTwoWayStream::isOk() {
    return delegate->isOk();
}

void yarp::os::impl::NameserTwoWayStream::reset() {
    delegate->reset();
}

void yarp::os::impl::NameserTwoWayStream::close() {
    delegate->close();
}

void yarp::os::impl::NameserTwoWayStream::beginPacket() {
    delegate->beginPacket();
}

void yarp::os::impl::NameserTwoWayStream::endPacket() {
    delegate->endPacket();
}

YARP_SSIZE_T yarp::os::impl::NameserTwoWayStream::read(const Bytes& b) {
    // assume it is ok for name_ser to go byte-by-byte
    // since this protocol will be phased out
    if (b.length()<=0) {
        return 0;
    }
    Bytes tmp(b.get(), 1);
    while (swallowRead.length()>0) {
        YARP_SSIZE_T r = delegate->getInputStream().read(tmp);
        if (r<=0) { return r; }
        swallowRead = swallowRead.substr(1, swallowRead.length()-1);
    }
    if (pendingRead.length()>0) {
        b.get()[0] = pendingRead[0];
        pendingRead = pendingRead.substr(1, pendingRead.length()-1);
        return 1;
    }
    YARP_SSIZE_T r = delegate->getInputStream().read(tmp);
    if (r<=0) { return r; }
    if (tmp.get()[0]=='\n') {
        pendingRead = "";
        swallowRead = "NAME_SERVER ";
    }
    return r;
}


yarp::os::impl::NameserCarrier::NameserCarrier() {
    firstSend = true;
}

yarp::os::ConstString yarp::os::impl::NameserCarrier::getName() {
    return "name_ser";
}

yarp::os::ConstString yarp::os::impl::NameserCarrier::getSpecifierName() {
    return "NAME_SER";
}

yarp::os::Carrier *yarp::os::impl::NameserCarrier::create() {
    return new NameserCarrier();
}

bool yarp::os::impl::NameserCarrier::checkHeader(const yarp::os::Bytes& header) {
    if (header.length()==8) {
        ConstString target = getSpecifierName();
        for (int i=0; i<8; i++) {
            if (!(target[i]==header.get()[i])) {
                return false;
            }
        }
        return true;
    }
    return false;
}

void yarp::os::impl::NameserCarrier::getHeader(const Bytes& header) {
    if (header.length()==8) {
        ConstString target = getSpecifierName();
        for (int i=0; i<8; i++) {
            header.get()[i] = target[i];
        }
    }
}


bool yarp::os::impl::NameserCarrier::requireAck() {
    return false;
}

bool yarp::os::impl::NameserCarrier::isTextMode() {
    return true;
}

bool yarp::os::impl::NameserCarrier::supportReply() {
    return true;
}

bool yarp::os::impl::NameserCarrier::canEscape() {
    return false;
}

bool yarp::os::impl::NameserCarrier::sendHeader(ConnectionState& proto) {
    yarp::os::ConstString target = getSpecifierName();
    yarp::os::Bytes b((char*)target.c_str(), 8);
    proto.os().write(b);
    proto.os().flush();
    return proto.os().isOk();
}

bool yarp::os::impl::NameserCarrier::expectSenderSpecifier(ConnectionState& proto) {
    Route route = proto.getRoute();
    route.setFromName("anon");
    proto.setRoute(route);
    return true;
}

bool yarp::os::impl::NameserCarrier::expectIndex(ConnectionState& proto) {
    YARP_UNUSED(proto);
    return true;
}

bool yarp::os::impl::NameserCarrier::sendAck(ConnectionState& proto) {
    YARP_UNUSED(proto);
    return true;
}

bool yarp::os::impl::NameserCarrier::expectAck(ConnectionState& proto) {
    YARP_UNUSED(proto);
    return true;
}

bool yarp::os::impl::NameserCarrier::respondToHeader(ConnectionState& proto) {
    // I am the receiver
    NameserTwoWayStream *stream =
        new NameserTwoWayStream(proto.giveStreams());
    proto.takeStreams(stream);
    return true;
}

bool yarp::os::impl::NameserCarrier::expectReplyToHeader(ConnectionState& proto) {
    YARP_UNUSED(proto);
    // I am the sender
    return true;
}

bool yarp::os::impl::NameserCarrier::write(ConnectionState& proto, SizedWriter& writer) {
    ConstString target = firstSend?"VER ":"NAME_SERVER ";
    Bytes b((char*)target.c_str(), target.length());
    proto.os().write(b);
    ConstString txt;
    // ancient nameserver can't deal with quotes
    for (size_t i=0; i<writer.length(); i++) {
        for (size_t j=0; j<writer.length(i); j++) {
            char ch = writer.data(i)[j];
            if (ch!='\"') {
                txt += ch;
            }
        }
    }
    Bytes b2((char*)txt.c_str(), txt.length());
    proto.os().write(b2);
    proto.os().flush();
    firstSend = false;
    return proto.os().isOk();
}
