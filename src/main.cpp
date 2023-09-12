#include "style-transfer/StyleTransfer.hpp"

#include <opencv2/imgcodecs.hpp>

int main()
{
    StyleTransfer detector("model/pointilism.onnx");
    cv::Mat image = cv::imread("./data/amber.jpg", cv::IMREAD_COLOR);
    std::vector<cv::Mat> results = detector.process(image);
    for (cv::Mat& image : results)
        cv::imwrite("./result.png", image);
    return 0;
}
