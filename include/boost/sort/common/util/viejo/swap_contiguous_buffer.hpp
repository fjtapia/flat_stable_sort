//----------------------------------------------------------------------------
/// @file   swap_contiguous_buffer.hpp
/// @brief
///
/// @author Copyright (c) 2010 2013 Francisco José Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanyingfile LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#ifndef __BOOST_SORT_UTIL_SWAP_CONTIGUOUS_BUFFER_HPP
#define __BOOST_SORT_UTIL_SWAP_CONTIGUOUS_BUFFER_HPP

#include <memory>
#include <cassert>
#include <iostream>
#include <boost/sort/common/util/traits.hpp>

namespace boost
{
namespace sort
{
namespace common
{
namespace util
{

//
//###########################################################################
//                                                                         ##
//    ################################################################     ##
//    #                                                              #     ##
//    #                      C L A S S                               #     ##
//    #          S W A P _ C O N T I G U O U S _ B U F F E R         #     ##
//    #                                                              #     ##
//    ################################################################     ##
//                                                                         ##
//###########################################################################
//
//---------------------------------------------------------------------------
/// @class  swap_contiguous_buffer
/// @brief
/// @remarks
//---------------------------------------------------------------------------
template <class iter_t>
struct swap_contiguous_buffer
{
//----------------------------------------------------------------------------
//                       DEFINITIONS
//----------------------------------------------------------------------------
typedef value_iter<iter_t> value_t ;

//----------------------------------------------------------------------------
//                       VARIABLES
//----------------------------------------------------------------------------
size_t N1, N2, NP ;
iter_t first1, first2, last ;
value_t * P ;

//----------------------------------------------------------------------------
//                      PUBLIC   CONSTRUCTOR
//----------------------------------------------------------------------------
public:

swap_contiguous_buffer (iter_t ini1, iter_t ini2 , iter_t end ,
                        value_t *P1, size_t NP1)
                        :NP(NP1),first1(ini1),first2(ini2),last(end),P(P1)
{   //-------------------------------------- begin ---------------------------
    assert ( (ini2-ini1) >=0  and (end-ini2)>= 0);
    N1 = size_t ( ini2-ini1);
    N2 = size_t ( end- ini2);
    if ( N1 == 0 or N2 == 0) return ;

    //------------------------------------------------------------------------
    //  if N1 == N2 swap contents
    //------------------------------------------------------------------------
    if ( N1 == N2 )
    {   swap_equal() ;
        return ;
    };
    assert ( NP > 0);
    if ( N1 > N2 )
    {   //--------------------------------------------------------------------
        // N1 > N2      N1 <= ( N2+ NP )
        //--------------------------------------------------------------------
        if ( N1 <= ( N2+ NP ))  swap_forward () ;
        else
        {   //--------------------------------------------------------------------
            // N1 > N2      N1 > ( N2+ NP ) N2 <= NP
            //--------------------------------------------------------------------
            if ( N2 <= NP )  swap_shift_forward() ;
            //--------------------------------------------------------------------
            // N1 > N2      N1 > ( N2+ NP ) N2 > NP
            //--------------------------------------------------------------------
            else swap_general_forward() ;
        };
    }
    else
    {   //--------------------------------------------------------------------
        // N2 > N1      N2 <= ( N1+ NP )
        //--------------------------------------------------------------------
        if ( N2 <= ( N1+ NP ))    swap_backward () ;
        else
        {   //--------------------------------------------------------------------
            // N2 > N1      N2 > ( N1+ NP ) N1 <= NP
            //--------------------------------------------------------------------
            if ( N1 <= NP)  swap_shift_backward() ;
            //--------------------------------------------------------------------
            // N2 > N1      N2 > ( N1+ NP ) N1 > NP
            //--------------------------------------------------------------------
            else swap_general_backward() ;
        };
    };
};
//----------------------------------------------------------------------------
//   N1 == N2
//----------------------------------------------------------------------------
void swap_equal ( void)
{   //----------------------------- begin -----------------------------------
    for ( iter_t it1 = first1, it2 = first2 ; it2 != last ; ++it1,++it2)
        std::swap ( *it1, *it2);
};
//----------------------------------------------------------------------------
// N1 > N2      N1 <= ( N2+ NP )
//----------------------------------------------------------------------------
void swap_forward ( void)
{   //--------------------------------- begin --------------------------------
    iter_t itx = first1 + N2 ;
    value_t * Px = P ;
    //size_t Nx = N1 - N2 ;
    for ( ; itx != first2 ; ++itx,++Px)
        ::new ( static_cast<void*> ( Px)) value_t ( *itx);
    iter_t it1 = first1 , it2 = first2, it3 = first1 + N2 ;
    for ( ;it2 != last ; ++it1,++it2,++it3)
    {   *it3 = std::move ( *it1);
        *it1 = std::move ( *it2);
    };
    itx = first1 + ( N2<<1);
    Px = P ;
    for ( ; itx != last ; ++itx, ++Px)
    {   *itx = std::move ( *Px);
        Px->~value_t () ;
    };
};

//----------------------------------------------------------------------------
// N1 > N2      N1 > ( N2+ NP ) N2 <= NP
//----------------------------------------------------------------------------
void swap_shift_forward( void)
{   //----------------------------- begin ------------------------------------
    iter_t itx = first2 ;
    value_t *Px = P ;
    for ( ; itx != last ; ++itx,++Px)
        ::new ( static_cast<void*> ( Px)) value_t ( *itx);
    iter_t it1 = first2 -1, it2 = first2 + ( N2-1) ;
    for ( size_t i =0 ; i < N1 ; ++i, --it1,--it2)
        *it2 = std::move ( *it1);
    it1 =first1 ;
    Px = P ;
    for ( size_t i = 0 ; i < N2 ; ++i, ++it1,++Px)
    {   *it1 = std::move ( *Px);
        Px->~value_t () ;
    };
};
//----------------------------------------------------------------------------
// N1 > N2      N1 > ( N2+ NP ) N2 > NP
//----------------------------------------------------------------------------
void swap_general_forward(void)
{   //------------------------- begin ----------------------------------------
    iter_t it1 = first1, it2 = first2 ;
    for ( size_t i =0 ; i < N2 ; ++i,++it1,++it2)
        std::swap ( *it1, *it2);
    swap_contiguous_buffer ( first1 + N2, first2 , last , P , NP);
}
//----------------------------------------------------------------------------
// N2 > N1      N2 <= ( N1+ NP )
//----------------------------------------------------------------------------
void swap_backward ( void)
{   //--------------------------------- begin --------------------------------
    iter_t itx = first2 + N1 ;
    value_t *Px = P ;
    //size_t Nx = N2 - N1 ;
    for ( ; itx != last ; ++itx,++Px)
        ::new ( static_cast<void*> ( Px)) value_t ( *itx);

    iter_t it1 =first2 -1 ,it2 = first1 + ( N1+ N2 -1),it3 = first2 + (N1-1);
    iter_t it_end= first2 -1 ;
    for ( ;it3 != it_end; --it1,--it2,--it3)
    {   *it2 = std::move ( *it1);
        *it1 = std::move (*it3);
    };
    itx = first2;
    Px = P ;
    it_end =first2 + ( N2-N1) ;
    for ( ; itx != it_end ; ++itx, ++Px)
    {   *itx = std::move ( *Px);
        Px->~value_t () ;
    };
};
//----------------------------------------------------------------------------
// N2 > N1      N2 > ( N1+ NP ) N1 <= NP
//----------------------------------------------------------------------------
void swap_shift_backward( void)
{   //----------------------------- begin ------------------------------------
    iter_t itx = first1 ;
    value_t *Px = P ;
    for ( ; itx != first2 ; ++itx,++Px)
        ::new ( static_cast<void*> ( Px)) value_t ( *itx);
    iter_t it1 = first1, it2 = first2  ;
    for ( ; it2 != last ; ++it1,++it2)
        *it1 = std::move ( *it2);
    it1 =first1+N2 ;
    Px = P ;
    for (  ; it1 != last ;  ++it1,++Px)
    {   *it1 = std::move ( *Px);
        Px->~value_t () ;
    };
};
//----------------------------------------------------------------------------
// N2 > N1      N2 > ( N1+ NP ) N1 > NP
//----------------------------------------------------------------------------
void swap_general_backward ( void )
{   //------------------------- begin ----------------------------------------
    iter_t it1 = first1, it2 = first2 ;
    for ( size_t i =0 ; i < N1 ; ++i,++it1,++it2)
        std::swap ( *it1, *it2);
    swap_contiguous_buffer ( first2 , first2+ N1 , last , P , NP);
}

//---------------------------------------------------------------------------
};//               End of class mutex_read_read
//---------------------------------------------------------------------------

//***************************************************************************
};//    End namespace util
};//    End namespace common
};//    End namespace sort
};//    End namespace boost
//***************************************************************************
#endif
