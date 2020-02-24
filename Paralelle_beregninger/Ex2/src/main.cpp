#include <iostream>
#include <cstring>
#include "utilities/OBJLoader.hpp"
#include "utilities/lodepng.h"
#include "rasteriser.hpp"
#include "openmpi/mpi.h"
#include "mpi.h"
// #include <openmpi/mpi.hpp>
#define MASTER 0
#define LOADER 0
#define f(A) ((A*4535435 + 15423443)%200)

// Declarations
// std::vecotr<Mesh> receiveMeshes(int rank);
void broadcastMeshes( std::vector<Mesh> &meshes);
void broadcastMesh(Mesh *c,  const MPI_Datatype &f3_type,const MPI_Datatype &f4_type);
void broadcastToAll(std::vector<int>* data, int* size);
MPI_Datatype makeFloat3Type();
MPI_Datatype makeFloat4Type();


int main(int argc, char **argv) {
	MPI_Init ( &argc, &argv );

	std::string input("../input/spheres.obj");
	std::string output("../output/spheres.png");
	unsigned int width = 1920;
	unsigned int height = 1080;
	unsigned int depth = 3;

	for (int i = 1; i < argc; i++) {
		if (i < argc -1) {
			if (std::strcmp("-i", argv[i]) == 0) {
				input = argv[i+1];
			} else if (std::strcmp("-o", argv[i]) == 0) {
				output = argv[i+1];
			} else if (std::strcmp("-w", argv[i]) == 0) {
				width = (unsigned int) std::stoul(argv[i+1]);
			} else if (std::strcmp("-h", argv[i]) == 0) {
				height = (unsigned int) std::stoul(argv[i+1]);
			} else if (std::strcmp("-d", argv[i]) == 0) {
				depth = (int) std::stoul(argv[i+1]);
			}
		}
	}

	std::cout << "Loading '" << input << "' file... " << std::endl;




	int rank, nProcesses;
	MPI_Comm_rank ( MPI_COMM_WORLD, &rank );
    MPI_Comm_size ( MPI_COMM_WORLD, &nProcesses );
	

	std::vector<Mesh> meshs;
	if(rank == LOADER){
		meshs = loadWavefront(input, false);
	}
	else{
		#ifndef ONLY_BROADCAST
			meshs = loadWavefront(input, false);	
		#endif //ONLY_BROADCAST
	}

	broadcastMeshes(meshs); 





	int rotationAngle; 
	output[ output.length() -5] = 'A'+ (rank %26); 

	// Manage rotation angles
	if(rank == MASTER){
		std::cout<<"MAster\n";
		//Master stuff
		int* rotations = new int[nProcesses];
		for(int i =0; i< nProcesses; ++i){
			if(i != MASTER){
				rotations[i] = f(i); 
				MPI_Send(&rotations[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			}
		}
		for(int i =0; i< nProcesses; ++i){
			// get ack
			if(i != MASTER){
				MPI_Recv(&rotationAngle,1 , MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}
		delete[] rotations;

		rotationAngle= f(MASTER); 

	}
	else{
		std::cout<<"Slave\n";
		
		MPI_Recv(&rotationAngle,1 , MPI_INT, MASTER,  0,  MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Send( &rotationAngle, 1, MPI_INT, MASTER, 0,  MPI_COMM_WORLD); // Return an ack

		//Ask for stuff
	}

	std::cout<< "Altered output: "<< output << "\n"; 
	std::vector<unsigned char> frameBuffer = rasterise(meshs, width, height, depth,0,  rank, nProcesses);
	std::cout << rank<<": FInished!\n";
	MPI_Barrier(MPI_COMM_WORLD);

	std::cout << "Waiting for others\n";
	MPI_Barrier(MPI_COMM_WORLD);
	std::cout << "Writing image to '" << output << "'..." << std::endl;
	if(rank ==0){
		unsigned error = lodepng::encode(output, frameBuffer, width, height);
	
	if(error)
	{
		std::cout << "An error occurred while writing the image file: " << error << ": " << lodepng_error_text(error) << std::endl;
	}
	}
	std::cout<< "Done Rank" << rank<<"\n"; 
	MPI_Finalize();
	return 0;
}




void broadcastMeshes( std::vector<Mesh> &meshes){


	MPI_Datatype f3_type = makeFloat3Type();
	MPI_Datatype f4_type = makeFloat4Type();

	int size = meshes.size(); 
	MPI_Bcast(&size, 1, MPI_INT, LOADER, MPI_COMM_WORLD); 
	Mesh dummy_mesh("dummy_mesh");
	meshes.resize(size, dummy_mesh); 
	for(int i =0; i< size; ++i){
		Mesh* c =&meshes[i];
		
		broadcastMesh(c, f3_type,f4_type); 
	}



}

void broadcastMesh(Mesh *c,  const MPI_Datatype &f3_type,const MPI_Datatype &f4_type){
		// c = current

		int nVertecies, nTextures, nNormals; 

		//Vertecies
		nVertecies = c->vertices.size(); 
		MPI_Bcast(&nVertecies, 1, MPI_INT, LOADER, MPI_COMM_WORLD);
		c->vertices.resize(nVertecies); 
		MPI_Bcast(c->vertices.data(), nVertecies, f4_type, LOADER, MPI_COMM_WORLD);

		// Textures
		nTextures = c->textures.size(); 
		MPI_Bcast(&nTextures, 1, MPI_INT, LOADER, MPI_COMM_WORLD);
		c->textures.resize(nTextures); 
		MPI_Bcast(c->textures.data(), nTextures, f3_type, LOADER, MPI_COMM_WORLD);

		// Normals
		nNormals = c->normals.size(); 
		MPI_Bcast(&nNormals, 1, MPI_INT, LOADER, MPI_COMM_WORLD);
		c->normals.resize(nNormals); 
		MPI_Bcast(c->normals.data(), nNormals, f3_type, LOADER, MPI_COMM_WORLD);
		
		

}


MPI_Datatype makeFloat3Type(){
	//TODO Find a way to make these datatypes
	float3 float3_n; 
	const int nitems = 3; 
	int blocklengths[3]= {1,1,1};
	MPI_Datatype types[3] = {MPI_FLOAT,MPI_FLOAT,MPI_FLOAT};
	MPI_Datatype float3Type;
	MPI_Aint ofsets[3];
	ofsets[0] = offsetof(float3, x);
	ofsets[1] = offsetof(float3, y);
	ofsets[2] = offsetof(float3, z);

	MPI_Type_create_struct(nitems, blocklengths, ofsets, types, &float3Type	 );
	MPI_Type_commit(&float3Type); 

	return float3Type; 

}


MPI_Datatype makeFloat4Type(){
	//TODO Find a way to make these datatypes
	float4 float4_n; 
	const int nitems = 4; 
	int blocklengths[4]= {1,1,1, 1};
	MPI_Datatype types[4] = {MPI_FLOAT,MPI_FLOAT,MPI_FLOAT, MPI_FLOAT};
	MPI_Datatype float4Type;
	MPI_Aint ofsets[4];
	ofsets[0] = offsetof(float4, x);
	ofsets[1] = offsetof(float4, y);
	ofsets[2] = offsetof(float4, z);
	ofsets[3] = offsetof(float4, w);


	MPI_Type_create_struct(nitems, blocklengths, ofsets, types, &float4Type	 );
	MPI_Type_commit(&float4Type); 

	return float4Type; 

}

//! The float types are not viritual and do not inherit, so they do not need a vptr in memory.
// => Their size is n* size(float)