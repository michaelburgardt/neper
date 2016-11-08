/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_prc_mesh_3d_.h"

void
nev_print_prc_mesh_3d (prc::oPRCFile *pfile, struct PRINT Print,
		       struct TESS Tess, struct NODES Nodes,
		       struct MESH *Mesh,
		       struct MESHDATA *MeshData)
{
  (void) Print;

  int i, j, k;
  double pts[4][3];
  double col[3];
  char name[50];
  struct MESH Skin;
  uint32_t PPI[1][3] = { {0, 1, 2} };

  // All elsets are printed
  for (i = 1; i <= Mesh[3].ElsetQty; i++)
  {
    neut_mesh_set_zero (&Skin);

    neut_mesh_poly_boundmesh (Tess, i, Mesh[2], &Skin);

    prc::PRCoptions grpopt;
    grpopt.no_break = true;
    grpopt.do_break = false;
    grpopt.tess = true;

    sprintf (name, "poly%d", i);
    (*pfile).begingroup (name, &grpopt);

    // taking the colour of the first element of the elset
    for (j = 0; j < 3; j++)
      col[j] = MeshData[3].Col[Mesh[3].Elsets[i][1]][j] / 255.;

    // arguments: diffuse, specular, emissive, ambient, transparency, shininess
    prc::PRCmaterial materialGrain (prc::RGBAColour (col[0], col[1], col[2], 1),
				    prc::RGBAColour (0.9 * col[0], 0.9 * col[1], 0.9 * col[2], 1),
				    prc::RGBAColour (0.2 * col[0], 0.2 * col[1], 0.2 * col[2], 1),
				    prc::RGBAColour (0.05 * col[0], 0.05 * col[1], 0.05 * col[2], 1),
				    1. - 0.,
				    0.7);

    for (j = 1; j <= Skin.EltQty; j++)
    {
      for (k = 0; k < 3; k++)
	ut_array_1d_memcpy (pts[k], 3,
			    Nodes.NodeCoo[Skin.EltNodes[j][k]]);
      ut_array_1d_memcpy (pts[3], 3, pts[0]);

      (*pfile).addTriangles (3, pts,
			 1, PPI, materialGrain, 0, NULL, NULL,
			 0, NULL, NULL, 0, NULL, NULL, 0, NULL, NULL,
			 25.8419);

      (*pfile).addLine (4, pts, prc::RGBAColour (0.0, 0.0, 0.0));
    }

    (*pfile).endgroup ();
  }

  return;
}
