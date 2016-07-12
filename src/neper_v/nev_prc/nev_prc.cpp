/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2015, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_prc_.h"

void
   nev_prc (char **expargv, int *pi, struct PRINT Print,
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
  prc::oPRCFile file("test.prc");

  const int N_COLOURS = 32;
  prc::RGBAColour colours[N_COLOURS];
  for(int i = 0; i < N_COLOURS; ++i)
  {
    colours[i%N_COLOURS].R = 0.0;
    colours[i%N_COLOURS].G = (i%N_COLOURS)/static_cast<double>(N_COLOURS);
    colours[i%N_COLOURS].B = 0.95;
    colours[i%N_COLOURS].A = 0.75;
  }

  prc::PRCmaterial materials[N_COLOURS];
  for(int i = 0; i < N_COLOURS; ++i)
  {
    materials[i%N_COLOURS].diffuse.R = 0.0;
    materials[i%N_COLOURS].diffuse.G = (i%N_COLOURS)/static_cast<double>(N_COLOURS);
    materials[i%N_COLOURS].diffuse.B = 0.95;
    materials[i%N_COLOURS].diffuse.A = 0.75;
    materials[i%N_COLOURS].specular.R = 0.01*0.0;
    materials[i%N_COLOURS].specular.G = 0.01*(i%N_COLOURS)/static_cast<double>(N_COLOURS);
    materials[i%N_COLOURS].specular.B = 0.01*0.95;
    materials[i%N_COLOURS].specular.A = 0.01*0.75;
    materials[i%N_COLOURS].emissive.R = 0.20*0.0;
    materials[i%N_COLOURS].emissive.G = 0.20*(i%N_COLOURS)/static_cast<double>(N_COLOURS);
    materials[i%N_COLOURS].emissive.B = 0.20*0.95;
    materials[i%N_COLOURS].emissive.A = 0.20*0.75;
    materials[i%N_COLOURS].ambient.R  = 0.05*0.0;
    materials[i%N_COLOURS].ambient.G  = 0.05*(i%N_COLOURS)/static_cast<double>(N_COLOURS);
    materials[i%N_COLOURS].ambient.B  = 0.05*0.95;
    materials[i%N_COLOURS].ambient.A  = 0.05*0.75;
    materials[i%N_COLOURS].alpha      = 0.75;
    materials[i%N_COLOURS].shininess  = 0.1;
  }

  for (int i = 1; i <= Tess.VerQty; i++)
    if (Tess.VerState[i] != -1)
       file.addPoint(Tess.VerCoo[i], prc::RGBAColour(1.0,1.0,1.0));


  prc::PRCoptions grpopt;
  grpopt.no_break = true;
  grpopt.do_break = false;
  grpopt.tess = true;

  file.begingroup("lignes",&grpopt);

  for (int i = 1; i <= Tess.EdgeQty; i++) {
    if (Tess.EdgeState[i] != -1) {
      double points[2][3];
      points[0][0] = Tess.VerCoo[Tess.EdgeVerNb[i][0]][0];
      points[0][1] = Tess.VerCoo[Tess.EdgeVerNb[i][0]][1];
      points[0][2] = Tess.VerCoo[Tess.EdgeVerNb[i][0]][2];
      points[1][0] = Tess.VerCoo[Tess.EdgeVerNb[i][1]][0];
      points[1][1] = Tess.VerCoo[Tess.EdgeVerNb[i][1]][1];
      points[1][2] = Tess.VerCoo[Tess.EdgeVerNb[i][1]][2];

      file.addLine(2, points, prc::RGBAColour(1.0,0.0,0.0)); 
    }
  }

  file.endgroup();
  
  prc::PRCmaterial materialBase(prc::RGBAColour(0.0,0.0,0.0,1), prc::RGBAColour(1,1,1,1),
                                prc::RGBAColour(0.0,0.0,0.0,1), prc::RGBAColour(0.0,0.0,0.0,1),
                                1.0,0.0);

  for (int i=1; i<= Tess.PolyQty; i++) {
    char name[50];
    sprintf(name,"G%d",i);
    file.begingroup(name,&grpopt);
    for (int j=1; j<= Tess.PolyFaceQty[i]; j++) {
      for(int k=1; k+2<= Tess.FaceVerQty[ Tess.PolyFaceNb[i][j] ]; k++) {
        double points[3][3];
        points[0][0] = Tess.VerCoo[ Tess.FaceVerNb[ Tess.PolyFaceNb[i][j] ][1] ][0];
        points[0][1] = Tess.VerCoo[ Tess.FaceVerNb[ Tess.PolyFaceNb[i][j] ][1] ][1];
        points[0][2] = Tess.VerCoo[ Tess.FaceVerNb[ Tess.PolyFaceNb[i][j] ][1] ][2];
        points[1][0] = Tess.VerCoo[ Tess.FaceVerNb[ Tess.PolyFaceNb[i][j] ][k+1] ][0];
        points[1][1] = Tess.VerCoo[ Tess.FaceVerNb[ Tess.PolyFaceNb[i][j] ][k+1] ][1];
        points[1][2] = Tess.VerCoo[ Tess.FaceVerNb[ Tess.PolyFaceNb[i][j] ][k+1] ][2];
        points[2][0] = Tess.VerCoo[ Tess.FaceVerNb[ Tess.PolyFaceNb[i][j] ][k+2] ][0];
        points[2][1] = Tess.VerCoo[ Tess.FaceVerNb[ Tess.PolyFaceNb[i][j] ][k+2] ][1];
        points[2][2] = Tess.VerCoo[ Tess.FaceVerNb[ Tess.PolyFaceNb[i][j] ][k+2] ][2];
        uint32_t  PPI[1][3] = {{0,1,2}};
        file.addTriangles(3 /*points*/,points,
  			1 /* tris */,PPI,materials[i%N_COLOURS],0,NULL,NULL,0,NULL,NULL,0,NULL,NULL,0,NULL,NULL,25.8419);
      }
    }
    file.endgroup();
  }
  
  file.finish();

  printf("Generated a prc file with %d grains\n",Tess.PolyQty);

  return;

}
