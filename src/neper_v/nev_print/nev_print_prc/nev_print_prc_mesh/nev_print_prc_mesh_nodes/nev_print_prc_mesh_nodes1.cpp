/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_prc_mesh_nodes_.h"

void
nev_print_prc_mesh_nodes (prc::oPRCFile *pfile, struct PRINT Print,
			  struct NODES Nodes)
{
  int i, qty;

  qty = 0;
  for (i = 1; i <= Nodes.NodeQty; i++)
    if (Print.shownode[i])
    {
      (*pfile).addPoint (Nodes.NodeCoo[i], prc::RGBAColour (0.0, 0.0, 0.0));
      qty++;
    }

  if (qty)
    ut_print_message (0, 4,
		      "Number of nodes      reduced by %3.0f\%% (to %d).\n",
		      0., qty);

  return;
}
