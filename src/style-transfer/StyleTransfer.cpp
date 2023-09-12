#include "StyleTransfer.hpp"
#include <cassert>
#include <iostream>
#include <opencv2/imgproc.hpp>

constexpr const int width_ = 224;
constexpr const int height_ = 224;
constexpr const int channel = 3;

cv::Mat StyleTransfer::process(cv::Mat image)
{
    std::size_t inputCount = session.GetInputCount();
    std::size_t outputCount = session.GetOutputCount();

    Ort::AllocatorWithDefaultOptions allocator;

    std::vector<Ort::AllocatedStringPtr> inputNamesPtr;
    std::vector<const char*> inputNames;
    inputNamesPtr.reserve(inputCount);
    inputNames.reserve(inputCount);

    for (size_t i = 0; i < inputCount; i++)
    {
        auto inputName = session.GetInputNameAllocated(i, allocator);
        inputNames.push_back(inputName.get());
        inputNamesPtr.push_back(std::move(inputName));
    }

    std::vector<Ort::AllocatedStringPtr> outputNamesPtr;
    std::vector<char*> outputNames;
    for (size_t i = 0; i < outputCount; i++)
    {
        auto outputName = session.GetOutputNameAllocated(i, allocator);
        outputNames.push_back(outputName.get());
        outputNamesPtr.push_back(std::move(outputName));
    }

    std::vector<int64_t> inputShape = session.GetInputTypeInfo(0).GetTensorTypeAndShapeInfo().GetShape();

    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
    cv::Size originalSize(image.cols, image.rows);
    cv::resize(image, image, cv::Size(width_, height_));

    std::vector<float> imgData;
    imgData.resize(width_ * height_ * channel);

    for (int c = 0; c < channel; ++c)
    {
        for (int i = 0; i < height_; ++i)
        {
            for (int j = 0; j < width_; ++j)
            {
                imgData[c * height_ * width_ + i * width_ + j] = image.at<cv::Vec3b>(i, j)[c] / 255.0f;
            }
        }
    }

    Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    Ort::Value inputTensor = Ort::Value::CreateTensor<float>(memoryInfo, imgData.data(), imgData.size(), inputShape.data(), inputShape.size());

    std::vector<Ort::Value> outputTensors = session.Run(Ort::RunOptions {nullptr}, inputNames.data(), &inputTensor, 1, outputNames.data(), 1);

    std::vector<cv::Mat> outputImages;

    for (auto& tensor : outputTensors)
    {
        if (tensor.IsTensor())
        {
            std::vector<int64_t> dimensions = tensor.GetTensorTypeAndShapeInfo().GetShape();
            float* data = tensor.GetTensorMutableData<float>();

            std::vector<cv::Mat> channels;
            int channel_size = dimensions[2] * dimensions[3];
            for (int i = 0; i < dimensions[1]; ++i)
            {
                channels.push_back(cv::Mat(cv::Size(dimensions[3], dimensions[2]), CV_32F, data + i * channel_size));
            }

            cv::Mat mat;
            cv::merge(channels, mat);

            // Conversione da float a uchar e da RGB a BGR
            cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
            cv::resize(mat, mat, originalSize);

            outputImages.push_back(mat);
        }
    }
    assert(outputImages.size() == 1);
    return outputImages[0];
}

StyleTransfer::StyleTransfer(const std::string& modelPath) : session(env, modelPath.c_str(), Ort::SessionOptions {})
{
}
