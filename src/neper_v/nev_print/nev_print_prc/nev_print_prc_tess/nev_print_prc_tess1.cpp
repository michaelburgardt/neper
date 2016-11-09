/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_prc_tess_.h"

void
nev_print_prc_tess (prc::oPRCFile *pfile, struct PRINT Print, struct TESS Tess,
		    struct TESSDATA TessData)
{
  prc::PRCoptions grpopt;
  grpopt.no_break = true;
  grpopt.do_break = false;
  grpopt.tess = true;

  (*pfile).begingroup ("tess", &grpopt);

  nev_print_prc_tess_polys (pfile, Print, Tess, TessData);

  nev_print_prc_tess_faces (pfile, Print, Tess, TessData);

  nev_print_prc_tess_edges (pfile, Print, Tess, TessData);

  nev_print_prc_tess_vers (pfile, Print, Tess, TessData);

  (*pfile).endgroup ();

  return;
}
