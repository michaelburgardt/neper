/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_prc_utils_.h"

void
nev_print_prc_mesh2d (prc::oPRCFile file, struct NODES Nodes, struct MESH Mesh,
		      int *showelt, int **rgb, char *coltype, int showshadow)
{
  int i, j, showeltqty;
  // double points[3][3];
  double col[3];
  double points[3][3];

  (void) showshadow;

  if (Nodes.NodeQty == 0 || Mesh.EltQty == 0)
    return;

  showeltqty = showelt ? ut_array_1d_int_sum (showelt + 1, Mesh.EltQty)
    : Mesh.EltQty;

  if (showeltqty == 0)
    return;

  if (strcmp (coltype, "elt"))
    ut_print_message (2, 2, "Element colouring from nodes is not available.");

  for (i = 1; i <= Nodes.NodeQty; i++)
    file.addPoint (Nodes.NodeCoo[i], prc::RGBAColour (0.0, 0.0, 0.0));

  prc::PRCoptions grpopt;
  grpopt.no_break = true;
  grpopt.do_break = false;
  grpopt.tess = true;

  file.begingroup ("Surface", &grpopt);

  if (strcmp (Mesh.EltType, "tri") == 0)
  {
    for (i = 1; i <= Mesh.EltQty; i++)
      if (!showelt || showelt[i])
      {
	for (j = 0; j < 3; j++)
	  col[j] = rgb[i][j] / 255.;

	prc::PRCmaterial materialGrain (prc::RGBAColour (col[0], col[1], col[2], 1),	// Diffuse
					prc::RGBAColour (0.9 * col[0], 0.9 * col[1], 0.9 * col[2], 1),	// Specular
					prc::RGBAColour (0.2 * col[0], 0.2 * col[1], 0.2 * col[2], 1),	// Emissive
					prc::RGBAColour (0.05 * col[0], 0.05 * col[1], 0.05 * col[2], 1),	// Ambiant
					1. - 0, /* transparency */
					0.7     /* shininess    */ );

	for (j = 0; j < 3; j++)
	  ut_array_1d_memcpy (points[j], 3,
			      Nodes.NodeCoo[Mesh.EltNodes[i][j]]);
	uint32_t PPI[1][3] = { {0, 1, 2} };
	file.addTriangles (3 /*points */ , points,
			   1 /* tris */ , PPI, materialGrain, 0, NULL, NULL,
			   0, NULL, NULL, 0, NULL, NULL, 0, NULL, NULL,
			   25.8419);
      }

    file.endgroup ();
  }

  return;
}
