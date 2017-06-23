//----------------------------------------------------------------------------
/// @file algorithms.hpp
/// @brief This file contains the description of several low level algorithms
///
/// @author Copyright (c) 2010 2015 Francisco José Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanying file LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#ifndef __BOOST_SORT_UTIL_ALGORITHM_HPP
#define __BOOST_SORT_UTIL_ALGORITHM_HPP

#include <boost/sort/util/metaprog.hpp>
#include <boost/sort/util/definition.hpp>
#include <boost/sort/util/config.hpp>

#include <cassert>


namespace boost
{
namespace sort
{
namespace util
{
//using metaprog::MASK_32 ;
//using metaprog::MS1B_32 ;
//using metaprog::MASK_64 ;
//using metaprog::MS1B_64 ;
//using metaprog::TMSB ;
//namespace  metaprog = boost::countertree::util::metaprog  ;

//
//###########################################################################
//                                                                         ##
//    ################################################################     ##
//    #                                                              #     ##
//    #            S T R U C T      C N F < 4 >                      #     ##
//    #                                                              #     ##
//    ################################################################     ##
//                                                                         ##
//###########################################################################
//
template <unsigned NByte>
struct cnf
{

//##########################################################################
//                                                                        ##
//         M E T A P R O G R A M M I N G     F U N C T I O N S            ##
//                                                                        ##
//##########################################################################
//---------------------------------------------------------------------------
//  function : MASK
/// @brief Calculate the mask for a number of bits
/// @param [in] NBits : Number to examine
/// @exception none
/// @return Mask
/// @remarks
//---------------------------------------------------------------------------
static constexpr inline uint32_t MASK ( uint32_t NBits) noexcept
{   //--------------------------- begin ---------------------
    return metaprog::MASK_32 ( NBits);
};

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
//---------------------------------------------------------------------------
//  function : AlignSize32
/// @brief Return the aligned size of the size received as parameter
/// @param [in] NBits :  2^NBits Is the size of the alignment
/// @param [in] N : size without alignment
/// @exception none
/// @return New size alligned
//---------------------------------------------------------------------------
static constexpr inline uint32_t ALIGN_SIZE ( uint32_t N, uint32_t NBits ) noexcept
{   //--------------------------- begin ---------------------------------
    return (N<=(1U<<NBits))?(1<<NBits):((N+MASK(NBits))&(~MASK(NBits)));
};

//---------------------------------------------------------------------------
//  function : BitsAlign32
/// @brief return the number of bits of the alignment of a block
/// @param [in] NBits : Number of bits of the element
/// @exception none
/// @return The block alignmen is 2 ^ result
//---------------------------------------------------------------------------
static constexpr inline uint32_t BITS_ALIGN ( uint32_t BitsSize ) noexcept
{   //-------------------------- begin -----------------------------
    return (BitsSize < 10) ?( (BitsSize < PW2_HW_ALIGN)?BitsSize:PW2_HW_ALIGN) :
                            ( (BitsSize > 29)?28:( ((BitsSize-2) >>2 )<<2));
};

//---------------------------------------------------------------------------
//  function : SizeAligned_32
/// @brief Return the aligned size of the size received as parameter
/// @param [in] N : size without alignment
/// @exception none
/// @return New size alligned
//---------------------------------------------------------------------------
static constexpr inline uint32_t SIZE_ALIGNED ( uint32_t  N) noexcept
{   return ALIGN_SIZE (N , BITS_ALIGN (MS1B (N)));
};
//##########################################################################
//                                                                        ##
//                G L O B A L     V A R I A B L E S                       ##
//                                                                        ##
//##########################################################################


//##########################################################################
//                                                                        ##
//                I N L I N E      F U N C T I O N S                      ##
//                                                                        ##
//##########################################################################

//---------------------------------------------------------------------------
//  function : ResetBit32
/// @brief Set to zero the bit Pos in a uint32_t element N
/// @param [in] N : word where set the bit to zero
/// @param [in] Pos : position of the bit in  set to zero
//---------------------------------------------------------------------------
static inline void ResetBit ( uint32_t &N , uint32_t Pos ) noexcept
{   //------------------ begin --------------------------
    ( N &= (~(1U<<Pos)));
};
//---------------------------------------------------------------------------
//  function : SetBit32
/// @brief Set to one the bit Pos in a uint32_t element N
/// @param [in] N : word where set  the bit to one
/// @param [in] Pos : position of the bit in  set to one
//---------------------------------------------------------------------------
static inline void SetBit ( uint32_t &N , uint32_t Pos ) noexcept
{   //------------------ begin --------------------------
    ( N |= (1U<<Pos));
};
//---------------------------------------------------------------------------
//  function :ReadBit32
/// @brief Return the content of the bit Pos in the uint32_t element N
/// @param [in] N : uint32_t element from read the birt
/// @param [in] Pos : Position of the bit readed
/// @return Value of the bit (true :1 , false :0)
//---------------------------------------------------------------------------
static inline bool ReadBit ( uint32_t N , uint32_t Pos ) noexcept
{   //------------------ begin --------------------------
    return (  (N & (1U<<Pos )) != 0 );
};

//---------------------------------------------------------------------------
//  function :bitScanForward
/// @author Martin Läuter(1997),Charles E.Leiserson,Harald Prokop,Keith H.Randall
/// @brief return the least significant one bit
/// @param [in]V1 bitboard to scan
/// @return index (0..31) of least significant one bit
/// @remarks "Using de Bruijn Sequences to Index a 1 in a Computer Word"
//---------------------------------------------------------------------------
static inline uint32_t BitScanForward (uint32_t V) noexcept
{   //------------------------- begin --------------------------------
    return metaprog::DeBruijn_32[((uint32_t)((V & ((~V)+1)) * 0x077CB531U)) >> 27];
};
//---------------------------------------------------------------------------
//  function : NBits
/// @brief Obtain the number of bits equal or greater than N
/// @param [in] N : Number to examine
/// @exception none
/// @return Number of bits
//---------------------------------------------------------------------------
static inline uint32_t NBits ( uint32_t N) noexcept
{   //----------------------- begin -------------------------------------
    int Pos = (N & 0xffff0000U)?16:0 ;
    if ((N>>Pos) & 0xff00U) Pos += 8 ;
    return (  metaprog::TMSB [ N >> Pos ] + Pos );
}

static inline uint32_t  BitScanReverse  ( uint32_t N )
{   //--------------------------- begin ----------------------------
    int NB = NBits ( N);
    return  ( NB == 0 )? MAX32 : NB-1;
};


}; // end template <unsigned NByte> cnf

//
//###########################################################################
//                                                                         ##
//    ################################################################     ##
//    #                                                              #     ##
//    #            S T R U C T      C N F < 8 >                      #     ##
//    #                                                              #     ##
//    ################################################################     ##
//                                                                         ##
//###########################################################################
//
template < >
struct cnf<8>
{
//##########################################################################
//                                                                        ##
//         M E T A P R O G R A M M I N G     F U N C T I O N S            ##
//                                                                        ##
//##########################################################################
//---------------------------------------------------------------------------
//  function : Mask64
/// @brief Calculate the mask for a number of bits
/// @param [in] NBits : Number to examine
/// @exception none
/// @return Mask
/// @remarks
//---------------------------------------------------------------------------
static constexpr inline uint64_t MASK ( uint64_t NBits) noexcept
{   //------------------------------- begin -------------------
    return metaprog::MASK_64(NBits);
};
//---------------------------------------------------------------------------
//  function : MS1B_64
/// @brief Obtain the position of the most significant one in N
/// @param [in] N : Number to examine
/// @exception none
/// @return Position of the first one
//---------------------------------------------------------------------------
constexpr static inline int  MS1B  ( uint64_t N )
{   return metaprog::MS1B_64<32> (N,0);
};
//---------------------------------------------------------------------------
//  function : AlignSize64
/// @brief Return the aligned size of the size received as parameter
/// @param [in] NBits :  2^NBits Is the size of the alignment
/// @param [in] N : size without alignment
/// @exception none
/// @return New size alligned
//---------------------------------------------------------------------------
static constexpr inline uint64_t ALIGN_SIZE ( uint64_t N, uint64_t NBits ) noexcept
{   //--------------------------- begin ---------------------------------
    return (N<=(1ULL<<NBits))?(1ULL<<NBits):((N+MASK(NBits))&(~MASK(NBits)));
};

//---------------------------------------------------------------------------
//  function : BitsAlign64
/// @brief return the number of bits of the alignment of a block
/// @param [in] NBits : Number of bits of the element
/// @exception none
/// @return The block alignmen is 2 ^ result
//---------------------------------------------------------------------------
static constexpr inline uint64_t BITS_ALIGN ( uint64_t BitsSize ) noexcept
{   //-------------------------- begin -----------------------------
    return (BitsSize < 10) ?( (BitsSize < PW2_HW_ALIGN)?BitsSize:PW2_HW_ALIGN) :
                            ( (BitsSize > 29)?28:( ((BitsSize-2) >>2 )<<2));
};
//---------------------------------------------------------------------------
//  function : SizeAligned_64
/// @brief Return the aligned size of the size received as parameter
/// @param [in] N : size without alignment
/// @exception none
/// @return New size alligned
//---------------------------------------------------------------------------
static constexpr inline uint64_t SIZE_ALIGNED ( uint64_t  N) noexcept
{   return ALIGN_SIZE (N , BITS_ALIGN (MS1B (N)));
};
//##########################################################################
//                                                                        ##
//                G L O B A L     V A R I A B L E S                       ##
//                                                                        ##
//##########################################################################


//##########################################################################
//                                                                        ##
//                I N L I N E      F U N C T I O N S                      ##
//                                                                        ##
//##########################################################################
//---------------------------------------------------------------------------
//  function : ResetBit64
/// @brief Set to zero the bit Pos in a uint64_t element N
/// @param [in] N : word where set the bit to zero
/// @param [in] Pos : position of the bit  set to zero
//---------------------------------------------------------------------------
static inline void ResetBit ( uint64_t &N , uint64_t Pos ) noexcept
{   //------------------ begin --------------------------
    ( N &= (~(1ULL<<Pos)));
};
//---------------------------------------------------------------------------
//  function : SetBit64
/// @brief Set to one the bit Pos in a uint64_t element N
/// @param [in] N : word where set  the bit to one
/// @param [in] Pos : position of the bit in  set to one
//---------------------------------------------------------------------------
static inline void SetBit ( uint64_t &N , uint64_t Pos ) noexcept
{   //------------------ begin --------------------------
    ( N |= (1ULL<<Pos));
};
//---------------------------------------------------------------------------
//  function : ReadBit64
/// @brief Return the content of the bit Pos in the uint64_t element N
/// @param [in] N : uint64_t element from read the bit
/// @param [in] Pos : Position of the bit readed
/// @return Value of the bit (true :1 , false :0)
//---------------------------------------------------------------------------
static inline bool ReadBit ( uint64_t N , uint64_t Pos ) noexcept
{   //------------------ begin --------------------------
    return (  (N & (1ULL<<Pos )) != 0 );
};

//---------------------------------------------------------------------------
//  function :bitScanForward64
/// @author Martin Läuter(1997),Charles E.Leiserson,Harald Prokop,Keith H.Randall
/// @brief return the least significant one bit
/// @param [in] bb bitboard to scan
/// @return index (0..63) of least significant one bit
/// @remarks "Using de Bruijn Sequences to Index a 1 in a Computer Word"
//---------------------------------------------------------------------------
static inline uint32_t BitScanForward(uint64_t bb) noexcept
{   //------------------------- begin --------------------------------
    return (metaprog::DeBruijn_64[((bb & ((~bb)+1)) * 0x07EDD5E59A4E28C2ULL) >> 58]);
};
//---------------------------------------------------------------------------
//  function : NBits
/// @brief Obtain the number of bits equal or greater than N
/// @param [in] N : Number to examine
/// @exception none
/// @return Number of bits
//---------------------------------------------------------------------------
static inline uint32_t NBits ( uint64_t N)
{   //----------------------- begin -------------------------------------
    uint32_t  Pos = ( N & 0xffffffff00000000ULL)?32:0 ;
    if ( (N>>Pos) & 0xffff0000ULL ) Pos +=16  ;
    if ( (N>>Pos) & 0xff00ULL     ) Pos += 8 ;
    return ( metaprog::TMSB [ N >> Pos ] + Pos );
}


static inline uint32_t  BitScanReverse  ( uint64_t N )
{   //------------------ begin ---------------------------
    int NB = NBits ( N);
    return  ( NB == 0)?MAX32 : NB-1;
};


}; // end template < > cnf<8>

//##########################################################################
//                                                                        ##
//   A L I A S    M E T A P R O G R A M M I N G    F U N C T I O N S      ##
//                                                                        ##
//##########################################################################
constexpr auto MASK    = cnf<NByte>::MASK ;
constexpr auto MASK32  = cnf<4>::MASK ;
constexpr auto MASK64  = cnf<8>::MASK ;

constexpr auto MS1B = cnf<NByte>::MS1B ;
constexpr auto MS1B_32 = cnf<4>::MS1B ;
constexpr auto MS1B_64 = cnf<8>::MS1B ;

constexpr auto SIZE_ALIGNED     = cnf<NByte>::SIZE_ALIGNED ;
constexpr auto SIZE_ALIGNED32   = cnf<4>::SIZE_ALIGNED ;
constexpr auto SIZE_ALIGNED64   = cnf<8>::SIZE_ALIGNED ;

constexpr auto BITS_ALIGN     = cnf<NByte>::BITS_ALIGN ;
constexpr auto BITS_ALIGN32   = cnf<4>::BITS_ALIGN ;
constexpr auto BITS_ALIGN64   = cnf<8>::BITS_ALIGN ;


//##########################################################################
//                                                                        ##
//            A L I A S   I N L I N E    F U N C T I O N S                ##
//                                                                        ##
//##########################################################################

constexpr auto ResetBit   = cnf<NByte>::ResetBit;
constexpr auto ResetBit32 = cnf<4>::ResetBit;
constexpr auto ResetBit64 = cnf<8>::ResetBit;

constexpr auto SetBit   = cnf<NByte>::SetBit;
constexpr auto SetBit32 = cnf<4>::SetBit;
constexpr auto SetBit64 = cnf<8>::SetBit;

constexpr auto ReadBit    = cnf<NByte>::ReadBit ;
constexpr auto ReadBit32  = cnf<4>::ReadBit ;
constexpr auto ReadBit64  = cnf<8>::ReadBit ;

constexpr auto LS1B       = cnf<NByte>::BitScanForward ;
constexpr auto LS1B_32    = cnf<4>::BitScanForward ;
constexpr auto LS1B_64    = cnf<8>::BitScanForward ;


auto BitScanReverse32 = cnf<4>::BitScanReverse ;
auto BitScanReverse64 = cnf<8>::BitScanReverse ;
auto BitScanReverse = cnf<NByte>::BitScanReverse ;

auto NBits = cnf<NByte>::NBits ;
auto NBits32 = cnf<4>::NBits ;
auto NBits64 = cnf<8>::NBits ;


//****************************************************************************
};//    End namespace util
};//    End namespace sort
};//    End namespace boost
//****************************************************************************
#endif
