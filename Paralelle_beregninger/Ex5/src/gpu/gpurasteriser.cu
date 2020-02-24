#include "gpurasteriser.cuh"
#include "utilities/OBJLoader.hpp"
#include <vector>
#include <iomanip>
#include <chrono>
#include <limits>
#include <iostream>
#include <algorithm>
#include <device_functions.h>
#include "cuda_runtime.h"
#include "utilities/cuda_error_helper.hpp"
#include <chrono>

// Todo: Implement

////////////////////////////////////////////
// New code

class NEW_timer_t{
public:
	std::chrono::high_resolution_clock::time_point startTime;

	void start(){
		this-> startTime = std::chrono::high_resolution_clock::now();
	}
	void stop(){
		std::cout <<
				 std::chrono::duration_cast<std::chrono::milliseconds>( (std::chrono::high_resolution_clock::now() - startTime)).count()
		<<"\n";
		}
};
NEW_timer_t timer;
////////////////////////////////////////////


// UTILITY FUNCTIONS HAVE BEEN MOVED INTO THE KERNEL SOURCE FILE ITSELF
// CUDA relocatable and separable compilation is possible, but due to the many possible
// problems it can cause on different platforms, I decided to take the safe route instead
// and make sure it would compile fine for everyone. That implies moving everything into
// one file unfortunately.

class globalLight {
public:
	float3 direction;
	float3 colour;
	__host__ __device__ globalLight(float3 const vdirection, float3 const vcolour) : direction(vdirection), colour(vcolour) {}
};

__host__ __device__ float dotGPU(float3 a, float3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

__host__ __device__ float3 normalizeGPU(float3 v)
{
    float invLen = 1.0f / sqrtf(dotGPU(v, v));
    v.x *= invLen;
    v.y *= invLen;
    v.z *= invLen;
    return v;
}

// Utility function if you'd like to convert the depth buffer to an integer format.
__host__ __device__ int depthFloatToInt(float value) {
	value = (value + 1.0f) * 0.5f;
    return static_cast<int>(static_cast<double>(value) * static_cast<double>(16777216));
}

__host__ __device__ bool isPointInTriangle(
		float4 const &v0, float4 const &v1, float4 const &v2,
		unsigned int const x, unsigned int const y,
		float &u, float &v, float &w) {
		u = (((v1.y - v2.y) * (x    - v2.x)) + ((v2.x - v1.x) * (y    - v2.y))) /
				 	 (((v1.y - v2.y) * (v0.x - v2.x)) + ((v2.x - v1.x) * (v0.y - v2.y)));
		if (u < 0) {
			return false;
		}
		v = (((v2.y - v0.y) * (x    - v2.x)) + ((v0.x - v2.x) * (y    - v2.y))) /
					(((v1.y - v2.y) * (v0.x - v2.x)) + ((v2.x - v1.x) * (v0.y - v2.y)));
		if (v < 0) {
			return false;
		}
		w = 1 - u - v;
		if (w < 0) {
			return false;
		}
		return true;
}

__host__ __device__ float3 computeInterpolatedNormal(
		float3 const &normal0,
		float3 const &normal1,
		float3 const &normal2,
		float3 const &weights
	) {
	float3 weightedN0, weightedN1, weightedN2;

	weightedN0.x = (normal0.x * weights.x);
	weightedN0.y = (normal0.y * weights.x);
	weightedN0.z = (normal0.z * weights.x);

	weightedN1.x = (normal1.x * weights.y);
	weightedN1.y = (normal1.y * weights.y);
	weightedN1.z = (normal1.z * weights.y);

	weightedN2.x = (normal2.x * weights.z);
	weightedN2.y = (normal2.y * weights.z);
	weightedN2.z = (normal2.z * weights.z);

	float3 weightedNormal;

	weightedNormal.x = weightedN0.x + weightedN1.x + weightedN2.x;
	weightedNormal.y = weightedN0.y + weightedN1.y + weightedN2.y;
	weightedNormal.z = weightedN0.z + weightedN1.z + weightedN2.z;

	return normalizeGPU(weightedNormal);
}

__host__ __device__ float computeDepth(
		float4 const &v0, float4 const &v1, float4 const &v2,
		float3 const &weights) {
	return weights.x * v0.z + weights.y * v1.z + weights.z * v2.z;
}





// ORIGINAL SOURCE FILE IS STARTING HERE

struct workItemGPU {
    float scale;
    float3 distanceOffset;

    workItemGPU(float& scale_, float3& distanceOffset_) : scale(scale_), distanceOffset(distanceOffset_) {}
    __host__ __device__ workItemGPU() : scale(1), distanceOffset(make_float3(0, 0, 0)) {}
};

__device__ void runVertexShader( float4 &vertex,
                      float3 positionOffset,
                      float scale,
					  unsigned int const width,
					  unsigned int const height,
				  	  float const rotationAngle = 0)
{
	float const pi = 3.1415926f;
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

	scale *= 3.0f;
	mat4x4 scaleMatrix(
		scale/*X*/,	0,			0,				0,
		0, 			scale/*Y*/, 0,				0,
		0, 			0,			scale/*Z*/, 	0,
		0, 			0,			0,				1);

	mat4x4 const rotationMatrixX(
		1,			0,				0, 				0,
		0, 			cosf(0), 	-sinf(0),	0,
		0, 			sinf(0),	cosf(0), 	0,
		0, 			0,				0,				1);

	float const rotationAngleRad = (pi / 4.0f) + (rotationAngle / (180.0f/pi));

	mat4x4 const rotationMatrixY(
		cosf(rotationAngleRad), 0, sinf(rotationAngleRad), 0,
		0, 1, 0, 0,
		-sinf(rotationAngleRad), 0, cosf(rotationAngleRad), 	0,
		0, 0, 0, 1);

	mat4x4 const rotationMatrixZ(
		cosf(pi),	-sinf(pi),	0,			0,
		sinf(pi), 	cosf(pi), 	0,			0,
		0,				0,				1,			0,
		0, 				0,				0,			1);

	mat4x4 const MVP =
		projectionMatrix * translationMatrix * rotationMatrixX * rotationMatrixY * rotationMatrixZ * scaleMatrix;

		float4 transformed = (MVP * vertex);

    vertex.x = transformed.x / transformed.w;
    vertex.y = transformed.y / transformed.w;
    vertex.z = transformed.z / transformed.w;
    vertex.w = 1.0;

    vertex.x = (vertex.x + 0.5f) * (float) width;
    vertex.y = (vertex.y + 0.5f) * (float) height;
}


__device__ float3 runFragmentShader(
						GPUMesh &mesh,
						unsigned int triangleIndex,
						float3 const &weights)
{
	float3 normal = computeInterpolatedNormal(
            mesh.normals[3 * triangleIndex + 0],
            mesh.normals[3 * triangleIndex + 1],
            mesh.normals[3 * triangleIndex + 2],
			weights);

    float3 colour = make_float3(0.0f, 0.0f, 0.0f);

    const unsigned int lightSourceCount = 1;
    const globalLight lightSources[lightSourceCount] = {{make_float3(0.3f, 0.5f, 1.0f), make_float3(1.0f, 1.0f, 1.0f)}};

	for (int lightSource = 0; lightSource < lightSourceCount; lightSource++) {
		globalLight l = lightSources[lightSource];
		float lightNormalDotProduct =
			normal.x * l.direction.x + normal.y * l.direction.y + normal.z * l.direction.z;

		float3 diffuseReflectionColour;
		diffuseReflectionColour.x = mesh.objectDiffuseColour.x * l.colour.x;
		diffuseReflectionColour.y = mesh.objectDiffuseColour.y * l.colour.y;
		diffuseReflectionColour.z = mesh.objectDiffuseColour.z * l.colour.z;

		colour.x += diffuseReflectionColour.x * lightNormalDotProduct;
		colour.y += diffuseReflectionColour.y * lightNormalDotProduct;
		colour.z += diffuseReflectionColour.z * lightNormalDotProduct;
	}

    colour.x = fminf(fmaxf(colour.x, 0.0f), 1.0f);
    colour.y = fminf(fmaxf(colour.y, 0.0f), 1.0f);
    colour.z = fminf(fmaxf(colour.z, 0.0f), 1.0f);

    return colour;
}







////////////////////////////////////////////////
// New code

// We iterate over each pixel in the triangle's bounding box
__device__ void iterateOverTriangle(	int minx,
													int miny,
													int maxx,
													int maxy,
													GPUMesh &mesh,
													unsigned int triangleIndex,
													float4 v0,
													float4 v1,
													float4 v2,
													unsigned char* frameBuffer,
													int* depthBuffer,
													int width
	){
	for (unsigned int x = minx; x < maxx; x++) {
		for (unsigned int y = miny; y < maxy; y++) {
			float u, v, w;
			// For each point in the bounding box, determine whether that point lies inside the triangle
			if (isPointInTriangle(v0, v1, v2, x, y, u, v, w)) {
				// If it does, compute the distance between that point on the triangle and the screen
				float pixelDepth = computeDepth(v0, v1, v2, make_float3(u, v, w));
				// If the point is closer than any point we have seen thus far, render it.
				// Otherwise it is hidden behind another object, and we can throw it away
				// Because it will be invisible anyway.
								if (pixelDepth >= -1 && pixelDepth <= 1)
								{
									int myDepth = depthFloatToInt(pixelDepth);
					int newDepth = atomicMin(&depthBuffer[y * width + x], myDepth);

					// I realise this does not solve the race condition.
										// However, it does reduce the probability it occurs.
										// Solving this properly requires implementing a full-blown tile renderer.
										// And I think it's more important to keep things as simple as possible here,
										// so you can understand what is going on.
					if(myDepth < newDepth) {
						float3 pixelColour = runFragmentShader(mesh, triangleIndex, make_float3(u, v, w));

						if(myDepth == depthBuffer[y * width + x]) {
								frameBuffer[4 * (x + (width * y)) + 0] = pixelColour.x * 255.0f;
								frameBuffer[4 * (x + (width * y)) + 1] = pixelColour.y * 255.0f;
								frameBuffer[4 * (x + (width * y)) + 2] = pixelColour.z * 255.0f;
								frameBuffer[4 * (x + (width * y)) + 3] = 255;
						}
					}
				}
			}
		}
	}
}

__device__ float4 sub(const float4& v0, const float4& v1){
			float4 out;
			out.x = v0.x-v1.x;
			out.y = v0.y-v1.y;
			out.z = v0.z-v1.z;
			out.w = v0.w-v1.w;
			return out;


}
__device__ float sq(float in){
	return in*in;
}
__device__ float chross(const float4& v0, const float4& v1){
		float out=0;
		out+=sq(v0.x*v1.y - v1.x*v0.y); //z²
		out+=sq(v1.x*v0.z - v0.x*v1.z); //z²
		out+=sq(v0.y*v1.z - v1.z*v0.y); //z²
		return out;
}
__device__ bool f_isLarge(const float4& v0, const float4& v1, const float4& v2){
			float4 edges[3];
			edges[0] = sub(v1,v0);
			edges[1] = sub(v2,v0);
			float area= chross(edges[0], edges[1]);

			return area > 10000;
}

__device__ float4 getVec(const float4 &base, int source){
				float4 out;
				out.x = __shfl_sync(0xFFFFFFFF, base.x, source);
				out.y = __shfl_sync(0xFFFFFFFF, base.y, source);
				out.z = __shfl_sync(0xFFFFFFFF, base.z, source);
				out.w = __shfl_sync(0xFFFFFFFF, base.w, source);

				// w is always 1
				return out;

}

////////////////////////////////////////////////





/**
 * The main procedure which rasterises all triangles on the framebuffer
 * @param transformedMesh         Transformed mesh object
 * @param frameBuffer             frame buffer for the rendered image
 * @param depthBuffer             depth buffer for every pixel on the image
 * @param width                   width of the image
 * @param height                  height of the image
 */
__device__ void rasteriseTriangle( float4 &v0, float4 &v1, float4 &v2,
                        GPUMesh &mesh,
                        unsigned int triangleIndex,
                        unsigned char* frameBuffer,
                        int* depthBuffer,
                        unsigned int const width,
                        unsigned int const height ) {

    // Compute the bounding box of the triangle.
    // Pixels that are intersecting with the triangle can only lie in this rectangle

	int minx = int(floorf(fminf(fminf(v0.x, v1.x), v2.x)));
	int maxx = int(ceilf(fmaxf(fmaxf(v0.x, v1.x), v2.x)));
	int miny = int(floorf(fminf(fminf(v0.y, v1.y), v2.y)));
	int maxy = int(ceilf(fmaxf(fmaxf(v0.y, v1.y), v2.y)));

	// Make sure the screen coordinates stay inside the window
    // This ensures parts of the triangle that are outside the
    // view of the camera are not drawn.
	minx = max(minx, (unsigned int) 0);
	maxx = min(maxx, width);
	miny = max(miny, (unsigned int) 0);
	maxy = min(maxy, height);

	////////////////////////////////////////////////
	// New code

	// For some reason, dividing the work into a grid is disasterous, while lines work fine.
	// So we do that to be safe


	// Debugging is quite hart here, since printf causes a silent crash.
	// It might have something to do qith limited register space or cache space(?)
	int blockSize = (maxx -minx)*(maxy-miny);
	bool isLarge = f_isLarge(v0,v1,v2);
	if(true &&isLarge){
		int x =0;
		x =   threadIdx.x ;


		// x += ((threadIdx.y-1)); // treadIdx-y has started to be in the range [8..15]
		// printf("g(%i, %i, %i), %i, (%i, %i)\n",
		// 				blockIdx.x, blockIdx.y, blockIdx.z,
		// 				triangleIndex,
		// 				x, blockDim.x
		// 			);


		// int y = threadIdx.x/8;
		// There were some overlap-issues, so we go for an overkill
		int Nminx = ( x   *(maxx - minx))/32  + minx;
		int Nmaxx = ((x+1)*(maxx - minx))/32 + minx;
		// int Nminy =  (    y * (maxy-miny))/4 + miny;
		// int Nmaxy =  ((1+ y)* (maxy-miny))/4 + miny;

		// miny = Nminy;
		// maxy = Nmaxy;
		maxx = x == 31 ? maxx : Nmaxx;
		minx = x ==0 ? minx : Nminx;

		// Forces the check to be inside the frame
	minx = max(minx, (unsigned int) 0);
	maxx = min(maxx, width);
	miny = max(miny, (unsigned int) 0);
	maxy = min(maxy, height);

	}




	iterateOverTriangle(			minx,
														miny,
														maxx,
														maxy,
														mesh,
														triangleIndex,
														v0,
														v1,
														v2,
														frameBuffer,
														depthBuffer,
														width);


	////////////////////////////////////////////////

// Removed the loop and made it a function

}





////////////////////////////////////////////////////////////////////
// New code

class NEW_GPUQueue{
	// Work queue on the GPU
public:

	float* scales;
	float* xList;
	float* yList;
	float* zList;
	unsigned long length;

   __device__  inline workItemGPU get(const int index) const{

		workItemGPU output;
		output.scale = scales[index];
		output.distanceOffset.x = xList[index];
		output.distanceOffset.y = yList[index];
		output.distanceOffset.z = zList[index];


		return output;

	}
	__host__   inline void set(const int &index, const workItemGPU &input ){
		// Implement this
		this-> scales[index]= input.scale;
		this-> xList[index]= input.distanceOffset.x;
		this-> yList[index]= input.distanceOffset.y;
		this-> zList[index]= input.distanceOffset.z;

	}

	NEW_GPUQueue(unsigned long totalLength){
		this-> length = totalLength;
		this-> scales = new float[totalLength];
		this-> xList  = new float[totalLength];
		this-> yList  = new float[totalLength];
		this-> zList  = new float[totalLength];


	}
	NEW_GPUQueue(){
		// Invalid dummy-queue
		this-> length = 0;
		this-> scales = nullptr;
		this-> xList  = nullptr;
		this-> yList  = nullptr;
		this-> zList  = nullptr;
	}
	NEW_GPUQueue(const NEW_GPUQueue &blueprint){
		this-> length =   blueprint.length;
		this-> scales =   blueprint.scales;
		this-> xList  =  blueprint.xList;
		this-> yList  =  blueprint.yList;
		this-> zList  =  blueprint.zList;


	}



};










////////////////////////////////////////////////////////////////////



__global__ void renderMeshes(
        unsigned long totalItemsToRender,
        NEW_GPUQueue workQueue,
        GPUMesh* meshes,
        unsigned int meshCount,
        unsigned int width,
        unsigned int height,
        unsigned char* frameBuffer,
        int* depthBuffer
) {
	unsigned int item = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned int triangleIndex = blockIdx.y * blockDim.y + threadIdx.y;
	unsigned int meshIndex = blockIdx.z;
	float4 v0,v1,v2;
	int isLarge = false;
	workItemGPU objectToRender;
	int Mask;
	if(item >= totalItemsToRender || meshIndex >= meshCount || triangleIndex >= meshes[meshIndex].vertexCount / 3) {

		// isLarge = false;

	}
	else{
    //for(unsigned int item = 0; item < totalItemsToRender; item++) {
	//for (unsigned int meshIndex = 0; meshIndex < meshCount; meshIndex++) {
    //for(unsigned int triangleIndex = 0; triangleIndex < meshes[meshIndex].vertexCount / 3; triangleIndex++) {

	objectToRender = workQueue.get(item);



	v0 = meshes[meshIndex].vertices[triangleIndex * 3 + 0];
	v1 = meshes[meshIndex].vertices[triangleIndex * 3 + 1];
	v2 = meshes[meshIndex].vertices[triangleIndex * 3 + 2];

	runVertexShader(v0, objectToRender.distanceOffset, objectToRender.scale, width, height);
	runVertexShader(v1, objectToRender.distanceOffset, objectToRender.scale, width, height);
	runVertexShader(v2, objectToRender.distanceOffset, objectToRender.scale, width, height);
	isLarge = f_isLarge(v0,v1,v2);
	Mask = __ballot_sync(0xFFFFFFFF, isLarge); // The mask is a queue.
	if( !isLarge ){ // Do the easy work
		rasteriseTriangle(v0, v1, v2, meshes[meshIndex], triangleIndex, frameBuffer, depthBuffer, width, height);
	}
	}


// Because everything else crashes, we now share the information form the
//difficult triangles, and
unsigned int PopC =__popc(Mask);
for(unsigned int i =0 ; i< PopC; ++i){
		//Everyone has the same triangle index, but in different items
		int first =  __ffs(Mask)-1;
		Mask -=(1<< (first)); // Deletes the bit

		// Share the most important data.
		int firstTrig= __shfl_sync(0xFFFFFFFF ,  triangleIndex, first);
		int firstItem= __shfl_sync(0xFFFFFFFF ,  item, first);
		int firstMesh = __shfl_sync(0xFFFFFFFF ,  meshIndex, first);


// Others do the work


	// I can not risk shuffeling, since it might break everything, but I'd
	// try to thansfer this information more elegantly (Preferably with a generic function)
	// So that I'd never have to look at it again.
																	// template <typename T>
																	// T share(T in, source){
																	//  T out;
																	//	int* ptrTo = (int*)&out;
																	//	int* ptrFrom = (int*)&in;
																	//	for(int i =0; i< (sizeof(T)+sizeof(int) -1)/sizeof(int) ; ++i){
																	//		*ptrOut = __shfl_sync(00xFFFFFFFF, *ptrIn, source);
																	//		++ptrOut;
																	//		++ptrin;
																	//		}
																	//	return out;
																	// }


// Now we do the exact same thing again, but with the shared variables instead
	workItemGPU objectToRender = workQueue.get(firstItem);


	// I suspect a slilent stack overflow or something. It we try to shuffle the vector everything crashes
	// We will insead abuse that we are acting on the same memory to read in a single cache-line

	// I want to run:
	// float4 Hv0 = getVec( v0, first);
	// float4 Hv1 = getVec( v1, first);
	// float4 Hv2 = getVec( v2, first);

	// But I must run: This causes extra latency from the memory
	float4 Hv0 = meshes[firstMesh].vertices[firstTrig * 3 + 0];
	float4 Hv1 = meshes[firstMesh].vertices[firstTrig * 3 + 1];
	float4 Hv2 = meshes[firstMesh].vertices[firstTrig * 3 + 2];


	runVertexShader(Hv0, objectToRender.distanceOffset, objectToRender.scale, width, height);
	runVertexShader(Hv1, objectToRender.distanceOffset, objectToRender.scale, width, height);
	runVertexShader(Hv2, objectToRender.distanceOffset, objectToRender.scale, width, height);

	// Because it works in this specific case, we put the functionality for
	// sharing large triangles inside the rasteriseTriangle, instead of outside it,
	// like reasonable people


	rasteriseTriangle(Hv0, Hv1, Hv2, meshes[firstMesh], firstTrig, frameBuffer, depthBuffer, width, height);



}

}



void fillWorkQueue(
        NEW_GPUQueue workQueue,
        float largestBoundingBoxSide,
        int depthLimit,
        unsigned long* nextIndexInQueue,
        float scale = 1.0,
        float3 distanceOffset = {0, 0, 0}) {

    // Queue a work item at the current scale and location
    workQueue.set(*nextIndexInQueue, {scale, distanceOffset});
    (*nextIndexInQueue)++;

    // Check whether we've reached the recursive depth of the fractal we want to reach
    depthLimit--;
    if(depthLimit == 0) {
        return;
    }

    // Now we recursively draw the meshes in a smaller size
    for(int offsetX = -1; offsetX <= 1; offsetX++) {
        for(int offsetY = -1; offsetY <= 1; offsetY++) {
            for(int offsetZ = -1; offsetZ <= 1; offsetZ++) {
                float3 offset = make_float3(offsetX,offsetY,offsetZ);
                // We draw the new objects in a grid around the "main" one.
                // We thus skip the location of the object itself.
                if(offsetX == 0 && offsetY == 0 && offsetZ == 0) {
                    continue;
                }

                float smallerScale = scale / 3.0f;
                float3 displacedOffset = make_float3(
                        distanceOffset.x + offset.x * (largestBoundingBoxSide / 2.0f) * scale,
                        distanceOffset.y + offset.y * (largestBoundingBoxSide / 2.0f) * scale,
                        distanceOffset.z + offset.z * (largestBoundingBoxSide / 2.0f) * scale
                );

                fillWorkQueue(workQueue, largestBoundingBoxSide, depthLimit, nextIndexInQueue, smallerScale, displacedOffset);
            }
        }
    }

}

__global__ void initialiseFramebuffer(unsigned char* frameBuffer, int width, int height) {
	unsigned int threadIndex = blockDim.x * blockIdx.x + threadIdx.x;

	if(threadIndex >= 4 * width * height) {
		return;
	}

		frameBuffer[threadIndex] = 255* (threadIndex % 4 == 3);

}

__global__ void initialiseDepthBuffer(int* depthBuffer, int width, int height) {
	unsigned int threadIndex = blockIdx.x * blockDim.x + threadIdx.x;

	if(threadIndex >= width * height) {
		return;
	}

	depthBuffer[threadIndex] = 16777216;
}

// This function kicks off the rasterisation process.
std::vector<unsigned char> rasteriseGPU(std::string inputFile, unsigned int width, unsigned int height, unsigned int depthLimit) {
    std::cout << "Rendering an image on the GPU.." << std::endl;
    std::cout << "Loading '" << inputFile << "' file... " << std::endl;

    std::vector<GPUMesh> meshes = loadWavefrontGPU(inputFile, false);

    // We first need to allocate some buffers.
    // The framebuffer contains the image being rendered.
    unsigned char* frameBuffer = new unsigned char[width * height * 4];
    // The depth buffer is used to make sure that objects closer to the camera occlude/obscure objects that are behind it
    for (unsigned int i = 0; i < (4 * width * height); i+=4) {
		frameBuffer[i + 0] = 0;
		frameBuffer[i + 1] = 0;
		frameBuffer[i + 2] = 0;
		frameBuffer[i + 3] = 255;
	}

	int* depthBuffer = new int[width * height];
	for(unsigned int i = 0; i < width * height; i++) {
    	depthBuffer[i] = 1;
    }

    float3 boundingBoxMin = make_float3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    float3 boundingBoxMax = make_float3(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

    std::cout << "Rendering image... " << std::endl;

    for(unsigned int i = 0; i < meshes.size(); i++) {
        for(unsigned int vertex = 0; vertex < meshes.at(i).vertexCount; vertex++) {
            boundingBoxMin.x = std::min(boundingBoxMin.x, meshes.at(i).vertices[vertex].x);
            boundingBoxMin.y = std::min(boundingBoxMin.y, meshes.at(i).vertices[vertex].y);
            boundingBoxMin.z = std::min(boundingBoxMin.z, meshes.at(i).vertices[vertex].z);

            boundingBoxMax.x = std::max(boundingBoxMax.x, meshes.at(i).vertices[vertex].x);
            boundingBoxMax.y = std::max(boundingBoxMax.y, meshes.at(i).vertices[vertex].y);
            boundingBoxMax.z = std::max(boundingBoxMax.z, meshes.at(i).vertices[vertex].z);
        }
    }

    float3 boundingBoxDimensions = make_float3(
            boundingBoxMax.x - boundingBoxMin.x,
            boundingBoxMax.y - boundingBoxMin.y,
            boundingBoxMax.z - boundingBoxMin.z);
    float largestBoundingBoxSide = std::max(std::max(boundingBoxDimensions.x, boundingBoxDimensions.y), boundingBoxDimensions.z);


    unsigned char* device_frameBuffer;
    int* device_depthBuffer;

    checkCudaErrors(cudaMalloc(&device_frameBuffer, width * height * 4 * sizeof(unsigned char)));
    checkCudaErrors(cudaMalloc(&device_depthBuffer, width * height * sizeof(int)));

    const unsigned int initialisationBlockSize = 256;

    unsigned int blockCountFrameBuffer = ((width * height * 4) / initialisationBlockSize) + 1;
    initialiseFramebuffer<<<blockCountFrameBuffer, initialisationBlockSize>>>(device_frameBuffer, width, height);

    unsigned int blockCountDepthBuffer = ((width * height) / initialisationBlockSize) + 1;
    initialiseDepthBuffer<<<blockCountDepthBuffer, initialisationBlockSize>>>(device_depthBuffer, width, height);

    checkCudaErrors(cudaDeviceSynchronize());



    // Each recursion level splits up the lowest level nodes into 28 smaller ones.
    // This regularity means we can calculate the total number of objects we need to render
    // which we can of course preallocate
    unsigned long totalItemsToRender = 0;
    for(unsigned long level = 0; level < depthLimit; level++) {
        totalItemsToRender += std::pow(26ul, level);
    }

		////////////////////////
		// Cnanged code
		NEW_GPUQueue workQueue(totalItemsToRender); //! (changed)
    // workItemGPU* workQueue = new workItemGPU[totalItemsToRender];

		////////////////////////

    std::cout << "Number of items to be rendered: " << totalItemsToRender << std::endl;

    unsigned long counter = 0;

    fillWorkQueue(workQueue, largestBoundingBoxSide, depthLimit, &counter);


		////////////////////////
		// Old code
		// unsigned long workQueueSizeBytes = totalItemsToRender * sizeof(workItemGPU);

    //workItemGPU* device_workQueue;


    // checkCudaErrors(cudaMalloc(&device_workQueue, workQueueSizeBytes));
    // checkCudaErrors(cudaMemcpy(device_workQueue, workQueue, workQueueSizeBytes, cudaMemcpyHostToDevice));
		/////////////////////////////
		/////////////////////////////////////////////
		// NEW CODE
		NEW_GPUQueue device_workQueue; // No argumetns makes an empty dummy (changed)
		device_workQueue.length = totalItemsToRender;// Just in case
		int listSize = sizeof(float)* totalItemsToRender;// Same for whole queue
		checkCudaErrors(cudaMalloc( &(device_workQueue.scales),  listSize));
		checkCudaErrors(cudaMalloc( &(device_workQueue.xList) ,  listSize));
		checkCudaErrors(cudaMalloc( &(device_workQueue.yList) ,  listSize));
		checkCudaErrors(cudaMalloc( &(device_workQueue.zList) ,  listSize));


    checkCudaErrors(cudaMemcpy(device_workQueue.scales, workQueue.scales, listSize, cudaMemcpyHostToDevice));
    checkCudaErrors(cudaMemcpy(device_workQueue.xList , workQueue.xList , listSize, cudaMemcpyHostToDevice));
		checkCudaErrors(cudaMemcpy(device_workQueue.yList , workQueue.yList , listSize, cudaMemcpyHostToDevice));
		checkCudaErrors(cudaMemcpy(device_workQueue.zList , workQueue.zList , listSize, cudaMemcpyHostToDevice));

		/////////////////////////////////////////////

	std::vector<GPUMesh> host_meshArray(meshes.begin(), meshes.end());
	for(int i = 0; i < meshes.size(); i++) {
		size_t vertexBufferSize = meshes.at(i).vertexCount * sizeof(float4);
		size_t normalBufferSize = meshes.at(i).vertexCount * sizeof(float3);

		checkCudaErrors(cudaMalloc(&host_meshArray.at(i).vertices, vertexBufferSize));
		checkCudaErrors(cudaMalloc(&host_meshArray.at(i).normals, normalBufferSize));

		checkCudaErrors(cudaMemcpy(host_meshArray.at(i).vertices, meshes.at(i).vertices, vertexBufferSize, cudaMemcpyHostToDevice));
		checkCudaErrors(cudaMemcpy(host_meshArray.at(i).normals, meshes.at(i).normals, normalBufferSize, cudaMemcpyHostToDevice));
	}

	// Block x axis: Job queue
	// Block y axis: max vertex count
	// Block z axis: meshCount

	const unsigned int threadsPerWorkQueueBlock = 32;

	const unsigned int threadsPerVertexBlock = 3;
	GPUMesh* device_meshArray;
	checkCudaErrors(cudaMalloc(&device_meshArray, meshes.size() * sizeof(GPUMesh)));
	checkCudaErrors(cudaMemcpy(device_meshArray, host_meshArray.data(), meshes.size() * sizeof(GPUMesh), cudaMemcpyHostToDevice));

	unsigned long maxMeshSize = 0;
	for(int i = 0; i < meshes.size(); i++) {
		maxMeshSize = std::max(maxMeshSize, meshes.at(i).vertexCount);
	}



	int jobQueueBlockCount = (totalItemsToRender / threadsPerWorkQueueBlock) + 1;
	int vertexBlockCount = (maxMeshSize / threadsPerVertexBlock) + 1;

	dim3 gridDimensions(  jobQueueBlockCount, vertexBlockCount, meshes.size());
	dim3 blockDimensions( threadsPerWorkQueueBlock, threadsPerVertexBlock, 1);
	timer.start();
	renderMeshes<<<gridDimensions, blockDimensions>>>(
		totalItemsToRender, device_workQueue,
		device_meshArray, meshes.size(),
		width, height, device_frameBuffer, device_depthBuffer);

	checkCudaErrors(cudaDeviceSynchronize());
	timer.stop();
    std::cout << "Finished!" << std::endl;

    // Copy the output picture into a vector so that the image dump code is happy :)
    std::vector<unsigned char> outputFramebuffer(frameBuffer, frameBuffer + (width * height * 4));

   	checkCudaErrors(cudaMemcpy(outputFramebuffer.data(), device_frameBuffer, width * height * 4 * sizeof(unsigned char), cudaMemcpyDeviceToHost));
   	cudaDeviceReset();
		// TODO Free everything at the end
    return outputFramebuffer;
}
