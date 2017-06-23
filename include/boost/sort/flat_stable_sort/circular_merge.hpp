//----------------------------------------------------------------------------
/// @file circular_merge.hpp
/// @brief
///
/// @author Copyright (c) 2010 Francisco José Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanyingfile LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#ifndef __SORT_UTIL_CIRCULAR_MERGE_HPP
#define __SORT_UTIL_CIRCULAR_MERGE_HPP

#include <functional>
#include <memory>
#include <type_traits>
#include <vector>
#include <tools/util_iterator.hpp>
#include <tools/algorithm.hpp>
#include <tools/circular.hpp>


namespace sort
{
namespace util
{

//-----------------------------------------------------------------------------
//  function : merge two contiguous buffers sorted
/// @brief
/// @param [in/out]
/// @exception
/// @return
/// @remarks This merge is stable
//-----------------------------------------------------------------------------
template <  class iter_t,
            class Compare =std::less<typename iter_value<iter_t>::type >
         >
inline void circular_merge( iter_t first1, iter_t first2, iter_t last,
                        Compare comp ,
                        circular <typename iter_value<iter_t>::type > &C  )
{   //------------------------- begin ----------------------
    if ( first1 == first2 or first2 == last or
        not comp ( *first2, * (first2 -1))) return ;

    //------------------------------------------------------------------------
    C.clear() ;
    if ( not comp (*first1 , * (last -1) ))
    {   swap_contiguous_buffer<iter_t>(first1,first2,last,C.Ptr(),C.max_size());
        return ;
    }
    iter_t it1  = first1, it2 = first2 ;

    //------------------------------------------------------------------------
    // First half process
    //------------------------------------------------------------------------
    while (it1 != first2 and not comp ( *it2, *it1) ) ++it1 ;
    while ( it1 != first2 and not C.full() )
    {
        C.push_back ( std::move ( *it1));
        (not comp (*it2, C.front()))? C.pop_move_front (*(it1++))
                                     : (*(it1++) = std::move ( * (it2++)) );
        //bool SW = not comp (  *it2, C.front());
        //*(it1++) = (SW)? std::move ( C.front()):std::move ( * (it2++));
        //if ( SW) C.pop_front() ;
    };
    if ( it1 != first2 and C.full() )
    {   iter_t Alfa = first2 ;
        while  ( Alfa != it2 )
        {   C.pop_move_front( *(Alfa++) );
            //*Alfa = std::move ( C.front());
            //C.pop_front() ;
        };
        merge_circular ( first2, it2, last, comp, C);
        merge_circular ( it1, first2, last, comp, C);
    }
    else
    {   //--------------------------------------------------------------------
        // Second half process
        //--------------------------------------------------------------------
        while ( it2 != last and not C.empty() )
        {   bool SW = not comp ( *it2, C.front());
            *(it1++) =(SW)?std::move ( C.front()):std::move ( *(it2++) );
            if ( SW) C.pop_front() ;
        };
        while ( not C.empty() )
        {   *(it1++) =std::move ( C.front());
            C.pop_front() ;
        };
    };
} ;
//****************************************************************************
};//    End namespace util
};//    End namespace sort
//****************************************************************************
//
#endif
