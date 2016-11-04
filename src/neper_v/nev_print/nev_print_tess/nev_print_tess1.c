/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_tess_.h"

void
nev_print_tess (FILE * file, struct PRINT Print, struct TESS Tess,
		struct TESSDATA TessData)
{
  int *polylist = NULL, *polyfacelist = NULL, *facelist = NULL,
      *edgelist = NULL, *verlist = NULL, *seedlist = NULL;
  int polyfaceqty;

  nev_print_tess_pre (Print, Tess, TessData, &seedlist, &verlist, &edgelist,
		      &facelist, &polylist, &polyfacelist, &polyfaceqty);

  nev_print_tess_polyfaces (file, Print, Tess, TessData, polyfacelist,
			    polyfaceqty);

  nev_print_tess_faces (file, Print, Tess, TessData, facelist);

  nev_print_tess_edges (file, Print, Tess, TessData, edgelist);

  nev_print_tess_vers (file, Print, Tess, TessData, verlist);

  nev_print_tess_seeds (file, Print, Tess, TessData, seedlist);

  ut_free_1d_int (polylist);
  ut_free_1d_int (polyfacelist);
  ut_free_1d_int (facelist);
  ut_free_1d_int (edgelist);
  ut_free_1d_int (verlist);
  ut_free_1d_int (seedlist);

  return;
}
