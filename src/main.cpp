#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include "C:\Users\Jackson\Desktop\Projects\myRenderer\include\ppm.h"
#include "C:\Users\Jackson\Desktop\Projects\myRenderer\include\obj.h"
using namespace std;

// function to draw line between two points
void line(int x0, int y0, int x1, int y1, string r, string g, string bl, PPM& image) {
    // bresenham's algorithm
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1; // step in x direction
    int sy = (y0 < y1) ? 1 : -1; // step in y direction
    int err = dx - dy; // error value

    while (true) {
        image.edit_pixel(x0, y0, r, g, bl); // set pixel

        if (x0 == x1 && y0 == y1) break; // exit condition

        int err2 = err * 2;
        if (err2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (err2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

int main(int argc, char** argv) {
    // test for ppm read and write
    /*
    if (argc != 2) {
        cerr << "missing input file: ./run <filename>" << endl;
    }
    PPM image;
    image.read_ppm(argv[1]);
    image.write_ppm("C:\\Users\\Jackson\\Desktop\\Projects\\myRenderer\\output\\out.ppm");
    */
   // test for pixel edit
   /*
   PPM image;
   image.create_ppm(5, 5, 255);
   image.edit_pixel(4, 2, "255", "0", "0");
   image.write_ppm("C:\\Users\\Jackson\\Desktop\\Projects\\myRenderer\\output\\out.ppm");
   */
   // draw red triangle on black ppm file
   /*
   PPM image;
   image.create_ppm(1000, 1000, 255);
   line(499, 0, 999, 999, "255", "0", "0", image);
   line(999, 999, 0, 999, "255", "0", "0", image);
   line(0, 999, 499, 0, "255", "0", "0", image);
   image.write_ppm("C:\\Users\\Jackson\\Desktop\\Projects\\myRenderer\\output\\out.ppm");
   */
   // test obj read
   /*
    OBJ input;
    input.readFile("C:\\Users\\Jackson\\Desktop\\Projects\\myRenderer\\obj\\african_head.obj");
    input.writeObj("C:\\Users\\Jackson\\Desktop\\Projects\\myRenderer\\output\\out.obj");
    */
    // implement wire frame ppm from obj input
    // declare
    PPM image;
    OBJ input;
    vector<float> vert;
    vector<float> vert2;
    vector<int> face;
    int x0, y0, x1, y1;
    int width, height;
    // specify height and width
    width = 1000;
    height = 1000;
    // get input file and read
    if (argc != 2) {
        cerr << "missing input file: ./run <filename.obj>" << endl;
    }
    input.readFile(argv[1]);
    // create ppm file
    image.create_ppm(width, height, 255);
    // draw faces
    for (size_t i = 0; i < input.faceCount(); i++) {
        face = input.getFace(i);
        for (size_t j = 0; j < face.size(); j++) {
            // get cords for v0 and v1
            vert = input.getVert(face[j] - 1);
            if (j == (face.size() - 1)) {
                vert2 = input.getVert(face[0] - 1);
            } else {
                vert2 = input.getVert(face[j + 1] - 1);
            }
            // transform to 3D space
            x0 = (vert[0] + 1) * (width / 2);
            y0 = (vert[1] + 1) * (height / 2);
            x1 = (vert2[0] + 1) * (width / 2);
            y1 = (vert2[1] + 1) * (height / 2);
            if (!(x0 == 0)) {
                x0 -= 1;
            } 
            if (!(y0 == 0)) {
                y0 -= 1;
            } 
            if (!(x1 == 0)) {
                x1 -= 1;
            } 
            if (!(y1 == 0)) {
                y1 -= 1;
            }
            //cout << x0 << " " << y0 << " " << x1 << " " << y1 << endl;
            // Check if coordinates are within image bounds and draw line
            if (x0 >= 0 && x0 < width && y0 >= 0 && y0 < height && x1 >= 0 && x1 < width && y1 >= 0 && y1 < height) {
                line(x0, y0, x1, y1, "255", "255", "255", image);
            } else {
                cerr << "Warning: Coordinates out of bounds" << endl;
                cerr << vert[0] << " " << vert[1] << " " << vert2[0] << " " << vert2[1] << endl;
                cerr << x0 << " " << y0 << " " << x1 << " " << y1 << endl;
            }
        }
    }
    // write ppm file
    image.write_ppm("C:\\Users\\Jackson\\Desktop\\Projects\\myRenderer\\output\\out.ppm");
}