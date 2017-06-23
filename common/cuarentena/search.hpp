//----------------------------------------------------------------------------
/// @file search.hpp
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
#ifndef __BOOST_SORT_UTIL_SEARCH_HPP
#define __BOOST_SORT_UTIL_SEARCH_HPP

#include <functional>
#include <memory>
#include <type_traits>
#include <vector>
#include <boost/sort/util/util_iterator.hpp>
#include <boost/sort/util/algorithm.hpp>
#include <boost/sort/algorithm/insertion_sort.hpp>


namespace tsort
{
namespace util
{

//-----------------------------------------------------------------------------
//  function : find_first_internal
/// @brief find if an value exist in the range [first, last). If exist the
///        iterator it_out point it and return true .
///        If don't exist, the pointed by it_out is unespecified, and the
///        function return false
/// @param [in] first : iterator to the first element of the range
/// @param [in] last : iterator to the last element of the range
/// @param [in] val : value to find
/// @param [in] comp : object for to compare two value_t objects
/// @return iterator to the element found
/// @remarks
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template <  class iter_t ,
            class value_t = typename iter_value<iter_t>::type ,
            typename compare = std::less<value_t>
         >
inline iter_t find_first_internal ( iter_t first, iter_t last ,
                             const value_t &val,compare &comp )
{   //---------------------------- begin -------------------------------------
    auto N =( last - first );
    assert ( N >= 0 );
    if ( N == 0) return last ;
    iter_t LI = first , LS = first + (N-1),it_out = first;
    while ( LI != LS)
    {   it_out = LI + ( (LS -LI)>>1);
        if ( comp ( *it_out,val)) LI = it_out +1 ; else LS = it_out ;
    };
    return (comp (*LS, val))?last:LS ;
};
//
//-----------------------------------------------------------------------------
//  function : find_last_internal
/// @brief find the last value equal than val or the first value greater in the
///        range [first, last). If exist the iterator it_out point it and
///        return true .
///        If don't exist, the pointed by it_out is unespecified, and the
///        function return false
/// @param [in] first : iterator to the first element of the range
/// @param [in] last : iterator to the last element of the range
/// @param [in] val : value to find
/// @param [in] comp : object for to compare two value_t objects
/// @return iterator to the element found
/// @remarks
//-----------------------------------------------------------------------------
template <  class iter_t ,
            class value_t = typename iter_value<iter_t>::type ,
            typename compare = std::less<value_t>               >
inline iter_t find_last_internal ( iter_t first, iter_t last ,
                            const value_t &val, compare &comp )
{   //---------------------------- begin -------------------------------------
    auto N =( last - first );
    assert ( N >= 0 );
    if ( N == 0) return last ;
    iter_t LI = first , LS = first + (N-1),it_out = first ;;
    while ( LI != LS)
    {   it_out = LI + ( (LS -LI+1)>>1);
        if ( comp (val, *it_out  )) LS = it_out -1 ; else LI = it_out ;
    };
    //return ( comp (val,*LS))?last:LS ;
    return LS ;
};
//-----------------------------------------------------------------------------
//  function : find_first_rep
/// @brief find if an value exist in the range [first, last). If exist the
///        iterator it_out point it and return true .
///        If don't exist, the pointed by it_out is unespecified, and the
///        function return false
/// @param [in] first : iterator to the first element of the range
/// @param [in] last : iterator to the last element of the range
/// @param [in] val : value to find
/// @param [in] comp : object for to compare two value_t objects
/// @return iterator to the element found
/// @remarks
//-----------------------------------------------------------------------------
template <  class iter_t ,
            class value_t = typename iter_value<iter_t>::type ,
            typename compare = std::less<value_t>               >
inline iter_t find_first_rep ( iter_t first, iter_t last ,
                               const value_t &val,compare comp  = compare())
{   //---------------------------- begin -------------------------------------
    iter_t itAux = find_first_internal( first, last, val,comp);
    if ( itAux != last and comp (val, *itAux)) itAux = last ;
    return itAux ;
};
//
//-----------------------------------------------------------------------------
//  function : find_equal_greater
/// @brief find the first value equal than val or the first value greater in the
///        range [first, last). If exist the iterator it_out point it and
///        return true .
///        If don't exist, the pointed by it_out is unespecified, and the
///        function return false
/// @param [in] first : iterator to the first element of the range
/// @param [in] last : iterator to the last element of the range
/// @param [in] val : value to find
/// @param [in] comp : object for to compare two value_t objects
/// @return iterator to the element found
/// @remarks
//-----------------------------------------------------------------------------
template <  class iter_t ,
            class value_t = typename iter_value<iter_t>::type ,
            typename compare = std::less<value_t>               >
inline iter_t find_equal_greater ( iter_t first, iter_t last ,
                                   const value_t &val,compare comp = compare() )
{   //---------------------------- begin -------------------------------------
    return find_first_internal( first, last, val,comp);
};
//
//-----------------------------------------------------------------------------
//  function : find_equal_less
/// @brief find the last value equal than val or the first value greater in the
///        range [first, last). If exist the iterator it_out point it and
///        return true .
///        If don't exist, the pointed by it_out is unespecified, and the
///        function return false
/// @param [in] first : iterator to the first element of the range
/// @param [in] last : iterator to the last element of the range
/// @param [in] val : value to find
/// @param [in] comp : object for to compare two value_t objects
/// @return iterator to the element found
/// @remarks
//-----------------------------------------------------------------------------
template <  class iter_t ,
            class value_t = typename iter_value<iter_t>::type ,
            typename compare = std::less<value_t>               >
inline iter_t find_equal_less ( iter_t first, iter_t last ,
                                const value_t &val, compare comp= compare() )
{   //---------------------------- begin -------------------------------------
    iter_t itAux = find_last_internal( first, last, val,comp);
    return ( comp (val,*itAux))?last:itAux ;
};
//
//-----------------------------------------------------------------------------
//  function : find_last_rep
/// @brief find the last value equal than val or the first value greater in the
///        range [first, last). If exist the iterator it_out point it and
///        return true .
///        If don't exist, the pointed by it_out is unespecified, and the
///        function return false
/// @param [in] first : iterator to the first element of the range
/// @param [in] last : iterator to the last element of the range
/// @param [in] val : value to find
/// @param [in] comp : object for to compare two value_t objects
/// @return iterator to the element found
/// @remarks
//-----------------------------------------------------------------------------
template <  class iter_t ,
            class value_t = typename iter_value<iter_t>::type ,
            typename compare = std::less<value_t>               >
inline iter_t  find_last_rep ( iter_t first, iter_t last ,
                               const value_t &val, compare comp = compare() )
{   //---------------------------- begin -------------------------------------
    iter_t itAux = find_last_internal( first, last, val,comp);
    if ( comp (val,*itAux)) itAux = last ;
    if ( itAux != last and comp (*itAux,val)) itAux = last ;
    return itAux ;
};

//----------------------------------------------------------------------------
//  function : lower_bound
/// @brief find the value val in the range. If there are repeated
///        elements, return an const_iterator to the first of them.
///        If the key don't exist return the iterator to the first element
///        greater than the key
/// @param [in] first : iterator to the first element of the range
/// @param [in] last : iterator to the last element of the range
/// @param [in] val : value to find
/// @param [in] comp : object for to compare two value_t objects
/// @return iterator to the element found
/// @remarks
//-----------------------------------------------------------------------------
template <  class iter_t ,
            class value_t = typename iter_value<iter_t>::type ,
            typename compare = std::less<value_t>               >
inline iter_t  lower_bound ( iter_t first, iter_t last ,
                             const value_t &val, compare comp = compare() )
{   //---------------------------- begin -------------------------------------
    return find_first_internal( first, last, val,comp);
};
//----------------------------------------------------------------------------
//  function :upper_bound
/// @brief return an const_iterator to the first element greater than K. If
///        don't exist return last
/// @param [in] first : iterator to the first element of the range
/// @param [in] last : iterator to the last element of the range
/// @param [in] val : value to find
/// @param [in] comp : object for to compare two value_t objects
/// @return iterator to the element found
/// @remarks
//-----------------------------------------------------------------------------
template <  class iter_t ,
            class value_t = typename iter_value<iter_t>::type ,
            typename compare = std::less<value_t>               >
inline iter_t  upper_bound ( iter_t first, iter_t last ,
                               const value_t &val, compare comp = compare() )
{   //---------------------------- begin -------------------------------------
    iter_t itAux = find_last_internal( first, last, val,comp);
    if ( itAux != last and not  comp (val,*itAux)) ++itAux ;
    return itAux ;
};
//----------------------------------------------------------------------------
//  function :equal_range
/// @brief return an const_iterator to the first element greater than K. If
///        don't exist return last
/// @param [in] first : iterator to the first element of the range
/// @param [in] last : iterator to the last element of the range
/// @param [in] val : value to find
/// @param [in] comp : object for to compare two value_t objects
/// @return iterator to the element found
/// @remarks
//-----------------------------------------------------------------------------
template <  class iter_t ,
            class value_t = typename iter_value<iter_t>::type ,
            typename compare = std::less<value_t>               >
inline std::pair<iter_t, iter_t> equal_range ( iter_t first, iter_t last ,
                                               const value_t &val,
                                               compare comp = compare() )
{   //---------------------------- begin -------------------------------------
    return std::make_pair(lower_bound(first, last, val,comp),
                          upper_bound(first, last, val,comp));
};
//****************************************************************************
};//    End namespace util
};//    End namespace tsort
//****************************************************************************
//
#endif
