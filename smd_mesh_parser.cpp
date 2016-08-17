#include "mesh-parser.h"

std::vector<TRIANGLE*> read_SMD_mesh(char* filename){
  	std::string line;

	std::ifstream infile(filename);

	std::vector<TRIANGLE*> mesh;
 	
 	while(std::getline(infile, line)){
//
 		if(line.find("slow_body") || line.find("slow_helmet") || line.find("Deathclaw")){

 			TRIANGLE* triangle = (TRIANGLE*)malloc(sizeof(TRIANGLE));

 			for(int i= 0; i < 3; ++i){
 				int ind;
 				FLTVECT* vertex = (FLTVECT*)malloc(sizeof(FLTVECT));
 				std::getline(infile, line);
 				sscanf(line.c_str(), "  %d %f %f %f %f %f %f", &ind, &vertex->x, &vertex->y, &vertex->z, &vertex->normal_x, &vertex->normal_y, &vertex->normal_z);
 				//printf("%f %f %f %f %f %f\n",vertex->x, vertex->y, vertex->z, vertex->normal_x, vertex->normal_y, vertex->normal_z);
 				if(i == 0)
 					triangle->a = vertex;
 				if(i == 1)
 					triangle->b = vertex;
 				if(i == 2)
 					triangle->c = vertex;
 			}

 			mesh.push_back(triangle);
 		
 		}
  	}

  	return mesh;
}