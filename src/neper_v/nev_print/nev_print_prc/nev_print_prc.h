/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2015, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

  extern void nev_print_prc (char *, struct PRINT,
		       struct TESS, struct TESSDATA,
		       struct TESR, struct TESRDATA,
		       struct NODES, struct MESH *,
		       struct NODEDATA, struct MESHDATA *,
		       int, struct NODES *, struct MESH *,
		       struct NODEDATA *, struct MESHDATA **,
		       int **SElt2dElt3d,
		       struct CSYSDATA, struct POINT, struct POINTDATA);

#ifdef __cplusplus
}
#endif
