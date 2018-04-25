/* This file is part of the Neper software sizeage. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_coo_.h"

int
net_tess_opt_init_sset_coo_centre_randpt (struct TESS Dom, struct POINT Point,
			 gsl_rng *r, double rad, double penetration,
			 double *coo, double *pdist)
{
  int status;

  net_tess_opt_init_sset_coo_centre_randpt_pick (Dom, Point, r, coo);
  status = net_tess_opt_init_sset_coo_centre_randpt_test (Dom, Point, rad, penetration, coo, pdist);

  return status;
}

int
net_tess_opt_init_sset_coo_centre_randpt_cluster (struct TESS Dom, struct POINT Point,
				 gsl_rng *r, struct POINT Point2,
				 double penetration,
				 double *coo, double *pdist)
{
  int i, status = 0;
  double rad2, *coo2 = ut_alloc_1d (3);
  double *val = ut_alloc_1d (Point2.PointQty + 1);

  net_tess_opt_init_sset_coo_centre_randpt_pick (Dom, Point, r, coo);

  (void) rad2;
  (void) i;
  (void) penetration;

  for (i = 1; i <= Point2.PointQty; i++)
  {
    ut_array_1d_add (coo, Point2.PointCoo[i], 3, coo2);
    rad2 = Point2.PointRad[i];
    status = net_tess_opt_init_sset_coo_centre_randpt_test (Dom, Point, rad2, penetration,
					   coo2, val + i);
  }

  (*pdist) = ut_array_1d_sum (val + 1, Point2.PointQty);

  ut_free_1d (val);
  ut_free_1d (coo2);

  return status;
}

int
net_tess_opt_init_sset_coo_centre_randpt_pick (struct TESS Dom, struct POINT Point,
			      gsl_rng *r, double *coo)
{
  int i;
  struct POLY Poly;

  neut_poly_set_zero (&Poly);
  net_tess_poly (Dom, 1, &Poly);

  // do not change the below loop to keep 3 gsl_rng_uniform calls /
  // centre for all dimensions.
  for (i = 0; i < 3; i++)
    coo[i] = Point.BBox[i][0] + gsl_rng_uniform (r) * (Point.BBox[i][1] -
	Point.BBox[i][0]);

  for (i = Point.Dim; i < 3; i++)
    coo[i] = ut_array_1d_mean (Point.BBox[i], 2);

  neut_poly_free (&Poly);

  return 0;
}

int
net_tess_opt_init_sset_coo_centre_randpt_test (struct TESS Dom, struct POINT Point,
			      double rad,
			      double penetration, double *coo,
			      double *pdist)
{
  int i, status, l, m, n;
  double dist;
  double *ptcoo = ut_alloc_1d (3);

  status = 0;
  if (Dom.FaceQty > 0 && neut_tess_point_inpoly_std (Dom, coo, 1) == 0)
    status = -2;

  else if (pdist)
  {
    (*pdist) = 0;
    for (i = 1; i <= Point.PointQty; i++)
      for (l = -Point.Periodic[0]; l <= Point.Periodic[0]; l++)
      {
	ptcoo[0] = Point.PointCoo[i][0] + l * (Point.BBox[0][1] - Point.BBox[0][0]);
	for (m = -Point.Periodic[1]; m <= Point.Periodic[1]; m++)
	{
	  ptcoo[1] = Point.PointCoo[i][1] + m * (Point.BBox[1][1] - Point.BBox[1][0]);
	  for (n = -Point.Periodic[2]; n <= Point.Periodic[2]; n++)
	  {
	    ptcoo[2] = Point.PointCoo[i][2] + n * (Point.BBox[2][1] - Point.BBox[2][0]);
	    dist = ut_space_dist (ptcoo, coo) - Point.PointRad[i] - rad;
	    (*pdist) = ut_num_min (dist, (*pdist));
	  }
	}
      }

    if (*pdist < - penetration * rad)
      status = -1;
  }

  ut_free_1d (ptcoo);

  return status;
}
