/*
 * TcpNewRenoLinux.h
 *
 *  Created on: Oct 11, 2020
 *      Author: Z0013039
 */

#ifndef TCPNEWRENOLINUX_H_
#define TCPNEWRENOLINUX_H_

#include "inet/common/INETDefs.h"
#include "inet/transportlayer/tcp/flavours/TcpTahoeRenoFamily.h"
#include "inet/transportlayer/tcp/flavours/TcpNewReno.h"

using namespace inet;
using namespace inet::tcp;


#define TCP_MAX_QUICKACKS   16 //MAX number of quick acks in Linux
#define TCP_INCREASED_INITIAL_WINDOW   65 // 10 /* TCP initial congestion window as per rfc6928 */
#define DELAYED_ACK_TIMEOUT    0.2   // 200ms (RFC 1122: MUST be less than 0.5 seconds): redefining from TcpBaseAlg.cc
#define MIN_SEGMENT_FOR_DELAYED_ACK    2
#define MIN_LINUX_REXMIT_TIMEOUT     0.05   // 50ms
#define MAX_PERSIST_TIMEOUT    60   // 60s // This is just borrowed from TcpBaseAlgo

class TcpNewRenoLinuxStateVariables : public TcpTahoeRenoFamilyStateVariables
{
public:
    TcpNewRenoLinuxStateVariables();
    ~TcpNewRenoLinuxStateVariables();

    int tcp_quick_ack_counter = 0;    // Keep track of number of quick ACKs
};

/**
 * Implements TCP NewReno and customize it.
 * Initial congestion window size and delayed ACK algo is updated as per current linux
 */
class TcpNewRenoLinux : public TcpNewReno
{
protected:
    //int const TCP_MAX_QUICKACKS = 16;
    //int const TCP_INCREASED_INITIAL_WINDOW = 10;
    TcpNewRenoLinuxStateVariables*& state;    // alias to TcpAlgorithm's 'state'

    /** Create and return a TcpNewRenoStateVariables object. */
    virtual TcpStateVariables* createStateVariables() override
    {
        return new TcpNewRenoLinuxStateVariables();
    }

    virtual void processPersistTimer(TcpEventCode& event) override;

public:
    /** Ctor */
    TcpNewRenoLinux();

    virtual void established(bool active) override;
    virtual void receiveSeqChanged() override;
    virtual void ackSent() override;

};



#endif /* TCPNEWRENOLINUX_H_ */
