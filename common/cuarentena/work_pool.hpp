//----------------------------------------------------------------------------
/// @file work_pool.hpp
/// @brief Basic layer for to simplify the use of atomic functions
/// @author Copyright(c) 2010 2015 Francisco José Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanyingfile LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#ifndef __SORT_UTIL_WORK_POOL_HPP
#define __SORT_UTIL_WORK_POOL_HPP

#include <deque>
#include <memory>
#include <atomic>
#include <thread>
#include <iostream>
#include <sort/tools/deque_cnc.hpp>
#include <sort/tools/atomic.hpp>
#include <sort/algorithm/intro_sort.hpp>
#include <sort/util/range.hpp>
#include <sort/util/block_merge.hpp>

namespace sort
{
namespace util
{
typedef std::atomic<uint32_t> atomic_t ;
//-----------------------------------------------------------------------------
//               D E F I N I T I O N S
//-----------------------------------------------------------------------------
struct token_mono
{	//--------------------------- variables -----------------------------
	atomic_t *pfather ;

	//---------------------------- functions ------------------------------
	token_mono ( atomic_t *p1 = nullptr ) :pfather (p1){};

	template <typename ...Args>
	virtual void exec ( Args &&...args ) = 0;

	virtual ~token ()
	{	if ( pfather != nullptr ) tools::atomic_sub<uint32_t>( *pfather, 1);
	} ;
};

//-----------------------------------------------------------------------------
//               D E F I N I T I O N S
//-----------------------------------------------------------------------------
struct  token_dual
{	//---------------------- Variables -------------------------------------
	atomic_t *pfather ;
	atomic_t son ;
	uint32_t mode ;

	//------------------------- Functions -------------------------------------
	token_dual ( atomic_t *pf, atomic_t as ): pfather(pf),son(as),mode(0){	};
	atomic_t  & get_son(void){ return son ;};
	
	template <typename ...Args>
	virtual bool exec ( Args &&...args ) = 0;
	
	virtual ~token_dual ()
	{	if ( pfather != nullptr ) tools::atomic_sub<uint32_t>( *pfather, 1);
	} ;
};

//-----------------------------------------------------------------------------
//               T Y P E   D E F I N I T I O N
//-----------------------------------------------------------------------------
typedef std::unique_ptr <token_mono> ptr_tkmono;
typedef std::unique_ptr <token_dual> ptr_tkdual ;

typedef sort::tools::deque_cnc <ptr_tkmono>  deque_tkmono ;
typedef sort::tools::deque_cnc <ptr_tkdual>  deque_tkdual ;

//-----------------------------------------------------------------------------
//              S T R U C T    D I S P A T C H E R
//-----------------------------------------------------------------------------
struct dispatcher
{	//--------------------------- Variables ----------------------------------
	deque_tkmono  dq_mono ;
	deque_tkdual  dq_dual ;
	atomic_t  N ;
	
	//--------------------- Functions -----------------------------------------
	dispatcher (uint32_t NN = 1):N(NN){};
	bool final ()
	{ 	return ( tools::atomic_read<uint32_t>(N) == 0) ;
	};
};
//----------------------------------------------------------------------------
//             S T R U C T   E X E C U T O R
//----------------------------------------------------------------------------
struct executor 
{ 	//--------------------- variables -----------------------
	dispatcher &dp ;

	//--------------------- functions -----------------------
	executor ( dispatcher &d ):dp(d){} ;
	
	template <typename ...Args>
	void exec (Args &&...args  )
	{	ptr_tkmono ptkm  ( nullptr) ;
		ptr_tkdual ptkd  ( nullptr ) ;

		while ( not final())
		{	if ( dp.stk_mono.pop_move_back( ptkm) )
			{	ptkm->exec(args) ;
				delete (ptkm.release()) ;
			}
        	else
        	{	if (  dp.stk_dual.pop_move_back( ptkd) )
				{	bool SW =ptkd->exec( args) ;
					if ( SW ) delete (ptkd.release()) ;
					else  dqm.emplace_front ( std::move ( ptkd)) ;
				}
				else  std::this_thread::yield() ;
			};
		};
	};
};

//****************************************************************************
}; // End namespace util
}; // End namespace sort
//****************************************************************************
#endif
