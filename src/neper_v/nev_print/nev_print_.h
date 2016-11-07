/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"neut_v.h"
#include "nev_print_header/nev_print_header.h"
#include "nev_print_utils/nev_print_utils.h"
#include "nev_print_mesh/nev_print_mesh.h"
#include "nev_print_tess/nev_print_tess.h"
#include "nev_print_tesr/nev_print_tesr.h"
#include "nev_print_foot/nev_print_foot.h"
#include "nev_print_point/nev_print_point.h"

#include "nev_print_prc/nev_print_prc.h"

#include "../nev_data/nev_data.h"

#include "nev_print.h"

extern void nev_print_pov2png (char *, char *, int, int, int, int);

extern void nev_print_pov (char *body, struct PRINT Print, struct TESS
			   Tess, struct TESSDATA TessData, struct TESR
			   Tesr, struct TESRDATA TesrData, struct NODES
			   Nodes, struct MESH* Mesh, struct NODEDATA
			   NodeData, struct MESHDATA *MeshData, int
			   SQty, struct NODES *SNodes, struct MESH*
			   SMesh2D, struct NODEDATA *SNodeData, struct
			   MESHDATA **SMeshData, int **SElt2dElt3d,
			   struct CSYSDATA CsysData, struct POINT Point,
			   struct POINTDATA PointData);
