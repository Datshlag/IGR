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

		//Indexes of the triangles in the bboxx
		std::vector<int> trianglesIndexes;

		//means of the position, and normals of the triangles in the bbox
		Vec3f meanNormal;
		Vec3f meanColor;
		Vec3f meanPos;

	public:
		Bbox() { }
		Bbox(const Mesh &mesh, std::vector<int> _trianglesIndexes): trianglesIndexes(_trianglesIndexes) {

			meanNormal = Vec3f(0.0,0.0,0.0);
			meanPos = Vec3f(0.0,0.0,0.0);

			std::vector<Triangle> triangles = mesh.triangles();
			std::vector<Vec3f> positions = mesh.positions();
			std::vector<Vec3f> normals = mesh.normals();

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

		const std::vector<Bbox> split(const Mesh &mesh) {

			int i = getLargestDim();


			std::vector<Triangle> triangles = mesh.triangles();
			std::vector<Vec3f> positions = mesh.positions();

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

		void setMinCorner(const Vec3f &_minCorner) { minCorner = _minCorner; }
		void setMaxCorner(const Vec3f &_maxCorner) { maxCorner = _maxCorner; }

		const Vec3f getMinCorner() { return minCorner; }
		const Vec3f getMaxCorner() { return maxCorner; }
		const Vec3f getMeanPos() { return meanPos; }
		const Vec3f getMeanNormal() { return meanNormal; }
		const int getDensity() { return trianglesIndexes.size(); }
		const std::vector<int> getTrianglesIndexes() { return trianglesIndexes; }

		const int getLargestDim() { 

			float xDim, yDim, zDim;
			xDim = maxCorner[0] - minCorner[0];
			yDim = maxCorner[1] - minCorner[1];
			zDim = maxCorner[2] - minCorner[2];

			if((xDim >= yDim) && (xDim >= zDim)) return 0;
			else if(yDim >= zDim) return 1;
			return 2;
		}

		const bool isEmpty() { return trianglesIndexes.empty(); }
		const void drawBbox(const Mesh &mesh, std::vector<float> &colors) {

			Vec3f randColor = Vec3f(rand()%255/255.0, rand()%255/255.0, rand()%255/255.0);
			std::vector<Triangle> triangles = mesh.triangles();
			std::vector<Vec3f> positions = mesh.positions();
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