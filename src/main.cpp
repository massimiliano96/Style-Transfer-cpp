#include <iostream>
#include <map>
#include <opencv2/imgcodecs.hpp>
#include <string>
#include "style-transfer/StyleTransferFactory.hpp"

const std::map<std::string, STYLE_TRANSFER_TYPE> choiceOptions = {{"1", STYLE_TRANSFER_TYPE::CANDY},
                                                                  {"2", STYLE_TRANSFER_TYPE::MOSAIC},
                                                                  {"3", STYLE_TRANSFER_TYPE::POINTILISM},
                                                                  {"4", STYLE_TRANSFER_TYPE::RAIN_PRINCESS},
                                                                  {"5", STYLE_TRANSFER_TYPE::UDNIE}};

int main()
{
    std::cout << "CHOOSE A STYLE TRANSFER TYPE:\n";
    std::cout << "[1] Candy\n"
              << "[2] Mosaic\n"
              << "[3] Pointilism\n"
              << "[4] Rain Princess\n"
              << "[5] Udnie\n";

    std::string input;
    std::getline(std::cin, input);

    while (choiceOptions.find(input) == choiceOptions.end())
    {
        std::cout << "Invalid choice!\n\n";

        std::cout << "CHOOSE A STYLE TRANSFER TYPE:\n";
        std::cout << "[1] Candy\n"
                  << "[2] Mosaic\n"
                  << "[3] Pointilism\n"
                  << "[4] Rain Princess\n"
                  << "[5] Udnie\n";
        std::getline(std::cin, input);
    }

    STYLE_TRANSFER_TYPE type = choiceOptions.find(input)->second;

    std::shared_ptr<StyleTransfer> detector = StyleTransferFactory::getStyleTransfer(type);

    std::string imagePath;
    std::cout << "Provide the path of an image: ";
    std::cin >> imagePath;

    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);

    cv::Mat processsedImage = detector->process(image);
    cv::imwrite("./processsedImage.png", processsedImage);
    return 0;
}
