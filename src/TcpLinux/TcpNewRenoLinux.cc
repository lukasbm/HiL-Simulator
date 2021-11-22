/*
 * TcpNewRenoLinux.cc
 *
 *  Created on: Oct 11, 2020
 *      Author: Z0013039
 */

#include <algorithm>    // min,max

#include "inet/transportlayer/tcp/Tcp.h"
#include "TcpNewRenoLinux.h"

using namespace inet;
using namespace tcp;

Register_Class(TcpNewRenoLinux);

TcpNewRenoLinuxStateVariables::TcpNewRenoLinuxStateVariables()// Max possible val
{
    //Initialise tcp_quick_ack_counter to 0
    tcp_quick_ack_counter = 0;
    rexmit_timeout = MIN_LINUX_REXMIT_TIMEOUT;
}

TcpNewRenoLinuxStateVariables::~TcpNewRenoLinuxStateVariables()
{
}

TcpNewRenoLinux::TcpNewRenoLinux() : TcpNewReno(),
state((TcpNewRenoLinuxStateVariables*&)TcpAlgorithm::state)
{
}

void TcpNewRenoLinux::established(bool active)
{
    TcpNewReno::established(active);
    /* TCP initial congestion window as per rfc6928 */
    // Override snd_cwnd as per the above specification
    state->snd_cwnd = TCP_INCREASED_INITIAL_WINDOW * state->snd_mss;
}

void TcpNewRenoLinux::receiveSeqChanged()
{
    //Borrow from the method implementation in TcpBaseAlg class
    // If we send a data segment already (with the updated seqNo) there is no need to send an additional ACK
    if (state->full_sized_segment_counter == 0 && !state->ack_now && state->last_ack_sent == state->rcv_nxt && !delayedAckTimer->isScheduled()) {    // ackSent?
        // tcpEV << "ACK has already been sent (possibly piggybacked on data)\n";
    }
    else {
        // RFC 2581, page 6:
        // "3.2 Fast Retransmit/Fast Recovery
        // (...)
        // In addition, a TCP receiver SHOULD send an immediate ACK
        // when the incoming segment fills in all or part of a gap in the
        // sequence space."
        if (state->lossRecovery)
            state->ack_now = true; // although not mentioned in [Stevens, W.R.: TCP/IP Illustrated, Volume 2, page 861] seems like we have to set ack_now

        // *** Adding the special validation of adaptive ACK algo of Linux
        if (!state->delayed_acks_enabled || state->tcp_quick_ack_counter < TCP_MAX_QUICKACKS) {    // delayed ACK disabled or/and if the total quick ACKs is less than 16
            EV_INFO << "rcv_nxt changed to " << state->rcv_nxt << ", (delayed ACK disabled) sending ACK now\n";
            conn->sendAck();
            state->tcp_quick_ack_counter++;
        }
        else {    // delayed ACK enabled
            if (state->ack_now) {
                EV_INFO << "rcv_nxt changed to " << state->rcv_nxt << ", (delayed ACK enabled, but ack_now is set) sending ACK now\n";
                conn->sendAck();
            }
            // RFC 1122, page 96: "in a stream of full-sized segments there SHOULD be an ACK for at least every second segment."
            //else if (state->full_sized_segment_counter >= 2) {
            else if (state->full_sized_segment_counter >= MIN_SEGMENT_FOR_DELAYED_ACK) {
                EV_INFO << "rcv_nxt changed to " << state->rcv_nxt << ", (delayed ACK enabled, but full_sized_segment_counter=" << state->full_sized_segment_counter << ") sending ACK now\n";
                conn->sendAck();
            }
            else {
                EV_INFO << "rcv_nxt changed to " << state->rcv_nxt << ", (delayed ACK enabled and full_sized_segment_counter=" << state->full_sized_segment_counter << ") scheduling ACK\n";
                if (!delayedAckTimer->isScheduled()) // schedule delayed ACK timer if not already running
                    conn->scheduleTimeout(delayedAckTimer, DELAYED_ACK_TIMEOUT);
            }
        }
    }
}

// Override the rexmit timer to Linux min RTO
void TcpNewRenoLinux::processPersistTimer(TcpEventCode& event)
{
    // setup and restart the PERSIST timer
    // FIXME Calculation of PERSIST timer is not as simple as done here!
    // It depends on RTT calculations and is bounded to 5-60 seconds.
    // This simplified PERSIST timer calculation generates values
    // as presented in [Stevens, W.R.: TCP/IP Illustrated, Volume 1, chapter 22.2]
    // (5, 5, 6, 12, 24, 48, 60, 60, 60...)
    if (state->persist_factor == 0)
        state->persist_factor++;
    else if (state->persist_factor < 64)
        state->persist_factor = state->persist_factor * 2;

    state->persist_timeout = state->persist_factor * 1.5;    // 1.5 is a factor for typical LAN connection [Stevens, W.R.: TCP/IP Ill. Vol. 1, chapter 22.2]

    // PERSIST timer is bounded to 5-60 seconds
    if (state->persist_timeout < MIN_LINUX_REXMIT_TIMEOUT)
        state->rexmit_timeout = MIN_LINUX_REXMIT_TIMEOUT;

    if (state->persist_timeout > MAX_PERSIST_TIMEOUT)
        state->rexmit_timeout = MAX_PERSIST_TIMEOUT;

    conn->scheduleTimeout(persistTimer, state->persist_timeout);

    // sending persist probe
    conn->sendProbe();

}

void TcpNewRenoLinux::ackSent()
{
    TcpNewReno::ackSent();
}



