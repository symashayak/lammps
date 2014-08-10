/*
//@HEADER
// ************************************************************************
//
//   Kokkos: Manycore Performance-Portable Multidimensional Arrays
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

/// \file Kokkos_Serial.hpp
/// \brief Declaration and definition of Kokkos::Serial device.

#ifndef KOKKOS_SERIAL_HPP
#define KOKKOS_SERIAL_HPP

#include <cstddef>
#include <iosfwd>
#include <Kokkos_Parallel.hpp>
#include <Kokkos_Layout.hpp>
#include <Kokkos_HostSpace.hpp>
#include <Kokkos_MemoryTraits.hpp>
#include <impl/Kokkos_Tags.hpp>

/*--------------------------------------------------------------------------*/

namespace Kokkos {

/// \class Serial
/// \brief Kokkos device for non-parallel execution
///
/// A "device" represents a parallel execution model.  It tells Kokkos
/// how to parallelize the execution of kernels in a parallel_for or
/// parallel_reduce.  For example, the Threads device uses Pthreads or
/// C++11 threads on a CPU, the OpenMP device uses the OpenMP language
/// extensions, and the Cuda device uses NVIDIA's CUDA programming
/// model.  The Serial device executes "parallel" kernels
/// sequentially.  This is useful if you really do not want to use
/// threads, or if you want to explore different combinations of MPI
/// and shared-memory parallel programming models.
class Serial {
public:
  //! \name Type declarations that all Kokkos devices must provide.
  //@{

  //! The tag (what type of kokkos_object is this).
  typedef Impl::DeviceTag       kokkos_tag ;
  //! The device type (same as this class).
  typedef Serial                device_type ;
  //! The size_type typedef best suited for this device.
  typedef HostSpace::size_type  size_type ;
  //! This device's preferred memory space.
  typedef HostSpace             memory_space ;
  typedef Serial                scratch_memory_space ;
  //! This device's preferred array layout.
  typedef LayoutRight           array_layout ;
  /// \brief This device's host mirror type.
  ///
  /// Serial is a host device, so the host mirror type is the same as
  /// the device type itself.
  typedef Serial                host_mirror_device_type ;

  //@}

  /// \brief True if and only if this method is being called in a
  ///   thread-parallel function.
  ///
  /// For the Serial device, this method <i>always</i> returns false,
  /// because parallel_for or parallel_reduce with the Serial device
  /// always execute sequentially.
  inline static int in_parallel() { return false ; }

  /** \brief  Set the device in a "sleep" state.
   *
   * This function sets the device in a "sleep" state in which it is
   * not ready for work.  This may consume less resources than if the
   * device were in an "awake" state, but it may also take time to
   * bring the device from a sleep state to be ready for work.
   *
   * \return True if the device is in the "sleep" state, else false if
   *   the device is actively working and could not enter the "sleep"
   *   state.
   */
  static bool sleep();

  /// \brief Wake the device from the 'sleep' state so it is ready for work.
  ///
  /// \return True if the device is in the "ready" state, else "false"
  ///  if the device is actively working (which also means that it's
  ///  awake).
  static bool wake();

  /// \brief Wait until all dispatched functors complete.
  ///
  /// The parallel_for or parallel_reduce dispatch of a functor may
  /// return asynchronously, before the functor completes.  This
  /// method does not return until all dispatched functors on this
  /// device have completed.
  static void fence() {}

  static void initialize( unsigned threads_count = 1 ,
                          unsigned use_numa_count = 0 ,
                          unsigned use_cores_per_numa = 0 ,
                          bool allow_asynchronous_threadpool = false) {}

  static int is_initialized() { return 1 ; }

  //! Free any resources being consumed by the device.
  static void finalize() {}

  //! Print configuration information to the given output stream.
  static void print_configuration( std::ostream & , const bool detail = false );

  //--------------------------------------------------------------------------

  static inline int team_max() { return 1 ; }
  static inline int team_recommended() { return 1 ; }
  KOKKOS_INLINE_FUNCTION static unsigned hardware_thread_id() { return 0 ; }
  KOKKOS_INLINE_FUNCTION static unsigned max_hardware_threads() { return 1 ; }


  //--------------------------------------------------------------------------

  inline int league_rank() const { return m_league_rank ; }
  inline int league_size() const { return m_league_size ; }
  inline int team_rank() const { return 0 ; }
  inline int team_size() const { return 1 ; }

  inline void team_barrier() {}

  template< class ArgType >
  KOKKOS_INLINE_FUNCTION
  ArgType team_scan( const ArgType & value , ArgType * const global_accum = 0 )
    {
      const ArgType tmp = global_accum ? *global_accum : ArgType(0) ;
      if ( global_accum ) { *global_accum += value ; }
      return tmp ;
    }

  inline std::pair<size_t,size_t> work_range( size_t n ) const
    { return std::pair<size_t,size_t>(0,n); }

  void * get_shmem( const int size ) const ;

  static void * resize_reduce_scratch( const unsigned );
  static void * resize_shared_scratch( const unsigned );

  Serial( const int rank , const int size )
    : m_league_rank(rank) , m_league_size(size) , m_shmem_iter(0) {}

  Serial()
    : m_league_rank(0) , m_league_size(0) , m_shmem_iter(0) {}

private:
  int m_league_rank ;
  int m_league_size ;
  mutable int m_shmem_iter ;
};

} // namespace Kokkos

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

namespace Kokkos {
namespace Impl {

//----------------------------------------------------------------------------

template< class FunctorType , class WorkSpec >
class ParallelFor< FunctorType , WorkSpec , Serial > {
public:

  ParallelFor( const FunctorType & functor , const size_t work_count )
    {
      for ( size_t iwork = 0 ; iwork < work_count ; ++iwork ) {
        functor( iwork );
      }
    }
};

template< class FunctorType , class WorkSpec >
class ParallelReduce< FunctorType , WorkSpec , Serial > {
public:

  typedef ReduceAdapter< FunctorType >  Reduce ;
  typedef typename Reduce::pointer_type pointer_type ;

  ParallelReduce( const FunctorType  & functor ,
                  const size_t         work_count ,
                  pointer_type         result = 0 )
    {
      if ( 0 == result ) {
        result = (pointer_type ) Serial::resize_reduce_scratch( Reduce::value_size( functor ) );
      }

      Reduce::init( functor , result );

      for ( size_t iwork = 0 ; iwork < work_count ; ++iwork ) {
        functor( iwork , Reduce::reference( result ) );
      }

      Reduce::final( functor , result );
    }

  void wait() {}
};

template< class FunctorType , class WorkSpec >
class ParallelScan< FunctorType , WorkSpec , Kokkos::Serial >
{
public:
  typedef ReduceAdapter< FunctorType >   Reduce ;
  typedef typename Reduce::pointer_type  pointer_type ;

  inline
  ParallelScan( const FunctorType & functor , const size_t work_count )
  {
    pointer_type result = (pointer_type ) Serial::resize_reduce_scratch( Reduce::value_size( functor ) );

    Reduce::init( functor , result );

    for ( size_t iwork = 0 ; iwork < work_count ; ++iwork ) {
      functor( iwork , Reduce::reference( result ) , true );
    }
  }

  void wait() {}
};

//----------------------------------------------------------------------------

template< class FunctorType >
class ParallelFor< FunctorType , ParallelWorkRequest , Serial > {
public:

  ParallelFor( const FunctorType         & functor
             , const ParallelWorkRequest & work )
    {
      Serial::resize_shared_scratch( FunctorShmemSize< FunctorType >::value( functor ) );

      for ( size_t iwork = 0 ; iwork < work.league_size ; ++iwork ) {
        functor( Serial(iwork,work.league_size) );
      }
    }
};

template< class FunctorType >
class ParallelReduce< FunctorType , ParallelWorkRequest , Serial > {
public:

  typedef ReduceAdapter< FunctorType >  Reduce ;
  typedef typename Reduce::pointer_type pointer_type ;

  ParallelReduce( const FunctorType         & functor
                , const ParallelWorkRequest & work
                ,       pointer_type          result_ptr = 0
                )
    {
      Serial::resize_shared_scratch( FunctorShmemSize< FunctorType >::value( functor ) );

      if ( ! result_ptr ) {
        result_ptr = (pointer_type) Serial::resize_reduce_scratch( Reduce::value_size( functor ) );
      }

      typename Reduce::reference_type update = Reduce::init( functor , result_ptr );
      
      for ( size_t iwork = 0 ; iwork < work.league_size ; ++iwork ) {
        functor( Serial(iwork,work.league_size) , update );
      }

      Reduce::final( functor , result_ptr );
    }

  inline
  void wait() const {}
};

} // namespace Impl
} // namespace Kokkos

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

namespace Kokkos {

template < class WorkArgTag >
class TeamPolicy< Kokkos::Serial , WorkArgTag > {
private:

  const int m_league_size ;

public:

  typedef Impl::ExecutionPolicyTag   kokkos_tag ;      ///< Concept tag
  typedef Kokkos::Serial             execution_space ; ///< Execution space

  inline int team_size() const { return 1 ; }
  inline int league_size() const { return m_league_size ; }

  /** \brief  Specify league size, request team size */
  TeamPolicy( execution_space & , int league_size_request , int /* team_size_request */ )
    : m_league_size( league_size_request )
    { }

  TeamPolicy( int league_size_request , int /* team_size_request */ )
    : m_league_size( league_size_request )
    { }

  class member_type {
  private:
    const int m_league_rank ;
    const int m_league_size ;
  public:

    KOKKOS_INLINE_FUNCTION
    execution_space::scratch_memory_space  team_shmem() const
      { return execution_space::scratch_memory_space(); }

    KOKKOS_INLINE_FUNCTION int league_rank() const { return m_league_rank ; }
    KOKKOS_INLINE_FUNCTION int league_size() const { return m_league_size ; }
    KOKKOS_INLINE_FUNCTION int team_rank() const { return 0 ; }
    KOKKOS_INLINE_FUNCTION int team_size() const { return 1 ; }

    KOKKOS_INLINE_FUNCTION void team_barrier() const {}

    /** \brief  Intra-team exclusive prefix sum with team_rank() ordering
     *          with intra-team non-deterministic ordering accumulation.
     *
     *  The global inter-team accumulation value will, at the end of the
     *  league's parallel execution, be the scan's total.
     *  Parallel execution ordering of the league's teams is non-deterministic.
     *  As such the base value for each team's scan operation is similarly
     *  non-deterministic.
     */
    template< typename Type >
    KOKKOS_INLINE_FUNCTION Type team_scan( const Type & value , Type * const global_accum ) const
      {
        const Type tmp = global_accum ? *global_accum : Type(0) ;
        if ( global_accum ) { *global_accum += value ; }
        return tmp ;
      }

    /** \brief  Intra-team exclusive prefix sum with team_rank() ordering.
     *
     *  The highest rank thread can compute the reduction total as
     *    reduction_total = dev.team_scan( value ) + value ;
     */
    template< typename Type >
    KOKKOS_INLINE_FUNCTION Type team_scan( const Type & ) const
      { return Type(0); }

    //----------------------------------------

    member_type( int rank , int size ) : m_league_rank(rank), m_league_size(size) {}
  };
};

}

namespace Kokkos {
namespace Impl {

template< class FunctorType >
class ParallelFor< FunctorType , Kokkos::TeamPolicy< Kokkos::Serial , void > , Kokkos::Serial >
{
public:
  typedef Kokkos::TeamPolicy< Kokkos::Serial , void > Policy ;

  ParallelFor( const FunctorType & functor
             , const Policy      & policy )
    {
      Serial::resize_shared_scratch( FunctorShmemSize< FunctorType >::value( functor ) );

      for ( int ileague = 0 ; ileague < policy.league_size() ; ++ileague ) {
        functor( typename Policy::member_type(ileague,policy.league_size()) );
      }
    }
};

template< class FunctorType >
class ParallelReduce< FunctorType , Kokkos::TeamPolicy< Kokkos::Serial , void > , Kokkos::Serial > {
public:

  typedef Kokkos::TeamPolicy< Kokkos::Serial , void > Policy ;
  typedef ReduceAdapter< FunctorType >  Reduce ;
  typedef typename Reduce::pointer_type pointer_type ;

  template< class ViewType >
  ParallelReduce( const FunctorType  & functor
                , const Policy       & policy
                , const ViewType     & result
                )
    {
      Serial::resize_shared_scratch( FunctorShmemSize< FunctorType >::value( functor ) );

      pointer_type result_ptr = result.ptr_on_device();

      if ( ! result_ptr ) {
        result_ptr = (pointer_type) Serial::resize_reduce_scratch( Reduce::value_size( functor ) );
      }

      typename Reduce::reference_type update = Reduce::init( functor , result_ptr );
      
      for ( int ileague = 0 ; ileague < policy.league_size() ; ++ileague ) {
        functor( typename Policy::member_type(ileague,policy.league_size()) , update );
      }

      Reduce::final( functor , result_ptr );
    }
};

} // namespace Impl
} // namespace Kokkos

#endif /* #define KOKKOS_SERIAL_HPP */

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

