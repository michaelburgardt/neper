/*%% This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_prc_.h"

void
nev_print_prc (char *body, struct PRINT Print,
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
  char *filename = ut_alloc_1d_char (strlen (body) + 5);

  sprintf (filename, "%s.prc", body);

  prc::oPRCFile file (filename);

  (void) Tesr;
  (void) TesrData;
  (void) SQty;
  (void) SNodes;
  (void) SMesh2D;
  (void) SNodeData;
  (void) SMeshData;
  (void) SElt2dElt3d;
  (void) CsysData;
  (void) Point;
  (void) PointData;
  (void) Print;
  (void) TessData;

  ut_print_message (0, 1, "Printing image (prc)...\n");

  if (Print.showtess == 1)
  {
    ut_print_message (0, 2, "Printing tessellation...\n");
    nev_print_prc_tess (&file, Print, Tess, TessData);
  }

  if (Print.showmesh == 1)
  {
    ut_print_message (0, 2, "Printing mesh...\n");
    nev_print_prc_mesh (&file, Print, Tess, Nodes, Mesh, NodeData,
			MeshData);
  }

  file.finish ();

  ut_free_1d_char (filename);

  return;
}
