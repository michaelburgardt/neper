/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2015, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nev_print_prc_.h"

void
nev_print_prc (char **expargv, int *pi, struct PRINT Print,
	 struct TESS Tess, struct TESSDATA TessData,
	 struct TESR Tesr, struct TESRDATA TesrData,
	 struct NODES Nodes, struct MESH *Mesh,
	 struct NODEDATA NodeData, struct MESHDATA *MeshData,
	 int SQty, struct NODES *SNodes, struct MESH *SMesh2D,
	 struct NODEDATA *SNodeData,
	 struct MESHDATA **SMeshData,
	 int **SElt2dElt3d, struct CSYSDATA CsysData,
	 struct POINT Point, struct POINTDATA PointData)
{
  int i, ver, edge, face, poly, node, elt;
  double points[3][3];
  float Color[3];
  char name[50];
  struct MESH Skin;
  char *filename = ut_alloc_1d_char (strlen (expargv[(*pi) + 1]) + 10);

  sprintf (filename, "%s.prc", expargv[++(*pi)]);
  ut_print_message (0, 1, "Generating prc file %s...\n", filename);
  prc::oPRCFile file (filename);

  (void) Tesr;
  (void) TesrData;
  (void) NodeData;
  (void) MeshData;
  (void) SQty;
  (void) SNodes;
  (void) SMesh2D;
  (void) SNodeData;
  (void) SMeshData;
  (void) SElt2dElt3d;
  (void) CsysData;
  (void) Point;
  (void) PointData;

  // tessellation ------------------------------------------------------
  if (Print.showtess == 1)
  {
    ut_print_message (0, 2, "Adding tessellation to prc file\n");

    for (ver = 1; ver <= Tess.VerQty; ver++)
      file.addPoint (Tess.VerCoo[ver], prc::RGBAColour (0.0, 0.0, 0.0));

    prc::PRCoptions grpopt;
    grpopt.no_break = true;
    grpopt.do_break = false;
    grpopt.tess = true;

    file.begingroup ("lignes", &grpopt);

    for (edge = 1; edge <= Tess.EdgeQty; edge++)
    {
      for (i = 0; i < 3; i++)
	ut_array_1d_memcpy (points[i], 3,
			    Tess.VerCoo[Tess.EdgeVerNb[edge][i]]);
      file.addLine (2, points, prc::RGBAColour (0.0, 0.0, 0.0));
    }

    file.endgroup ();

    for (poly = 1; poly <= Tess.PolyQty; poly++)
    {
      sprintf (name, "G%d", poly);
      file.begingroup (name, &grpopt);

      for (i = 0; i < 3; i++)
	Color[i] = TessData.Col[3][poly][i] / 255.;

      prc::PRCmaterial materialGrain (prc::RGBAColour (Color[0], Color[1], Color[2], 1),	// Diffuse
				      prc::RGBAColour (0.9 * Color[0], 0.9 * Color[1], 0.9 * Color[2], 1),	// Specular
				      prc::RGBAColour (0.2 * Color[0], 0.2 * Color[1], 0.2 * Color[2], 1),	// Emissive
				      prc::RGBAColour (0.05 * Color[0], 0.05 * Color[1], 0.05 * Color[2], 1),	// Ambiant
				      1. -
				      TessData.
				      trs[3][poly] /* transparency */ ,
				      0.7 /* Shininess */ );

      for (face = 1; face <= Tess.PolyFaceQty[poly]; face++)
	for (ver = 1;
	     ver + 2 <= Tess.FaceVerQty[Tess.PolyFaceNb[poly][face]]; ver++)
	{
	  points[0][0] =
	    Tess.VerCoo[Tess.FaceVerNb[Tess.PolyFaceNb[poly][face]][1]][0];
	  points[0][1] =
	    Tess.VerCoo[Tess.FaceVerNb[Tess.PolyFaceNb[poly][face]][1]][1];
	  points[0][2] =
	    Tess.VerCoo[Tess.FaceVerNb[Tess.PolyFaceNb[poly][face]][1]][2];
	  points[1][0] =
	    Tess.VerCoo[Tess.
			FaceVerNb[Tess.PolyFaceNb[poly][face]][ver + 1]][0];
	  points[1][1] =
	    Tess.VerCoo[Tess.
			FaceVerNb[Tess.PolyFaceNb[poly][face]][ver + 1]][1];
	  points[1][2] =
	    Tess.VerCoo[Tess.
			FaceVerNb[Tess.PolyFaceNb[poly][face]][ver + 1]][2];
	  points[2][0] =
	    Tess.VerCoo[Tess.
			FaceVerNb[Tess.PolyFaceNb[poly][face]][ver + 2]][0];
	  points[2][1] =
	    Tess.VerCoo[Tess.
			FaceVerNb[Tess.PolyFaceNb[poly][face]][ver + 2]][1];
	  points[2][2] =
	    Tess.VerCoo[Tess.
			FaceVerNb[Tess.PolyFaceNb[poly][face]][ver + 2]][2];
	  uint32_t PPI[1][3] = { {0, 1, 2} };
	  file.addTriangles (3 /*points */ , points,
			     1 /* tris */ , PPI, materialGrain, 0, NULL, NULL,
			     0, NULL, NULL, 0, NULL, NULL, 0, NULL, NULL,
			     25.8419);
	}

      file.endgroup ();
    }
  }

  if (Print.showmesh == 1)
  {
    for (node = 1; node <= Nodes.NodeQty; node++)
      file.addPoint (Nodes.NodeCoo[node], prc::RGBAColour (0.0, 0.0, 0.0));

    for (poly = 1; poly <= Tess.PolyQty; poly++)
    {
      neut_mesh_set_zero (&Skin);

      neut_mesh_poly_boundmesh (Tess, poly, Mesh[2], &Skin);

      prc::PRCoptions grpopt;
      grpopt.no_break = true;
      grpopt.do_break = false;
      grpopt.tess = true;

      sprintf (name, "G%d", poly);
      file.begingroup (name, &grpopt);

      for (i = 0; i < 3; i++)
	Color[i] = TessData.Col[3][poly][i] / 255.;

      prc::PRCmaterial materialGrain (prc::RGBAColour (Color[0], Color[1], Color[2], 1),	// Diffuse
				      prc::RGBAColour (0.9 * Color[0], 0.9 * Color[1], 0.9 * Color[2], 1),	// Specular
				      prc::RGBAColour (0.2 * Color[0], 0.2 * Color[1], 0.2 * Color[2], 1),	// Emissive
				      prc::RGBAColour (0.05 * Color[0], 0.05 * Color[1], 0.05 * Color[2], 1),	// Ambiant
				      1. -
				      TessData.
				      trs[3][poly] /* transparency */ ,
				      0.7 /* Shininess */ );

      for (elt = 1; elt <= Skin.EltQty; elt++)
      {
	for (i = 0; i < 3; i++)
	  ut_array_1d_memcpy (points[i], 3,
			      Nodes.NodeCoo[Skin.EltNodes[elt][i]]);
	uint32_t PPI[1][3] = { {0, 1, 2} };
	file.addTriangles (3 /*points */ , points,
			   1 /* tris */ , PPI, materialGrain, 0, NULL, NULL,
			   0, NULL, NULL, 0, NULL, NULL, 0, NULL, NULL,
			   25.8419);
      }
      file.endgroup ();

      neut_mesh_free (&Skin);
    }

    ut_print_message (0, 2, "Adding inner mesh to prc file\n");

    // outside mesh --------------------------------------------------------------
    struct NODES N;
    struct MESH M1D = Mesh[1], M2D = Mesh[2], M3D = Mesh[3];
    int elt3dfaceqty = neut_elt_boundqty (M3D.EltType, 3);
    int *elt2delt3d = NULL;
    int *nodes_new_old = NULL;
    int elt2dnodeqty;
    int **seq2 = NULL;

    if (M3D.EltQty == 0)
      return;

    neut_nodes_set_zero (&N);
    neut_mesh_set_zero (&M1D);
    neut_mesh_set_zero (&M2D);
    neut_elt_boundlist (M3D.EltType, 2, &seq2, &elt2dnodeqty, NULL);

    int **printelt3dface = NULL;

    printelt3dface = ut_alloc_2d_int (M3D.EltQty + 1, elt3dfaceqty);

    ut_print_message (0, 3, "Reducing data...\n");

    nev_print_mesh_3d_compress (Print, Nodes, M3D,
				&N, &M2D, &printelt3dface, &elt2delt3d,
				&nodes_new_old);
    nev_print_mesh_2d_compress (M2D, NULL, &M1D);

    ut_print_message (0, 4,
		      "Number of 3D elt faces reduced by %2.0f\%% (to %d).\n",
		      100 - ut_num_percent (M2D.EltQty,
					    Print.showelt3d[0] *
					    elt3dfaceqty), M2D.EltQty);

    ut_print_message (0, 4,
		      "Number of 3D elt edges reduced by %2.0f\%% (to %d).\n",
		      100 - ut_num_percent (M1D.EltQty,
					    M2D.EltQty * elt2dnodeqty),
		      M1D.EltQty);

    ut_print_message (0, 2, "Adding outside mesh to prc file\n");

/*
    for (i = 1; i <= N.NodeQty; i++)
    {
      node = (nodes_new_old) ? nodes_new_old[i] : i;
      ut_array_1d_memcpy (N.NodeCoo[i], 3, NodeData.Coo[node]);
    }
*/

    prc::PRCoptions grpopt;
    grpopt.no_break = true;
    grpopt.do_break = false;
    grpopt.tess = true;

    file.begingroup ("Surface", &grpopt);

    for (node = 1; node <= N.NodeQty; node++)
      file.addPoint (N.NodeCoo[node], prc::RGBAColour (0.0, 0.0, 0.0));

    for (edge = 1; edge <= M1D.EltQty; edge++)
    {
      for (i = 0; i < 2; i++)
	ut_array_1d_memcpy (points[i], 3,
			    N.NodeCoo[M1D.EltNodes[edge][i]]);
      file.addLine (2, points, prc::RGBAColour (0.0, 0.0, 0.0));
    }

    for (i = 0; i < 3; i++)
      Color[i] = TessData.Col[3][1][i] / 255.;

    prc::PRCmaterial materialGrain (prc::RGBAColour (Color[0], Color[1], Color[2], 1),	// Diffuse
				    prc::RGBAColour (0.9 * Color[0], 0.9 * Color[1], 0.9 * Color[2], 1),	// Specular
				    prc::RGBAColour (0.2 * Color[0], 0.2 * Color[1], 0.2 * Color[2], 1),	// Emissive
				    prc::RGBAColour (0.05 * Color[0], 0.05 * Color[1], 0.05 * Color[2], 1),	// Ambiant
				    1. -
				    TessData.trs[3][1] /* transparency */ ,
				    0.7 /* Shininess */ );

    for (elt = 1; elt <= M2D.EltQty; elt++)
    {
      for (i = 0; i < 3; i++)
	ut_array_1d_memcpy (points[i], 3,
	                    N.NodeCoo[M2D.EltNodes[elt][i]]);
      uint32_t PPI[1][3] = { {0, 1, 2} };
      file.addTriangles (3 /*points */ , points,
			 1 /* tris */ , PPI, materialGrain, 0, NULL, NULL, 0,
			 NULL, NULL, 0, NULL, NULL, 0, NULL, NULL, 25.8419);
    }
    file.endgroup ();

    ut_free_1d_int (nodes_new_old);
  }

  file.finish ();

  ut_print_message (0, 2, "Generated %s prc format file\n", filename);
  ut_print_message (0, 2, "with %d grains\n", Tess.PolyQty);

  ut_free_1d_char (filename);

  return;
}
