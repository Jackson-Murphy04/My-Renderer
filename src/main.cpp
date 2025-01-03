#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include "C:\Users\Jackson\Desktop\Projects\myRenderer\include\ppm.h"
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

   // create image PPM image object
   PPM image;
   image.create_ppm(1000, 1000, 255);
   line(499, 0, 999, 999, "255", "0", "0", image);
   line(999, 999, 0, 999, "255", "0", "0", image);
   line(0, 999, 499, 0, "255", "0", "0", image);
   image.write_ppm("C:\\Users\\Jackson\\Desktop\\Projects\\myRenderer\\output\\out.ppm");
   // 

   // implement line drawing
}