#include <iostream>
#include <vector>
using namespace std;

class OBJ {
protected:
    vector<vector<float>> verts;
    vector<vector<int>> faces;
public:
    bool readFile(string fileName);
    bool writeObj(string fileName);
    vector<float> getVert(int index);
    vector<int> getFace(int index);
    size_t faceCount();
};