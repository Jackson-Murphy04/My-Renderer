#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include "C:\Users\Jackson\Desktop\Projects\myRenderer\include\obj.h"
using namespace std;

bool OBJ::readFile(string fileName) {
    // declare
    string input;
    string test;
    float in;
    int next;
    char trash;
    vector<float> vec;
    vector<int> fec;
    vector<int> tex;
    vector<float> vTex;
    stringstream ss;
    stringstream s2;
    // open file
    ifstream fin;
    fin.open(fileName);
    // make sure opens
    if (!fin.is_open()) {
        cerr << "input file failed to open" << endl;
        return false;
    }
    // pull each line
    while(getline(fin, input)) {
        // put input line into string stream
        ss << input;
        // read first string from stream
        ss >> test;
        // if v or f add following vals to appropriate vectors
        if (test == "v") {
            while(ss >> in) {
                vec.push_back(in);
            }
            verts.push_back(vec);
            vec.clear();
        }
        if (test == "f") {
            while(ss >> test) {
                s2 << test;
                s2 >> next;
                fec.push_back(next);
                if (s2 >> trash) {
                    s2 >> next;
                    tex.push_back(next);
                }
                s2.str("");
                s2.clear();
            }
            faces.push_back(fec);
            textures.push_back(tex);
            fec.clear();
            tex.clear();
        }
        if (test == "vt") {
            while(ss >> in) {
                vTex.push_back(in);
            }
            tVerts.push_back(vTex);
            vTex.clear();
        }
        ss.str("");
        ss.clear();
    }
    // close file
    fin.close();
    // get max and min
    if (!verts.empty()) {
        for (size_t i = 0; i < verts.size(); i++) {
            if (verts[i].size() > 0) {
                if (verts[i][0] > maxX) {
                    maxX = verts[i][0];
                }
                if (verts[i][0] < minX) {
                    minX = verts[i][0];
                }
            }
            if (verts[i].size() > 1) {
                if (verts[i][1] > maxY) {
                    maxY = verts[i][1];
                }
                if (verts[i][1] < minY) {
                    minY = verts[i][1];
                }
            }
            if (verts[i].size() > 1) {
                if (verts[i][2] > maxZ) {
                    maxZ = verts[i][2];
                }
                if (verts[i][2] < minZ) {
                    minZ = verts[i][2];
                }
            }
        }
    }
    // return
    return true;
}

bool OBJ::writeObj(string fileName) {
    // open file stream
    ofstream fout;
    fout.open(fileName);
    // make sure opens
    if (!fout.is_open()) {
        cerr << "output file failed to open" << endl;
        return false;
    }
    // write verts
    for(size_t i = 0; i < verts.size(); i++) {
        fout << "v ";
        for(size_t j = 0; j < verts[i].size(); j++) {
            fout << verts[i][j] << " ";
        }
        fout << endl;
    }
    // write faces
    for(size_t i = 0; i < faces.size(); i++) {
        fout << "f ";
        for(size_t j = 0; j < faces[i].size(); j++) {
            fout << faces[i][j] << " ";
        }
        fout << endl;
    }
    // return
    return true;
}

vector<float> OBJ::getVert(int index) {
    return verts[index];
}

vector<float> OBJ::getTVert(int index) {
    return tVerts[index];
}

vector<int> OBJ::getFace(int index) {
    return faces[index];
}

vector<int> OBJ::getTexture(int index) {
    return textures[index];
}

size_t OBJ::faceCount() {
    return faces.size();
}

float OBJ::getMaxX() {
    return maxX;
}

float OBJ::getMaxY() {
    return maxY;
}

float OBJ::getMaxZ() {
    return maxZ;
}

float OBJ::getMinX() {
    return minX;
}

float OBJ::getMinY() {
    return minY;
}

float OBJ::getMinZ() {
    return minZ;
}

vector<vector<int>> OBJ::getAllFaces() {
    return faces;
}

void OBJ::editZBuffer(int x, int y, float z) {
    //zBuffer[index] = z;
    zBuffer[y][x] = z;
}

void OBJ::initZBuffer(int r, int c) {
    // init image vectors
    zBuffer.clear();
    zBuffer.resize(r);
    for (size_t i = 0; i < zBuffer.size(); i++) {
        zBuffer[i].resize(c, numeric_limits<float>::infinity());
    }
}

float OBJ::getZBuffer(int x, int y) {
    //return zBuffer[index];
    return zBuffer[y][x];
}