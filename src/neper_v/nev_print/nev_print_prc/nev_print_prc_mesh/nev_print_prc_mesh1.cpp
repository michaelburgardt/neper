/* This (*pfile) is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING (*pfile) in the top-level directory. */

#include"nev_print_prc_mesh_.h"

void
nev_print_prc_mesh (prc::oPRCFile *pfile, struct PRINT Print,
		struct TESS Tess, struct NODES Nodes,
		struct MESH *Mesh,
		struct NODEDATA NodeData, struct MESHDATA *MeshData)
{
  (void) NodeData;

  prc::PRCoptions grpopt;
  grpopt.no_break = true;
  grpopt.do_break = false;
  grpopt.tess = true;

  (*pfile).begingroup ("mesh", &grpopt);

  nev_print_prc_mesh_3d (pfile, Print, Tess, Nodes, Mesh, MeshData);

  nev_print_prc_mesh_nodes (pfile, Print, Nodes);

  (*pfile).endgroup ();

  return;
}
