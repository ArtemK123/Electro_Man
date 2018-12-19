#ifndef MATRIX_H
#define MATRIX_H

#include <memory>
#include <string>

class Matrix {
private:
    std::unique_ptr<int[]> m_data;
    int m_width;
    int m_height;


public:
    int getWidth();
    int getHeight();
    int& operator() (int x, int y);
    int  operator() (int x, int y) const;

    Matrix(int width, int height);
    ~Matrix();
};
#endif // MATRIX_H
