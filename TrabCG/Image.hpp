#pragma once
cv::Mat* createZerosMat(int rows, int cols) {
    cv::Mat* mat = new cv::Mat(rows, cols, 16, cv::Vec3b(0, 0, 0));
    return mat;
}