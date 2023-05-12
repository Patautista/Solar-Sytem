#pragma once
class Image {
public:
    Image(int width, int height) {
        this->width = width;
        this->height = height;
        this->matrix = new int* [height];
        for (int i = 0; i < height; i++) {
            this->matrix[i] = new int[width * 3];
            for (int j = 0; j < width * 3; j++) {
                this->matrix[i][j] = 0;
            }
        }
    }

    ~Image() {
        for (int i = 0; i < height; i++) {
            delete[] this->matrix[i];
        }
        delete[] this->matrix;
    }

    int* operator[](int i) {
        return this->matrix[i];
    }

    // Outros métodos e atributos podem ser adicionados conforme necessário

private:
    int width;
    int height;
    int** matrix;
};

cv::Mat* createZerosMat(int rows, int cols) {
    cv::Mat* mat = new cv::Mat(rows, cols, 16, cv::Vec3b(0, 0, 0));
    return mat;
}