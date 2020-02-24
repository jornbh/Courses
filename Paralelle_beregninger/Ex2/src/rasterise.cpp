#include "rasteriser.hpp"
#include "utilities/lodepng.h"
#include <vector>
#include <iomanip>
#include <chrono>
#include <limits>
#include "openmpi/mpi.h"
#include <cmath>
const std::vector<globalLight> lightSources = { {{0.3f, 0.5f, 1.0f}, {1.0f, 1.0f, 1.0f}} };
static int ROTATION_ANGLE=0; 
// static std::vector<int> VISITED; 
typedef struct perfCounter {
	unsigned long meshs = 0;
	unsigned long triagnles = 0;
} perfCounter;

perfCounter counter = {};

static unsigned int SIZE;//!
static unsigned int RANK;//!
void runVertexShader( Mesh &mesh,
                      Mesh &transformedMesh,
                      float3 positionOffset,
                      float scale,
					  unsigned int const width,
					  unsigned int const height,
				  	  float const rotationAngle = ROTATION_ANGLE)
{
	float const pi = std::acos(-1);
	// The matrices defined below are the ones used to transform the vertices and normals.

	// This projection matrix assumes a 16:9 aspect ratio, and an field of view (FOV) of 90 degrees.
	mat4x4 const projectionMatrix(
		0.347270,   0, 			0, 		0,
		0,	  		0.617370, 	0,		0,
		0,	  		0,			-1, 	-0.2f,
		0,	  		0,			-1,		0);

	mat4x4 translationMatrix(
		1,			0,			0,			0 + positionOffset.x /*X*/,
		0,			1,			0,			0 + positionOffset.y /*Y*/,
		0,			0,			1,			-10 + positionOffset.z /*Z*/,
		0,			0,			0,			1);

	mat4x4 scaleMatrix(
		scale/*X*/,	0,			0,				0,
		0, 			scale/*Y*/, 0,				0,
		0, 			0,			scale/*Z*/, 	0,
		0, 			0,			0,				1);

	mat4x4 const rotationMatrixX(
		1,			0,				0, 				0,
		0, 			std::cos(0), 	-std::sin(0),	0,
		0, 			std::sin(0),	std::cos(0), 	0,
		0, 			0,				0,				1);

	float const rotationAngleRad = (pi / 4.0f) + (rotationAngle / (180.0f/pi));

	mat4x4 const rotationMatrixY(
		std::cos(rotationAngleRad),		0,			std::sin(rotationAngleRad), 	0,
		0, 								1, 			0,								0,
		-std::sin(rotationAngleRad), 	0,			std::cos(rotationAngleRad), 	0,
		0, 								0,			0,								1);

	mat4x4 const rotationMatrixZ(
		std::cos(pi),	-std::sin(pi),	0,			0,
		std::sin(pi), 	std::cos(pi), 	0,			0,
		0,				0,				1,			0,
		0, 				0,				0,			1);

	mat4x4 const MVP =
		projectionMatrix * translationMatrix * rotationMatrixX * rotationMatrixY * rotationMatrixZ * scaleMatrix;

	for (unsigned int i = 0; i < mesh.vertices.size(); i++) {
		float4 currentVertex = mesh.vertices.at(i);
		float4 transformed = (MVP * currentVertex);
		currentVertex = transformed / transformed.w;
		currentVertex.x = (currentVertex.x + 0.5f) * (float) width;
		currentVertex.y = (currentVertex.y + 0.5f) * (float) height;
		transformedMesh.vertices.at(i) = currentVertex;
	}
}

long long getTreeSize(int depth, int nBranches){
	long long current =1, accumulator =0; 
	for(int i =0; i< depth; ++i){
		accumulator += current; 
		current*= nBranches;
	}
	return accumulator; 
}
void runFragmentShader( std::vector<unsigned char> &frameBuffer,
						unsigned int const baseIndex,
						Face const &face,
						float3 const &weights )
{
	float3 normal = face.getNormal(weights);

	float3 colour(0);
	for (globalLight const &l : lightSources) {
		float3 lightNormal = normal * l.direction;
		colour += (face.parent.material.Kd * l.colour) * (lightNormal.x + lightNormal.y + lightNormal.z);
	}

	colour = colour.clamp(0.0f, 1.0f);
	frameBuffer.at(4 * baseIndex + 0) = colour.x * 255.0f;
	frameBuffer.at(4 * baseIndex + 1) = colour.y * 255.0f;
	frameBuffer.at(4 * baseIndex + 2) = colour.z * 255.0f;
	frameBuffer.at(4 * baseIndex + 3) = 255;
}

/**
 * The main procedure which rasterises all triangles on the framebuffer
 * @param transformedMesh         Transformed mesh object
 * @param frameBuffer             frame buffer for the rendered image
 * @param depthBuffer             depth buffer for every pixel on the image
 * @param width                   width of the image
 * @param height                  height of the image
 */
void rasteriseTriangles( Mesh &transformedMesh,
                         std::vector<unsigned char> &frameBuffer,
                         std::vector<float> &depthBuffer,
                         unsigned int const width,
                         unsigned int const height )
{
	for (unsigned int i = 0; i < transformedMesh.faceCount(); i++) {

		Face face = transformedMesh.getFace(i);
		unsigned int minx = int(std::floor(std::min(std::min(face.v0.x, face.v1.x), face.v2.x)));
		unsigned int maxx = int(std::ceil (std::max(std::max(face.v0.x, face.v1.x), face.v2.x)));
		unsigned int miny = int(std::floor(std::min(std::min(face.v0.y, face.v1.y), face.v2.y)));
		unsigned int maxy = int(std::ceil (std::max(std::max(face.v0.y, face.v1.y), face.v2.y)));

		// Let's make sure the screen coordinates stay inside the window
		minx = std::max(minx, (unsigned int) 0);
		maxx = std::min(maxx, width);
		miny = std::max(miny, (unsigned int) 0);
		maxy = std::min(maxy, height);

		// We iterate over each pixel in the triangle's bounding box
		for(unsigned int x = minx; x < maxx; x++) {
			for(unsigned int y = miny; y < maxy; y++) {
				float u,v,w;
				if(face.inRange(x,y,u,v,w)){
					float pixelDepth = face.getDepth(u,v,w);
					if( pixelDepth >= -1 && pixelDepth <= 1 && pixelDepth < depthBuffer.at(y * width + x)) {
						depthBuffer.at(y * width + x) = pixelDepth;
						runFragmentShader(frameBuffer, x + (width * y), face, float3(u,v,w));
					}
				}
			}
		}
	}
}




void work_renderMeshFractal(
				std::vector<Mesh> &meshes,
				std::vector<Mesh> &transformedMeshes,
				unsigned int width,
				unsigned int height,
				std::vector<unsigned char> &frameBuffer,
				std::vector<float> &depthBuffer,
				float largestBoundingBoxSide,
				int depthLimit,
				float scale = 1.0,
				float3 distanceOffset = {0, 0, 0},
				unsigned long baseIndex =1, 
				unsigned long start =0, 
				unsigned long end =-1) {



	// Start by rendering the mesh at this depth
	
	if( start<= baseIndex){
	// VISITED.push_back(baseIndex);

	for (unsigned int i = 0; i < meshes.size(); i++) {

		Mesh &mesh = meshes.at(i);
		Mesh &transformedMesh = transformedMeshes.at(i);

		rasteriseTriangles(transformedMesh, frameBuffer, depthBuffer, width, height);
		runVertexShader(mesh, transformedMesh, distanceOffset, scale, width, height);
	}
	}
	// Check whether we've reached the recursive depth of the fractal we want to reach
	depthLimit--;


	const int branch = 26;
	int step = getTreeSize( depthLimit, branch);

	if(depthLimit <= 0) {
		return;
	}
	int iterator=0; 


	// Now we recursively draw the meshes in a smaller size
	for(int offsetX = -1; offsetX <= 1; offsetX++) {
		for(int offsetY = -1; offsetY <= 1; offsetY++) {
			for(int offsetZ = -1; offsetZ <= 1; offsetZ++  ) {
				float3 offset(offsetX,offsetY,offsetZ);
				// We draw the new objects in a grid around the "main" one.
				// We thus skip the location of the object itself.
				

				if( offset == 0) continue; 
				
				int new_baseIndex = (iterator*step) +1+ baseIndex;
				++iterator;

				if(  !(new_baseIndex >= - step+ (long)start +1) 
					|| end   <  new_baseIndex){

					continue; 
				}


				float smallerScale = scale / 3.0;
				float3 displacedOffset(
					distanceOffset + offset * (largestBoundingBoxSide / 2.0f) * scale
				);
				 

				work_renderMeshFractal(meshes, 
										transformedMeshes, 
										width, 
										height, 
										frameBuffer, 
										depthBuffer, 
										largestBoundingBoxSide, 
										depthLimit, 
										smallerScale, 
										displacedOffset, 
										new_baseIndex, 
										start, 
										end);
				
			}
		}
	}//end ofsetX


	

}

void renderMeshFractal(
	
				std::vector<Mesh> &meshes,
				std::vector<Mesh> &transformedMeshes,
				unsigned int width,
				unsigned int height,
				std::vector<unsigned char> &frameBuffer,
				std::vector<float> &depthBuffer,
				float largestBoundingBoxSide,
				int depthLimit
			
			
			){
				const int branch = 26; 
				long long nJobs = getTreeSize(depthLimit, branch); 
				unsigned long start = (nJobs* RANK)/SIZE; 
				start += (start == 0); 
				unsigned long end = (nJobs*(RANK+1))/SIZE; 
				end -= (RANK!=(SIZE-1)); 
				int baseIndex =1; 
				// VISITED.resize(0); 
				std::cout << "INFO "<<RANK <<", "<<start<< ", "<<end << ", "<<nJobs<< "\n"; 
				work_renderMeshFractal(
				meshes,
				transformedMeshes,
				width,
				height,
				frameBuffer,
				depthBuffer,
				largestBoundingBoxSide,
				depthLimit,
				1.0,
				float3{0, 0, 0},
				baseIndex, 
				start, 
				end );

				std::cout << "Rank, start, end"<<RANK<< " "<< start<< " "<< end<<"\n"; 
				// for(int i = start; i<end; ++i){
				// 	// if(RANK ==0)
				// 	// // 	std::cout<< "("<<VISITED[i-start]<<"), " ; 
				// 	// if(VISITED[i-start]!= i){
				// 		// // std::cout << "NOT VISITED "<< RANK <<" vec: "<< VISITED[i-start]<< " ind:"<<i<<"\n"; 

				// 	// }
				// }
				


			}





































// This function kicks off the rasterisation process.
std::vector<unsigned char> rasterise(	std::vector<Mesh> &meshes, 
										unsigned int width,
										unsigned int height,
										unsigned int depthLimit,
										int rotationAngle, 
										int rank, 
										int size
	) {

	RANK = rank;
	SIZE = size; 
	ROTATION_ANGLE = rotationAngle;
	// We first need to allocate some buffers.
	// The framebuffer contains the image being rendered.
	std::vector<unsigned char> frameBuffer;
	// The depth buffer is used to make sure that objects closer to the camera occlude/obscure objects that are behind it
	std::vector<float> depthBuffer;
	frameBuffer.resize(width * height * 4, 0);
	for (unsigned int i = 3; i < (4 * width * height); i+=4) {
		frameBuffer.at(i) = 255;
	}
	depthBuffer.resize(width * height, 1);

	float3 boundingBoxMin(std::numeric_limits<float>::max());
	float3 boundingBoxMax(std::numeric_limits<float>::min());
	std::cout << "Rendering image... " << std::flush;

	std::vector<Mesh> transformedMeshes;
	for(unsigned int i = 0; i < meshes.size(); i++) {
		transformedMeshes.push_back(meshes.at(i).clone());

		for(unsigned int vertex = 0; vertex < meshes.at(i).vertices.size(); vertex++) {
			boundingBoxMin.x = std::min(boundingBoxMin.x, meshes.at(i).vertices.at(vertex).x);
			boundingBoxMin.y = std::min(boundingBoxMin.y, meshes.at(i).vertices.at(vertex).y);
			boundingBoxMin.z = std::min(boundingBoxMin.z, meshes.at(i).vertices.at(vertex).z);

			boundingBoxMax.x = std::max(boundingBoxMax.x, meshes.at(i).vertices.at(vertex).x);
			boundingBoxMax.y = std::max(boundingBoxMax.y, meshes.at(i).vertices.at(vertex).y);
			boundingBoxMax.z = std::max(boundingBoxMax.z, meshes.at(i).vertices.at(vertex).z);
		}
	}

	float3 boundingBoxDimensions = boundingBoxMax - boundingBoxMin;
	float largestBoundingBoxSide = std::max(std::max(boundingBoxDimensions.x, boundingBoxDimensions.y), boundingBoxDimensions.z);


	renderMeshFractal(meshes, transformedMeshes, width, height, frameBuffer, depthBuffer, largestBoundingBoxSide, depthLimit);//depthLimit);

	// Depth buffer is now changed



	// Merge buffers
	std::vector<float> recDepthBuffer;
	recDepthBuffer.resize( depthBuffer.size());

	std::vector<unsigned char> workFrameBuffer;
	workFrameBuffer.resize( frameBuffer.size());
	for(unsigned int i =0; i< workFrameBuffer.size(); ++i){
		workFrameBuffer[i] = 255; 
	}


	MPI_Allreduce(depthBuffer.data(), recDepthBuffer.data(), depthBuffer.size(),MPI_FLOAT, MPI_MIN, MPI_COMM_WORLD); 

	for(unsigned int i =0; i< depthBuffer.size(); ++i){
		if(recDepthBuffer[i] >=  depthBuffer[i] ){
			workFrameBuffer[4*i    ] = frameBuffer[4*i    ]; 
			workFrameBuffer[4*i + 1] = frameBuffer[4*i + 1]; 
			workFrameBuffer[4*i + 2] = frameBuffer[4*i + 2]; 
			workFrameBuffer[4*i + 3] = frameBuffer[4*i + 3]; 
		}

	}	


	std::vector<unsigned char> recvBuffer;
	recvBuffer.resize( workFrameBuffer.size()); 
	// MPI_Allreduce(workFrameBuffer.data(), recvBuffer.data(), workFrameBuffer.size() 
	// 				,MPI_CHAR, MPI_BAND, MPI_COMM_WORLD); 
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Reduce(workFrameBuffer.data(), recvBuffer.data(), workFrameBuffer.size() 
					,MPI_CHAR, MPI_BAND, 0, MPI_COMM_WORLD);

	std::cout << "finished! "<< rank << std::endl;
	
	return recvBuffer;


}




