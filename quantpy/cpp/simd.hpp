/**
 * @file simd.hpp
 * @brief SIMD (Single Instruction Multiple Data) utility functions
 * @author Kasper Rantamäki
 * @date 2026-01-10
 */

#ifndef SIMD_HPP
#define SIMD_HPP


// Define the size of the SIMD vector registers
#define SIMD_SIZE 32  // Bytes  (Note this should be changed to something that can be altered at compile time)


namespace simd {


  /**
   * @struct simd
   * @brief Templated struct for SIMD vectors
   * @tparam T  The type held in the SIMD vector
   * @var simd::data The actual SIMD vector held within the struct
   */
  template <typename T>
  struct simd {
    typedef T data __attribute__ ((__vector_size__ (SIMD_SIZE)));
  };


  /**
   * @brief Template function that chooses either the vectorized or non-vectorized datatype based on a boolean flag.
   * @details Does the choice at compile time and is thus similar to std::conditional_t, but doesn't evaluate the 
   * unchosen option.
   * @tparam T          The type held in the SIMD vector
   * @tparam vectorize  Boolean flag telling if a SIMD vector type is to be returned
   * @returns           Uninitialized instance of the type T or a SIMD vector holding type T depending on the 
   * value of the 'vectorize' template parameter
   */
  template <typename T, bool vectorize>
  auto choose_simd() {
    if constexpr (vectorize) {
      typename simd<T>::data a;
      return a;
    }
    else {
      T a;
      return a;
    }
  }


}


#endif