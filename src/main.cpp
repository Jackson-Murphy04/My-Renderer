#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <algorithm>
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

// Function to subtract two 3D vectors
vector<float> operator-(const vector<float>& a, const vector<float>& b) {
    return {a[0] - b[0], a[1] - b[1], a[2] - b[2]};
}

// Function to calculate the dot product of two 3D vectors
float operator*(const vector<float>& a, const vector<float>& b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

// Function to calculate the cross product of two 3D vectors
vector<float> cross_product(const vector<float>& a, const vector<float>& b) {
    return {
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0]
    };
}

// fill faces helpers
// calculate area
float triangle_area(vector<float> a, vector<float> b, vector<float> c) {
    return 0.5 * abs(a[0] * (b[1] - c[1]) + b[0] * (c[1] - a[1]) + c[0] * (a[1] - b[1]));
}
// Check if a point is inside the triangle formed by a, b, c
bool is_point_in_triangle(vector<float> p, vector<float> a, vector<float> b, vector<float> c) {
    float area_abc = triangle_area(a, b, c);
    float area_pbc = triangle_area(p, b, c);
    float area_pca = triangle_area(p, c, a);
    float area_pab = triangle_area(p, a, b);
    // Compute weights
    float alpha = area_pbc / area_abc;
    float beta = area_pca / area_abc;
    float gamma = area_pab / area_abc;
    // Check if the point is inside the triangle
    return (alpha >= 0 && beta >= 0 && gamma >= 0 && abs((alpha + beta + gamma)- 1) < .0001);
}
// Check if a vertex forms an ear with its neighbors
bool is_ear(vector<int> polygon, int i, OBJ& input) {
    // declare
    vector<float> pVert;
    vector<float> cVert;
    vector<float> nVert;
    // get cords
    int n = polygon.size();
    int prev = polygon[(i - 1 + n) % n];
    pVert = input.getVert(prev - 1);
    int curr = polygon[i];
    cVert = input.getVert(curr - 1);
    int next = polygon[(i + 1) % n];
    nVert = input.getVert(next - 1);
    // Check if no other vertex is inside the triangle
    for (int j = 0; j < n; j++) {
        if (j == (i - 1 + n) % n || j == i || j == (i + 1) % n) continue;
        if (is_point_in_triangle(input.getVert(polygon[j] - 1), pVert, cVert, nVert)) return false;
    }
    return true;
}

// barycentric helper functions
// calculate bary cords
bool barycentric_coords(vector<float> p, vector<float> a, vector<float> b, vector<float> c) {
    float area_abc = triangle_area(a, b, c);
    float area_pbc = triangle_area(p, b, c);
    float area_pca = triangle_area(p, c, a);
    float area_pab = triangle_area(p, a, b);
    // Compute weights
    float alpha = area_pbc / area_abc;
    float beta = area_pca / area_abc;
    float gamma = area_pab / area_abc;
    // Check if the point is inside the triangle
    return (alpha >= 0 && beta >= 0 && gamma >= 0 && abs((alpha + beta + gamma)- 1) < .08);
}

float barycentricZ(vector<float> p, vector<float> a, vector<float> b, vector<float> c, vector<float> z) {
    // declare
    float z1;
    // areas
    float area_abc = triangle_area(a, b, c);
    float area_pbc = triangle_area(p, b, c);
    float area_pca = triangle_area(p, c, a);
    float area_pab = triangle_area(p, a, b);
    // Compute weights
    float alpha = area_pbc / area_abc;
    float beta = area_pca / area_abc;
    float gamma = area_pab / area_abc;
    // compute z bary val
    z1 = alpha * z[0] + beta * z[1] + gamma * z[2];
    return z1;
}

// function to fill faces 
void fill(vector<int> face, int width, int height, string r, string g, string bl, PPM& image, OBJ& input) {
    // declare 
    float minX = .8;
    float minY = .8;
    float maxX = -.8;
    float maxY = -.8;
    float x, y, z;
    vector<float> vert;
    vector<vector<vector<float>>> triangles; // all triangles of face, all verts of triangle, vert 
    vector<vector<float>> temp;
    vector<float> a;
    vector<float> b;
    vector<float> c;
    vector<float> zV;
    // use ear clipping to triangulate polygon
    while (face.size() > 3) {
        bool ear_found = false;
        for (size_t i = 0; i < face.size(); i++) {
            if (is_ear(face, i, input)) {
                int prev = (i - 1 + face.size()) % face.size();
                int next = (i + 1) % face.size();
                // push the triangle formed by the vertices
                temp.push_back(input.getVert(face[prev] - 1)); 
                temp.push_back(input.getVert(face[i] - 1));   
                temp.push_back(input.getVert(face[next] - 1)); 
                triangles.push_back(temp);
                temp.clear();
                face.erase(face.begin() + i);
                ear_found = true;
                break;
            }
        }
        if (!ear_found) {
            cerr << "Error: Failed to find an ear!" << endl;
            break;
        }
    }
    if (face.size() == 3) {
        temp.push_back(input.getVert(face[0] - 1));
        temp.push_back(input.getVert(face[1] - 1));
        temp.push_back(input.getVert(face[2] - 1));
        triangles.push_back(temp);
        temp.clear();
    }
    // loop triangles 
    for (size_t j = 0; j < triangles.size(); j++) { 
        // Reset bounding box for each triangle
        minX = numeric_limits<float>::max();
        minY = numeric_limits<float>::max();
        maxX = numeric_limits<float>::lowest();
        maxY = numeric_limits<float>::lowest();

        // loop verts and update bounding box for face
        for (size_t i = 0; i < triangles[j].size(); i++) {
            // get vert and store to x and y
            vert = triangles[j][i];
            x = vert[0];
            y = vert[1];
            z = vert[2];
            // Find the larger dimension
            float maxDimension = max(input.getMaxX() - input.getMinX(), 
                                    input.getMaxY() - input.getMinY());

            // Calculate centers
            float centerX = (input.getMaxX() + input.getMinX()) / 2.0f;
            float centerY = (input.getMaxY() + input.getMinY()) / 2.0f;

            // normalize cords to (-.8,.8) grid while preserving aspect ratio and centering
            x = ((x - centerX) / maxDimension) * 2;
            y = ((y - centerY) / maxDimension) * 2;
            // transform to 2D space
            x = (x + 1) * (width / 2);
            y = (y + 1) * (height / 2);
            if (!(x == 0)) {
                x -= 1;
            } 
            if (!(y == 0)) {
                y -= 1;
            }
            // Check if coordinates are within image bounds
            if (!(x >= 0 && x < width && y >= 0 && y < height)) {
                cerr << "Warning: fil Coordinates out of bounds" << endl;
                cerr << vert[0] << " " << vert[1] << endl;
                cerr << x << " " << y << endl;
            }
            // get bounding box
            if (minX > x) {
                minX = x;
            }
            if (minY > y) {
                minY = y;
            }
            if (maxX < x) {
                maxX = x;
            }
            if (maxY < y) {
                maxY = y;
            }
            //set triangle cords to new cords
            triangles[j][i][0] = x;
            triangles[j][i][1] = y;
            triangles[j][i][2] = z;
        }
        // barycentric coordinates to see if pixel is in bounding box and color
        // triangles[j] is our triangle
        zV = {triangles[j][0][2], triangles[j][1][2], triangles[j][2][2]};
        triangles[j][0].pop_back();
        triangles[j][1].pop_back();
        triangles[j][2].pop_back();
        a = triangles[j][0];
        b = triangles[j][1];
        c = triangles[j][2];
        // Iterate over pixels in bounding box
        for (float y1 = minY; y1 <= maxY; y1++) {
            for (float x1 = minX; x1 <= maxX; x1++) { 
                vector<float> p = {x1, y1};
                if (barycentric_coords(p, a, b, c)) {
                    // Point is inside the triangle, color it
                    // calculate and normalize z val of face and compare to z buffer and only draw pixel if z < zbuffer
                    z = barycentricZ(p, a, b, c, zV); // z[0] = a.z, z[1] = b.z, z[2] = c.z 
                    if (z < input.getZBuffer(x1, y1) - 0.5) {
                        input.editZBuffer(x1, y1, z);
                        image.edit_pixel(x1, y1, r, g, bl);
                    }
                } 
            }
        }
    } 
}

int main(int argc, char** argv) {
    // Declare a start time variable
    auto start_time = chrono::high_resolution_clock::now(); // Start timing
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
    width = 800;
    height = 800;
    // get input file and read
    if (argc != 2 && argc != 3) {
        cerr << "missing input file: ./run <object.obj> <texture.ppm>" << endl;
    }
    if (argc == 2) {
        input.readFile(argv[1]);
    } 
    if (argc == 3) {
        image.read_texture(argv[2]);
        // also need to extract the texture cord for each vert and save to a vector
    }
    // create ppm file
    image.create_ppm(width, height, 255);

    // wire frame render
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
            // normalize cords to -.8 - .8 grid 
            vert[0] = ((vert[0] - input.getMinX()) / (input.getMaxX() - input.getMinX())) * 1.6 - .8;
            vert[1] = ((vert[1] - input.getMinY()) / (input.getMaxY() - input.getMinY())) * 1.6 - .8;
            vert2[0] = ((vert2[0] - input.getMinX()) / (input.getMaxX() - input.getMinX())) * 1.6 - .8;
            vert2[1] = ((vert2[1] - input.getMinY()) / (input.getMaxY() - input.getMinY())) * 1.6 - .8;
            // transform to 2D space
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
    image.vFlip();
    image.write_ppm("C:\\Users\\Jackson\\Desktop\\Projects\\myRenderer\\output\\wireFrame.ppm");

    input.readFile(argv[1]);
    image.create_ppm(width, height, 255);
    // full render
    //loop through faces and fill each face
    // define light direction
    int count = 0;
    vector<float> light_dir = {0, 0, -1};
    // init zBuffer
    input.initZBuffer(width, height);
    for (size_t i = 0; i < input.getAllFaces().size(); i++) {
        if ((int)i % (int)(input.getAllFaces().size() * 0.05) == 0) {
            // Calculate elapsed time
            auto current_time = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::seconds>(current_time - start_time).count(); // Get elapsed time in seconds
            cout << i << " faces out of " << input.getAllFaces().size() << " rendered " << count << "% done. Elapsed time: " << duration << " seconds." << endl;
            count += 5;
        }
        // calculate light val
        vector<int> face = input.getFace(i);
        vector<vector<float>> screenCords(3);
        vector<vector<float>> worldCords(3);
        for (int j = 0; j < 3; j++) {
            vector<float> v = input.getVert(face[j] - 1);
            screenCords[j] = {((v[0] + 1) * width / 2), ((v[1] + 1) * height / 2)};
            worldCords[j] = v;
        }
        vector<float> n(3);
        n = cross_product((worldCords[2] - worldCords[0]), (worldCords[1] - worldCords[0]));
        // normalize n
        float length = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
        if (length > 0) {
            n[0] /= length;
            n[1] /= length;
            n[2] /= length;
        }
        float intensity = n * light_dir;
        if (intensity > 0) {
            // fill with color (intensity * 255)
            fill(input.getFace(i), width, height, to_string(intensity * 255), to_string(intensity * 255), to_string(intensity * 255), image, input);
        }
    }
    // flip image
    image.vFlip();
    // write ppm file
    image.write_ppm("C:\\Users\\Jackson\\Desktop\\Projects\\myRenderer\\output\\render.ppm");
    // write a z buffer image
    float z_near = 0.8;
    float z_far = -0.8;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float z = input.getZBuffer(x, y);
            int intensity = (z == z_far) ? 0 : static_cast<int>(255 * (z - z_near) / (z_far - z_near));
            intensity = clamp(intensity, 0, 255);
            image.edit_pixel(x, y, to_string(intensity), to_string(intensity), to_string(intensity)); // Grayscale
        }
    }
    image.vFlip();
    image.write_ppm("C:\\Users\\Jackson\\Desktop\\Projects\\myRenderer\\output\\zBuffer.ppm");
}