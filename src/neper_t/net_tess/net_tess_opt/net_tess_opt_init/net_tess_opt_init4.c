/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_.h"

void
net_tess_opt_init_tesrobj_pts (struct TOPT *pTOpt)
{
  int i, j, count, **pts = NULL;
  char *message = ut_alloc_1d_char (100);

  ut_print_message (0, 4, "Listing cell voxels... ");

  for (i = 1; i <= (*pTOpt).CellQty; i++)
  {
    ut_print_progress (stdout, i, (*pTOpt).CellQty, "%5.1f%%", message);

    if (ut_string_inlist ((*pTOpt).objective, NEUT_SEP_NODEP, "surf")
     || ut_string_inlist ((*pTOpt).objective, NEUT_SEP_NODEP, "surf0")
     || ut_string_inlist ((*pTOpt).objective, NEUT_SEP_NODEP, "surf1")
     || ut_string_inlist ((*pTOpt).objective, NEUT_SEP_NODEP, "surf2"))
      neut_tesr_cell_boundpoints ((*pTOpt).tartesr, i,
				  &pts, &(*pTOpt).tarcellptqty[i],
				  (*pTOpt).faceconn);

    // take them all if empty bounds
    else if (!strncmp ((*pTOpt).objective, "vol", 3) || (*pTOpt).tarcellptqty[i] == 0)
      neut_tesr_cell_points ((*pTOpt).tartesr, i,
			     &pts, &(*pTOpt).tarcellptqty[i]);

    (*pTOpt).tarcellpts[i] = ut_alloc_2d ((*pTOpt).tarcellptqty[i], 3);

    for (j = 0; j < (*pTOpt).tarcellptqty[i]; j++)
      neut_tesr_pos_coo ((*pTOpt).tartesr, pts[j], (*pTOpt).tarcellpts[i][j]);

    ut_free_2d_int_ (&pts, (*pTOpt).tarcellptqty[i]);
  }

  (*pTOpt).tarcellptqty[0] = ut_array_1d_int_sum ((*pTOpt).tarcellptqty + 1,
						  (*pTOpt).tartesr.CellQty);

  count =
    ut_array_1d_int_nbofthisval ((*pTOpt).tarcellptqty + 1,
				 (*pTOpt).tartesr.CellQty, 0);
  if (count > 0)
    ut_print_message (2, 4, " %d %s no voxel!\n", count,
		      (count == 1) ? "cell has" : "cells have");


  ut_free_1d_char (message);

  return;
}

void
net_tess_opt_init_tesrobj_pre (struct TOPT *pTOpt)
{
  (*pTOpt).tarcellpts = ut_alloc_1d_ppdouble ((*pTOpt).tartesr.CellQty + 1);
  (*pTOpt).tarcellptqty = ut_alloc_1d_int ((*pTOpt).tartesr.CellQty + 1);
  (*pTOpt).tarcellrefval = ut_alloc_1d ((*pTOpt).tartesr.CellQty + 1);

  return;
}

void
net_tess_opt_init_tesrobj_scale (struct TOPT *pTOpt)
{
  int i;

  (*pTOpt).tartesrscale = ut_alloc_1d ((*pTOpt).Dim);
  neut_tesr_cells_anisoxyz ((*pTOpt).tartesr, (*pTOpt).tartesrscale);
  for (i = 0; i < (*pTOpt).Dim; i++)
    (*pTOpt).tartesrscale[i] = 1. / (*pTOpt).tartesrscale[i];
  ut_print_message (0, 4, "Scaling input tesr by %.2f x %.2f x %.2f...\n",
		    (*pTOpt).tartesrscale[0],
		    (*pTOpt).tartesrscale[1],
		    (*pTOpt).tartesrscale[2]);
  neut_tesr_scale (&((*pTOpt).tartesr),
		   (*pTOpt).tartesrscale[0],
		   (*pTOpt).tartesrscale[1],
		   (*pTOpt).tartesrscale[2], NULL);

  return;
}

void
net_tess_opt_init_tesrobj_rasterscale (struct TOPT *pTOpt)
{
  double vsizemax = ut_array_1d_max ((*pTOpt).tartesr.vsize, 3);

  ut_print_message (0, 4, "Scaling input tesr's raster by %.2f x %.2f x %.2f...\n",
		    (*pTOpt).tartesr.vsize[0] / vsizemax,
		    (*pTOpt).tartesr.vsize[1] / vsizemax,
		    (*pTOpt).tartesr.vsize[2] / vsizemax);
  neut_tesr_rasterscale (&((*pTOpt).tartesr),
			 (*pTOpt).tartesr.vsize[0] / vsizemax,
			 (*pTOpt).tartesr.vsize[1] / vsizemax,
			 (*pTOpt).tartesr.vsize[2] / vsizemax);

  return;
}

void
net_tess_opt_init_tesrobj_post (struct TOPT *pTOpt)
{
  int i, j;

  // Printing reduction message
  ut_print_message (0, 4, "Number of voxels reduced by %.1f%% (to %d).\n",
		    100.0 - (*pTOpt).tarcellptqty[0] * 100.0 /
		    neut_tesr_voxqty ((*pTOpt).tartesr),
		    (*pTOpt).tarcellptqty[0]);

  // Setting tarcellptcells
  (*pTOpt).tarcellptscells = ut_alloc_1d_ppint ((*pTOpt).tartesr.CellQty + 1);
  for (i = 1; i <= (*pTOpt).tartesr.CellQty; i++)
  {
    (*pTOpt).tarcellptscells[i]
      = ut_alloc_1d_pint ((*pTOpt).tarcellptqty[i] + 1);
    for (j = 0; j < (*pTOpt).tarcellptqty[i]; j++)
    {
      (*pTOpt).tarcellptscells[i][j]
	= ut_alloc_1d_int ((*pTOpt).CellSCellQty[i]);
      ut_array_1d_int_memcpy ((*pTOpt).tarcellptscells[i][j],
			      (*pTOpt).CellSCellQty[i],
			      (*pTOpt).CellSCellList[i]);
    }
  }

  return;
}
