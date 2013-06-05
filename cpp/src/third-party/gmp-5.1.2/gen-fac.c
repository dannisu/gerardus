/* Generate data for combinatorics: fac_ui, bin_uiui, ...

Copyright 2002, 2011, 2012 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library.  If not, see http://www.gnu.org/licenses/.  */

#include <stdio.h>
#include <stdlib.h>

#include "bootstrap.c"

int
mpz_remove_twos (mpz_t x)
{
  int r = 0;
  for (;mpz_even_p (x);r++)
    mpz_tdiv_q_2exp (x, x, 1);
  return r;
}

/* returns 0 on success		*/
int
gen_consts (int numb, int nail, int limb)
{
  mpz_t x, mask, y, last;
  unsigned long a, b;
  unsigned long ofl, ofe;

  printf ("/* This file is automatically generated by gen-fac.c */\n\n");
  printf ("#if GMP_NUMB_BITS != %d\n", numb);
  printf ("Error , error this data is for %d GMP_NUMB_BITS only\n", numb);
  printf ("#endif\n");
#if 0
  printf ("#if GMP_LIMB_BITS != %d\n", limb);
  printf ("Error , error this data is for %d GMP_LIMB_BITS only\n", limb);
  printf ("#endif\n");
#endif

  printf
    ("/* This table is 0!,1!,2!,3!,...,n! where n! has <= GMP_NUMB_BITS bits */\n");
  printf
    ("#define ONE_LIMB_FACTORIAL_TABLE CNST_LIMB(0x1),CNST_LIMB(0x1");
  mpz_init_set_ui (x, 1);
  mpz_init (last);
  for (b = 2;; b++)
    {
      mpz_mul_ui (x, x, b);	/* so b!=a       */
      if (mpz_sizeinbase (x, 2) > numb)
	break;
      printf ("),CNST_LIMB(0x");
      mpz_out_str (stdout, 16, x);
    }
  printf (")\n");

  printf
    ("\n/* This table is 0!,1!,2!/2,3!/2,...,n!/2^sn where n!/2^sn is an */\n");
  printf
    ("/* odd integer for each n, and n!/2^sn has <= GMP_NUMB_BITS bits */\n");
  printf
    ("#define ONE_LIMB_ODD_FACTORIAL_TABLE CNST_LIMB(0x1),CNST_LIMB(0x1),CNST_LIMB(0x1");
  mpz_set_ui (x, 1);
  for (b = 3;; b++)
    {
      for (a = b; (a & 1) == 0; a >>= 1);
      mpz_set (last, x);
      mpz_mul_ui (x, x, a);
      if (mpz_sizeinbase (x, 2) > numb)
	break;
      printf ("),CNST_LIMB(0x");
      mpz_out_str (stdout, 16, x);
    }
  printf (")\n");
  printf
    ("#define ODD_FACTORIAL_TABLE_MAX CNST_LIMB(0x");
  mpz_out_str (stdout, 16, last);
  printf (")\n");

  ofl = b - 1;
  printf
    ("#define ODD_FACTORIAL_TABLE_LIMIT (%lu)\n", ofl);
  mpz_init (mask);
  mpz_setbit (mask, numb);
  mpz_sub_ui (mask, mask, 1);
  printf
    ("\n/* Previous table, continued, values modulo 2^GMP_NUMB_BITS */\n");
  printf
    ("#define ONE_LIMB_ODD_FACTORIAL_EXTTABLE CNST_LIMB(0x");
  mpz_and (x, x, mask);
  mpz_out_str (stdout, 16, x);
  mpz_init (y);
  mpz_bin_uiui (y, b, b/2);
  b++;
  for (;; b++)
    {
      for (a = b; (a & 1) == 0; a >>= 1);
      if (a == b) {
	mpz_divexact_ui (y, y, a/2+1);
	mpz_mul_ui (y, y, a);
      } else
	mpz_mul_2exp (y, y, 1);
      if (mpz_sizeinbase (y, 2) > numb)
	break;
      mpz_mul_ui (x, x, a);
      mpz_and (x, x, mask);
      printf ("),CNST_LIMB(0x");
      mpz_out_str (stdout, 16, x);
    }
  printf (")\n");
  ofe = b - 1;
  printf
    ("#define ODD_FACTORIAL_EXTTABLE_LIMIT (%lu)\n", ofe);

  printf
    ("\n/* This table is 1!!,3!!,...,(2n+1)!! where (2n+1)!! has <= GMP_NUMB_BITS bits */\n");
  printf
    ("#define ONE_LIMB_ODD_DOUBLEFACTORIAL_TABLE CNST_LIMB(0x1");
  mpz_set_ui (x, 1);
  for (b = 3;; b+=2)
    {
      mpz_set (last, x);
      mpz_mul_ui (x, x, b);
      if (mpz_sizeinbase (x, 2) > numb)
	break;
      printf ("),CNST_LIMB(0x");
      mpz_out_str (stdout, 16, x);
    }
  printf (")\n");
  printf
    ("#define ODD_DOUBLEFACTORIAL_TABLE_MAX CNST_LIMB(0x");
  mpz_out_str (stdout, 16, last);
  printf (")\n");

  printf
    ("#define ODD_DOUBLEFACTORIAL_TABLE_LIMIT (%lu)\n", b - 2);

  printf
    ("\n/* This table x_1, x_2,... contains values s.t. x_n^n has <= GMP_NUMB_BITS bits */\n");
  printf
    ("#define NTH_ROOT_NUMB_MASK_TABLE (GMP_NUMB_MASK");
  for (b = 2;b <= 8; b++)
    {
      mpz_root (x, mask, b);
      printf ("),CNST_LIMB(0x");
      mpz_out_str (stdout, 16, x);
    }
  printf (")\n");

  mpz_add_ui (mask, mask, 1);
  printf
    ("\n/* This table contains inverses of odd factorials, modulo 2^GMP_NUMB_BITS */\n");
  printf
    ("\n/* It begins with (2!/2)^-1=1 */\n");
  printf
    ("#define ONE_LIMB_ODD_FACTORIAL_INVERSES_TABLE CNST_LIMB(0x1");
  mpz_set_ui (x, 1);
  for (b = 3;b <= ofe - 2; b++)
    {
      for (a = b; (a & 1) == 0; a >>= 1);
      mpz_mul_ui (x, x, a);
      mpz_invert (y, x, mask);
      printf ("),CNST_LIMB(0x");
      mpz_out_str (stdout, 16, y);
    }
  printf (")\n");

  ofe = (ofe / 16 + 1) * 16;

  printf
    ("\n/* This table contains 2n-popc(2n) for small n */\n");
  printf
    ("\n/* It begins with 2-1=1 (n=1) */\n");
  printf
    ("#define TABLE_2N_MINUS_POPC_2N 1");
  for (b = 4; b <= ofe; b += 2)
    {
      mpz_set_ui (x, b);
      printf (",%lu",b - mpz_popcount (x));
    }
  printf ("\n");
  printf
    ("#define TABLE_LIMIT_2N_MINUS_POPC_2N %lu\n", ofe + 1);


  ofl = (ofl + 1) / 2;
  printf
    ("#define ODD_CENTRAL_BINOMIAL_OFFSET (%lu)\n", ofl);
  printf
    ("\n/* This table contains binomial(2k,k)/2^t */\n");
  printf
    ("\n/* It begins with ODD_CENTRAL_BINOMIAL_TABLE_MIN */\n");
  printf
    ("#define ONE_LIMB_ODD_CENTRAL_BINOMIAL_TABLE ");
  for (b = ofl;; b++)
    {
      mpz_bin_uiui (x, 2 * b, b);
      mpz_remove_twos (x);
      if (mpz_sizeinbase (x, 2) > numb)
	break;
      if (b != ofl)
	printf ("),");
      printf("CNST_LIMB(0x");
      mpz_out_str (stdout, 16, x);
    }
  printf (")\n");

  ofe = b - 1;
  printf
    ("#define ODD_CENTRAL_BINOMIAL_TABLE_LIMIT (%lu)\n", ofe);

  printf
    ("\n/* This table contains the inverses of elements in the previous table. */\n");
  printf
    ("#define ONE_LIMB_ODD_CENTRAL_BINOMIAL_INVERSE_TABLE CNST_LIMB(0x");
  for (b = ofl; b <= ofe; b++)
    {
      mpz_bin_uiui (x, 2 * b, b);
      mpz_remove_twos (x);
      mpz_invert (x, x, mask);
      mpz_out_str (stdout, 16, x);
      if (b != ofe)
	printf ("),CNST_LIMB(0x");
    }
  printf (")\n");

  printf
    ("\n/* This table contains the values t in the formula binomial(2k,k)/2^t */\n");
  printf
    ("#define CENTRAL_BINOMIAL_2FAC_TABLE ");
  for (b = ofl; b <= ofe; b++)
    {
      mpz_bin_uiui (x, 2 * b, b);
      printf ("%d", mpz_remove_twos (x));
      if (b != ofe)
	printf (",");
    }
  printf ("\n");

#if 0
  mpz_set_ui (x, 1);
  mpz_mul_2exp (x, x, limb + 1);	/* x=2^(limb+1)        */
  mpz_init (y);
  mpz_set_ui (y, 10000);
  mpz_mul (x, x, y);		/* x=2^(limb+1)*10^4     */
  mpz_set_ui (y, 27182);	/* exp(1)*10^4      */
  mpz_tdiv_q (x, x, y);		/* x=2^(limb+1)/exp(1)        */
  printf ("\n/* is 2^(GMP_LIMB_BITS+1)/exp(1) */\n");
  printf ("#define FAC2OVERE CNST_LIMB(0x");
  mpz_out_str (stdout, 16, x);
  printf (")\n");


  printf
    ("\n/* FACMULn is largest odd x such that x*(x+2)*...*(x+2(n-1))<=2^GMP_NUMB_BITS-1 */\n\n");
  mpz_init (z);
  mpz_init (t);
  for (a = 2; a <= 4; a++)
    {
      mpz_set_ui (x, 1);
      mpz_mul_2exp (x, x, numb);
      mpz_root (x, x, a);
      /* so x is approx sol       */
      if (mpz_even_p (x))
	mpz_sub_ui (x, x, 1);
      mpz_set_ui (y, 1);
      mpz_mul_2exp (y, y, numb);
      mpz_sub_ui (y, y, 1);
      /* decrement x until we are <= real sol     */
      do
	{
	  mpz_sub_ui (x, x, 2);
	  odd_products (t, x, a);
	  if (mpz_cmp (t, y) <= 0)
	    break;
	}
      while (1);
      /* increment x until > real sol     */
      do
	{
	  mpz_add_ui (x, x, 2);
	  odd_products (t, x, a);
	  if (mpz_cmp (t, y) > 0)
	    break;
	}
      while (1);
      /* dec once to get real sol */
      mpz_sub_ui (x, x, 2);
      printf ("#define FACMUL%lu CNST_LIMB(0x", a);
      mpz_out_str (stdout, 16, x);
      printf (")\n");
    }
#endif

  return 0;
}

int
main (int argc, char *argv[])
{
  int nail_bits, limb_bits, numb_bits;

  if (argc != 3)
    {
      fprintf (stderr, "Usage: gen-fac_ui limbbits nailbits\n");
      exit (1);
    }
  limb_bits = atoi (argv[1]);
  nail_bits = atoi (argv[2]);
  numb_bits = limb_bits - nail_bits;
  if (limb_bits < 2 || nail_bits < 0 || numb_bits < 1)
    {
      fprintf (stderr, "Invalid limb/nail bits %d,%d\n", limb_bits,
	       nail_bits);
      exit (1);
    }
  gen_consts (numb_bits, nail_bits, limb_bits);
  return 0;
}
