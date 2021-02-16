#ifndef _EOS_BITOPS_H
#define _EOS_BITOPS_H

#include "types.h"

#define BIT(nr)	(1 << (nr))
#define TEST_BIT(n, bit)	(((n) >> (bit)) & 1)
#define CLEAR_BIT(n, bit)	((n) &= ~(1 << (bit)))
#define SET_BIT(n, bit)		((n) |= 1 << (bit))
#define TOGGLE_BIT(n, bit)	do { if (TEST_BIT(n, bit)) CLEAR_BIT(n, bit); else SET_BIT(n, bit);} while (0)

#define CAPABLE(cap, request_cap)	(((cap) & (request_cap)) == (request_cap))

#define DECLARE_BITMAP(name, bits)		u8 name[(bits + 7) / 8]
#define BITMAP_TEST_BIT(name, bit)		TEST_BIT(name[bit / 8], bit % 8)
#define BITMAP_CLEAR_BIT(name, bit)		CLEAR_BIT(name[bit / 8], bit % 8)
#define BITMAP_SET_BIT(name, bit)		SET_BIT(name[bit / 8], bit % 8)

#endif