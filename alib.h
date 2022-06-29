
//  ALIB: A C-Style programming utility header for tedious tasks.
//
//  Any instance of array creation or object creation is handled using the C-style malloc()/free() functions!
//  
//  Note: this is the C version! It won't have some things such as std::string utilities, so make sure to use https://github.com/avetharun/avetharun/alib.hpp!
//



/** - https://github.com/avetharun/avetharun/blob/main/ave_libs.hpp
 @copyright Avetharun 2021-2022 - GNU-GPL3-a
 Let's keep this simple. It follows the GNU GPL3 license, with very few modifications and exceptions.
 @ You can:
  - Use this commercially
  - Distribute modified versions
  - Modify this file and/or projects
  - Patent projects that use this file
  - Use this file and/or project privately, ie. for use in an internal server (See limitations & Exceptions)
  - Use freely under the following conditions
 @ Conditions:
  - You must disclose the source file and/or author when distributing this file
  - A copy of this license must be attached to the file, in verbatim
  - The same (or similar) license must be used if you modify and license your version
  - Changes must be listed. ie. modifying source code must have your name and change in the file.
    - To follow this it must either be
      A: Beside the change (on top or on bottom, in a comment)
      B: In the AUTHORs section of the file/project
      C: in any changelog that references the file's changes
 @ Limitations / What_you_cannot_do
  - The user (you) are not liable for the harmful actions this program or file may cause, if any.
  - Keep code closed source (*See exceptions)
 @b Exceptions
  - If code is closed source, it must be in a private setting. Examples are as follows:
    EXA: A server used to host and/or distribute files
    EXB: Used as a base for porting and/or in microcontrollers
 */
#define _ALIB_FQUAL static inline
#ifndef NDEBUG
// Production builds should set NDEBUG=1
#define NDEBUG false
#endif

#define alib_malloca(TYPE, length) (TYPE*)malloc(length)
#define alib_malloct(TYPE) (TYPE*)malloc(sizeof(TYPE))

#define alib_max(a,b) (((a) > (b)) ? (a) : (b))
#define alib_min(a,b) (((a) < (b)) ? (a) : (b))

_ALIB_FQUAL void alib_reqlen(size_t* sz, char* arr) {
    if ((*sz) == 0) {
        (*sz) = strlen(arr);
    }
}

#ifndef itoa
_ALIB_FQUAL char* itoa(int num, char* buffer, int base) {
    int curr = 0;

    if (num == 0) {
        // Base case
        buffer[curr++] = '0';
        buffer[curr] = '\0';
        return buffer;
    }

    int num_digits = 0;

    if (num < 0) {
        if (base == 10) {
            num_digits++;
            buffer[curr] = '-';
            curr++;
            // Make it positive and finally add the minus sign
            num *= -1;
        }
        else
            // Unsupported base. Return NULL
            return NULL;
    }

    num_digits += (int)floor(log(num) / log(base)) + 1;

    // Go through the digits one by one
    // from left to right
    while (curr < num_digits) {
        // Get the base value. For example, 10^2 = 1000, for the third digit
        int base_val = (int)pow(base, num_digits - 1 - curr);

        // Get the numerical value
        int num_val = num / base_val;

        char value = num_val + '0';
        buffer[curr] = value;

        curr++;
        num -= base_val * num_val;
    }
    buffer[curr] = '\0';
    return buffer;
}
#endif
#ifndef ftoa
_ALIB_FQUAL char* ftoa(float f)
{
    static char        buf[17];
    char* cp = buf;
    unsigned long    l, rem;

    if (f < 0) {
        *cp++ = '-';
        f = -f;
    }
    l = (unsigned long)f;
    f -= (float)l;
    rem = (unsigned long)(f * 1e6);
    sprintf(cp, "%lu.%6.6lu", l, rem);
    return buf;
}
#endif

#if defined(ALIB_FORCE_BINARY) || (!defined(ALIB_NO_BINARY))
// Sourced from https://bytes.com/topic/c/answers/219656-literal-binary
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
// Binary encoding into hex (internal+user-runnable)
#define B8(d) ((unsigned char)B8__(HEX__(d)))
#define B16(dmsb,dlsb) (((unsigned short)B8(dmsb)<<8) \
+ B8(dlsb))
#define B32(dmsb,db2,db3,dlsb) (((unsigned long)B8(dmsb)<<24) \
+ ((unsigned long)B8(db2)<<16) \
+ ((unsigned long)B8(db3)<<8) \
+ B8(dlsb))
#define B64(dmsb,db2,db3,db4,db5,db6,db7,dlsb) \
(\
	((unsigned long long int)B32(dmsb,db2,db3,db4)<<32  ) \
+	((unsigned long long int)B32(db5,db6,db7,dlsb)	   ) \
)
 // Internal bit manipulation
#define imp_bitenb(val,n)   ( ( val ) |=  (1<<(n))) // Sets bit n to 1
#define imp_bitdis(val,n)   ( ( val ) &= ~(1<<(n))) // Sets bit n to 0
#define imp_flipbit(val,n)  ( ( val ) ^=  (1<<(n))) // Inverts bit n
#define imp_getbitv(val,n)   ( ( val ) &   (1<<(n))) // Gets bit n from data val
/*---------------------------------------*/
/* User macros */

// Binary number creation to unsigned long
// Binary manipulation 
//
// Sets bit nbit to value bv in variable var
#define setbitv(var,nbit,val) (val > 0) ? imp_bitenb(var,nbit) : imp_bitdis(var,nbit)
#define setbitsv(var,val,amt) for(int nbit=0; nbit < amt; nbit++) { setbitv(var,nbit,val); }
#define setbitssv(var,start,val,amt) for(int nbit=start; nbit < start+amt; nbit++) { setbitv(var,nbit,val); }
// Sets bit nbit to TRUE/1
#define bitenablev(var,nbit)              imp_bitenb(var,nbit)
// Sets bit nbit to FALSE/0
#define bitdisablev(var,nbit)             imp_bitdis(var,nbit)
#define getbitv(var,nbit)                 imp_getbitv(var,nbit)
#define flipbitv(var, nbit)               imp_flipbit(var,nbit)
#define Binary8(d) ((unsigned char)B8__(HEX__(d))) 
#define Binary16(dmsb,dlsb) (((unsigned short)B8(dmsb)<<8) \
+ B8(dlsb))
#define Binary32(dmsb,db2,db3,dlsb) (((unsigned long)B8(dmsb)<<24) \
+ ((unsigned long)B8(db2)<<16) \
+ ((unsigned long)B8(db3)<<8) \
+ B8(dlsb))
#define Binary64(dmsb,db2,db3,db4,db5,db6,db7,dlsb) \
(\
	((unsigned long long int)B32(dmsb,db2,db3,db4)<<32  ) \
+	((unsigned long long int)B32(db5,db6,db7,dlsb)	   ) \
)
#endif // ALIB_NO_BINARY

#if defined(ALIB_FORCE_CONCAT) || (!defined(ALIB_NO_CONCAT))

#define __concat_internal3(a, b, c) a##b##c
#define __concat_internal2(a, b) a##b
#define __concat4__internal(a,b,c,d) __concat_internal2(a,b)##__concat_internal2(c,d)
#define concat4(a,b,c,d) __concat4__internal(a,b,c,d)
#define concat3(a, b, c) __concat_internal3(a, b, c)
#define concat2(a, b) __concat_internal2(a, b)
#endif // ALIB_NO_CONCAT


// Note: ignore any "function definition for typedef_func_ty" or "Y is not defined" errors. They're temporary.
#define d_typedef_func_ty(return_z, name, ...) typedef return_z (*name)(__VA_ARGS__);



#if defined(ALIB_FORCE_NONAMES) || (!defined(ALIB_NO_NONAMES))
// This may require some explaining. I use these for creating pseudo unnamed functions and variables, at compile time.
// Obviously, since they're unnamed, they're single use. Unless you reverse the noname function via _nng(line with _nn)
// What it should output is obviously based upon compiler, but what it'll look like on GCC is roughly this:
// ____34_unl__noname_line_34_LINEPOS_34_un_
#define ___nn_internal_concat__(a, b, c) a##b##c
#define ___nn_internal_concat(a, b, c) ___nn_internal_concat__(a, b, c)
#define _s_nn_internal__(a) ___nn_internal_concat(a, _LINEPOS_, __LINE__)
#define _s_nn_internal__2__ _s_nn_internal__(___nn_internal_concat(_noname, _line_, __LINE__))
#define _s_nn_internal__2_1_ ___nn_internal_concat(_, ___nn_internal_concat(__, __LINE__, _unl), _)
#define _s_nn_internal__3__ ___nn_internal_concat(_s_nn_internal__2_1_, _s_nn_internal__2__, _un)

#define _g_nn_internal__(a, line) ___nn_internal_concat(a, _LINEPOS_, line)
#define _g_nn_internal__2__(line) _g_nn_internal__(___nn_internal_concat(_noname, _line_, line), line)
#define _g_nn_internal__2_1_(line) ___nn_internal_concat(_, ___nn_internal_concat(__, line, _unl), _)
#define _g_nn_internal__3__(line) ___nn_internal_concat(_g_nn_internal__2_1_(line), _g_nn_internal__2__(line), _un)

#define _nn_impl ___nn_internal_concat(_, _s_nn_internal__3__, _) // Create no-name object
#define _nn _nn_impl
#define _nng(line) ___nn_internal_concat(_, _g_nn_internal__3__(line), _) // Get no-name object at file line (LINE)

#endif // ALIB_NO_NONAMES
_ALIB_FQUAL int alib_log(int base, int n) {
#ifndef _CMATH_
    // 75% as fast as cmath, if using recursion. If we have cmath avalible, use that instead.
    return (n > base - 1)
        ? 1 + alib_log(n / base, base)
        : 0;
#else
    return (int)(log(n) / log(base));
#endif
}
_ALIB_FQUAL long alib_abs(signed long val) {
	if (val < 0) {val *= -1;}
	return val;
}
_ALIB_FQUAL bool alib_wrange(int min, int max, int val) {
    return ((val - max) * (val - min) <= 0);
}
// Taken from python's usage of math.isclose()
_ALIB_FQUAL bool alib_fclose(float first, float second, float rel_tol = 1e-09, float abs_tol = 0.0) {
    return alib_abs(first - second) <= alib_max(rel_tol * alib_max(alib_abs(first), alib_abs(second)), abs_tol);
}
_ALIB_FQUAL bool alib_dclose(double first, double second, double rel_tol = 1e-09, double abs_tol = 0.0) {
    return alib_abs(first - second) <= alib_max(rel_tol * alib_max(alib_abs(first), alib_abs(second)), abs_tol);
}
_ALIB_FQUAL int alib_endswith(const char* str, const char* suffix)
{
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix > lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

_ALIB_FQUAL int alib_beginswith(const char* str, const char* prefix)
{
    if (!str || !prefix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lenprefix = strlen(prefix);
    if (lenprefix > lenstr)
        return 0;
    return strncmp(str, prefix, lenprefix) == 0;
}

_ALIB_FQUAL int alib_chreq(const char* src, const char* match) {
    size_t sl = strlen(src);
    size_t ml = strlen(match);
    if (ml > sl) { return false; }
    return strncmp(src, match, ml);
}
// Occurances of char `c` in `src`
_ALIB_FQUAL size_t alib_chrocc(const char* src, char c, size_t len = 0) {
    alib_reqlen(&len, src);
    size_t occ = 0;
    for (size_t i = 0; i < len; i++) {
        if (src[i] == c) { occ++; }
    }
    return occ;
}
// get byte at offset of array 
_ALIB_FQUAL char alib_get_byte(void* data, int offset) {
    return ((char*)data)[offset];
}
// get byte at array[0]
_ALIB_FQUAL char alib_get_byte(void* data) {
    return ((char*)data)[0];
}
// set byte at offset of array 
_ALIB_FQUAL void alib_set_byte(void* data, char byte, int offset) {
    reinterpret_cast<char*>(data)[offset] = byte;
}
// set byte at array[0]
_ALIB_FQUAL void alib_set_byte(void* data, char byte) {
    reinterpret_cast<char*>(data)[0] = byte;
}
#ifdef __GNUC__
#define VSCPRINTF "implemented"
_ALIB_FQUAL int vscprintf(const char* format, va_list ap)
{
    va_list ap_copy;
    va_copy(ap_copy, ap);
    int retval = vsnprintf(NULL, 0, format, ap_copy);
    va_end(ap_copy);
    return retval;
}
#endif
_ALIB_FQUAL size_t alib_2d_ar_pos(size_t pitch, size_t x, size_t y, size_t bytes_per_step = 4) {
    return y * pitch + x * bytes_per_step;
}
_ALIB_FQUAL const char* alib_chrrepl(const char* in, char match, char repl_value) {
    while (*(in++)) {
        if (*in == match) {
            (*(char*)in) = repl_value;
        }
    }
    return in;
}
_ALIB_FQUAL void alib_copy_signed(signed int a, signed int* b) {
    *b = (a < 0) ? -*b : (*b < 0) ? -*b : *b;
}
_ALIB_FQUAL double alib_percent(long double num, double percent) {
    long double _n_d100 = (num / 100);
    return lroundl(_n_d100 * percent);
}
_ALIB_FQUAL float alib_percentf(float num, float percent) {
    float _n_d100 = (num * 0.01);
    return lroundf(_n_d100 * percent);
}
