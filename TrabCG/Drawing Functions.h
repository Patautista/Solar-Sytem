namespace lib {

    void floodFill(cv::Mat& image, int startRow, int startCol, cv::Vec3b fillColor) {
        int numRows = image.rows;
        int numCols = image.cols;

        // Check if the starting pixel is out of bounds
        if (startRow < 0 || startRow >= numRows || startCol < 0 || startCol >= numCols)
            return;

        // Get the original color of the starting pixel
        cv::Vec3b startColor = image.at<cv::Vec3b>(startRow, startCol);

        // Check if the starting pixel is already the fill color
        if (startColor == fillColor)
            return;

        // Create a queue to hold the pixels to be processed
        std::queue<std::pair<int, int>> pixelQueue;

        // Add the starting pixel to the queue
        pixelQueue.push(std::make_pair(startRow, startCol));

        // Perform the flood fill
        while (!pixelQueue.empty()) {
            // Get the current pixel from the queue
            std::pair<int, int> currentPixel = pixelQueue.front();
            pixelQueue.pop();

            int row = currentPixel.first;
            int col = currentPixel.second;

            // Check if the current pixel is within the image bounds and has the same color as the starting pixel
            if (row >= 0 && row < numRows && col >= 0 && col < numCols && image.at<cv::Vec3b>(row, col) == startColor) {
                // Set the pixel color to the fill color
                image.at<cv::Vec3b>(row, col) = fillColor;

                // Add the neighboring pixels to the queue for processing
                pixelQueue.push(std::make_pair(row - 1, col)); // Top
                pixelQueue.push(std::make_pair(row + 1, col)); // Bottom
                pixelQueue.push(std::make_pair(row, col - 1)); // Left
                pixelQueue.push(std::make_pair(row, col + 1)); // Right
            }
        }
    }

    void floodFillWithMatrix(cv::Mat& target, const cv::Mat& source, cv::Point seedPoint) {
        if (seedPoint.x < 0 || seedPoint.y < 0 || seedPoint.x >= target.cols || seedPoint.y >= target.rows) {
            return; // Invalid seed point, return without filling
        }

        cv::Mat visitedMask = cv::Mat::zeros(target.size(), CV_8UC1);
        std::queue<cv::Point> queue;


        // Get the original color of the starting pixel
        cv::Vec3b startColor = target.at<cv::Vec3b>(seedPoint.x, seedPoint.y);

        queue.push(seedPoint);
        visitedMask.at<uchar>(seedPoint) = 255;

        while (!queue.empty()) {
            cv::Point currentPoint = queue.front();
            queue.pop();
            cv::Point currentPointsource = cv::Point(currentPoint.x % source.cols, currentPoint.y % source.rows);
            target.at<cv::Vec3b>(currentPoint) = source.at<cv::Vec3b>(currentPointsource);

            // Add neighbors to the queue if they are within the target matrix and haven't been visited
            std::vector<cv::Point> neighbors = {
                cv::Point(currentPoint.x + 1, currentPoint.y),
                cv::Point(currentPoint.x - 1, currentPoint.y),
                cv::Point(currentPoint.x, currentPoint.y + 1),
                cv::Point(currentPoint.x, currentPoint.y - 1)
            };

            for (const cv::Point& neighbor : neighbors) {
                if (neighbor.x >= 0 && neighbor.y >= 0 && neighbor.x < target.cols && neighbor.y < target.rows
                    && visitedMask.at<uchar>(neighbor) == 0 && target.at<cv::Vec3b>(neighbor) == startColor) {
                    queue.push(neighbor);
                    visitedMask.at<uchar>(neighbor) = 255;
                }
            }
        }
    }

    void TruncateTexture(cv::Mat& mat, cv::Mat& texture) {
        cv::Mat resizedTexture;
        cv::resize(texture, resizedTexture, mat.size());

        // Copy the pixel values from the resized texture image to the viewport
        resizedTexture.copyTo(mat);
    }

    void drawLine(cv::Mat& image, int x1, int y1, int x2, int y2, cv::Vec3b color) {
        int dx = std::abs(x2 - x1);
        int dy = std::abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int error = dx - dy;

        while (x1 != x2 || y1 != y2) {
            if (x1 >= 0 && x1 < image.cols && y1 >= 0 && y1 < image.rows) {
                image.at<cv::Vec3b>(y1, x1) = color;
            }

            int error2 = error * 2;

            if (error2 > -dy) {
                error -= dy;
                x1 += sx;
            }

            if (error2 < dx) {
                error += dx;
                y1 += sy;
            }
        }
    }

    void drawEllipse(cv::Mat& image, cv::Point center, int width, int height, cv::Vec3b color, int thickness) {
        int radiusX = width / 2;
        int radiusY = height / 2;

        int centerX = center.x;
        int centerY = center.y;

        double aspectRatio = (double)width / height;

        for (double angle = 0.0; angle < 2 * CV_PI; angle += 0.01) {
            int x1 = centerX + radiusX * cos(angle);
            int y1 = centerY + radiusY * sin(angle);

            int x2 = centerX + radiusX * cos(angle + 0.01);
            int y2 = centerY + radiusY * sin(angle + 0.01);

            drawLine(image, x1, y1, x2, y2, color);
        }
    }

    void drawCircle(cv::Mat& image, int xc, int yc, int radius, cv::Vec3b color) {
        int x = 0;
        int y = radius;
        int p = 1 - radius;

        while (x <= y) {
            if (xc + x >= 0 && xc + x < image.cols && yc + y >= 0 && yc + y < image.rows) {
                image.at<cv::Vec3b>(yc + y, xc + x) = color;
            }
            if (xc - x >= 0 && xc - x < image.cols && yc + y >= 0 && yc + y < image.rows) {
                image.at<cv::Vec3b>(yc + y, xc - x) = color;
            }
            if (xc + x >= 0 && xc + x < image.cols && yc - y >= 0 && yc - y < image.rows) {
                image.at<cv::Vec3b>(yc - y, xc + x) = color;
            }
            if (xc - x >= 0 && xc - x < image.cols && yc - y >= 0 && yc - y < image.rows) {
                image.at<cv::Vec3b>(yc - y, xc - x) = color;
            }
            if (xc + y >= 0 && xc + y < image.cols && yc + x >= 0 && yc + x < image.rows) {
                image.at<cv::Vec3b>(yc + x, xc + y) = color;
            }
            if (xc - y >= 0 && xc - y < image.cols && yc + x >= 0 && yc + x < image.rows) {
                image.at<cv::Vec3b>(yc + x, xc - y) = color;
            }
            if (xc + y >= 0 && xc + y < image.cols && yc - x >= 0 && yc - x < image.rows) {
                image.at<cv::Vec3b>(yc - x, xc + y) = color;
            }
            if (xc - y >= 0 && xc - y < image.cols && yc - x >= 0 && yc - x < image.rows) {
                image.at<cv::Vec3b>(yc - x, xc - y) = color;
            }

            if (p < 0) {
                p += 2 * x + 3;
            }
            else {
                p += 2 * (x - y) + 5;
                y--;
            }
            x++;
        }
    }


    void drawArc(cv::Mat& image, int centerX, int centerY, int radius, double startAngle, double endAngle, const cv::Vec3b& color) {
        double angleStep = 1.0 / radius; // Angle step size
        double currentAngle = startAngle; // Starting angle

        while (currentAngle <= endAngle) {
            // Calculate the coordinates of the point on the circle
            int x = static_cast<int>(centerX + radius * cos(currentAngle * CV_PI / 180.0));
            int y = static_cast<int>(centerY + radius * sin(currentAngle * CV_PI / 180.0));

            // Draw the point on the image
            image.at<cv::Vec3b>(x, y) = color;

            // Increment the angle
            currentAngle += angleStep;
        }
    }

    void drawA(cv::Mat& image, int startX, int startY, int size, cv::Vec3b color) {
        // Draw the slanted lines
        drawLine(image, startX, startY - size, startX + size, startY, color);
        drawLine(image, startX, startY - size, startX - size, startY, color);

        // Draw the horizontal line
        drawLine(image, startX + size / 2, startY - size / 2, startX - size / 2, startY - size / 2, color);
    }

    void drawE(cv::Mat& image, int startX, int startY, int size, cv::Vec3b color) {
        // Draw the vertical line
        drawLine(image, startX, startY, startX, startY + size, color);

        // Draw the top horizontal line
        drawLine(image, startX, startY, startX + size / 2, startY, color);


        // Draw the middle horizontal line
        drawLine(image, startX, startY + size / 2, startX + size / 2, startY + size / 2, color);

        // Draw the bottom horizontal line
        drawLine(image, startX, startY + size, startX + size / 2, startY + size, color);
    }

    void drawV(cv::Mat& image, int startX, int startY, int size, cv::Vec3b color) {
        // Draw the slanted lines
        drawLine(image, startX, startY + size, startX - size, startY, color);
        drawLine(image, startX, startY + size, startX + size, startY, color);
    }
}