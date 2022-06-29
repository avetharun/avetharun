
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
