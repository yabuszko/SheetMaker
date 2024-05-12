#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>

// generates file paths based on base name and number of images
std::vector<std::string> generateFilePaths(const std::string& baseName, int numImages) {
    std::vector<std::string> filePaths;

    for (int i = 1; i <= numImages; ++i) {
        std::stringstream ss;
        ss << baseName << i << ".png";
        filePaths.push_back(ss.str());
    }
    return filePaths;
}

// creates image sheet
void makeSheet(const std::string& baseName, int numImages, bool horizontal=true) {
    std::vector<std::string> filePaths = generateFilePaths(baseName, numImages);

    // name for sheet
    std::stringstream ss_;
    ss_ << baseName << ".png";
    std::string sheetName = ss_.str();

    // loading images
    std::vector<cv::Mat> images;
    for (const auto& path : filePaths) {
        cv::Mat img = cv::imread(path, cv::IMREAD_UNCHANGED);
        if (img.empty()) {
            std::cerr << "unable to load image " << path << std::endl;
            return;
        }
        images.push_back(img);
    }

    // dimensions of sheet
    int sheetWidth, sheetHeight;
    if (horizontal) {
        sheetWidth = images[0].cols * images.size();
        sheetHeight = images[0].rows;
    }
    else {
        sheetWidth = images[0].cols;
        sheetHeight = images[0].rows * images.size();
    }


    // sheet
    cv::Mat sheet(sheetHeight, sheetWidth, CV_8UC4, cv::Scalar(0, 0, 0, 0));

    // Paste each image onto canvas

    if (horizontal) {
        int x = 0;
        for (const auto& img : images) {
            cv::Rect roi(x, 0, img.cols, img.rows);
            img.copyTo(sheet(roi));
            x += img.cols;
        }
    }
    else {
        int y = 0;
        for (const auto& img : images) {
            cv::Rect roi(0, y, img.cols, img.rows);
            img.copyTo(sheet(roi));
            y += img.rows;
        }
    }

    // save sheet
    cv::imwrite(sheetName, sheet);
    std::cout << "saved sheet " << sheetName << std::endl;

}

int main() {
    std::string baseName;
    std::cout << "base name: ";  std::cin >> baseName;
    int numImages; 
    std::cout << "\nnumber of images: "; std::cin >> numImages;
    bool horizontal;
    std::cout << "\nhorizontal (1) or vertical (0)? "; std::cin >> horizontal;

    makeSheet(baseName, numImages, horizontal);

	return 0;
}
