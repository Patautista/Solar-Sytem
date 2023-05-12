#pragma once
void drawLineDDA(cv::Mat* mat, int x1, int y1, int x2, int y2) {
    if (mat == nullptr || mat->empty()) {
        // mat pointer is null or mat is empty
        return;
    }

    int dx = x2 - x1;
    int dy = y2 - y1;

    int steps = std::max(std::abs(dx), std::abs(dy));

    float xInc = dx / (float)steps;
    float yInc = dy / (float)steps;

    float x = x1;
    float y = y1;

    for (int i = 0; i <= steps; i++) {
        int px = (int)std::round(x);
        int py = (int)std::round(y);
        if (px >= 0 && px < mat->cols && py >= 0 && py < mat->rows) {
            // px and py are within the dimensions of the mat object
            cv::Vec3b color(255, 255, 255); // white color
            mat->at<cv::Vec3b>(py, px) = color;
        }
        x += xInc;
        y += yInc;
    }
}
