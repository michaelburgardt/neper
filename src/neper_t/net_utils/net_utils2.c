/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_utils_.h"

void
net_tess_tesr_cell (struct TESS Tess, int cell, struct TESR *pTesr)
{
  int i, j, k;
  double **bbox = ut_alloc_2d (3, 2);
  double *coo = ut_alloc_1d (3);
  int *pos = ut_alloc_1d_int (3);

  neut_tess_cell_bbox (Tess, cell, bbox);

  for (i = 0; i < 3; i++)
    neut_tesr_point_pos (*pTesr, bbox[i], (*pTesr).CellBBox[cell][i]);

  for (k = (*pTesr).CellBBox[cell][2][0]; k <= (*pTesr).CellBBox[cell][2][1]; k++)
    for (j = (*pTesr).CellBBox[cell][1][0]; j <= (*pTesr).CellBBox[cell][1][1]; j++)
      for (i = (*pTesr).CellBBox[cell][0][0]; i <= (*pTesr).CellBBox[cell][0][1]; i++)
      {
	ut_array_1d_int_set_3 (pos, i, j, k);
	neut_tesr_pos_coo (*pTesr, pos, coo);

	if (neut_tess_point_incell (Tess, coo, cell) == 1)
	  (*pTesr).VoxCell[i][j][k] = cell;

        /*
	if ((*pTesr).VoxCell[i][j][k] == 0 && !strncmp (Tess.Type, "periodic", 8))
	  for (kk = -Tess.Periodic[2]; kk <= Tess.Periodic[2] && (*pTesr).VoxCell[i][j][k] == 0; kk++)
	    for (jj = -Tess.Periodic[1]; jj <= Tess.Periodic[1] && (*pTesr).VoxCell[i][j][k] == 0; jj++)
	      for (ii = -Tess.Periodic[0]; ii <= Tess.Periodic[0] && (*pTesr).VoxCell[i][j][k] == 0; ii++)
	      {
		if (kk == 0 && jj == 0 && ii == 0)
		  continue;

		coo_per[0] = coo[0] + Tess.PeriodicDist[0] * ii;
		coo_per[1] = coo[1] + Tess.PeriodicDist[1] * jj;
		coo_per[2] = coo[2] + Tess.PeriodicDist[2] * kk;

		for (pid = 1; pid <= Tess.CellQty; pid++)
		  if (neut_tess_point_incell (Tess, coo_per, pid) == 1)
		  {
		    (*pTesr).VoxCell[i][j][k] = pid;
		    prevpid = pid;
		    break;
		  }
	      }
        */
      }

  ut_free_2d (bbox, 3);
  ut_free_1d_int (pos);
  ut_free_1d (coo);

  return;
}
