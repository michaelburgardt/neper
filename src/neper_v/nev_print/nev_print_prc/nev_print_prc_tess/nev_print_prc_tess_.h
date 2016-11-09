/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"oPRCFile.h"
#include"ut.h"
#include"neut_v.h"
#include "nev_print_prc_tess.h"

extern void nev_print_prc_tess_polys (prc::oPRCFile *file,
				     struct PRINT Print,
				     struct TESS Tess,
				     struct TESSDATA TessData);

extern void nev_print_prc_tess_edges (prc::oPRCFile *file,
				     struct PRINT Print,
				     struct TESS Tess,
				     struct TESSDATA TessData);

extern void nev_print_prc_tess_vers (prc::oPRCFile *file,
				     struct PRINT Print,
				     struct TESS Tess,
				     struct TESSDATA TessData);
