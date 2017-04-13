/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_.h"

void
net_tess_opt_init_target_cellqty (struct IN_T In, struct MTESS MTess,
				  struct TESS Tess, int poly, int *pCellQty)
{
  int varqty;
  char **vars = NULL;
  double *vals = NULL;
  char *tmp2 = ut_alloc_1d_char (100);
  double cellavsize;

  varqty = 2;
  vars = ut_alloc_2d_char (varqty, 10);
  vals = ut_alloc_1d (varqty);

  neut_tess_cellavsize (Tess, Tess.CellQty, &cellavsize);
  strcpy (vars[0], "size");
  neut_tess_cell_size (Tess, poly, vals);
  strcpy (vars[1], "sizerel");
  vals[1] = vals[0] / cellavsize;

  neut_mtess_tess_poly_mid (MTess, Tess, poly, &tmp2);
  if (strcmp (In.n[Tess.Level + 1], "from_morpho"))
  {
    net_multiscale_arg_0d_int_fscanf (In.n[Tess.Level + 1],
				      tmp2, varqty, vars, vals, pCellQty);
    (*pCellQty) = ut_num_max (*pCellQty, 1);
  }
  else
    (*pCellQty) = -1;

  ut_free_2d_char (vars, varqty);
  ut_free_1d (vals);
  ut_free_1d_char (tmp2);

  return;
}

#ifdef HAVE_NLOPT
void
net_tess_opt_init_parms_algo (char *optialgo, struct TOPT *pTOpt)
{
  ut_string_string (optialgo, &(*pTOpt).algoname);

  if (!strcmp (optialgo, "neldermead"))
    (*pTOpt).algo = NLOPT_LN_NELDERMEAD;
  else if (!strcmp (optialgo, "subplex"))
    (*pTOpt).algo = NLOPT_LN_SBPLX;
  else if (!strcmp (optialgo, "cobyla"))
    (*pTOpt).algo = NLOPT_LN_COBYLA;
  else if (!strcmp (optialgo, "bobyqa"))
    (*pTOpt).algo = NLOPT_LN_BOBYQA;
  else if (!strcmp (optialgo, "newuoa"))
    (*pTOpt).algo = NLOPT_LN_NEWUOA_BOUND;
  else if (!strcmp (optialgo, "praxis"))
    (*pTOpt).algo = NLOPT_LN_PRAXIS;
  else if (!strncmp (optialgo, "random", 5))
    (*pTOpt).algo = NLOPT_LN_PRAXIS;
  else if (!strncmp (optialgo, "lloyd", 5))
    (*pTOpt).algo = NLOPT_LN_PRAXIS;
  else
    abort ();

  return;
}
#endif

void
net_tess_opt_init_parms_objective (char *morphooptiobjective,
				   struct TOPT *pTOpt)
{
  int i, qty;
  char **list = NULL;

  ut_string_separate (morphooptiobjective, NEUT_SEP_DEP, &list, &qty);
  for (i = 0; i < qty; i++)
    ut_string_string (list[i], &(*pTOpt).objective);

  ut_free_2d_char (list, qty);

  for (i = 0; i < (*pTOpt).tarqty; i++)
  {
    if (!strcmp ((*pTOpt).tarvar[i], "tesr"))
    {
      if (!strcmp ((*pTOpt).objective, "default"))
	ut_string_string ("scaled,rasterscaled,surf", &(*pTOpt).objective);
    }
    if (!strcmp ((*pTOpt).tarvar[i], "centroid"))
      ut_string_fnrs ((*pTOpt).objective, "default", "L2", 1);
  }

  return;
}

void
net_tess_opt_init_target_bin (double xmin, double xmax,
			      double mean, int binqty, double *binx)
{
  int i, id;
  double min, binwidth = (xmax - xmin) / binqty;
  double *tmp = ut_alloc_1d (binqty);

  for (i = 0; i < binqty; i++)
    binx[i] = xmin + (i + .5) * binwidth;

  ut_array_1d_memcpy (tmp, binqty, binx);
  ut_array_1d_addval (tmp, binqty, -mean, tmp);
  id = ut_array_1d_absmin_index (tmp, binqty);
  min = binx[id] - mean;

  ut_array_1d_addval (binx, binqty, -min, binx);

  ut_free_1d (tmp);

  return;
}

void
net_tess_opt_init_ref (struct TOPT *pTOpt, double mean, int id)
{
  int i, print = 0;
  double fact = 1;

  print = ((*pTOpt).CellQty == -1);

  if (!strcmp ((*pTOpt).tarvar[id], "size"))
  {
    neut_tess_cellavsize ((*pTOpt).Dom, (*pTOpt).CellQty,
			  &((*pTOpt).tarrefval[id]));

    if ((*pTOpt).CellQty == -1)
    {
      neut_tess_cellavsize_cellqty ((*pTOpt).Dom, mean, &(*pTOpt).CellQty, NULL);
      neut_tess_cellavsize ((*pTOpt).Dom, (*pTOpt).CellQty,
			    &((*pTOpt).tarrefval[id]));
    }
  }

  else if (!strcmp ((*pTOpt).tarvar[id], "diameq")
	   || !strcmp ((*pTOpt).tarvar[id], "centroid")
	   || !strcmp ((*pTOpt).tarvar[id], "centroidsize")
	   || !strcmp ((*pTOpt).tarvar[id], "centroiddiameq")
           || !strcmp ((*pTOpt).tarvar[id], "tesr"))
  {
    if (!strcmp ((*pTOpt).tartype[id], "stat"))
    {
      fact = 0;
      if (strcmp ((*pTOpt).tarpdf0[id].type, "numerical"))
	abort ();

      for (i = 0; i < (*pTOpt).tarpdf0[id].size; i++)
	fact +=
	  (*pTOpt).tarpdf0[id].y[i]
	  * ut_fct_binwidth ((*pTOpt).tarpdf0[id], i)
	  * pow ((*pTOpt).tarpdf0[id].x[i], (*pTOpt).Dim);
      fact = pow (fact, 1. / (*pTOpt).Dim);
    }
    else
      fact = 1;

    if ((*pTOpt).CellQty != -1)
    {
      neut_tess_cellavdiameq ((*pTOpt).Dom, (*pTOpt).CellQty,
			      &((*pTOpt).tarrefval[id]));
      (*pTOpt).tarrefval[id] /= fact;
    }

    else
    {
      if (strcmp ((*pTOpt).tarvar[id], "tesr"))
      {
	neut_tess_cellavdiameq_cellqty ((*pTOpt).Dom, mean * fact, &(*pTOpt).CellQty, NULL);
	neut_tess_cellavdiameq ((*pTOpt).Dom, (*pTOpt).CellQty,
				&((*pTOpt).tarrefval[id]));
	(*pTOpt).tarrefval[id] /= fact;
      }
      else
      {
	(*pTOpt).CellQty = (*pTOpt).tartesr.CellQty;
	neut_tess_cellavdiameq ((*pTOpt).Dom, (*pTOpt).CellQty,
				&((*pTOpt).tarrefval[id]));
      }
    }
  }

  else
    (*pTOpt).tarrefval[id] = 1;

  if (print)
    ut_print_message (0, 4, "Number of cells: %d\n", (*pTOpt).CellQty);

  return;
}

void
net_tess_opt_init_tesrobj (struct TOPT *pTOpt)
{
  int i, partqty;
  char **parts = NULL;

  net_tess_opt_init_tesrobj_pre (pTOpt);

  ut_string_separate ((*pTOpt).objective, NEUT_SEP_NODEP, &parts, &partqty);

  for (i = 0; i < partqty; i++)
  {
    if (!strcmp (parts[i], "scaled"))
      net_tess_opt_init_tesrobj_scale (pTOpt);

    else if (!strcmp (parts[i], "rasterscaled"))
      net_tess_opt_init_tesrobj_rasterscale (pTOpt);

    else if (!strncmp (parts[i], "surf", 4)
          || !strcmp  (parts[i], "vol"))
      net_tess_opt_init_tesrobj_pts (pTOpt);

    else
      abort ();
  }

  net_tess_opt_init_tesrobj_post (pTOpt);

  ut_free_2d_char (parts, partqty);

  return;
}

void
net_tess_opt_init_target_cvl (struct IN_T In, int level, int var,
			      struct TOPT *pTOpt)
{
  int i;
  double min, sum = 0;

  sscanf (In.morphooptismooth[level], "%lf", &((*pTOpt).dissmoothing));
  ut_fct_set_normal ((*pTOpt).cvl + var, 0, (*pTOpt).dissmoothing);
  ut_fct_numericalfct ((*pTOpt).cvl[var], -3 * (*pTOpt).dissmoothing,
		       3 * (*pTOpt).dissmoothing, 100, (*pTOpt).cvl + var);

  min = ut_array_1d_min ((*pTOpt).cvl[var].y, (*pTOpt).cvl[var].size);
  ut_array_1d_addval ((*pTOpt).cvl[var].y, (*pTOpt).cvl[var].size, -min,
		      (*pTOpt).cvl[var].y);

  for (i = 0; i < (*pTOpt).cvl[var].size; i++)
    sum += ut_fct_binwidth ((*pTOpt).cvl[var], i) * (*pTOpt).cvl[var].y[i];
  ut_array_1d_scale ((*pTOpt).cvl[var].y, (*pTOpt).cvl[var].size, 1. / sum);

  return;
}

void
net_tess_opt_init_target_scale (struct TOPT *pTOpt, int *pos)
{
  int i;
  double domvol, sumvol, tmp, fact;

  if (!strcmp ((*pTOpt).tarvar[pos[0]], "centroidsize")
   || !strcmp ((*pTOpt).tarvar[pos[0]], "centroiddiameq"))
  {
    neut_tess_size ((*pTOpt).Dom, &domvol);

    sumvol = 0;
    for (i = 1; i <= (*pTOpt).CellQty; i++)
    {
      ut_space_diameq_size ((*pTOpt).Dim,
			    (*pTOpt).tarcellval[pos[0]][i][pos[1]],
			    &tmp);
      sumvol += tmp;
    }

    fact = pow (domvol / sumvol, 1. / (*pTOpt).Dim);
    for (i = 1; i <= (*pTOpt).CellQty; i++)
      (*pTOpt).tarcellval[pos[0]][i][pos[1]] *= fact;
    if (!ut_num_requal (fact, 1, 1e-3))
      ut_print_message (1, 4, "Cell diameqs scaled by a factor of %f.\n", fact);
  }

  return;
}
