#include <iostream>
#include <vector>
using namespace std;

class PPM {
protected:
    int rows;
    int cols;
    int max;
    // 3 vector first represents rows, then cols, then rgb vals
    vector<vector<vector<string>>> image;
public:
    bool read_ppm(string fileName);
    bool write_ppm(string fileName);
    bool create_ppm(int r, int c, int m);
    bool edit_pixel(int x, int y, string r, string g, string b);
    void test_output();
    int getCols() const;
    int getRows() const;
};

