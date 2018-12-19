#include "matrix.h"

Matrix::Matrix(int width, int height) {
    m_data = std::unique_ptr<int[]>(new int[static_cast<size_t>(width * height)]);
    for (int i = 0; i < width * height; i++) {
        m_data[static_cast<size_t>(i)] = 0;
    }
    m_width = width;
    m_height = height;
}

int Matrix::getWidth() {
    return m_width;
}

int Matrix::getHeight() {
    return m_height;
}

int& Matrix::operator() (int x, int y)
{
    if (x >= m_width && y >= m_height) {
        throw std::string("Bad index");
    }
    return m_data[static_cast<size_t>(y * m_width + x)];
}

int Matrix::operator() (int x, int y) const {
    if (x >= m_width && y >= m_height) {
        throw std::string("Bad index");
    }
    return m_data[static_cast<size_t>(y * m_width + x)];
}

Matrix::~Matrix() {

}
