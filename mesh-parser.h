#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <memory.h>
#include <sstream>
#include <fstream>
#include <vector>


typedef struct {
  float x;
  float y;
  float z;
  float normal_x, normal_y, normal_z;
  int length;
}FLTVECT;

typedef struct {
  int a;
  int b;
  int c;
  float normal_x;
  float normal_y;
  float normal_z;
}INT3VECT;

typedef struct {
  int nv;
  int nf;
  FLTVECT *vertex;
  INT3VECT *face;
  float max_x, min_x;
  float max_y, min_y;
  float max_z, min_z;

}SurFaceMesh;

typedef struct{
  float x, y, z;
  int length;
}vertexNormal;

typedef struct{
  FLTVECT* a;
  FLTVECT* b;
  FLTVECT* c; 
}TRIANGLE;

SurFaceMesh* readMeshFile();
void calculateNormal(FLTVECT a, FLTVECT b, FLTVECT c, INT3VECT* face);
void addVertexNormal(FLTVECT* a, float normal_x, float normal_y, float normal_z);
void averageVertexNormal(SurFaceMesh* mesh);
std::vector<TRIANGLE*> read_SMD_mesh(char* filename);