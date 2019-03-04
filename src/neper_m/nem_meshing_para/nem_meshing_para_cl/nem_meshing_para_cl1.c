/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_para_cl_.h"

int
nem_meshing_para_cl (char *clstring, char *clratiostring,
		     struct TESS Tess, struct TESR *pTesr,
		     struct NODES *pRNodes, struct MESH *Mesh,
		     struct MESHPARA *pMeshPara)
{
  if ((*pMeshPara).input == 't')
  {
    nem_meshing_para_cl_cell (clstring, pMeshPara, Tess);

    if (Tess.Dim == 3)
    {
      (*pMeshPara).face_cl = ut_alloc_1d (Tess.FaceQty + 1);
      neut_tess_val_poly2face (Tess, (*pMeshPara).poly_cl,
			       (*pMeshPara).face_cl);
    }

    if (Tess.Dim >= 2)
    {
      (*pMeshPara).edge_cl = ut_alloc_1d (Tess.EdgeQty + 1);
      neut_tess_val_face2edge (Tess, (*pMeshPara).face_cl,
			       (*pMeshPara).edge_cl);
    }

    if (Tess.Dim >= 1)
    {
      (*pMeshPara).ver_cl = ut_alloc_1d (Tess.VerQty + 1);
      neut_tess_val_edge2ver (Tess, (*pMeshPara).edge_cl,
			      (*pMeshPara).ver_cl);
    }
  }
  else if ((*pMeshPara).input == 'v')
  {
    nem_meshing_para_cl_cell_tesr (clstring, pMeshPara, *pTesr);

    if (!strcmp ((*pMeshPara).elttype, "tri"))
    {
      if (Tess.Dim == 3)
      {
	(*pMeshPara).face_cl = ut_alloc_1d (Tess.FaceQty + 1);
	neut_tess_val_poly2face (Tess, (*pMeshPara).poly_cl,
				 (*pMeshPara).face_cl);
      }

      if (Tess.Dim >= 2)
      {
	(*pMeshPara).edge_cl = ut_alloc_1d (Tess.EdgeQty + 1);
	neut_tess_val_face2edge (Tess, (*pMeshPara).face_cl,
				 (*pMeshPara).edge_cl);
      }

      if (Tess.Dim >= 1)
      {
	(*pMeshPara).ver_cl = ut_alloc_1d (Tess.VerQty + 1);
	neut_tess_val_edge2ver (Tess, (*pMeshPara).edge_cl,
				(*pMeshPara).ver_cl);
      }
    }
  }
  else if ((*pMeshPara).input == 'm')
  {
    nem_meshing_para_cl_poly_mesh (clstring, pMeshPara, *pRNodes, Mesh, Tess);

    if (!strcmp ((*pMeshPara).elttype, "tri"))
    {
      (*pMeshPara).face_cl = ut_alloc_1d (Tess.FaceQty + 1);
      (*pMeshPara).edge_cl = ut_alloc_1d (Tess.EdgeQty + 1);
      (*pMeshPara).ver_cl = ut_alloc_1d (Tess.VerQty + 1);
      neut_tess_val_poly2face (Tess, (*pMeshPara).poly_cl,
			       (*pMeshPara).face_cl);
      neut_tess_val_face2edge (Tess, (*pMeshPara).face_cl,
			       (*pMeshPara).edge_cl);
      neut_tess_val_edge2ver (Tess, (*pMeshPara).edge_cl,
			      (*pMeshPara).ver_cl);
    }
  }

  if (clratiostring)
  {
    int j, qty;
    double gmean;
    char **tmp = NULL;

    ut_string_separate (clratiostring, NEUT_SEP_DEP, &tmp, &qty);

    (*pMeshPara).clratio = ut_alloc_1d (3);
    ut_array_1d_set ((*pMeshPara).clratio, 3, 1);

    for (j = 0; j < qty; j++)
      ut_string_real (tmp[j], &((*pMeshPara).clratio[j]));
    gmean = ut_array_1d_gmean ((*pMeshPara).clratio, qty);
    ut_array_1d_scale ((*pMeshPara).clratio, qty, 1 / gmean);
  }

  return 0;
}
