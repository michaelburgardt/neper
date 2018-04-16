/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_.h"


double
net_tess_opt_comp_objective_fval (struct TOPT *pTOpt)
{
  int i;

  // Initializing problem & computing penalties ------------------------
  net_tess_opt_comp_objective_fval_init (pTOpt);

  // updating penalties for changed cells

  net_tess_opt_comp_objective_fval_cellpenalty (pTOpt);

  // Computing curcellval and curval -----------------------------------
  for (i = 0; i < (*pTOpt).tarqty; i++)
  {
    if (!strcmp ((*pTOpt).tarvar[i], "tesr"))
      net_tess_opt_comp_objective_fval_tesr (pTOpt, i);
    else
      net_tess_opt_comp_objective_fval_gen (pTOpt, i);
  }

  // Computing objval --------------------------------------------------
  net_tess_opt_comp_objective_fval_comp (pTOpt);

  return (*pTOpt).objval;
}
