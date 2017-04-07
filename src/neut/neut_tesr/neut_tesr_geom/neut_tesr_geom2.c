/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the tgeom-level directory. */

#include "neut_tesr_geom_.h"

int
neut_tesr_cell_boundpoints_test (struct TESR Tesr, int cell, int i, int j,
				 int k, int connec)
{
  int res = -1;

  if (connec < 0 || connec > 2)
    ut_print_message (2, 0, "connectivity = %d!\n", connec);

  if (Tesr.VoxCell[i - 1][j][k] == cell
      && Tesr.VoxCell[i + 1][j][k] == cell
      && Tesr.VoxCell[i][j - 1][k] == cell
      && Tesr.VoxCell[i][j + 1][k] == cell
      && (Tesr.Dim == 2 ||
	  (Tesr.VoxCell[i][j][k - 1] == cell
	   && Tesr.VoxCell[i][j][k + 1] == cell)))
    res = 0;

  if (res == 0 && connec > 0)
  {
    if (Tesr.VoxCell[i + 1][j - 1][k] == cell
	&& Tesr.VoxCell[i + 1][j + 1][k] == cell
	&& Tesr.VoxCell[i - 1][j - 1][k] == cell
	&& Tesr.VoxCell[i - 1][j + 1][k] == cell
	&& (Tesr.Dim == 2 ||
	    (Tesr.VoxCell[i + 1][j][k - 1] == cell
	     && Tesr.VoxCell[i + 1][j][k + 1] == cell
	     && Tesr.VoxCell[i - 1][j][k - 1] == cell
	     && Tesr.VoxCell[i - 1][j][k + 1] == cell
	     && Tesr.VoxCell[i][j + 1][k - 1] == cell
	     && Tesr.VoxCell[i][j + 1][k + 1] == cell
	     && Tesr.VoxCell[i][j - 1][k - 1] == cell
	     && Tesr.VoxCell[i][j - 1][k + 1] == cell)))
      res = 1;
  }

  if (res == 1 && connec > 1)
  {
    if (Tesr.Dim == 2 ||
	(Tesr.VoxCell[i - 1][j - 1][k - 1] == cell
	 && Tesr.VoxCell[i - 1][j - 1][k + 1] == cell
	 && Tesr.VoxCell[i - 1][j + 1][k - 1] == cell
	 && Tesr.VoxCell[i - 1][j + 1][k + 1] == cell
	 && Tesr.VoxCell[i + 1][j - 1][k - 1] == cell
	 && Tesr.VoxCell[i + 1][j - 1][k + 1] == cell
	 && Tesr.VoxCell[i + 1][j + 1][k - 1] == cell
	 && Tesr.VoxCell[i + 1][j + 1][k + 1] == cell))
      res = 2;
  }

  return res != connec;
}
