/** @file
 * Functions for communicating and exchanging amplitudes between compute
 * nodes, when running in distributed mode, using the C MPI standard.
 */

#ifndef COMM_ROUTINES_HPP
#define COMM_ROUTINES_HPP

#include "quest/include/types.h"
#include "quest/include/qureg.h"



/*
 * STATE EXCHANGE METHODS
 */

void comm_exchangeAmpsToBuffers(Qureg qureg, qindex sendInd, qindex recvInd, qindex numAmps, int pairRank);

void comm_exchangeAmpsToBuffers(Qureg qureg, int pairRank);

void comm_exchangeBuffers(Qureg qureg, qindex numAmpsAndRecvInd, int pairRank);

void comm_asynchSendBuffer(Qureg qureg, qindex numElems, int pairRank);

void comm_receiveArrayToBuffer(Qureg qureg, qindex numElems, int pairRank);



/*
 * REDUCTION METHODS
 */

void comm_reduceAmp(qcomp* localAmp);



#endif // COMM_ROUTINES_HPP