//----------------------------------------------------------------------------
/// @file in_place_merge.hpp
/// @brief Intro Sort algorithm
///
/// @author Copyright (c) 2010 Francisco José Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanyingfile LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#ifndef __SORT_UTIL_IN_PLACE_MERGE_HPP
#define __SORT_UTIL_IN_PLACE_MERGE_HPP

#include <vector>
#include <type_traits>
#include <sort/tools/util_iterator.hpp>
#include <sort/util/range.hpp>

namespace sort
{
namespace util
{

//
//-----------------------------------------------------------------------------
//  function : in_place_merge_uncontiguous
/// @brief : merge two contiguous buffers
/// @tparam iter_t : iterator to the elements
/// @tparam compare : object for to compare two elements pointed by iter_t
///                   iterators
/// @param [in] first : iterator to the first element
/// @param [in] last : iterator to the element after the last in the range
/// @param [in] comp : object for to compare elements
/// @exception
/// @return true : not changes done
///         false : changes in the buffers
/// @remarks
//-----------------------------------------------------------------------------
template <class iter1_t  , class iter2_t , class iter3_t, class compare >
bool in_place_merge_uncontiguous ( const range<iter1_t> &src1,
		                           const range<iter2_t> &src2,
                                   const range<iter3_t> &aux, compare comp)
{	//------------------- metaprogramming ------------------------------------
	typedef range<iter1_t>         				range1_t;
	typedef range<iter2_t>         				range2_t;	
	typedef range<iter3_t>         				range3_t;	
    typedef typename range<iter1_t>::value_t type1 ;
    typedef typename range<iter2_t>::value_t type2 ;    
    typedef typename range<iter3_t>::value_t type3 ; 
       
    static_assert ( std::is_same<type1, type2>::value,
                   "Incompatible iterators\n");
    static_assert ( std::is_same<type3, type2>::value,
                   "Incompatible iterators\n");                   
    //--------------------- code -------------------------------------------
    if ( src1.size() == 0 or src2.size() == 0 ) return true ;
    if ( not comp ( *src2.first, *(src1.last -1)) ) return true ;
#if __DEBUG_SORT != 0
    assert ( aux.size() >= src1.size() ) ;
#endif
    iter1_t it1 = src1.first ;
    iter2_t it2 = src2.first ;    
    while ( it1 != src1.last and not comp ( *it2, *it1)) ++it1;
    range1_t R1 ( it1, src1.last) ;
    range3_t Rx = init_move ( aux, R1);
	iter3_t itx = aux.first ;
	
	while ( ( it1 != src1.last) and (it2 != src2.last))
		*(it1++) = (not comp (*it2,*itx))?std::move(*(itx++)):std::move(*(it2++));
	R1.first = it1 ;
	Rx.first = itx ;
	if ( it2 == src2.last)
	{	while ( R1.first != R1.last)
			*(R1.first ++) = std::move ( *(Rx.first ++));
		init_move ( src2, Rx ) ;
	}
	else
	{	range2_t R2 ( it2 , src2.last ) ;
		half_merge ( src2, Rx, R2, comp);
	};
    return false ;
};                                    

//
//-----------------------------------------------------------------------------
//  function : in_place_merge
/// @brief : merge two contiguous buffers
/// @tparam iter_t : iterator to the elements
/// @tparam compare : object for to compare two elements pointed by iter_t
///                   iterators
/// @param [in] first : iterator to the first element
/// @param [in] last : iterator to the element after the last in the range
/// @param [in] comp : object for to compare elements
/// @exception
/// @return true : not changes done
///         false : changes in the buffers
/// @remarks
//-----------------------------------------------------------------------------
template <class iter1_t  , class iter2_t , class compare >
inline range<iter1_t> in_place_merge (const range<iter1_t> &src1,
									  const range<iter1_t> &src2,
									  const range<iter2_t> &buf, compare  comp )
{   //---------------------------- begin ------------------------------------
	typedef range<iter1_t>         				range1_t;
	typedef range<iter2_t>         				range2_t;	
	typedef typename range<iter1_t>::value_t 	type1 ;
    typedef typename range<iter2_t>::value_t 	type2 ;    
           
    static_assert ( std::is_same<type1, type2>::value,
                   "Incompatible iterators\n");
    //---------------------------- begin --------------------------------------
#if __DEBUG_SORT != 0
    assert ( src1.last == src2.first) ;
#endif
    if ( src1.size() == 0 ) return src2 ;
    if ( src2.size() == 0 ) return src1 ; 
    if ( not comp (*src2.first , * ( src1.last -1)))
    	return concat ( src1, src2);
#if __DEBUG_SORT != 0
    assert ( buf.size() >= src1.size() ) ;
#endif
	iter1_t it1 = src1.first ;
	while ( it1 != src1.last and not comp ( *src2.first, *it1)) ++it1 ;
	if ( it1 == src1.last ) return concat ( src1, src2 ) ;
	range1_t RA1 ( src1.first , it1 ) , RA2 ( it1 , src1.last ) ;
	range2_t Rx = init_move ( buf, RA2 ) ;
	return concat ( RA1, half_merge ( concat(RA2, src2), Rx, src2, comp));
};
//
//-----------------------------------------------------------------------------
//  function : in_place_merge_big
/// @brief : merge two contiguous buffers
/// @tparam iter_t : iterator to the elements
/// @tparam compare : object for to compare two elements pointed by iter_t
///                   iterators
/// @param [in] first : iterator to the first element
/// @param [in] last : iterator to the element after the last in the range
/// @param [in] comp : object for to compare elements
/// @exception
/// @return true : not changes done
///         false : changes in the buffers
/// @remarks
//-----------------------------------------------------------------------------
template <class iter1_t  , class iter2_t , class compare >
inline range<iter1_t> in_place_merge_big (const range<iter1_t> &src1,
									      const range<iter1_t> &src2,
									      const range<iter2_t> &buf, compare  comp )
{   //---------------------------- begin ------------------------------------
	typedef range<iter1_t>         				range1_t;
	typedef range<iter2_t>         				range2_t;	
	typedef typename range<iter1_t>::value_t 	type1 ;
    typedef typename range<iter2_t>::value_t 	type2 ;    
           
    static_assert ( std::is_same<type1, type2>::value,
                   "Incompatible iterators\n");
    //---------------------------- begin --------------------------------------
#if __DEBUG_SORT != 0
    assert ( src1.last == src2.first) ;
#endif
    if ( src1.size() == 0 ) return src2 ;
    if ( src2.size() == 0 ) return src1 ; 
    if ( not comp (*src2.first , * ( src1.last -1)) )
    return concat ( src1, src2);
#if __DEBUG_SORT != 0
    assert ( buf.size() >= src1.size() ) ;
#endif
    range1_t RA1= src1, RA2 = src1 ;
    if ( not comp ( *src2.first , *src1.first))
    {	iter1_t itx = upper_bound ( src1.first, src2.first, *src1.first, comp);
		RA1.last = itx;
		RA2.first =itx ;
	    range2_t Rbuf = move ( buf, RA2 ) ;
		return concat (RA1,half_merge_big ( concat(RA2,src2), Rbuf, src2,comp));
	}
	else
	{	range2_t Rbuf = move ( buf, src1 ) ;
		return half_merge_big ( concat(src1,src2), Rbuf, src2,comp);
	};
};
//
//****************************************************************************
};//    End namespace util
};//    End namespace sort
//****************************************************************************
//
#endif
