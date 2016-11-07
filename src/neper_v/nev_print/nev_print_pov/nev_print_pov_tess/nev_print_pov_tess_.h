/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"neut_v.h"
// #include"../../nefm/nefm.h"
#include "nev_print_pov_tess.h"
#include "../nev_print_pov_utils/nev_print_pov_utils.h"

extern void nev_print_pov_tess_pre (struct PRINT Print, struct TESS Tess,
    struct TESSDATA TessData, int **pseedlist, int **pverlist, int
    **pedgelist, int **pfacelist, int **ppolylist, int **ppolyfacelist);

extern void nev_print_pov_tess_polyfaces (FILE *file, struct PRINT Print, struct TESS
    Tess, struct TESSDATA TessData, int *polyfacelist);
extern void nev_print_pov_tess_faces (FILE *file, struct PRINT Print, struct
    TESS Tess, struct TESSDATA TessData, int *facelist);
extern void nev_print_pov_tess_edges (FILE *file, struct PRINT Print, struct
    TESS Tess, struct TESSDATA TessData, int *edgelist);
extern void nev_print_pov_tess_vers  (FILE *file, struct PRINT Print, struct
    TESS Tess, struct TESSDATA TessData, int *verlist);
extern void nev_print_pov_tess_seeds (FILE *file, struct PRINT Print, struct
    TESS Tess, struct TESSDATA TessData, int *seedlist);
