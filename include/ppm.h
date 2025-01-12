#include <iostream>
#include <vector>
using namespace std;

class PPM {
protected:
    int rows;
    int cols;
    int max;
    int tRows;
    int tCols;
    int tMax;
    // 3 vector first represents rows, then cols, then rgb vals
    vector<vector<vector<string>>> image;
    vector<vector<vector<string>>> texture;
public:
    bool read_ppm(string fileName);
    bool read_texture(string fileName);
    bool write_ppm(string fileName);
    bool create_ppm(int r, int c, int m);
    bool edit_pixel(int x, int y, string r, string g, string b);
    void test_output();
    void vFlip();
    int getCols() const;
    int getRows() const;
    int getTCols() const;
    int getTRows() const;
    vector<string> getColor(int x, int y);
};

