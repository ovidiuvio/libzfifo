/*
	MIT License

	Copyright(c)[2022][Ovidiu Ionescu]

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this softwareand associated documentation files(the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions :

	The above copyright noticeand this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#pragma once
 
#include <stdint.h>

#define IN	
#define OUT
#define OPT		

/* linking helpers */
#ifdef  FIFO_EXPORTS
#   ifdef _WIN32
#		define FIFO_API		__declspec(dllexport)
#	else
#		define FIFO_API		__attribute__((visibility("default")))
#	endif
#else
#	ifdef _WIN32
#		define FIFO_API		__declspec(dllimport)
#	elif defined(__GNUC__)
#		define FIFO_API		__attribute__((visibility("default")))
#	endif
#endif  

#ifdef _WIN32
#	define FIFO_CALL        __cdecl
#else
#	define FIFO_CALL
#endif

/* C Api */
#ifdef __cplusplus
extern "C" {
#endif

/*
	FIFO API return codes
 */
enum 
{
	/* null ptr provided in api */
	FIFO_ST_NULLPTR = -2,
	/* out of memory*/
	FIFO_ST_OOM		= -1,
	/* ok */
	FIFO_ST_OK		= 0,
};

/* 
	FIFO data element 
 */
typedef
struct fifo_elm_t
{
	/* element data pointer */
	void*			data;
	/* size of memory to which data points */
	const uint32_t	size;

	int32_t		(* const free)		(IN struct fifo_elm_t** elm);
} fifo_elm_t;

/* 
	FIFO operations 
 */
typedef
struct fifo_t
{
	/*
		@function: clean	- deletes all elements from FIFO
		@fifo:				- fifo object reference
		@return:			- status code
	*/
	int32_t		(* const clear)		(IN struct fifo_t* fifo);

	/*
		@function: is_empty	- checks if fifo is empty
		@fifo:				- fifo object reference
		@return:			- returns 0 if fifo not empty, other value if empty
	*/
	int32_t		(* const is_empty)  (IN struct fifo_t* fifo);

	/*
		@function: push		- adds element to front
		@fifo:				- fifo object reference
		@data:				- pointer or memory to store
		@size:				- size of the memory to store, if 0 then store just the pointer
		@return:			- status code
	*/
	int32_t		(* const push)		(IN struct fifo_t* fifo, IN void* data, IN uint32_t size);
	/*
		@function: pop		- pop tail element
		@fifo:				- fifo object reference
		@return:			- @fifo_elm_t
	*/
	fifo_elm_t*	(* const pop)		(IN struct fifo_t* fifo);
	/*
		@function: count	- return the number of elements in fifo
		@fifo:				- fifo object reference
		@return:			- number of elements
	*/
	uint32_t    (* const count)		(IN struct fifo_t* fifo);
} fifo_t;

/*
	FIFO creation / destruction
 */
FIFO_API  fifo_t*	FIFO_CALL	fifo_init();
FIFO_API  int32_t	FIFO_CALL	fifo_destroy(IN fifo_t** fifo);

#ifdef __cplusplus
}
#endif
