/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_.h"

void
nev_print (char **expargv, int *pi, struct PRINT Print,
	   struct TESS Tess, struct TESSDATA TessData,
	   struct TESR Tesr, struct TESRDATA TesrData,
	   struct NODES Nodes, struct MESH *Mesh,
	   struct NODEDATA NodeData, struct MESHDATA *MeshData,
	   int SQty, struct NODES *SNodes, struct MESH *SMesh2D,
	   struct NODEDATA *SNodeData,
	   struct MESHDATA **SMeshData,
	   int **SElt2dElt3d, struct CSYSDATA CsysData,
	   struct POINT Point, struct POINTDATA PointData)
{
  char *body = NULL;

  (*pi)++;
  ut_string_body (expargv[(*pi)], ".", &body);

  if (ut_string_inlist (Print.format, NEUT_SEP_NODEP, "pov")
   || ut_string_inlist (Print.format, NEUT_SEP_NODEP, "png"))
    nev_print_pov (body, Print, Tess, TessData, Tesr, TesrData, Nodes,
		   Mesh, NodeData, MeshData, SQty, SNodes, SMesh2D,
		   SNodeData, SMeshData, SElt2dElt3d, CsysData, Point,
		   PointData);

  if (ut_string_inlist (Print.format, NEUT_SEP_NODEP, "prc")
   || ut_string_inlist (Print.format, NEUT_SEP_NODEP, "pdf"))
    nev_print_prc (body, Print, Tess, TessData, Tesr, TesrData, Nodes,
		   Mesh, NodeData, MeshData, SQty, SNodes, SMesh2D,
		   SNodeData, SMeshData, SElt2dElt3d, CsysData, Point,
		   PointData);

  ut_free_1d_char (body);

  return;
}
