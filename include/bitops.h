#ifndef _EOS_BITOPS_H
#define _EOS_BITOPS_H

#define BIT(nr)	(1 << (nr))
#define TEST_BIT(n, bit)	(((n) >> (bit)) & 1)
#define CLEAR_BIT(n, bit)	((n) &= ~(1 << (bit)))
#define SET_BIT(n, bit)		((n) |= 1 << (bit))
#define TOGGLE_BIT(n, bit)	do { if (TEST_BIT(n, bit)) CLEAR_BIT(n, bit); else SET_BIT(n, bit);} while (0)

#define CAPABLE(cap, request_cap)	(((cap) & (request_cap)) == (request_cap))

#endif