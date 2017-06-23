//----------------------------------------------------------------------------
/// @file circular_merge_sort.hpp
/// @brief Quick Sort algorithm
///
/// @author Copyright(c) 2010 2015 Francisco José Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanyingfile LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#ifndef __BOOST_SORT_ALGORITHM_CIRCULAR_MERGE_SORT_HPP
#define __BOOST_SORT_ALGORITHM_CIRCULAR_MERGE_SORT_HPP

#include <functional>
#include <memory>
#include <type_traits>
#include <vector>
#include <boost/sort/util/algorithm.hpp>
#include <boost/sort/util/util_iterator.hpp>
#include <boost/sort/algorithm/insertion_sort.hpp>
#include <boost/sort/algorithm/token.hpp>
#include <boost/sort/algorithm/indirect.hpp>
#include <boost/sort/util/buffer.hpp>



namespace boost
{
namespace sort
{
namespace algorithm
{

using namespace boost::sort::util;


template < class iter_t,
           typename compare=std::less<typename iter_value<iter_t>::type > >
struct circular_merge_sort_tag
{
//------------------------------------------------------------------------
//                 Definitions and constants
//------------------------------------------------------------------------
typedef typename iter_value<iter_t>::type           value_t ;
static const uint32_t MSort_min = 36 ;
//---------------------------------------------------------------------------
//  function : circular_merge_sort
/// @brief Merge Sort algorithm ( stable sort)
/// @param [in] first : first iterator to the elements to sort
/// @param [in] last : last iterator to the elements to sort
/// @param [in] P : pointer of a data area with at least (N+1)/2 elements
/// @exception
/// @return
/// @remarks
//---------------------------------------------------------------------------
circular_merge_sort_tag (iter_t first, iter_t last, compare comp = compare() )
{   //-------------------------- begin ------------------------------
    auto N = last - first ;
    assert ( N >= 0 );
    if ( N < MSort_min)
    {   insertion_sort ( first, last , comp);
        return  ;
    };
    uint32_t NB = NBits ( N);
    //if ( NB > 8 ) NB-=3 ;
    if ( NB > 8 ) NB-=3 ;
    circular<value_t> C ( NB);
    circular_merge_sort_internal ( first , last , comp,C );

};

//---------------------------------------------------------------------------
//  function : circular_merge_sort_internal
/// @brief Merge Sort algorithm ( stable sort)
/// @param [in] first : first iterator to the elements to sort
/// @param [in] last : last iterator to the elements to sort
/// @param [in] P : pointer of a data area with at least (N+1)/2 elements
/// @exception
/// @return
/// @remarks
//---------------------------------------------------------------------------
static inline void circular_merge_sort_internal (   iter_t first ,
                                                    iter_t last,
                                                    compare comp,
                                                    circular<value_t> & C )
{   //---------------------------- begin ------------------------------------
    auto N = last - first ;
    assert ( N >= 0 );
    size_t N2 = ( N+1)>>1 ;

    if ( N2 > MSort_min)
    {   circular_merge_sort_internal ( first , first + N2, comp, C );
        circular_merge_sort_internal ( first + N2 , last , comp, C );
    }
    else
    {   insertion_sort ( first , first + N2,comp );
        insertion_sort ( first + N2 , last ,comp );
    };
    merge_circular<iter_t,compare> ( first, (first+N2) , last , comp, C);

//----------------------------------------------------------------------------
};//        End function merge_sort_internal
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
};//        End of class merge_sort_tag
//----------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  function : merge_sort
/// @brief Merge Sort algorithm ( stable sort)
/// @param [in] first : first iterator to the elements to sort
/// @param [in] last : last iterator to the elements to sort
/// @param [in] P : pointer of a data area with at least (N+1)/2 elements
/// @exception
/// @return
/// @remarks
//---------------------------------------------------------------------------
template < class iter_t,
           typename compare = std::less<typename iter_value<iter_t>::type>
         >
void circular_merge_sort (iter_t first, iter_t last, compare comp = compare() )
{   //----------------------------- begin ----------------------------
    circular_merge_sort_tag<iter_t,compare> ( first, last , comp);
};

//############################################################################
//                                                                          ##
//                I N D I R E C T     F U N C T I O N S                     ##
//                                                                          ##
//############################################################################
template < class iter_t,
           typename compare = std::less<typename iter_value<iter_t>::type>
         >
void indirect_circular_merge_sort ( iter_t first, iter_t last ,
                                    compare comp = compare() )
{   //------------------------------- begin--------------------------
    typedef less_ptr_no_null <iter_t, compare>      compare_ptr ;

    std::vector<iter_t> VP ;
    create_index ( first , last , VP);
    circular_merge_sort  ( VP.begin() , VP.end(), compare_ptr(comp) );
    sort_index ( first , VP) ;
};
//
//****************************************************************************
};//    End namespace algorithm
};//    End namespace sort
};//    End namespace boost
//****************************************************************************
//
#endif
