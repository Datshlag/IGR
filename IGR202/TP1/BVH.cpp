#include "BVH.h"

unsigned int BVH::max_density = 20;

BVH::~BVH() {

    delete leftChild;
    delete rightChild;
}

//We recursively build the bvh by splitting the current bvh into two new bvh. This constructor is used for the first call only.
BVH::BVH(const Mesh &_mesh): mesh(_mesh) {

    //Very important to use a reference here, otherwise we would copy all the data !
    const vector<Vec3f>& positions = mesh.positions();

    //All triangles belong to first BHV
    vector<int> v(mesh.triangles().size());
    for(unsigned int i = 0; i < v.size(); i++) {

        v[i]=i;
    }
    indexes = v;

    //We compute max and min corners and create first bbox and meanPos for first split
    float minX, minY, minZ;
    minX = minY = minZ = FLT_MAX;
    float maxX, maxY, maxZ;
    maxX = maxY = maxZ = FLT_MIN;

    Vec3f currPos;
    Vec3f meanPos = Vec3f(0, 0, 0);
    float posX, posY, posZ;

    for(unsigned int i = 0; i < positions.size(); i++) {

        currPos = positions.at(i);
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

    meanPos *= (float) 1.0/positions.size();

    bbox = Bbox(Vec3f(minX, minY, minZ), Vec3f(maxX, maxY, maxZ), meanPos);

    vector<int> subIndexes1 (0);
    vector<int> subIndexes2 (0);

    Bbox subBbox1;
    Bbox subBbox2;

    //Splitting the indexes in two
    split(subIndexes1, subIndexes2, subBbox1, subBbox2);

    //Creating the two childs.
    leftChild = new BVH(_mesh, subIndexes1, subBbox1);
    rightChild = new BVH(_mesh, subIndexes2, subBbox2);

    //Emptying the indexes since we won't need them here.
    indexes = vector<int> ();
}

//We recursively build the bvh by splitting the current bvh into two new bvh. This constructor is used for every call after the first one.
//Because we add as a parameter the indexes of the triangle in the father.
BVH::BVH(const Mesh &_mesh, const vector<int> &_indexes, const Bbox &_bbox): mesh(_mesh), indexes(_indexes), bbox(_bbox), leftChild(NULL), rightChild(NULL) {

    if (indexes.size() > max_density) {

        vector<int> subIndexes1 (0);
        vector<int> subIndexes2 (0);
        Bbox subBbox1;
        Bbox subBbox2;

        split(subIndexes1, subIndexes2, subBbox1, subBbox2);

        leftChild = new BVH(_mesh, subIndexes1, subBbox1);
        rightChild = new BVH(_mesh, subIndexes2, subBbox2);

        indexes = vector<int> ();
    }
}

//This method is used to split the current set of indexes into two distinct sets.
void BVH::split (vector<int> &subIndexes1, 
            vector<int> &subIndexes2,
            Bbox &subBbox1,
            Bbox &subBbox2) const {

    //We choose which dimension to split
    float xDim, yDim, zDim;
    int split;
    xDim = bbox.maxCorner[0] - bbox.minCorner[0];
    yDim = bbox.maxCorner[1] - bbox.minCorner[1];
    zDim = bbox.maxCorner[2] - bbox.minCorner[2];

    if ((xDim >= yDim) && (xDim >= zDim)) split = 0;
    else if (yDim >= zDim) split = 1;
    else split = 2;

    //We then divide the set of triangles between the two subsets. 
    //In the meantime we compute the new mean positions, and the bounding volumes of the children.
    float minX1, minY1, minZ1;
    minX1 = minY1 = minZ1 = FLT_MAX;
    float maxX1, maxY1, maxZ1;
    maxX1 = maxY1 = maxZ1 = - FLT_MAX;

    float minX2, minY2, minZ2;
    minX2 = minY2 = minZ2 = FLT_MAX;
    float maxX2, maxY2, maxZ2;
    maxX2 = maxY2 = maxZ2 = - FLT_MAX;

    Vec3f meanPos1, meanPos2;
    meanPos1 = meanPos2 = Vec3f(0, 0, 0);

    //Using references to prevent any expensive copy.
    const vector<Triangle> &triangles = mesh.triangles();
    const vector<Vec3f> &positions = mesh.positions();

    Triangle currTri;
    Vec3f V0, V1, V2, bar;
    float posX, posY, posZ;

    Vec3f meanPos = bbox.meanPos;

    for (unsigned int i = 0; i < indexes.size(); i++) {

        currTri = triangles[indexes[i]];
        V0 = positions.at(currTri[0]);
        V1 = positions.at(currTri[1]);
        V2 = positions.at(currTri[2]);
        bar = (V0 + V1 + V2) * (float)1.0/3.0;

        if (bar[split] >= meanPos[split]) {

            //Adding to the subset index list
            subIndexes1.push_back(indexes[i]);
            meanPos1 += bar;

            //Looking for bbox bounds
            posX = V0[0];
            posY = V0[1];
            posZ = V0[2];

            if(posX < minX1) minX1 = posX;
            if(posY < minY1) minY1 = posY;
            if(posZ < minZ1) minZ1 = posZ;

            if(posX > maxX1) maxX1 = posX;
            if(posY > maxY1) maxY1 = posY;
            if(posZ > maxZ1) maxZ1 = posZ;

            posX = V1[0];
            posY = V1[1];
            posZ = V1[2];

            if(posX < minX1) minX1 = posX;
            if(posY < minY1) minY1 = posY;
            if(posZ < minZ1) minZ1 = posZ;

            if(posX > maxX1) maxX1 = posX;
            if(posY > maxY1) maxY1 = posY;
            if(posZ > maxZ1) maxZ1 = posZ;

            posX = V2[0];
            posY = V2[1];
            posZ = V2[2];

            if(posX < minX1) minX1 = posX;
            if(posY < minY1) minY1 = posY;
            if(posZ < minZ1) minZ1 = posZ;

            if(posX > maxX1) maxX1 = posX;
            if(posY > maxY1) maxY1 = posY;
            if(posZ > maxZ1) maxZ1 = posZ;
        }

        else {

            //Adding to the subset index list
            subIndexes2.push_back(indexes[i]);
            meanPos2 += bar;

            //Looking for bbox bounds
            posX = V0[0];
            posY = V0[1];
            posZ = V0[2];

            if(posX < minX2) minX2 = posX;
            if(posY < minY2) minY2 = posY;
            if(posZ < minZ2) minZ2 = posZ;

            if(posX > maxX2) maxX2 = posX;
            if(posY > maxY2) maxY2 = posY;
            if(posZ > maxZ2) maxZ2 = posZ;

            posX = V1[0];
            posY = V1[1];
            posZ = V1[2];

            if(posX < minX2) minX2 = posX;
            if(posY < minY2) minY2 = posY;
            if(posZ < minZ2) minZ2 = posZ;

            if(posX > maxX2) maxX2 = posX;
            if(posY > maxY2) maxY2 = posY;
            if(posZ > maxZ2) maxZ2 = posZ;

            posX = V2[0];
            posY = V2[1];
            posZ = V2[2];

            if(posX < minX2) minX2 = posX;
            if(posY < minY2) minY2 = posY;
            if(posZ < minZ2) minZ2 = posZ;

            if(posX > maxX2) maxX2 = posX;
            if(posY > maxY2) maxY2 = posY;
            if(posZ > maxZ2) maxZ2 = posZ;
        }
    }

    meanPos1 *= (float)1.0/subIndexes1.size();
    meanPos2 *= (float)1.0/subIndexes2.size();

    subBbox1 = Bbox(Vec3f(minX1, minY1, minZ1), Vec3f(maxX1, maxY1, maxZ1), meanPos1);
    subBbox2 = Bbox(Vec3f(minX2, minY2, minZ2), Vec3f(maxX2, maxY2, maxZ2), meanPos2);
}

//Called to get the positions of the lines of the different bounding volumes.
vector<float> BVH::getLinesPositions() {

    vector<float> linePos = vector<float> ();
    //We use a default depth of 10.
    computeLinesPositions(linePos, 10);
    return linePos;
}

//Called recursively to compute the lines of the bounding boxes of the hierarchy.
void BVH::computeLinesPositions (vector<float> &linePos, unsigned int depth) {

    if(depth == 0) return;
    // 4 segments for x coord
    // First

    linePos.push_back(bbox.minCorner[0]);
    linePos.push_back(bbox.minCorner[1]);
    linePos.push_back(bbox.minCorner[2]);

    linePos.push_back(bbox.maxCorner[0]);
    linePos.push_back(bbox.minCorner[1]);
    linePos.push_back(bbox.minCorner[2]);

    // Second
    linePos.push_back(bbox.minCorner[0]);
    linePos.push_back(bbox.maxCorner[1]);
    linePos.push_back(bbox.minCorner[2]);

    linePos.push_back(bbox.maxCorner[0]);
    linePos.push_back(bbox.maxCorner[1]);
    linePos.push_back(bbox.minCorner[2]);

    // Third
    linePos.push_back(bbox.minCorner[0]);
    linePos.push_back(bbox.minCorner[1]);
    linePos.push_back(bbox.maxCorner[2]);

    linePos.push_back(bbox.maxCorner[0]);
    linePos.push_back(bbox.minCorner[1]);
    linePos.push_back(bbox.maxCorner[2]);

    // Fourth
    linePos.push_back(bbox.minCorner[0]);
    linePos.push_back(bbox.maxCorner[1]);
    linePos.push_back(bbox.maxCorner[2]);

    linePos.push_back(bbox.maxCorner[0]);
    linePos.push_back(bbox.maxCorner[1]);
    linePos.push_back(bbox.maxCorner[2]);



    // 4 segments for y coord
    // First
    linePos.push_back(bbox.minCorner[0]);
    linePos.push_back(bbox.minCorner[1]);
    linePos.push_back(bbox.minCorner[2]);

    linePos.push_back(bbox.minCorner[0]);
    linePos.push_back(bbox.maxCorner[1]);
    linePos.push_back(bbox.minCorner[2]);


    // Second
    linePos.push_back(bbox.maxCorner[0]);
    linePos.push_back(bbox.minCorner[1]);
    linePos.push_back(bbox.minCorner[2]);

    linePos.push_back(bbox.maxCorner[0]);
    linePos.push_back(bbox.maxCorner[1]);
    linePos.push_back(bbox.minCorner[2]);


    // Third
    linePos.push_back(bbox.minCorner[0]);
    linePos.push_back(bbox.minCorner[1]);
    linePos.push_back(bbox.maxCorner[2]);

    linePos.push_back(bbox.minCorner[0]);
    linePos.push_back(bbox.maxCorner[1]);
    linePos.push_back(bbox.maxCorner[2]);


    // Fourth
    linePos.push_back(bbox.maxCorner[0]);
    linePos.push_back(bbox.minCorner[1]);
    linePos.push_back(bbox.maxCorner[2]);

    linePos.push_back(bbox.maxCorner[0]);
    linePos.push_back(bbox.maxCorner[1]);
    linePos.push_back(bbox.maxCorner[2]);



    // 4 segments for z coord
    // First
    linePos.push_back(bbox.minCorner[0]);
    linePos.push_back(bbox.minCorner[1]);
    linePos.push_back(bbox.minCorner[2]);

    linePos.push_back(bbox.minCorner[0]);
    linePos.push_back(bbox.minCorner[1]);
    linePos.push_back(bbox.maxCorner[2]);

    // Second
    linePos.push_back(bbox.maxCorner[0]);
    linePos.push_back(bbox.minCorner[1]);
    linePos.push_back(bbox.minCorner[2]);

    linePos.push_back(bbox.maxCorner[0]);
    linePos.push_back(bbox.minCorner[1]);
    linePos.push_back(bbox.maxCorner[2]);

    // Third
    linePos.push_back(bbox.minCorner[0]);
    linePos.push_back(bbox.maxCorner[1]);
    linePos.push_back(bbox.minCorner[2]);

    linePos.push_back(bbox.minCorner[0]);
    linePos.push_back(bbox.maxCorner[1]);
    linePos.push_back(bbox.maxCorner[2]);

    // Fourth
    linePos.push_back(bbox.maxCorner[0]);
    linePos.push_back(bbox.maxCorner[1]);
    linePos.push_back(bbox.minCorner[2]);

    linePos.push_back(bbox.maxCorner[0]);
    linePos.push_back(bbox.maxCorner[1]);
    linePos.push_back(bbox.maxCorner[2]);


    // Call recursively for the children
    if(leftChild) leftChild->computeLinesPositions(linePos, depth - 1);
    if(rightChild) rightChild->computeLinesPositions(linePos, depth - 1);
}