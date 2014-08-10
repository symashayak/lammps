/*
//@HEADER
// ************************************************************************
//
//                             Kokkos
//         Manycore Performance-Portable Multidimensional Arrays
//
//              Copyright (2012) Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions?  Contact  H. Carter Edwards (hcedwar@sandia.gov)
//
// ************************************************************************
//@HEADER
*/

#include <Kokkos_Core.hpp>
#include <impl/Kokkos_Error.hpp>
#include <cctype>
#include <cstring>
#include <iostream>
#include <cstdlib>

namespace Kokkos {
  void initialize() {
    if(!Impl::is_same<Impl::DefaultDeviceType, Impl::DefaultDeviceType::host_mirror_device_type>::value){
      Impl::DefaultDeviceType::host_mirror_device_type::initialize();
    }
    Impl::DefaultDeviceType::initialize();
  }

  void initialize(int narg, char* arg[]) {
    int nthreads = -1;
    int numa = -1;
    int device = -1;

    int iarg = 0;

    while (iarg < narg) {
      if (strcmp(arg[iarg],"--threads") == 0) {
         if (iarg+2 > narg)
            Impl::throw_runtime_exception("Error: expecting an integer number after command line argument '--threads'. Raised by Kokkos::initialize(int narg, char* argc[]).");
         nthreads = atoi(arg[iarg+1]);
         iarg+=2;
      } else if (strcmp(arg[iarg],"--numa") == 0) {
         if (iarg+2 > narg)
            Impl::throw_runtime_exception("Error: expecting an integer number after command line argument '--numa'. Raised by Kokkos::initialize(int narg, char* argc[]).");
         numa = atoi(arg[iarg+1]);
         iarg+=2;
      } else if (strcmp(arg[iarg],"--device") == 0) {
         if (iarg+2 > narg)
            Impl::throw_runtime_exception("Error: expecting an integer number after command line argument '--device'. Raised by Kokkos::initialize(int narg, char* argc[]).");
         device = atoi(arg[iarg+1]);
         iarg+=2;
      } else if (strcmp(arg[iarg],"--ngpus") == 0) {
         if (iarg+2 > narg)
            Impl::throw_runtime_exception("Error: expecting one or two integer numbers after command line argument '--ngpus'. Raised by Kokkos::initialize(int narg, char* argc[]).");
         int ngpu = atoi(arg[iarg+1]);
         iarg += 2;

         int skip_gpu = 9999;
         if (iarg+2 < narg && isdigit(arg[iarg+2][0])) {
           skip_gpu = atoi(arg[iarg+2]);
           iarg++;
         }

         char *str;
         if ((str = getenv("SLURM_LOCALID"))) {
           int local_rank = atoi(str);
           device = local_rank % ngpu;
           if (device >= skip_gpu) device++;
         }
         if ((str = getenv("MV2_COMM_WORLD_LOCAL_RANK"))) {
           int local_rank = atoi(str);
           device = local_rank % ngpu;
           if (device >= skip_gpu) device++;
         }
         if ((str = getenv("OMPI_COMM_WORLD_LOCAL_RANK"))) {
           int local_rank = atoi(str);
           device = local_rank % ngpu;
           if (device >= skip_gpu) device++;
         }
      } else if (strcmp(arg[iarg],"--help") == 0) {
         std::cout << std::endl;
         std::cout << "-------------------------------" << std::endl;
         std::cout << "-Kokkos command line arguments-" << std::endl;
         std::cout << "-------------------------------" << std::endl;
         std::cout << std::endl;
         std::cout << "--help               : print this message" << std::endl;
         std::cout << "--threads INT        : specify total number of threads or" << std::endl;
         std::cout << "                       number of threads per numa region if " << std::endl;
         std::cout << "                       used in conjunction with '--numa' option. " << std::endl;
         std::cout << "--numa INT           : specify number of numa regions used by process." << std::endl;
         std::cout << "--device INT         : specify device id to be used by kokkos. " << std::endl;
         std::cout << "--ndevices INT [INT] : used when running MPI jobs. Specify number of" << std::endl;
         std::cout << "                       of devices per node to be used. Process to device" << std::endl;
         std::cout << "                       mapping happens by obtaining the local mpi rank" << std::endl;
         std::cout << "                       and assigning devices round robbin. The optional" << std::endl;
         std::cout << "                       second argument allows for an exisiting device" << std::endl;
         std::cout << "                       to be ignored. This is most usefull on workstations" << std::endl;
         std::cout << "                       with multiple GPUs of which one is used to drive" << std::endl;
         std::cout << "                       screen output." << std::endl;
         std::cout << std::endl;
         std::cout << "-------------------------------" << std::endl;
         std::cout << std::endl;
         iarg++;
      } else
      iarg++;
    }


    if(!Impl::is_same<Impl::DefaultDeviceType, Impl::DefaultDeviceType::host_mirror_device_type>::value) {
      if(nthreads>0) {
        if(numa>0)
          Impl::DefaultDeviceType::host_mirror_device_type::initialize(nthreads,numa);
        else
          Impl::DefaultDeviceType::host_mirror_device_type::initialize(nthreads);
      } else
        Impl::DefaultDeviceType::host_mirror_device_type::initialize();
    }

    #ifdef KOKKOS_HAVE_CUDA
    if(Impl::is_same<Impl::DefaultDeviceType, Kokkos::Cuda>::value) {
      if(device>-1)
        Kokkos::Cuda::initialize(device);
      else
        Kokkos::Cuda::initialize();
    } else
    #endif
    {
      if(nthreads>0) {
        if(numa>0)
          Impl::DefaultDeviceType::initialize(nthreads,numa);
        else
          Impl::DefaultDeviceType::initialize(nthreads);
      } else
        Impl::DefaultDeviceType::initialize();
    }
  }

  void finalize() {
    if(!Impl::is_same<Impl::DefaultDeviceType, Impl::DefaultDeviceType::host_mirror_device_type>::value) {
      Impl::DefaultDeviceType::host_mirror_device_type::finalize();
    }
    Impl::DefaultDeviceType::finalize();
  }

  void fence() {
    if(!Impl::is_same<Impl::DefaultDeviceType, Impl::DefaultDeviceType::host_mirror_device_type>::value) {
      Impl::DefaultDeviceType::host_mirror_device_type::fence();
    }
    Impl::DefaultDeviceType::fence();
  }
}
