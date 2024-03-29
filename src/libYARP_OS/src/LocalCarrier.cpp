/*
 * Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
 * Copyright (C) 2006-2010 RobotCub Consortium
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#include <yarp/os/impl/LocalCarrier.h>
#include <yarp/os/Portable.h>
#include <yarp/os/impl/Logger.h>

using namespace yarp::os;

yarp::os::impl::LocalCarrierManager yarp::os::impl::LocalCarrier::manager;

yarp::os::impl::LocalCarrierManager::LocalCarrierManager() :
        senderMutex(1),
        receiverMutex(1),
        received(0),
        sender(nullptr),
        receiver(nullptr)
{
}

void yarp::os::impl::LocalCarrierManager::setSender(LocalCarrier *sender) {
    senderMutex.wait();
    this->sender = sender;
}

yarp::os::impl::LocalCarrier *yarp::os::impl::LocalCarrierManager::getReceiver() {
    received.wait();
    LocalCarrier *result = receiver;
    sender = nullptr;
    senderMutex.post();
    return result;
}

yarp::os::impl::LocalCarrier *yarp::os::impl::LocalCarrierManager::getSender(LocalCarrier *receiver) {
    receiverMutex.wait();
    this->receiver = receiver;
    LocalCarrier *result = sender;
    received.post();
    receiverMutex.post();
    return result;
}

void yarp::os::impl::LocalCarrierManager::revoke(LocalCarrier *carrier) {
    if (sender == carrier) {
        senderMutex.post();
    }
}


void yarp::os::impl::LocalCarrierStream::attach(LocalCarrier *owner, bool sender) {
    this->owner = owner;
    this->sender = sender;
    done = false;
}

InputStream& yarp::os::impl::LocalCarrierStream::getInputStream()
{
    return *this;
}

OutputStream& yarp::os::impl::LocalCarrierStream::getOutputStream()
{
    return *this;
}

const Contact& yarp::os::impl::LocalCarrierStream::getLocalAddress()
{
    return localAddress;
}

const Contact& yarp::os::impl::LocalCarrierStream::getRemoteAddress()
{
    return remoteAddress;
}

bool yarp::os::impl::LocalCarrierStream::setTypeOfService(int tos)
{
    YARP_UNUSED(tos);
    return true;
}

YARP_SSIZE_T yarp::os::impl::LocalCarrierStream::read(const yarp::os::Bytes& b)
{
    yAssert(false);
    return b.length();
}

void yarp::os::impl::LocalCarrierStream::write(const yarp::os::Bytes& b)
{
    YARP_UNUSED(b);
    yAssert(false);
}

void yarp::os::impl::LocalCarrierStream::reset()
{
}

void yarp::os::impl::LocalCarrierStream::beginPacket()
{
}

void yarp::os::impl::LocalCarrierStream::endPacket()
{
}

void yarp::os::impl::LocalCarrierStream::interrupt() {
    done = true;
}

void yarp::os::impl::LocalCarrierStream::close() {
    if (owner != nullptr) {
        LocalCarrier *owned = owner;
        owner = nullptr;
        owned->shutdown();
    }
    done = true;
}

bool yarp::os::impl::LocalCarrierStream::isOk() {
    return !done;
}


yarp::os::impl::LocalCarrier::LocalCarrier() : peerMutex(1), sent(0), received(0) {
    ref = nullptr;
    peer = nullptr;
    doomed = false;
}

yarp::os::impl::LocalCarrier::~LocalCarrier() {
    shutdown();
}

yarp::os::Carrier *yarp::os::impl::LocalCarrier::create() {
    return new LocalCarrier();
}

void yarp::os::impl::LocalCarrier::removePeer() {
    if (!doomed) {
        peerMutex.wait();
        peer = nullptr;
        peerMutex.post();
    }
}

void yarp::os::impl::LocalCarrier::shutdown() {
    if (!doomed) {
        doomed = true;
        peerMutex.wait();
        if (peer != nullptr) {
            peer->accept(nullptr);
            LocalCarrier *wasPeer = peer;
            peer = nullptr;
            wasPeer->removePeer();
        }
        peerMutex.post();
    }
}

yarp::os::ConstString yarp::os::impl::LocalCarrier::getName() {
    return "local";
}

bool yarp::os::impl::LocalCarrier::requireAck() {
    return false;
}

bool yarp::os::impl::LocalCarrier::isConnectionless() {
    return false;
}

bool yarp::os::impl::LocalCarrier::canEscape() {
    return false;
}

bool yarp::os::impl::LocalCarrier::isLocal() {
    return true;
}

yarp::os::ConstString yarp::os::impl::LocalCarrier::getSpecifierName() {
    return "LOCALITY";
}

bool yarp::os::impl::LocalCarrier::checkHeader(const Bytes& header) {
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

void yarp::os::impl::LocalCarrier::getHeader(const Bytes& header) {
    if (header.length()==8) {
        ConstString target = getSpecifierName();
        for (int i=0; i<8; i++) {
            header.get()[i] = target[i];
        }
    }
}

void yarp::os::impl::LocalCarrier::setParameters(const Bytes& header) {
    YARP_UNUSED(header);
}

bool yarp::os::impl::LocalCarrier::sendHeader(ConnectionState& proto) {
    portName = proto.getRoute().getFromName();

    manager.setSender(this);

    defaultSendHeader(proto);
    // now switch over to some local structure to communicate
    peerMutex.wait();
    peer = manager.getReceiver();
    //printf("sender %ld sees receiver %ld\n", (long int) this,
    //       (long int) peer);
    peerMutex.post();

    return true;
}

bool yarp::os::impl::LocalCarrier::expectExtraHeader(ConnectionState& proto) {
    portName = proto.getRoute().getToName();
    // switch over to some local structure to communicate
    peerMutex.wait();
    peer = manager.getSender(this);
    //printf("receiver %ld (%s) sees sender %ld (%s)\n",
    //       (long int) this, portName.c_str(),
    //       (long int) peer, peer->portName.c_str());
    Route route = proto.getRoute();
    route.setFromName(peer->portName);
    proto.setRoute(route);
    peerMutex.post();

    return true;
}

bool yarp::os::impl::LocalCarrier::becomeLocal(ConnectionState& proto, bool sender) {
    LocalCarrierStream *stream = new LocalCarrierStream();
    if (stream != nullptr) {
        stream->attach(this, sender);
    }
    proto.takeStreams(stream);
    //YARP_ERROR(Logger::get(), "*** don't trust local carrier yet ****");
    //exit(1);
    return true;
}

bool yarp::os::impl::LocalCarrier::write(ConnectionState& proto, SizedWriter& writer) {
    YARP_UNUSED(proto);
    yarp::os::Portable *ref = writer.getReference();
    if (ref != nullptr) {
        peerMutex.wait();
        if (peer != nullptr) {
            peer->accept(ref);
        } else {
            YARP_ERROR(Logger::get(),
                        "local send failed - write without peer");
        }
        peerMutex.post();
    } else {
        YARP_ERROR(Logger::get(),
                    "local send failed - no object");
    }

    return true;
}

bool yarp::os::impl::LocalCarrier::respondToHeader(ConnectionState& proto) {
    // i am the receiver

    return becomeLocal(proto, false);
}


bool yarp::os::impl::LocalCarrier::expectReplyToHeader(ConnectionState& proto) {
    // i am the sender

    return becomeLocal(proto, true);
}

bool yarp::os::impl::LocalCarrier::expectIndex(ConnectionState& proto) {

    YARP_DEBUG(Logger::get(), "local recv: wait send");
    sent.wait();
    YARP_DEBUG(Logger::get(), "local recv: got send");
    proto.setReference(ref);
    received.post();
    if (ref != nullptr) {
        YARP_DEBUG(Logger::get(), "local recv: received");
    } else {
        YARP_DEBUG(Logger::get(), "local recv: shutdown");
        proto.is().interrupt();
        return false;
    }

    return true;
}

void yarp::os::impl::LocalCarrier::accept(yarp::os::Portable *ref) {
    this->ref = ref;
    YARP_DEBUG(Logger::get(), "local send: send ref");
    sent.post();
    if (ref != nullptr && !doomed) {
        YARP_DEBUG(Logger::get(), "local send: wait receipt");
        received.wait();
        YARP_DEBUG(Logger::get(), "local send: received");
    }
}
