#include <iostream>
#include <vector>
#include <fstream>
#include "C:\Users\Jackson\Desktop\Projects\myRenderer\include\ppm.h"
using namespace std;

bool PPM::read_ppm(string fileName) {
    // declare
    string pixel;
    int col = 0;
    int row = 0;
    // open file stream
    ifstream fin;
    fin.open(fileName);
    // make sure opens
    if (!fin.is_open()) {
        cerr << "input file failed to open" << endl;
        return false;
    }
    // error check header is p3
    fin >> pixel;
    if (pixel != "P3") {
        cerr << "wrong file header type" << endl;
        return false;
    }
    fin >> cols;
    fin >> rows;
    fin >> max;
    // init image vectors
    image.resize(rows);
    for (size_t i = 0; i < image.size(); i++) {
        image[i].resize(cols);
        for (size_t j = 0; j < image[i].size(); j++) {
            image[i][j].resize(3);
        }
    }
    // read
    while(fin >> pixel) {
        image[row][col][0] = pixel;
        fin >> pixel;
        image[row][col][1] = pixel;
        fin >> pixel;
        image[row][col][2] = pixel;
        col++;
        if(col == cols) {
            row++;
            col = 0;
        }
    }
    // close file
    fin.close();
    //retrun
    return true;
}

bool PPM::read_texture(string fileName) {
    // declare
    string pixel;
    int col = 0;
    int row = 0;
    // open file stream
    ifstream fin;
    fin.open(fileName);
    // make sure opens
    if (!fin.is_open()) {
        cerr << "input file failed to open" << endl;
        return false;
    }
    // error check header is p3
    fin >> pixel;
    if (pixel != "P3") {
        cerr << "wrong file header type" << endl;
        return false;
    }
    fin >> tCols;
    fin >> tRows;
    fin >> tMax;
    // init image vectors
    texture.resize(tRows);
    for (size_t i = 0; i < texture.size(); i++) {
        texture[i].resize(tCols);
        for (size_t j = 0; j < texture[i].size(); j++) {
            texture[i][j].resize(3);
        }
    }
    // read
    while(fin >> pixel) {
        texture[row][col][0] = pixel;
        fin >> pixel;
        texture[row][col][1] = pixel;
        fin >> pixel;
        texture[row][col][2] = pixel;
        col++;
        if(col == tCols) {
            row++;
            col = 0;
        }
    }
    
    // close file
    fin.close();
    //retrun
    return true;
}


bool PPM::write_ppm(string fileName) {
    // open file stream
    ofstream fout;
    fout.open(fileName);
    // make sure opens
    if (!fout.is_open()) {
        cerr << "output file failed to open" << endl;
        return false;
    }
    // write info
    fout << "P3" << endl;
    fout << cols << " " << rows << endl;
    fout << max << endl;
    // write pixels
    for (size_t i = 0; i < image.size(); i++) {
        for (size_t j = 0; j < image[i].size(); j++) {
            for (size_t k = 0; k < image[i][j].size(); k++) {
                fout << image[i][j][k];
                fout << " ";
            }
            fout << " ";
        }
        fout << endl;
    }
    // close file
    fout.close();
    // retrun
    return true;
}

bool PPM::create_ppm(int r, int c, int m) {
    rows = r;
    cols = c;
    max = m;
    // init image vectors
    image.clear();
    image.resize(rows);
    for (size_t i = 0; i < image.size(); i++) {
        image[i].resize(cols);
        for (size_t j = 0; j < image[i].size(); j++) {
            image[i][j].resize(3, "0");
        }
    }
    return true;
}

bool PPM::edit_pixel(int x, int y, string r, string g, string b) {
    image[y][x][0] = r;
    image[y][x][1] = g;
    image[y][x][2] = b;
    return true;
}

void PPM::test_output() {
    cout << rows << endl;
    cout << cols << endl;
    cout << max << endl;
    for (size_t i = 0; i < image.size(); i++) {
        for (size_t j = 0; j < image[i].size(); j++) {
            for (size_t k = 0; k < image[i][j].size(); k++) {
                cout << image[i][j][k];
            }
            cout << " ";
        }
        cout << endl;
    }
}

void PPM::vFlip() {
    // declare
    vector<vector<vector<string>>> og;
    vector<vector<string>> temp;
    vector<vector<vector<string>>> done;
    // flip ppm image vector vertically 
    og = image;
    for (int i = og.size() - 1; i >= 0; i--) {
        for (int j = og[i].size() - 1; j >= 0; j--) {
            temp.push_back(og[i][j]);
        }
        done.push_back(temp);
        temp.clear();
    }
    image = done;
}

int PPM::getCols() const { return cols; }

int PPM::getRows() const { return rows; }