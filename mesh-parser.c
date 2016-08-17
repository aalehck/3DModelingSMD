/******************************************************************
To use this reader, you need to make sure your input mesh
has the keyword "OFF" in the beginning, followed by the mesh.
*******************************************************************/
#include "mesh-parser.h"

void calculateNormal(FLTVECT a, FLTVECT b, FLTVECT c, INT3VECT* face){
  
  float U[] = {b.x - a.x, b.y - a.y, b.z - a.z};
  float V[] = {c.x - a.x, c.y - a.y, c.z - a.z};

  face->normal_x = (U[1] * V[2]) - (U[2] * V[1]);
  face->normal_y = (U[2] * V[0]) - (U[0] * V[2]);
  face->normal_z = (U[0] * V[1]) - (U[1] * V[0]);

  float norm = sqrtf((face->normal_x*face->normal_x) + (face->normal_y*face->normal_y) + (face->normal_z*face->normal_z));

  face->normal_x = face->normal_x/norm;
  face->normal_y = face->normal_y/norm;
  face->normal_z = face->normal_z/norm;
}

void addVertexNormal(FLTVECT* a, float normal_x, float normal_y, float normal_z){
  a->normal_x += normal_x;
  a->normal_y += normal_y;
  a->normal_z += normal_z;

  ++a->length;
}

void averageVertexNormal(SurFaceMesh* mesh){
  for(int i = 0; i < mesh->nv; ++i){
    mesh->vertex[i].normal_x = mesh->vertex[i].normal_x/mesh->vertex[i].length;
    mesh->vertex[i].normal_y = mesh->vertex[i].normal_y/mesh->vertex[i].length;
    mesh->vertex[i].normal_z = mesh->vertex[i].normal_z/mesh->vertex[i].length;

    //printf("%f, %f, %f\n", mesh->vertex[i].normal_x, mesh->vertex[i].normal_y, mesh->vertex[i].normal_z);
  }
}

SurFaceMesh* readMeshFile(){
  int num,n,m;
  int a,b,c,d,e;
  float x,y,z;
  SurFaceMesh *surfmesh;
  char line[256];
  FILE *fin;


  if ((fin=fopen("sample_polygon.off", "r"))==NULL){
    printf("read error...\n");
    exit(0);
  };
  
  /* OFF format */
  while (fgets(line,256,fin) != NULL) {
    if (line[0]=='O' && line[1]=='F' && line[2]=='F')
      break;
  }
  fscanf(fin,"%d %d %d\n",&m,&n,&num);

  surfmesh = (SurFaceMesh*)malloc(sizeof(SurFaceMesh));
  surfmesh->max_x = 0.0f;
  surfmesh->max_y = 0.0f;
  surfmesh->max_z = 0.0f;
  surfmesh->nv = m;
  surfmesh->nf = n;
  surfmesh->vertex = (FLTVECT *)malloc(sizeof(FLTVECT)*surfmesh->nv);
  surfmesh->face = (INT3VECT *)malloc(sizeof(INT3VECT)*surfmesh->nf);

  for (n = 0; n < surfmesh->nv; n++) {
    fscanf(fin,"%f %f %f\n",&x,&y,&z);
    surfmesh->vertex[n].x = x;
    surfmesh->vertex[n].y = y;
    surfmesh->vertex[n].z = z;

    if(x > surfmesh->max_x){
      surfmesh->max_x = x;
    }
    
    if(y > surfmesh->max_y){
      surfmesh->max_y = y;
    }
    
    if(z > surfmesh->max_z){
      surfmesh->max_z = z;
    }

    if(n == 0){
      surfmesh->min_x;
      surfmesh->min_y;
      surfmesh->min_z;
    }
    else{
      if(x < surfmesh->min_x){
        surfmesh->max_x = x;
      }
      
      if(y < surfmesh->min_y){
        surfmesh->max_y = y;
      }
      
      if(z < surfmesh->min_z){
        surfmesh->max_z = z;
      }
    }
    
  }
  
  for (n = 0; n < surfmesh->nf; n++) {
    fscanf(fin,"%d %d %d %d\n",&a,&b,&c,&d);
    surfmesh->face[n].a = b;
    surfmesh->face[n].b = c;
    surfmesh->face[n].c = d;

    calculateNormal(surfmesh->vertex[b],  surfmesh->vertex[c],  surfmesh->vertex[d],  &surfmesh->face[n]);

    addVertexNormal(&surfmesh->vertex[b], surfmesh->face[n].normal_x, surfmesh->face[n].normal_y, surfmesh->face[n].normal_z);
    addVertexNormal(&surfmesh->vertex[c], surfmesh->face[n].normal_x, surfmesh->face[n].normal_y, surfmesh->face[n].normal_z);
    addVertexNormal(&surfmesh->vertex[d], surfmesh->face[n].normal_x, surfmesh->face[n].normal_y, surfmesh->face[n].normal_z);

    if(a != 3){
      printf("Errors: reading mesh .... \n");
    }
  }
  fclose(fin);

  averageVertexNormal(surfmesh);
  
  return surfmesh;
}
