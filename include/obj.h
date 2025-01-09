#include <iostream>
#include <vector>
using namespace std;

class OBJ {
protected:
    vector<vector<float>> verts;
    vector<vector<float>> tVerts;
    vector<vector<int>> faces;
    vector<vector<int>> textures;
    vector<vector<float>> zBuffer;
    float maxX = 0;
    float maxY = 0;
    float minX = 0;
    float minY = 0;
    float minZ = 0;
    float maxZ = 0;
public:
    bool readFile(string fileName);
    bool writeObj(string fileName);
    vector<float> getVert(int index);
    vector<float> getTVert(int index);
    vector<int> getFace(int index);
    vector<int> getTexture(int index);
    size_t faceCount();
    float getMaxX();
    float getMaxY();
    float getMinX();
    float getMinY();
    float getMaxZ();
    float getMinZ();
    vector<vector<int>> getAllFaces();
    void editZBuffer(int x, int y, float z);
    void initZBuffer(int r, int c);
    float getZBuffer(int x, int y);
};