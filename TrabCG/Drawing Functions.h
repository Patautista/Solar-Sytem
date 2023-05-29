namespace lib {

    cv::Mat floodFill(const cv::Mat& image, const cv::Point& startPixel, const cv::Scalar& newColor) {
        cv::Mat filledImage = image.clone();
        cv::Rect imageRect(0, 0, filledImage.cols, filledImage.rows);

        cv::Scalar originalColor = filledImage.at<cv::Scalar>(startPixel);

        // Check if the start pixel color is already the new color
        if (originalColor == newColor) {
            return filledImage;
        }

        // Auxiliary function to check if a position is within the image bounds
        auto isValidPosition = [&](int row, int col) {
            return imageRect.contains(cv::Point(col, row));
        };

        // Recursive function to fill the area
        std::function<void(int, int)> fill = [&](int row, int col) {
            if (!isValidPosition(row, col) || filledImage.at<cv::Scalar>(row, col) != originalColor) {
                return;
            }

            // Set the pixel with the new color
            filledImage.at<cv::Scalar>(row, col) = newColor;

            // Call the function recursively for the neighbors
            fill(row - 1, col);  // Upper neighbor
            fill(row + 1, col);  // Lower neighbor
            fill(row, col - 1);  // Left neighbor
            fill(row, col + 1);  // Right neighbor
        };

        // Call the fill function
        fill(startPixel.y, startPixel.x);

        return filledImage;
    }
}