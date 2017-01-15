#pragma once

#include "Vec3.h"
#include "Mesh.h"
#include "Triangle.h"
#include <cfloat>

class Bbox {

	private:
		//Corners of the bounding box
		Vec3f minCorner;
		Vec3f maxCorner;
		Vec3f bounds[2];

		//Indexes of the triangles in the bbox
		std::vector<int> trianglesIndexes;

		//Mesh 
		const Mesh *mesh;

		//means of the position, and normals of the triangles in the bbox
		Vec3f meanNormal;
		Vec3f meanColor;
		Vec3f meanPos;

	public:
		Bbox(): mesh(NULL) { }
		Bbox(const Mesh *_mesh, std::vector<int> _trianglesIndexes): trianglesIndexes(_trianglesIndexes), mesh(_mesh) {

			meanNormal = Vec3f(0.0,0.0,0.0);
			meanPos = Vec3f(0.0,0.0,0.0);

			std::vector<Triangle> triangles = mesh->triangles();
			std::vector<Vec3f> positions = mesh->positions();
			std::vector<Vec3f> normals = mesh->normals();

			float minX, minY, minZ;
			minX = minY = minZ = FLT_MAX;
			float maxX, maxY, maxZ;
			maxX = maxY = maxZ = FLT_MIN;

			Vec3f currPos;
			Triangle currTriangle;
			Vec3f currNormal;

			for(unsigned int j = 0; j < trianglesIndexes.size(); j++) {

				int i = trianglesIndexes[j];

				currTriangle = triangles[i];

				currPos = positions[currTriangle[0]];
				currNormal = normals[currTriangle[0]];
				if(currPos[0] < minX) minX = currPos[0];
				if(currPos[1] < minY) minY = currPos[1];
				if(currPos[2] < minZ) minZ = currPos[2];
				if(currPos[0] > maxX) maxX = currPos[0];
				if(currPos[1] > maxY) maxY = currPos[1];
				if(currPos[2] > maxZ) maxZ = currPos[2];
				meanPos += currPos * (float)1.0/3.0;
				meanNormal += currNormal * (float)1.0/3.0;

				currPos = positions[currTriangle[1]];
				currNormal = normals[currTriangle[1]];
				if(currPos[0] < minX) minX = currPos[0];
				if(currPos[1] < minY) minY = currPos[1];
				if(currPos[2] < minZ) minZ = currPos[2];
				if(currPos[0] > maxX) maxX = currPos[0];
				if(currPos[1] > maxY) maxY = currPos[1];
				if(currPos[2] > maxZ) maxZ = currPos[2];
				meanPos += currPos * (float)1.0/3.0;
				meanNormal += currNormal * (float)1.0/3.0;

				currPos = positions[currTriangle[2]];
				currNormal = normals[currTriangle[2]];
				if(currPos[0] < minX) minX = currPos[0];
				if(currPos[1] < minY) minY = currPos[1];
				if(currPos[2] < minZ) minZ = currPos[2];
				if(currPos[0] > maxX) maxX = currPos[0];
				if(currPos[1] > maxY) maxY = currPos[1];
				if(currPos[2] > maxZ) maxZ = currPos[2];
				meanPos += currPos * (float)1.0/3.0;
				meanNormal += currNormal * (float)1.0/3.0;

			}

			minCorner = Vec3f(minX, minY, minZ);
			maxCorner = Vec3f(maxX, maxY, maxZ);
			
			meanPos *= (float) 1.0/trianglesIndexes.size();
			meanNormal *= (float) 1.0/trianglesIndexes.size();

		}

		std::vector<Bbox> split() const {

			int i = getLargestDim();

			std::vector<Triangle> triangles = mesh->triangles();
			std::vector<Vec3f> positions = mesh->positions();

			Triangle currTriangle;

			std::vector<int> firstSonTrianglesList(0);
			std::vector<int> secondSonTrianglesList(0);

			Vec3f bar;
			bool splitOk = false;


			while(!splitOk) {

				for(unsigned int k = 0; k < trianglesIndexes.size(); k++) {

					int j = trianglesIndexes[k];
					currTriangle = triangles[j];
					bar = (positions[currTriangle[0]]+positions[currTriangle[1]]+positions[currTriangle[2]]) * (float)1.0/3.0;

					if(bar[i] >= meanPos[i]) { secondSonTrianglesList.push_back(j); }
					else { firstSonTrianglesList.push_back(j); }

				}

				if(!firstSonTrianglesList.empty()&&!secondSonTrianglesList.empty()) { splitOk = true; }
				else { i++; i = i%3; firstSonTrianglesList.clear(); secondSonTrianglesList.clear(); }// on suppose qu'il n'y a pas deux triangles semblables.
			}

			std::vector<Bbox> result(2);
			result[0] = Bbox(mesh, firstSonTrianglesList);
			result[1] = Bbox(mesh, secondSonTrianglesList);

			return result;
		}

		Vec3f getMinCorner() const { return minCorner; }
		Vec3f getMaxCorner() const { return maxCorner; }
		Vec3f getMeanPos() const { return meanPos; }
		Vec3f getMeanNormal() const { return meanNormal; }
		const Mesh* getMesh() const { return mesh; }
		int getDensity() const { return trianglesIndexes.size(); }
		std::vector<int> getTrianglesIndexes() const { return trianglesIndexes; }

		int getLargestDim() const { 

			float xDim, yDim, zDim;
			xDim = maxCorner[0] - minCorner[0];
			yDim = maxCorner[1] - minCorner[1];
			zDim = maxCorner[2] - minCorner[2];

			if((xDim >= yDim) && (xDim >= zDim)) return 0;
			else if(yDim >= zDim) return 1;
			return 2;
		}

		bool isEmpty() const { return trianglesIndexes.empty(); }
		void drawBbox(std::vector<float> &colors) const {

			Vec3f randColor = Vec3f(rand()%255/255.0, rand()%255/255.0, rand()%255/255.0);
			std::vector<Triangle> triangles = mesh->triangles();
			std::vector<Vec3f> positions = mesh->positions();
			Triangle currTri;

			for(unsigned int i = 0; i < trianglesIndexes.size(); i++) {

				int j = trianglesIndexes[i];
				currTri = triangles[j];
				colors[4 * currTri[0]] = randColor[0];
				colors[4 * currTri[0] + 1] = randColor[1];
				colors[4 * currTri[0] + 2] = randColor[2];

				colors[4 * currTri[1]] = randColor[0];
				colors[4 * currTri[1] + 1] = randColor[1];
				colors[4 * currTri[1] + 2] = randColor[2];

				colors[4 * currTri[2]] = randColor[0];
				colors[4 * currTri[2] + 1] = randColor[1];
				colors[4 * currTri[2] + 2] = randColor[2];

			}
		}

};