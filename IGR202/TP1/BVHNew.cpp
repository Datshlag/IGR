#include "BVHNew.h"

using namespace BVHNew;

BVHNew(): mesh(NULL) { }
BVHNew(const Mesh &mesh): mesh(&mesh) {

  nb_node++;
  std::vector<Vec3f> positions = mesh->positions();

  //All triangles belong to first BHV
  std::vector<int> v(mesh.triangles().size());
  for(unsigned int i = 0; i < v.size(); i++) {

    v[i]=i;
  }
  indexes = v;

  //We compute max and min corners and create first bbox and meanPos for first split
  float minX, minY, minZ;
  minX = minY = minZ = FLT_MAX;
  float maxX, maxY, maxZ;
  maxX = maxY = maxZ = FLT_MIN;

  for(int i = 0; i < positions.size(); i++) {

    currPos = positions[i];
    posX = currPos[0];
    posY = currPos[1];
    posZ = currPos[2];

    if(posX < minX) minX = posX;
    if(posY < minY) minY = posY;
    if(posZ < minZ) minZ = posZ;
    if(posX > maxX) maxX = posX;
    if(posY > maxY) maxY = posY;
    if(posZ > maxZ) maxZ = posZ;

    meanPos += currPos;
  }

  Bbox2 bbox = Bbox2(Vec3f(minX, minY, minZ), Vec3f(maxX, maxY, maxZ));
  meanPos *= (float) 1.0/positions.size();

  //We work on the first split : we choose which dimension to split
  float xDim, yDim, zDim;
  int split;
  xDim = maxCorner[0] - minCorner[0];
  yDim = maxCorner[1] - minCorner[1];
  zDim = maxCorner[2] - minCorner[2];

  if((xDim >= yDim) && (xDim >= zDim)) split = 0;
  else if(yDim >= zDim) split = 1;
  else split = 2;

  //Notice that we don't have to recompute all max and mins, we can deduce them
  //from the split direction and old extremums


}
