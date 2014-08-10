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

#include <Kokkos_Macros.hpp>
namespace Kokkos {
namespace Impl {

template<class ViewTraits>
class AtomicDataElement {
public:
  typedef typename ViewTraits::value_type value_type;
  typedef typename ViewTraits::const_value_type const_value_type;
  typedef typename ViewTraits::non_const_value_type non_const_value_type;
  volatile value_type* const ptr;

  KOKKOS_INLINE_FUNCTION
  AtomicDataElement(value_type* ptr_):ptr(ptr_){}

  KOKKOS_INLINE_FUNCTION
  const_value_type operator = (const_value_type& val) const {
    *ptr = val;
    return val;
  }
  KOKKOS_INLINE_FUNCTION
  const_value_type operator = (volatile const_value_type& val) const {
    *ptr = val;
    return val;
  }

  /*KOKKOS_INLINE_FUNCTION
  void inc() const {
    Kokkos::atomic_inc(ptr);
  }*/

  KOKKOS_INLINE_FUNCTION
  const_value_type operator ++ () const {
    const_value_type tmp = Kokkos::atomic_fetch_add(ptr,1);
    return tmp+1;
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator -- () const {
    const_value_type tmp = Kokkos::atomic_fetch_add(ptr,-1);
    return tmp-1;
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator += (const_value_type& val) const {
    const_value_type tmp = Kokkos::atomic_fetch_add(ptr,val);
    return tmp+val;
  }
  KOKKOS_INLINE_FUNCTION
  const_value_type operator += (volatile const_value_type& val) const {
    const_value_type tmp = Kokkos::atomic_fetch_add(ptr,val);
    return tmp+val;
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator -= (const_value_type& val) const {
    const_value_type tmp = Kokkos::atomic_fetch_add(ptr,-val);
    return tmp-val;
  }
  KOKKOS_INLINE_FUNCTION
  const_value_type operator -= (volatile const_value_type& val) const {
    const_value_type tmp = Kokkos::atomic_fetch_add(ptr,-val);
    return tmp-val;
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator *= (const_value_type& val) const {
    return Kokkos::atomic_mul_fetch(ptr,val);
  }
  KOKKOS_INLINE_FUNCTION
  const_value_type operator *= (volatile const_value_type& val) const {
    return Kokkos::atomic_mul_fetch(ptr,val);
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator /= (const_value_type& val) const {
    return Kokkos::atomic_div_fetch(ptr,val);
  }
  KOKKOS_INLINE_FUNCTION
  const_value_type operator /= (volatile const_value_type& val) const {
    return Kokkos::atomic_div_fetch(ptr,val);
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator %= (const_value_type& val) const {
    return Kokkos::atomic_mod_fetch(ptr,val);
  }
  KOKKOS_INLINE_FUNCTION
  const_value_type operator %= (volatile const_value_type& val) const {
    return Kokkos::atomic_mod_fetch(ptr,val);
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator &= (const_value_type& val) const {
    return Kokkos::atomic_and_fetch(ptr,val);
  }
  KOKKOS_INLINE_FUNCTION
  const_value_type operator &= (volatile const_value_type& val) const {
    return Kokkos::atomic_and_fetch(ptr,val);
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator ^= (const_value_type& val) const {
    return Kokkos::atomic_xor_fetch(ptr,val);
  }
  KOKKOS_INLINE_FUNCTION
  const_value_type operator ^= (volatile const_value_type& val) const {
    return Kokkos::atomic_xor_fetch(ptr,val);
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator |= (const_value_type& val) const {
    return Kokkos::atomic_or_fetch(ptr,val);
  }
  KOKKOS_INLINE_FUNCTION
  const_value_type operator |= (volatile const_value_type& val) const {
    return Kokkos::atomic_or_fetch(ptr,val);
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator <<= (const_value_type& val) const {
    return Kokkos::atomic_lshift_fetch(ptr,val);
  }
  KOKKOS_INLINE_FUNCTION
  const_value_type operator <<= (volatile const_value_type& val) const {
    return Kokkos::atomic_lshift_fetch(ptr,val);
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator >>= (const_value_type& val) const {
    return Kokkos::atomic_rshift_fetch(ptr,val);
  }
  KOKKOS_INLINE_FUNCTION
  const_value_type operator >>= (volatile const_value_type& val) const {
    return Kokkos::atomic_rshift_fetch(ptr,val);
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator + (const_value_type& val) const {
    return *ptr+val;
  }
  KOKKOS_INLINE_FUNCTION
  const_value_type operator + (volatile const_value_type& val) const {
    return *ptr+val;
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator - (const_value_type& val) const {
    return *ptr-val;
  }
  KOKKOS_INLINE_FUNCTION
  const_value_type operator - (volatile const_value_type& val) const {
    return *ptr-val;
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator * (const_value_type& val) const {
    return *ptr*val;
  }
  KOKKOS_INLINE_FUNCTION
  const_value_type operator * (volatile const_value_type& val) const {
    return *ptr*val;
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator / (const_value_type& val) const {
    return *ptr/val;
  }
  KOKKOS_INLINE_FUNCTION
  const_value_type operator / (volatile const_value_type& val) const {
    return *ptr/val;
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator % (const_value_type& val) const {
    return *ptr^val;
  }
  KOKKOS_INLINE_FUNCTION
  const_value_type operator % (volatile const_value_type& val) const {
    return *ptr^val;
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator ! () const {
    return !*ptr;
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator && (const_value_type& val) const {
    return *ptr&&val;
  }
  KOKKOS_INLINE_FUNCTION
  const_value_type operator && (volatile const_value_type& val) const {
    return *ptr&&val;
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator || (const_value_type& val) const {
    return *ptr|val;
  }
  KOKKOS_INLINE_FUNCTION
  const_value_type operator || (volatile const_value_type& val) const {
    return *ptr|val;
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator & (const_value_type& val) const {
    return *ptr&val;
  }
  KOKKOS_INLINE_FUNCTION
  const_value_type operator & (volatile const_value_type& val) const {
    return *ptr&val;
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator | (const_value_type& val) const {
    return *ptr|val;
  }
  KOKKOS_INLINE_FUNCTION
  const_value_type operator | (volatile const_value_type& val) const {
    return *ptr|val;
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator ^ (const_value_type& val) const {
    return *ptr^val;
  }
  KOKKOS_INLINE_FUNCTION
  const_value_type operator ^ (volatile const_value_type& val) const {
    return *ptr^val;
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator ~ () const {
    return ~*ptr;
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator << (const unsigned int& val) const {
    return *ptr<<val;
  }
  KOKKOS_INLINE_FUNCTION
  const_value_type operator << (volatile const unsigned int& val) const {
    return *ptr<<val;
  }

  KOKKOS_INLINE_FUNCTION
  const_value_type operator >> (const unsigned int& val) const {
    return *ptr>>val;
  }
  KOKKOS_INLINE_FUNCTION
  const_value_type operator >> (volatile const unsigned int& val) const {
    return *ptr>>val;
  }

  KOKKOS_INLINE_FUNCTION
  bool operator == (const_value_type& val) const {
    return *ptr == val;
  }
  KOKKOS_INLINE_FUNCTION
  bool operator == (volatile const_value_type& val) const {
    return *ptr == val;
  }

  KOKKOS_INLINE_FUNCTION
  bool operator != (const_value_type& val) const {
    return *ptr != val;
  }
  KOKKOS_INLINE_FUNCTION
  bool operator != (volatile const_value_type& val) const {
    return *ptr != val;
  }

  KOKKOS_INLINE_FUNCTION
  bool operator >= (const_value_type& val) const {
    return *ptr >= val;
  }
  KOKKOS_INLINE_FUNCTION
  bool operator >= (volatile const_value_type& val) const {
    return *ptr >= val;
  }

  KOKKOS_INLINE_FUNCTION
  bool operator <= (const_value_type& val) const {
    return *ptr <= val;
  }
  KOKKOS_INLINE_FUNCTION
  bool operator <= (volatile const_value_type& val) const {
    return *ptr <= val;
  }

  KOKKOS_INLINE_FUNCTION
  bool operator < (const_value_type& val) const {
    return *ptr < val;
  }
  KOKKOS_INLINE_FUNCTION
  bool operator < (volatile const_value_type& val) const {
    return *ptr < val;
  }

  KOKKOS_INLINE_FUNCTION
  bool operator > (const_value_type& val) const {
    return *ptr > val;
  }
  KOKKOS_INLINE_FUNCTION
  bool operator > (volatile const_value_type& val) const {
    return *ptr > val;
  }

  KOKKOS_INLINE_FUNCTION
  operator const_value_type () const {
    //return Kokkos::atomic_load(ptr);
    return *ptr;
  }

  KOKKOS_INLINE_FUNCTION
  operator volatile non_const_value_type () volatile const {
    //return Kokkos::atomic_load(ptr);
    return *ptr;
  }
};

template<class ViewTraits>
class AtomicViewDataHandle {
public:
  typename ViewTraits::value_type* ptr;
  KOKKOS_INLINE_FUNCTION
  AtomicViewDataHandle(typename ViewTraits::value_type* ptr_):ptr(ptr_){}

  template<class iType>
  KOKKOS_INLINE_FUNCTION
  AtomicDataElement<ViewTraits> operator[] (const iType& i) const {
    return AtomicDataElement<ViewTraits>(ptr+i);
  }
};

template<class ViewTraits>
class ViewDataHandle<ViewTraits,
typename enable_if<!is_same<typename ViewTraits::const_value_type,typename ViewTraits::value_type>::value &&
                   ViewTraits::memory_traits::Atomic
                  >::type> {
  typedef ViewDataHandle self_type;
public:
  enum {ReferenceAble = 0};
  typedef Impl::AtomicViewDataHandle<ViewTraits> type;
  typedef Impl::AtomicDataElement<ViewTraits> return_type;

  static type allocate(std::string label, size_t count) {
    return type((typename ViewTraits::value_type*)
                ViewTraits::memory_space::allocate( label ,
                typeid(typename ViewTraits::value_type) ,
                sizeof(typename ViewTraits::value_type) ,
                count ));
  }

  KOKKOS_INLINE_FUNCTION
  static typename ViewTraits::value_type* get_raw_ptr(type handle) {
    return handle.ptr;
  }
};

}
}
