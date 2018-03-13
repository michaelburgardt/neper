/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_gen_.h"

void
net_tess_opt_comp_objective_fval_gen (struct TOPT *pTOpt, int var)
{
  int i, cell;

  for (i = 0; i < (*pTOpt).cellchangedqty; i++)
  {
    cell = (*pTOpt).cellchanged[i];

    if ((*pTOpt).curcellpenalty[cell] == 0)
    {
      if (!strcmp ((*pTOpt).tarvar[var], "size"))
        net_tess_opt_comp_objective_fval_gen_size (pTOpt, var, cell);

      else if (!strcmp ((*pTOpt).tarvar[var], "diameq"))
        net_tess_opt_comp_objective_fval_gen_diameq (pTOpt, var, cell);

      else if (!strcmp ((*pTOpt).tarvar[var], "sphericity"))
        net_tess_opt_comp_objective_fval_gen_sphericity (pTOpt, var, cell);

      else if (!strcmp ((*pTOpt).tarvar[var], "convexity"))
        net_tess_opt_comp_objective_fval_gen_convexity (pTOpt, var, cell);

      else if (!strcmp ((*pTOpt).tarvar[var], "centroid"))
        net_tess_opt_comp_objective_fval_gen_centroid (pTOpt, var, cell);

      else if (!strcmp ((*pTOpt).tarvar[var], "centroidsize")
            || !strcmp ((*pTOpt).tarvar[var], "centroiddiameq"))
        net_tess_opt_comp_objective_fval_gen_centroidsize (pTOpt, var, cell);

      else
        abort ();
    }
  }

  if (!strcmp ((*pTOpt).tartype[var], "stat"))
    net_tess_opt_comp_objective_fval_gen_stat (pTOpt, var);
  else
    net_tess_opt_comp_objective_fval_gen_celldata (pTOpt, var);

  if (isnan ((*pTOpt).curval[var]))
  {
    printf ("(*pTOpt).curval[%d] is not-a-number.\n", var);
    ut_error_reportbug ();
  }

return;
}
