/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2015, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"neut_v.h"
//#include "../nev_print/nev_print_mesh/nev_print_mesh_.h"
//#include "../nev_print/nev_print_mesh/nev_print_mesh_3d/nev_print_mesh_3d_.h"
//#include "../nev_print/nev_print_mesh/nev_print_mesh_2d/nev_print_mesh_2d.h"
//#include "../nev_print/nev_print_mesh/nev_print_mesh_nodes/nev_print_mesh_nodes.h"

#include "oPRCFile.h"

#ifdef __cplusplus
extern "C"
{
#endif

  extern void nev_print_mesh_2d_compress (struct MESH Mesh2D, int *,
					  struct MESH *pM1D);

  extern void nev_print_mesh_3d_compress (struct PRINT Print, struct NODES
					  Nodes, struct MESH Mesh3D,
					  struct NODES *, struct MESH *,
					  int ***pprintelt3dface, int **,
					  int **);

  extern void nev_prc (char **, int *, struct PRINT,
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
