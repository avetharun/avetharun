#ifndef __lib_aveth_utils_hpp
#define __lib_aveth_utils_hpp
/**  //
//ㅤ//
/////
 @copyright Avetharun 2021 - GNU-GPL3-a
 Let's keep this simple. It follows the GNU GPL3 license, with very few modifications and exceptions.


 @b Youㅤcan:
  - Use this commercially
  - Distribute modified versions
  - Modify this file and/or projects
  - Patent projects that use this file
  - Use this file and/or project privately, ie. for use in an internal server (See limitations & Exceptions)
  - Use freely under the following conditions

 @b Conditions:
  - You must disclose the source file and/or author when distributing this file
  - A copy of this license must be attached to the file, in verbatim
  - The same (or similar) license must be used if you modify and license your version
  - Changes must be listed. ie. modifying source code must have your name and change in the file.
    - To follow this it must either be 
      A: Beside the change (on top or on bottom, in a comment)
      B: In the AUTHORs section of the file/project
      C: in any changelog that references the file's changes


 @b Limitationsㅤ/ㅤWhatㅤyouㅤcannotㅤdo
  - The user (you) are not liable for the harmful actions this program or file may cause, if any.
  - Keep code closed source (*See exceptions)


 @b Exceptions
  - If code is closed source, it must be in a private setting. Examples are as follows:
    EXA: A server used to host and/or distribute files
    EXB: Used as a base for porting and/or in microcontrollers
 */





#ifndef ALIB_NO_BINARY
// 
//      Binary & bit manipulation utilities
//    - Avetharun
// 


/* Helper macros. Unless you know what you're doing, don't run these. */

// Binary decoding to unsigned long v
#define HEX__(n) 0x##n##LU
#define B8__(x) ((x&0x0000000FLU)?1:0) \
+((x&0x000000F0LU)?2:0) \
+((x&0x00000F00LU)?4:0) \
+((x&0x0000F000LU)?8:0) \
+((x&0x000F0000LU)?16:0) \
+((x&0x00F00000LU)?32:0) \
+((x&0x0F000000LU)?64:0) \
+((x&0xF0000000LU)?128:0)
// is this nonsense to anyone else? haha... hah.. heh....... help.
// mf c++ magic heh

// Binary encoding into hex (internal+user-runnable)
#define B8(d) ((unsigned char)B8__(HEX__(d)))
#define B16(dmsb,dlsb) (((unsigned short)B8(dmsb)<<8) \
+ B8(dlsb))
#define B32(dmsb,db2,db3,dlsb) (((unsigned long)B8(dmsb)<<24) \
+ ((unsigned long)B8(db2)<<16) \
+ ((unsigned long)B8(db3)<<8) \
+ B8(dlsb))
/** \todo B64(v,v,v,v,v,v,v,v)
 * but that'll be a mess...
 */

// Internal bit manipulation
#define imp_bitenb(val,n)   ( ( val ) |=  (1<<(n))) // Sets bit n to 1
#define imp_bitdis(val,n)   ( ( val ) &= ~(1<<(n))) // Sets bit n to 0
#define imp_flipbit(val,n)  ( ( val ) ^=  (1<<(n))) // Inverts bit n
#define imp_getbit(val,n)   ( ( val ) &   (1<<(n))) // Gets bit n from data val





/*---------------------------------------*/
/* User macros */

// Binary number creation to unsigned long


// Bit manipulation 
//
// Sets bit nbit to value bv in variable var
#define bitset(var,nbit,bv) (val > 0) ? imp_bitenb(var,nbit) : imp_bitdis(var,nbit)
// Sets bit nbit to value bv in variable var
#define setbit(var,nbit,bv)             bitset(var,nbit,bv) // alias of bitset(...) | See bitset(...) macro
// Sets bit nbit to TRUE/1
#define bitenable(var,nbit)             imp_bitenb(var,nbit)
// Sets bit nbit to FALSE/0
#define bitdisable(var,nbit)            imp_bitdis(var,nbit)
// Get bit nbit from value var
#define getbit(var,nbit)                imp_getbit(val,nbit)

// Bit creation
// 

/** 
 * Re-implementation of line @e 66 because compiler complains if I do the following:
 * #define Binary8(b8v) B8(b8v)
 * > Too many/little values in constant
*/
// Convert 8 bits into an unsigned char
#define Binary8(d) ((unsigned char)B8__(HEX__(d))) 
// Convert 16 bits (Each byte is seperated!) into an unsigned short
#define Binary16(dmsb,dlsb) (((unsigned short)B8(dmsb)<<8) \
+ B8(dlsb))
// Convert 32 bits (Each byte is seperated!) into an unsigned long
#define Binary32(dmsb,db2,db3,dlsb) (((unsigned long)B8(dmsb)<<24) \
+ ((unsigned long)B8(db2)<<16) \
+ ((unsigned long)B8(db3)<<8) \
+ B8(dlsb))
// Todo: Binary64 to T<long long>

/** 
 * @todo @b BinaryㅤReading. Effectively reversing the above functions.
 * Planned implementation: Eventually.
 */



#endif // ALIB_NO_BINARY


#ifndef ALIB_NO_FILE_UTILS
#include <iosfwd>
#include <fstream>
#include <sstream>
void readFileBytes(const char* fname, char** out_, unsigned long long* size_) {
    // TODO: do this more elegantly.. But that's hard! So I won't do it. (:
	std::ifstream file(fname, std::ios::binary);
	std::streampos fsize = 0;
    	fsize = file.tellg();
    	file.seekg( 0, std::ios::end );
    	fsize = file.tellg() - fsize;
	file.seekg (0, std::ios::beg); // Go back to the beginning
	long long _sz = fsize;
	char* out_dir = (char*)malloc(_sz);
	file.read(out_dir, _sz);
	file.close();
	* out_ = out_dir;
	* size_ = _sz;
	// yay memory allocation magic!   
}
#endif // ALIB_NO_FILE_UTILS





#ifndef ALIB_NO_CONCAT

#define __concat_internal3(a, b, c) a##b##c
#define __concat_internal2(a, b) a##b
#define concat3(a, b, c) __concat_internal3(a, b, c)
#define concat2(a, b) __concat_internal2(a, b)
#endif // ALIB_NO_NONAMES

#ifndef ALIB_NO_NONAMES
// This may require some explaining. I use these for creating "unnamed" functions and variables, at compile time.
// Obviously, since they're unnamed, they're single use.
// The easiest way for me to use this, is for unnamed struct initializing lambda functions, which uses a struct similar to this:
/*
#include <functional>
struct inline_initializer{
    inline_initializer(std::function<void()> i) {
      i();
    }
};

inline_initializer _nn {
    [&] () {
        std::cout << "This runs before main()!";
    }
};

*/
// Since the variable is single use, as is obvious, 

// What it should output is obviously based upon compiler, but what it'll look like on GCC is roughly this:
// ____230__noname1232323_
// This is the first instance of _nn (), on line 23 of any file

#define ___nn_internal_concat__(a, b, c) a##b##c
#define ___nn_internal_concat(a, b, c) ___nn_internal_concat__(a, b, c)
#define _1_nn_internal__(a) ___nn_internal_concat(a, _LINEPOS_, __LINE__)
#define _1_nn_internal__2__ _1_nn_internal__(___nn_internal_concat(_noname, _line_, __LINE__))
#define _1_nn_internal__2_1_ ___nn_internal_concat(_, ___nn_internal_concat(__, __LINE__, _unl), _)
#define _1_nn_internal__3__ ___nn_internal_concat(_1_nn_internal__2_1_, _1_nn_internal__2__, _un)


#define _g_nn_internal__(a, line) ___nn_internal_concat(a, _LINEPOS_, line)
#define _g_nn_internal__2__(line) _g_nn_internal__(___nn_internal_concat(_noname, _line_, line), line)
#define _g_nn_internal__2_1_(line) ___nn_internal_concat(_, ___nn_internal_concat(__, line, _unl), _)
#define _g_nn_internal__3__(line) ___nn_internal_concat(_g_nn_internal__2_1_(line), _g_nn_internal__2__(line), _un)


#define _nn ___nn_internal_concat(_, _1_nn_internal__3__, _) // Create no-name object
#define _nng(line) ___nn_internal_concat(_, _g_nn_internal__3__(line), _) // Get no-name object at file line (LINE)

#endif // ALIB_NO_NONAMES






#endif // __lib_aveth_utils_hpp
