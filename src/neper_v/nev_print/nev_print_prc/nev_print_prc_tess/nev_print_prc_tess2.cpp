/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_prc_tess_.h"

void
nev_print_prc_tess_polys (prc::oPRCFile *pfile, struct PRINT Print, struct TESS Tess,
			  struct TESSDATA TessData)
{
  int i, j, k, l, face;
  double col[3];
  char name[20];
  double pts[3][3];
  uint32_t PPI[1][3] = { {0, 1, 2} };
  struct NODES N;
  struct MESH M;
  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M);

  prc::PRCoptions grpopt;
  grpopt.no_break = true;
  grpopt.do_break = false;
  grpopt.tess = true;

  (*pfile).begingroup ("polys", &grpopt);

  for (i = 1; i <= Tess.PolyQty; i++)
    if (Print.showpoly[i])
    {
      sprintf (name, "poly%d", i);
      (*pfile).begingroup (name, &grpopt);

      for (j = 0; j < 3; j++)
	col[j] = TessData.Col[3][i][j] / 255.;

      // arguments: diffuse, specular, emissive, ambiant, transparency, shininess
      prc::PRCmaterial materialGrain (prc::RGBAColour (col[0], col[1], col[2], 1),
				      prc::RGBAColour (0.9 * col[0], 0.9 * col[1], 0.9 * col[2], 1),
				      prc::RGBAColour (0.2 * col[0], 0.2 * col[1], 0.2 * col[2], 1),
				      prc::RGBAColour (0.05 * col[0], 0.05 * col[1], 0.05 * col[2], 1),
				      1. - TessData.trs[3][j],
				      0.7);

      for (j = 1; j <= Tess.PolyFaceQty[i]; j++)
      {
	face = Tess.PolyFaceNb[i][j];

	neut_tess_face_interpolmesh (Tess, face, &N, &M);

	for (k = 1; k <= M.EltQty; k++)
	{
	  for (l = 0; l < 3; l++)
	    ut_array_1d_memcpy (pts[l], 3, N.NodeCoo[M.EltNodes[k][l]]);

	  (*pfile).addTriangles (3, pts,
			     1, PPI, materialGrain, 0, NULL, NULL,
			     0, NULL, NULL, 0, NULL, NULL, 0, NULL, NULL,
			     25.8419);
	}
      }

      neut_nodes_free (&N);
      neut_mesh_free (&M);

      (*pfile).endgroup ();
    }

  (*pfile).endgroup ();

  return;
}

void
nev_print_prc_tess_edges (prc::oPRCFile *pfile, struct PRINT Print, struct TESS Tess,
			  struct TESSDATA TessData)
{
  int i, j;
  char name[20];
  double col[3];
  double pts[3][3];

  prc::PRCoptions grpopt;
  grpopt.no_break = true;
  grpopt.do_break = false;
  grpopt.tess = true;

  (*pfile).begingroup ("edges", &grpopt);

  for (i = 1; i <= Tess.EdgeQty; i++)
    if (Print.showedge[i])
    {
      sprintf (name, "edge%d", i);
      (*pfile).begingroup (name, &grpopt);

      for (j = 0; j < 2; j++)
      ut_array_1d_memcpy (pts[j], 3, Tess.VerCoo[Tess.EdgeVerNb[i][j]]);
      for (j = 0; j < 3; j++)
	col[j] = TessData.Col[1][i][j] / 255.;
      (*pfile).addLine (2, pts, prc::RGBAColour (col[0], col[1], col[2]));

      (*pfile).endgroup ();
    }

  (*pfile).endgroup ();

  return;
}

void
nev_print_prc_tess_vers (prc::oPRCFile *pfile, struct PRINT Print, struct TESS Tess,
		         struct TESSDATA TessData)
{
  int i, j;
  double col[3];
  char name[20];

  prc::PRCoptions grpopt;
  grpopt.no_break = true;
  grpopt.do_break = false;
  grpopt.tess = true;

  (*pfile).begingroup ("vers", &grpopt);

  for (i = 1; i <= Tess.VerQty; i++)
    if (Print.showver[i])
    {
      sprintf (name, "ver%d", i);
      (*pfile).begingroup (name, &grpopt);

      for (j = 0; j < 3; j++)
	col[j] = TessData.Col[0][i][j] / 255.;
      (*pfile).addPoint (Tess.VerCoo[i], prc::RGBAColour (col[0], col[1], col[2]));

      (*pfile).endgroup ();
    }

  (*pfile).endgroup ();

  return;
}
