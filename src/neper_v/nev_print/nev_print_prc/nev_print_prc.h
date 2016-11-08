/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

extern void nev_print_prc (char *body, struct PRINT Print, struct TESS
			   Tess, struct TESSDATA TessData, struct TESR
			   Tesr, struct TESRDATA TesrData, struct NODES
			   Nodes, struct MESH* Mesh, struct NODEDATA
			   NodeData, struct MESHDATA *MeshData, int
			   SQty, struct NODES *SNodes, struct MESH*
			   SMesh2D, struct NODEDATA *SNodeData, struct
			   MESHDATA **SMeshData, int **SElt2dElt3d,
			   struct CSYSDATA CsysData, struct POINT Point,
			   struct POINTDATA PointData);

#ifdef __cplusplus
}
#endif
