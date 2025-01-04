#include <iostream>
#include <vector>
using namespace std;

class OBJ {
protected:
    vector<vector<float>> verts;
    vector<vector<int>> faces;
    float maxX = 0;
    float maxY = 0;
    float minX = 0;
    float minY = 0;
public:
    bool readFile(string fileName);
    bool writeObj(string fileName);
    vector<float> getVert(int index);
    vector<int> getFace(int index);
    size_t faceCount();
    float getMaxX();
    float getMaxY();
    float getMinX();
    float getMinY();
};