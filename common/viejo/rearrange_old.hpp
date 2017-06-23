//----------------------------------------------------------------------------
/// @file rearrange.hpp
/// @brief Indirect algorithm
///
/// @author Copyright (c) 2016 Francisco Jose Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanying file LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#ifndef __BOOST_SORT_COMMON_REARRANGE_HPP
#ifndef __BOOST_SORT_COMMON_REARRANGE_HPP

//#include <boost/sort/common/atomic.hpp>
#include <boost/sort/common/compare_traits.hpp>
#include <functional>
#include <iterator>
#include <type_traits>
#include <vector>

namespace boost
{
namespace sort
{
namespace common
{
	
template <class T>
struct filter_pass
{
	typedef T key;
	const key & operator() ( const T & val)const { return val;};
};	
//
//-----------------------------------------------------------------------------
//  function : rearrange_iter
/// @brief This function transform a logical sort of the elements in the index  
///        of iterators in a physical sort. 
//
/// @param global_first : iterator to the first element of the data
/// @param [in] index : vector of the iterators
//-----------------------------------------------------------------------------
template < class Iter_t >
void rearrange_iter (Iter_t global_first, std::vector< Iter_t > &index)
{
	//-------------------------------------------------------------------------
	//          METAPROGRAMMING AND DEFINITIONS
	//-------------------------------------------------------------------------	
    typedef iter_value< Iter_t > value_t;

	//-------------------------------------------------------------------------
	//                     CODE
	//-------------------------------------------------------------------------
    size_t pos_dest = 0;
    size_t pos_src = 0;
    size_t pos_in_vector = 0;
    size_t nelem = index.size ( );
    Iter_t it_dest, it_src;

    while (pos_in_vector < nelem) 
    {
        while (pos_in_vector < nelem and
               (size_t (index[pos_in_vector] - global_first)) == pos_in_vector)
        {
            ++pos_in_vector;
        };

        if (pos_in_vector == nelem) return;
        pos_dest = pos_src = pos_in_vector;
        it_dest = global_first + pos_dest;
        value_t Aux = std::move (*it_dest);

        while ((pos_src = (size_t (index[pos_dest] - global_first))) !=
               pos_in_vector)
        {
            index[pos_dest] = it_dest;
            it_src = global_first + pos_src;
            *it_dest = std::move (*it_src);
            it_dest = it_src;
            pos_dest = pos_src;
        };

        *it_dest = std::move (Aux);
        index[pos_dest] = it_dest;
        ++pos_in_vector;
    };
};

//
//-----------------------------------------------------------------------------
//  function : rearrange_pos
/// @brief This function transform a logical sort of the elements in the index  
///        of iterators in a physical sort. 
//
/// @param global_first : iterator to the first element of the data
/// @param [in] index : vector of the iterators
//-----------------------------------------------------------------------------
template < class Iter_t, class Number >
void rearrange_pos (Iter_t global_first, std::vector< Number> &index)
{	
	//-------------------------------------------------------------------------
	//          METAPROGRAMMING AND DEFINITIONS
	//-------------------------------------------------------------------------
	static_assert ( std::is_integral<Number>::value, "Incompatible Types");
	typedef iter_value< Iter_t > value_t;

	//-------------------------------------------------------------------------
	//                     CODE
	//-------------------------------------------------------------------------
    size_t pos_dest = 0;
    size_t pos_src = 0;
    size_t pos_in_vector = 0;
    size_t nelem = index.size ( );
    Iter_t it_dest (global_first), it_src(global_first);

    while (pos_in_vector < nelem) 
    {
        while (pos_in_vector < nelem and
               index[pos_in_vector] == pos_in_vector)
        {
            ++pos_in_vector;
        };

        if (pos_in_vector == nelem) return;
        pos_dest = pos_src = pos_in_vector;
        it_dest = global_first + pos_dest;
        value_t Aux = std::move (*it_dest);

        while ((pos_src = index[pos_dest]) != pos_in_vector)
        {
            index[pos_dest] = it_dest - global_first;
            it_src = global_first + pos_src;
            *it_dest = std::move (*it_src);
            it_dest = it_src;
            pos_dest = pos_src;
        };

        *it_dest = std::move (Aux);
        index[pos_dest] = it_dest - global_first;
        ++pos_in_vector;
    };
};
//
//****************************************************************************
}; //    End namespace common
}; //    End namespace sort
}; //    End namespace boost
//****************************************************************************
//
#endif
