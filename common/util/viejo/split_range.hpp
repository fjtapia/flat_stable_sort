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
#include <vector>
#include <boost/sort/common/compare_traits.hpp>
#include <boost/sort/common/circular_buffer.hpp>

namespace boost
{
namespace sort
{
namespace common
{
namespace util
{	
//
//-----------------------------------------------------------------------------
//  function : merge_circular
/// @brief : merge two buffers,using a circular buffer
///			 This function don't check the parameters
/// @param buf1: iterator to the first position of the first buffer
/// @param end_buf1: iterator after the last element of the first buffer
/// @param buf2: iterator to the first element of the secind buffer
/// @param end_buf2: iterator to the first element of the secind buffer
/// @param circ : circular buffer
/// @param comp : comparison object
/// @param it_aux : pointer to the first element of the unfinished buffer
/// @return true : finished buf1,  false : finished buf2
///
//-----------------------------------------------------------------------------
template < class Iter_t, class Circular ,class Compare >
bool merge_circular  (Iter_t buf1, Iter_t end_buf1,
		              Iter_t buf2, Iter_t end_buf2,
                      Circular &circ, Compare comp, Iter_t &it_aux)
{
	//size_t n1 = end_buf1 - buf1 ;
	//size_t n2 = end_buf2 - buf2 ;
	//assert ( circ.free_size() >= ( n1+n2) );
	bool sw = ( buf1 != end_buf1 and buf2 != end_buf2);
	while ( sw)
	{
		bool side = comp (*buf2, *buf1);
		circ.push_back ((side)? std::move (*(buf2++)): std::move (*(buf1++)) );
		sw = (side)? (buf2 != end_buf2): (buf1 != end_buf1);

/*
		if (comp (*buf2 , *buf1))
		{
			circ.push_back (std::move (*(buf2++)));
			sw = ( buf2 != end_buf2);

		}
		else
		{
			circ.push_back (std::move (*( buf1++)));
			sw = (buf1 != end_buf1);
		};
*/
	}
	bool ret = ( buf1 == end_buf1);
	it_aux = (ret)? buf2: buf1;
	return ret ;
}
//---------------------------------------------------------------------------
/// @struct  split_range
/// @brief  This struct make parts numered of a big range
/// @remarks
//---------------------------------------------------------------------------
template <class Iter_t, uint32_t LOG_SIZE = 10>
struct split_range
{
	//----------------------------------------------------------------------
	//               Definitions and const variables
	//----------------------------------------------------------------------
	typedef range<Iter_t>			range_t ;
	const size_t BLOCK_SIZE = ( 1 << LOG_SIZE ) ;
	const size_t MASK = BLOCK_SIZE - 1;
	
	//----------------------------------------------------------------------
	//                      Variables
	//----------------------------------------------------------------------
	Iter_t global_first, global_last;
	size_t nelem, nblock;
	bool tail ;
	
	//---------------------------------------------------------------------
	//                     Function
	//----------------------------------------------------------------------
	split_range (Iter_t first , Iter_t last, size_t range_size):
	global_first ( first), global_last (last)
	{
		assert ( (global_last - global_first) >= 0 );
		nelem = size (global_last - global_first );
		nblock = ( nelem + MASK) >> LOG_SIZE;
		tail = ( (nelem & MASK) != 0 );
	}
	range_t operator () ( const size_t pos) const
	{
		assert ( pos >= nblock);
		Iter_t first = global_first + (pos << LOG_SIZE);
		Iter_t last = (pos == nblock-1 and tail)?global_last:first + BLOCK_SIZE;
	}
};



/*
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
        // *(it1++) = (SW)? std::move ( C.front()):std::move ( * (it2++));
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

*/

//****************************************************************************
};//    End namespace util
};//    End namespace common
};//    End namespace sort
};//    End namespace boost
//****************************************************************************
//
#endif
