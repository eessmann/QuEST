
#include "QuEST.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>




QuESTEnv env;
Qureg qureg;



long double timeFuncCall(void (*func)(void)) {

    // start timing
    struct timeval timeInst;
    gettimeofday(&timeInst, NULL);
    long double startTime = (
		timeInst.tv_sec + (long double) timeInst.tv_usec/pow(10,6));
        
    func();
    
    // stop timing
    gettimeofday(&timeInst, NULL);
    long double endTime = (
        timeInst.tv_sec + (long double) timeInst.tv_usec/pow(10,6));
        
    return endTime - startTime;
}


void timeRepeatFuncCalls(
    void (*func)(void), int numReps, 
    long double *avDur, long double *varDur
) {    
    long double sumDur = 0;
    long double sumSquaredDur = 0;
    
    // disable line-buffering to indicate progress
    setbuf(stdout, NULL);
    
    // repeatedly time the test
    for (int i=0; i<numReps; i++) {
        
        // indicate progress (only if numReps>1)
        for (int percent=0; percent<=100 && (numReps>1); percent+=10)
            if (i == floor(percent * numReps / 100))
                if (env.rank == 0) {
                    printf("%d%% ", percent);
                    break;
                }
                    
        // perform test
        long double dur = timeFuncCall(func);
        sumDur += dur;
        sumSquaredDur += dur*dur;
    }
        
    // E[dur] = sum(dur)/numReps
    *avDur = sumDur/numReps;
    
    // Var[dur] = E[dur^2] - E[dur]^2 = sum(dur^2)/N - E[dur]^2
    *varDur = (sumSquaredDur/numReps) - (*avDur)*(*avDur);
}


void qft() {
    
    int numQubits = 10;
    int qubits[10] = {5,12,14,7,6,0,2,8,1,4};
    
    applyQFT(qureg, qubits, numQubits);
}




int main() {
    
    srand(123456);
    
    env = createQuESTEnv();
    qureg = createQureg(20, env);
    
    // all tests: 
    //  - average of 10 repetitions
    //  - random (unnormalised) state amplitudes in [.5, 1] (real & imag)
    //  - 
    
    // new 20 qub time:  (full state)
    //      0.65
    // old 20 qub time:
    //      0.79
    
    // {0,1,2,3,4,5,6,7,8,9}
    // new 10-qubits of 20qb register time:
    //      0.24
    // old 10-qubits of 20qb register time:
    //      0.31
    
    // {5,2,6,8,9,1,0,3,4,7}
    // new 10-qubits of 20qb register time:
    //      0.24
    // old 10-qubits of 20qb register time:
    //      0.32
    
    // {1,11,19,15,7}
    // new 5-qubits of 20qb register time:
    //      0.09
    // old 5-qubits of 20qb register time:
    //      0.12
    
    // speedups: 18%, 23%, 25%, 25%
    
    // Disabling all Hadamards and Swaps, doing ONLY contiguous C-phases:
    // targs[10] = {5,12,14,7,6,0,2,8,1,4} on 20 qubit
    // new:
    //      0.18
    // old: 
    //      0.25
    // speedup = 28%
    
    
    
    //printf("input:\n\n");
    
    // initialise into a random state 
    for (long long int i=0; i<qureg.numAmpsPerChunk; i++) {
        qureg.stateVec.real[i] = .5+.5*rand()/(qreal) RAND_MAX;
        qureg.stateVec.imag[i] = .5+.5*rand()/(qreal) RAND_MAX;
        
        
        //printf("(%g)+I(%g), ", qureg.stateVec.real[i], qureg.stateVec.imag[i]);
    }
    
    //printf("\n\noutput:");
    
    long double avDur, varDur;
    timeRepeatFuncCalls(qft, 10, &avDur, &varDur);
    printf("dur: %Lg\n", avDur);
    
    //reportStateToScreen(qureg, env, 0);
    //printf("\n");

    destroyQureg(qureg, env);
    destroyQuESTEnv(env);
    return 0;
}