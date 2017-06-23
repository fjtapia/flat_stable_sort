//----------------------------------------------------------------------------
/// @file   scheduler.hpp
/// @brief  This file contains the implementation of the scheduler for
///         dispatch the works stored
///
/// @author Copyright (c) 2010 2015 Francisco José Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanyingfile LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#ifndef __BOOST_SORT_COMMON_SCHEDULER_HPP
#define __BOOST_SORT_COMMON_SCHEDULER_HPP

#include <boost/sort/common/spinlock.hpp>
#include <boost/sort/common/search.hpp>
#include <vector>
#include <deque>
#include <iostream>

namespace boost
{
namespace sort
{
namespace common
{

//
//###########################################################################
//                                                                         ##
//    ################################################################     ##
//    #                                                              #     ##
//    #           C L A S S      S C H E D U L E R                   #     ##
//    #                                                              #     ##
//    ################################################################     ##
//                                                                         ##
//###########################################################################

//
//---------------------------------------------------------------------------
/// @class  scheduler
/// @brief This class is a concurrent stack controled by a spin_lock
/// @remarks
//---------------------------------------------------------------------------
template < typename Func_t, typename Allocator = std::allocator< Func_t > >
struct scheduler
{
    //-----------------------------------------------------------------------
    //                     D E F I N I T I O N S
	//-----------------------------------------------------------------------

	struct work
	{
		size_t id ;
		Func_t fnc ;
		//------------------------------------------------------------------------
		//                       FUNCTIONS
		//------------------------------------------------------------------------
		work ( const size_t & id_thread, Func_t && func )
		       :id ( id_thread) , fnc ( std::move ( func)){};

		template < class ...Arg>
		work (const size_t & id_thread, Arg ...args )
		       :id ( id_thread) , fnc ( args ...){};
	};


	struct filter_work
	{	typedef size_t key;
		const key & operator() ( const work &w) const { return w.id;};
	};

	typedef work                   				  value_t ;
	typedef typename std::allocator_traits<Allocator>::template rebind_alloc<value_t> alloc_value_t ;
	typedef std::deque <value_t,Allocator>      deque_t ;
	typedef typename deque_t::iterator          it_deque;
	//typedef std::vector <value_t,alloc_value_t>     vector_t ;
	//typedef typename vector_t::iterator         it_vector;
	typedef std::unique_lock<spinlock_t>		lock_t ;
	typedef std::hash<std::thread::id> 			hash_t;
	typedef typename std::allocator_traits<Allocator>::template rebind_alloc<vector_t> alloc_vector_t ;
	//-----------------------------------------------------------------------
	//                   V A R I A B L E S 
	//-----------------------------------------------------------------------
	std::vector<deque_t, alloc_vector_t>	V ;
	mutable spinlock_t  	spl;
	size_t 					last_prior, nelem ;
	hash_t 					thash;
	filter_work 			flt;
	std::less<size_t> 		comp ;

	//------------------------------------------------------------------------
	//  function : scheduler
	/// @brief  constructor
	//------------------------------------------------------------------------
	scheduler ( void):V(),last_prior (0), nelem (0){  } ;

	//
	//-----------------------------------------------------------------------
	//  function : scheduler
	/// @brief  Copy & move constructor
	/// @param [in] VT : stack_cnc from where copy the data
	//-----------------------------------------------------------------------
	scheduler ( scheduler && VT) = delete ;
	scheduler ( const scheduler & VT) = delete ;
	//
	//------------------------------------------------------------------------
	//  function : ~scheduler
	/// @brief  Destructor
	//------------------------------------------------------------------------
	virtual ~scheduler (void) {  V.clear(); };
	//
	//------------------------------------------------------------------------
	//  function : operator =
	/// @brief Asignation operator
	/// @param [in] VT : stack_cnc from where copy the data
	/// @return Reference to the stack_cnc after the copy
	//------------------------------------------------------------------------
	scheduler & operator= (const scheduler &VT)  = delete ;
	//
	//------------------------------------------------------------------------
	//  function : size
	/// @brief Asignation operator
	/// @param [in] VT : stack_cnc from where copy the data
	/// @return Reference to the stack_cnc after the copy
	//------------------------------------------------------------------------
	size_t size (void) const
	{
		lock_t  s(spl);
		return nelem ;
	};
	//
	//------------------------------------------------------------------------
	//  function : clear
	/// @brief Delete all the elements of the stack_cnc.
	//------------------------------------------------------------------------
	void clear_all(void)
	{
		lock_t  s(spl);
		V.clear ( );
		last_prior = 0 ;
		nelem = 0 ;
	};
	//
	//------------------------------------------------------------------------
	//  function : clear_prior
	/// @brief Delete all the elements of the stack_cnc.
	//------------------------------------------------------------------------
	void clear_prior(uint32_t prior)
	{
		lock_t  s(spl);
		if ( prior < V.size() )
		{	nelem -= V[prior].size() ;
			V[prior].clear ( );
		};
		while ( V[last_prior].empty() ) last_prior -- ;
	};
	void reserve ( uint32_t nlevel, uint32_t nelems)
	{
		if ( nlevel > V.size())
			V.resize( nlevel);
		for ( uint32_t i =0 ; i < V.size() ; ++i)
		{
			if ( V[i].capacity() < nelems)
				V[i].reserve ( nelems);
		};
	}
/*
	//
	//------------------------------------------------------------------------
	//  function : insert
	/// @brief Insert one element in the back of the container
	/// @param [in] D : value to insert. Can ve a value, a reference or an
	///                 rvalue
	/// @return iterator to the element inserted
	/// @remarks This operation is O ( const )
	//------------------------------------------------------------------------
	void insert ( uint32_t prior, Func_t && f )
	{
		lock_t s(spl);
		size_t num = thash ( std::thread::get_id());
		while ( prior >= V.size() ) V.emplace_back() ;

		it_vector itdq = std::find (V[prior].first, V[prior].last, num);
		V[prior].insert (std::thread::get_id(),std::move (f));
		nelem ++ ;
		if ( prior > last_prior ) last_prior = prior ;
	};
*/
	//
	//------------------------------------------------------------------------
	//  function : insert_emplace
	/// @brief Insert one element in the back of the container
	/// @param [in] args :group of arguments for to build the object to insert
	/// @return iterator to the element inserted
	/// @remarks This operation is O ( const )
	//------------------------------------------------------------------------
	template <class ... Args>
	void insert_num ( size_t num, uint32_t prior, Args && ... args )
	{
		lock_t s(spl);
		while ( prior >= V.size() ) V.emplace_back() ;
		it_vector itdq = find_first (V[prior].begin(), V[prior].end(),
				                    num, comp, flt);
		V[prior].emplace (itdq, num,std::forward <Args>(args) ...);
		nelem ++ ;
		if ( prior > last_prior ) last_prior = prior ;
	};
	template <class ... Args>
	void insert ( uint32_t prior, Args && ... args )
	{
		size_t num = thash ( std::this_thread::get_id());
		insert_num ( num, prior, std::forward <Args>(args) ...);
	};

	//
	//------------------------------------------------------------------------
	//  function : insert
	/// @brief Insert one element in the back of the container
	/// @param [in] D : value to insert. Can ve a value, a reference or an rvalue
	/// @return iterator to the element inserted
	/// @remarks This operation is O ( const )
	//------------------------------------------------------------------------
	scheduler & insert (uint32_t prior, vector_t & V1)
	{
		lock_t s(spl);
		size_t num = thash ( std::this_thread::get_id());
		while ( prior >= V.size() ) V.emplace_back() ;
		it_vector itdq = find_first (V[prior].begin(), V[prior].end(), num, comp, flt);
		V[prior].insert (itdq,V1.begin(), V1.end());

		nelem += V1.size() ;
		if ( prior > last_prior ) last_prior = prior ;
		return *this ;
	};
	//
	//------------------------------------------------------------------------
	//  function : extract
	/// @brief erase the last element of the tree and return a copy
	/// @param [out] V : reference to a variable where copy the element
	/// @return code of the operation
	///         0- Element erased
	///         1 - Empty tree
	/// @remarks This operation is O(1)
	//------------------------------------------------------------------------
	bool extract_num ( size_t num ,uint32_t & prior, Func_t  & f)
	{
		lock_t s(spl);
		if ( nelem == 0 ) return false ;
		size_t pos = last_prior +1 ;
		while ( pos != 0)
		{
			if ( V[--pos].empty()) continue ;
			it_vector itaux = find_last (V[pos].begin(), V[pos].end(),
					                    num, comp, flt);
			if ( itaux == V[pos].end()) continue ;
			f = std::move ( itaux->fnc);
			V[pos].erase ( itaux);
			prior = pos ;
			--nelem;
			if ( nelem == 0) last_prior = 0 ;
			else
			{
				while ( V[last_prior].empty() ) --last_prior ;
			};
			//print ( std::cout);
			return true ;
		};
		// don't exist any work of the thread
		for ( size_t i =0 ; i < last_prior+1; ++i)
		{
			if ( V[i].empty()) continue;
			f = std::move ( V[i].back().fnc );
			V[i].pop_back() ;
			prior = i ;
			--nelem ;
			if ( nelem == 0) last_prior = 0 ;
			else
			{
				while ( V[last_prior].empty() ) --last_prior ;
			};
			//print ( std::cout);
			return true ;
		};
		//print ( std::cout);

		return false;
	};

	bool extract ( uint32_t & prior, Func_t  & f)
	{

		size_t num = thash(std::this_thread::get_id() );
		return extract_num ( num, prior, f );
	};

	//*************************************************************************
	//               P R I N T      F U N C T I O N S
	//************************************************************************
	std::ostream & print ( std::ostream & out, const work & w)const
	{
		out<<"["<<w.id<<", "<<w.fnc<<"] ";
		return out ;
	};
	std::ostream & print ( std::ostream & out, const vector_t &d )const
	{
		for ( uint32_t i =0  ; i < d.size() ; ++i) print ( out,d[i]);
		return out;
	};

	std::ostream & print ( std::ostream &out )const
	{

		out<<"last_prior :"<<last_prior<<"  nelem :"<<nelem<<std::endl;
		for ( uint32_t level =0 ; level < V.size() ;++level)
		{
			out<<"level "<<level<<" ";
			print ( out,V[level]);
			out<<std::endl;
		};
		return out;
	};


}; // end class scheduler
//*************************************************************************
//               P R I N T      F U N C T I O N S
//************************************************************************

template < typename Func_t, typename Allocator = std::allocator< Func_t > >
std::ostream & operator << ( std::ostream &out, const scheduler<Func_t,Allocator> &S)
{	std::unique_lock<spinlock_t>	s(S.spl);
	return S.print ( out);
};

//***************************************************************************
}; // end namespace common
}; // end namespace sort
}; // end namespace boost
//***************************************************************************
#endif
