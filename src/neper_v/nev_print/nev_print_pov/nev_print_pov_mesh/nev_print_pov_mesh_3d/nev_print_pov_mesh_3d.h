/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

extern void nev_print_pov_mesh_3d (FILE *, struct PRINT Print, struct NODES
			       Nodes, struct MESH *Mesh,
			       struct NODEDATA NodeData,
			       struct MESHDATA *MeshData, int ***);


extern void nev_print_pov_mesh_3d_compress (struct PRINT Print, struct NODES
					Nodes, struct MESH Mesh3D,
					struct NODES *, struct MESH *,
					int ***pprintelt3dface, int **,
					int **);

#ifdef __cplusplus
}
#endif
