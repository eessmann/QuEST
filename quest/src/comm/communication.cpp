/** @file
 * Functions for communicating and exchanging amplitudes between compute
 * nodes, when running in distributed mode, using the C MPI standard.
 */

#include "quest/include/modes.h"
#include "quest/include/types.h"

#include "quest/src/core/errors.hpp"

#if ENABLE_DISTRIBUTION
    #include <mpi.h>
#endif



/*
 * WARN ABOUT CUDA-AWARENESS
 */

#if ENABLE_DISTRIBUTION && ENABLE_GPU_ACCELERATION
    #include <mpi-ext.h>

    #ifndef MPIX_CUDA_AWARE_SUPPORT
        #warning "Could not ascertain whether MPI is CUDA-aware, so we will assume it is not. This means inter-GPU communication will be slowly routed through the CPU/RAM."
    #elif !MPIX_CUDA_AWARE_SUPPORT
        #warning "MPI compiler is not CUDA-aware, so inter-GPU communication will be slowly routed through the CPU/RAM"
    #endif

#endif



/*
 * MPI COMPLEX TYPE FLAG
 */

#if ENABLE_DISTRIBUTION

    #if (FLOAT_PRECISION == 1)
        #define MPI_QCOMP MPI_CXX_FLOAT_COMPLEX

    #elif (FLOAT_PRECISION == 2)
        #define MPI_QCOMP MPI_CXX_DOUBLE_COMPLEX

    // sometimes 'MPI_CXX_LONG_DOUBLE_COMPLEX' isn't defined
    #elif (FLOAT_PRECISION == 4) && defined(MPI_CXX_LONG_DOUBLE_COMPLEX)
        #define MPI_QCOMP MPI_CXX_LONG_DOUBLE_COMPLEX

    // in that case, fall back to the C type (identical memory layout)
    #else
        #define MPI_QCOMP MPI_C_LONG_DOUBLE_COMPLEX
    #endif

#endif



/*
 * MPI ENVIRONMENT MANAGEMENT
 * all of which is safely callable in non-distributed mode
 */


bool comm_isMpiCompiled() {
    return (bool) ENABLE_DISTRIBUTION;
}


bool comm_isMpiGpuAware() {

    // TODO: these checks may be OpenMPI specific, so that
    // non-OpenMPI MPI compilers are always dismissed as
    // not being CUDA-aware. Check e.g. MPICH method!

    // definitely not GPU-aware if compiler declares it is not
    #if defined(MPIX_CUDA_AWARE_SUPPORT) && ! MPIX_CUDA_AWARE_SUPPORT
        return false;
    #endif

    // check CUDA-awareness at run-time if we know it's principally supported
    #if defined(MPIX_CUDA_AWARE_SUPPORT)
        return (bool) MPIX_Query_cuda_support();
    #endif

    // if we can't ascertain CUDA-awareness, just assume no to avoid seg-fault
    return false;
}


void comm_init() {
#if ENABLE_DISTRIBUTION
    int isInit;
    MPI_Initialized(&isInit);

    // gracefully handle re-initialisation
    if (isInit)
        error_commAlreadyInit();
    
    MPI_Init(NULL, NULL);
#endif
}


void comm_end() {
#if ENABLE_DISTRIBUTION
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
#endif
}


int comm_getRank() {
#if ENABLE_DISTRIBUTION
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    return rank;
#else
    return 0;
#endif
}


int comm_getNumNodes() {
#if ENABLE_DISTRIBUTION
    int numNodes;
    MPI_Comm_size(MPI_COMM_WORLD, &numNodes);
    return numNodes;
#else
    return 1;
#endif
}


void comm_synch() {
#if ENABLE_DISTRIBUTION
    MPI_Barrier(MPI_COMM_WORLD);
#endif
}