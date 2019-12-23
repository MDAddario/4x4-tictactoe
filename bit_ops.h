// Data types
typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U32;
typedef unsigned long  U64;

typedef signed char  S8;
typedef signed short S16;
typedef signed int   S32;
typedef signed long  S64;

// Prototypes
void setBitU16   (U16*, U8, U8);
U16  getBitU16   (U16,  U8);
void toggleBitU16(U16*, U8);
void setBitU8    (U8*,  U8, U8);
U8   getBitU8    (U8,   U8);
void toggleBitU8 (U8*,  U8);
void printU16    (U16);
