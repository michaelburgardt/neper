/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_.h"

void
net_tess_opt_init_tesrobj_pre (struct TOPT *pTOpt)
{
  (*pTOpt).tarcellpts = ut_alloc_1d_ppdouble ((*pTOpt).tartesr.CellQty + 1);
  (*pTOpt).tarcellptqty = ut_alloc_1d_int ((*pTOpt).tartesr.CellQty + 1);
  (*pTOpt).tarcellfact = ut_alloc_1d ((*pTOpt).tartesr.CellQty + 1);
  ut_array_1d_set ((*pTOpt).tarcellfact + 1,
                   (*pTOpt).tartesr.CellQty, 1);

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
net_tess_opt_init_tesrobj_pts (struct TOPT *pTOpt)
{
  int i, j, count, **pts = NULL;
  char *message = ut_alloc_1d_char (100);

  ut_print_message (0, 4, "Listing cell voxels... ");

  for (i = 1; i <= (*pTOpt).CellQty; i++)
  {
    ut_print_progress (stdout, i, (*pTOpt).CellQty, "%5.1f%%", message);

    if (ut_string_inlist ((*pTOpt).objective, NEUT_SEP_NODEP, "surf"))
      neut_tesr_cell_boundpoints ((*pTOpt).tartesr, i,
				  &pts, &(*pTOpt).tarcellptqty[i],
				  0, 1);
    else
      ut_print_message (2, 3, "Could not process expression `'.\n", (*pTOpt).objective);

    // take them all if empty bounds
    if ((*pTOpt).tarcellptqty[i] == 0)
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
net_tess_opt_init_tesrobj_sample (char *sample, struct TOPT *pTOpt)
{
  int i, j, qty;
  char *fct = NULL;
  int fct_varqty = 0;
  char **fct_vars = NULL;
  char **fct_vals = NULL;
  int varqty;
  char **vars = NULL;
  double *vals = NULL;
  char *expr = NULL;
  int *allids = NULL, allidqty;
  int id, *ids = NULL;
  gsl_rng *r = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (r, 1);

  ut_string_function_separate (sample, &fct, &fct_vars, &fct_vals, &fct_varqty);

  if (fct_varqty != 1)
    ut_print_message (2, 3, "Could not process expression `'.\n", sample);

  ut_string_string (fct_vals[0], &expr);
  varqty = 1;
  vars = ut_alloc_1d_pchar (varqty);
  vals = ut_alloc_1d (varqty);
  ut_string_string ("voxnb", vars);

  allidqty = ut_array_1d_int_max ((*pTOpt).tarcellptqty + 1,
				  (*pTOpt).CellQty);
  allids = ut_alloc_1d_int (allidqty);
  ut_array_1d_int_set_id (allids, allidqty);
  ids = ut_alloc_1d_int (allidqty);

  for (i = 1; i <= (*pTOpt).CellQty; i++)
  {
    vals[0] = (*pTOpt).tarcellptqty[i];

    ut_math_eval_int (expr, varqty, vars, vals, &qty);

    ut_array_1d_int_choose (allids, (*pTOpt).tarcellptqty[i],
	                    ids, qty, r);
    (*pTOpt).tarcellfact[i] = (double) (*pTOpt).tarcellptqty[i] / qty;

    for (j = 0; j < qty; j++)
    {
      id = ids[j];
      ut_array_1d_memcpy ((*pTOpt).tarcellpts[i][j] , 3,
	                  (*pTOpt).tarcellpts[i][id]);
    }
    (*pTOpt).tarcellpts[i]
      = ut_realloc_2d_delline ((*pTOpt).tarcellpts[i],
			       (*pTOpt).tarcellptqty[i], qty);
    (*pTOpt).tarcellptqty[i] = qty;
  }

  (*pTOpt).tarcellptqty[0] =
    ut_array_1d_int_sum ((*pTOpt).tarcellptqty + 1, (*pTOpt).CellQty);

  ut_free_1d_char (fct);
  ut_free_2d_char (fct_vars, fct_varqty);
  ut_free_2d_char (fct_vals, fct_varqty);
  ut_free_2d_char (vars, varqty);
  ut_free_1d (vals);
  ut_free_1d_char (expr);
  ut_free_1d_int (allids);
  ut_free_1d_int (ids);
  gsl_rng_free (r);

  return;
}

void
net_tess_opt_init_tesrobj_post (struct TOPT *pTOpt)
{
  int i, j, oldqty = 0, newqty = 0;

  for (i = 1; i <= (*pTOpt).CellQty; i++)
  {
    oldqty += (*pTOpt).tarcellptqty[i] * (*pTOpt).tarcellfact[i];
    newqty += (*pTOpt).tarcellptqty[i];
  }

  // Printing reduction message
  ut_print_message (0, 4, "Number of interface voxels reduced by %7.3f%% (to %d).\n",
		    100.0 * (oldqty - newqty) / oldqty, newqty);
  ut_print_message (0, 4, "min = %d, max = %d.\n",
		    ut_array_1d_int_min ((*pTOpt).tarcellptqty + 1, (*pTOpt).CellQty),
		    ut_array_1d_int_max ((*pTOpt).tarcellptqty + 1, (*pTOpt).CellQty));

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

  // Setting tarcellptsdist
  (*pTOpt).tarcellptsdist = ut_alloc_1d_pdouble ((*pTOpt).tartesr.CellQty + 1);
  for (i = 1; i <= (*pTOpt).tartesr.CellQty; i++)
    (*pTOpt).tarcellptsdist[i] = ut_alloc_1d ((*pTOpt).tarcellptqty[i] + 1);

  return;
}
