/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2015, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"neut_v.h"

#include "oPRCFile.h"

extern "C" void nev_prc (char **, int *, struct PRINT,
                       struct TESS, struct TESSDATA,
                       struct TESR, struct TESRDATA,
                       struct NODES, struct MESH *,
                       struct NODEDATA, struct MESHDATA *,
                       int, struct NODES *, struct MESH *,
                       struct NODEDATA *, struct MESHDATA **,
                       int **SElt2dElt3d,
                       struct CSYSDATA, struct POINT, struct POINTDATA);
