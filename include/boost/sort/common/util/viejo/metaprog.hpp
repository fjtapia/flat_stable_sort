//----------------------------------------------------------------------------
/// @file metaprog.hpp
/// @brief This file contains the description of several functions
///         metaprogrammed
///
/// @author Copyright (c) 2010 2015 Francisco José Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanying file LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#ifndef __BOOST_SORT_COMMON_UTIL_METAPROG_HPP
#define __BOOST_SORT_COMMON_UTIL_METAPROG_HPP


#include <cassert>

//---------------------------------------------------------------------------
// The HW align is defined by the HW architecture. Alwais is a power of 2
// The value of PW2_HW_ALIGN is the power of two of the size of the alignment
//
// Value    Number of bytes
// ------------------------------
//  0       1
//  1       2
//  2       4
//  3       8
//  4       16
//  5       32
//
// In the Intel x64 the value is 4
//---------------------------------------------------------------------------


#ifndef PW2_HW_ALIGN
#define PW2_HW_ALIGN 4
#endif // PW2_HW_ALIGN


namespace boost
{
namespace sort
{
namespace common
{
namespace util
{	
//
//##########################################################################
//                                                                        ##
//   //#############################################################//    ##
//   //                                                             //    ##
//   //          32  B I T S     F U N C T I O N S                  //    ##
//   //                                                             //    ##
//   //#############################################################//    ##
//                                                                        ##
//##########################################################################

//
//---------------------------------------------------------------------------
//  function : MASK_32
/// @brief Calculate the mask for a number of bits
/// @param [in] NBits : Number to examine
/// @exception none
/// @return Mask
/// @remarks
//---------------------------------------------------------------------------
static constexpr inline uint32_t MASK_32 ( uint32_t NBits) noexcept
{   
    return ((1U<<NBits)-1);
};
//
//---------------------------------------------------------------------------
//  function : MS1B_32
/// @brief Obtain the position of the most significant one in N
/// @param [in] N : Number to examine
/// @exception none
/// @return Position of the first one
/// @remarks
//---------------------------------------------------------------------------
template <int NBits = 16 >
static constexpr inline int MS1B_32 ( uint32_t N, int Pos =0) noexcept
{   
    static_assert ( NBits < 32, " NBits too big ");
    return (((N>>Pos)& (~MASK_32(NBits)))== 0 )?
              MS1B_32<(NBits>>1)>(N,Pos):
              MS1B_32<(NBits>>1)>(N,Pos+NBits);
};
template< >
constexpr inline int MS1B_32<0> ( uint32_t, int Pos) noexcept
{   return Pos ;
};
//
//---------------------------------------------------------------------------
//  function : MS1B
/// @brief Obtain the position of the most significant one in N
/// @param [in] N : Number to examine
/// @exception none
/// @return Position of the first one
/// @remarks
//---------------------------------------------------------------------------
constexpr static inline int  MS1B  ( uint32_t N )
{   return metaprog::MS1B_32<16> (N,0);
};

//
//##########################################################################
//                                                                        ##
//   //#############################################################//    ##
//   //                                                             //    ##
//   //          32  B I T S     F U N C T I O N S                  //    ##
//   //                                                             //    ##
//   //#############################################################//    ##
//                                                                        ##
//##########################################################################
//
//---------------------------------------------------------------------------
//  function : Mask64
/// @brief Calculate the mask for a number of bits
/// @param [in] NBits : Number to examine
/// @exception none
/// @return Mask
/// @remarks
//---------------------------------------------------------------------------
static constexpr inline uint64_t MASK_64 ( uint64_t NBits) noexcept
{   
    return ((1ULL<<NBits)-1);
};
//
//---------------------------------------------------------------------------
//  function : MS1B_64
/// @brief Obtain the position of the most significant one in N
/// @param [in] N : Number to examine
/// @exception none
/// @return Position of the first one
//---------------------------------------------------------------------------
template <int NBits = 32 >
static constexpr inline int MS1B_64 ( uint64_t N, int Pos =0) noexcept
{   
    static_assert ( NBits < 64, " NBits too big ");

    return (((N>>Pos)& (~MASK_64(NBits)))== 0 )?
              MS1B_64<(NBits>>1)>(N,Pos):
              MS1B_64<(NBits>>1)>(N,Pos+NBits);
};
template<>
constexpr inline int MS1B_64<0> ( uint64_t, int Pos) noexcept
{   return Pos ;
};

//
//***************************************************************************
};//    End namespace util
};//    End namespace common
};//    End namespace sort
};//    End namespace boost
//***************************************************************************
//
#endif
