#include <stdio.h>
#include "bit_ops.h"

// Bit setter
void setBitU16(U16 *number, U8 bit, U8 toggle){

	if (toggle)
		*number |= 1 << bit;
	else
		*number &= ~(1 << bit);
	return;
}

// Bit getter
U16 getBitU16(U16 number, U8 bit){

	return number >> bit & 1;
}

// Bit toggler
void toggleBitU16(U16 *number, U8 bit){

	*number ^= 1 << bit;
	return;
}

// Bit setter
void setBitU8(U8 *number, U8 bit, U8 toggle){

	if (toggle)
		*number |= 1 << bit;
	else
		*number &= ~(1 << bit);
	return;
}

// Bit getter
U8 getBitU8(U8 number, U8 bit){

	return number >> bit & 1;
}

// Bit toggler
void toggleBitU8(U8 *number, U8 bit){

	*number ^= 1 << bit;
	return;
}

// Visualize a U16
void printU16(U16 number){

	U8 row, col;

	printf("/");
	for (row = 0; row < DIMENSION; row++)
		printf("---");
	printf("\\\n");

	for (row = 0; row < DIMENSION; row++){

		printf("|");

		for (col = 0; col < DIMENSION; col++){

			if (getBitU16(number, row * DIMENSION + col))
				printf(" 1 ");
			else
				printf(" 0 ");
		}
		printf("|\n");
	}

	printf("\\");
	for (row = 0; row < DIMENSION; row++)
		printf("---");
	printf("/\n");

	return;
}
