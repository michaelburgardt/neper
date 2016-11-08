/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#include "oPRCFile.h"
extern void nev_print_prc_mesh_3d (prc::oPRCFile *, struct PRINT Print,
				   struct TESS, struct NODES Nodes,
				   struct MESH *Mesh,
				   struct MESHDATA *MeshData);

extern void nev_print_prc_mesh_3d_compress (struct PRINT Print, struct NODES
					Nodes, struct MESH Mesh3D,
					struct NODES *, struct MESH *,
					int ***pprintelt3dface, int **,
					int **);

#ifdef __cplusplus
}
#endif
