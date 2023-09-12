#include "StyleTransfer.hpp"

#include <iostream>

std::vector<cv::Mat> StyleTransfer::process(cv::Mat image)
{
    std::size_t inputCount = session.GetInputCount();
    std::cout << "Number of model inputs: " << inputCount << std::endl;
    std::size_t outputCount = session.GetOutputCount();
    std::cout << "Number of model outputs: " << outputCount << std::endl;

    Ort::AllocatorWithDefaultOptions allocator;

    std::vector<Ort::AllocatedStringPtr> inputNamesPtr;
    std::vector<const char*> inputNames;
    inputNamesPtr.reserve(inputCount);
    inputNames.reserve(inputCount);

    for (size_t i = 0; i < inputCount; i++)
    {
        auto inputName = session.GetInputNameAllocated(i, allocator);
        std::cout << "Input " << i << " : name = " << inputName.get() << std::endl;
        inputNames.push_back(inputName.get());
        inputNamesPtr.push_back(std::move(inputName));
    }

    std::vector<Ort::AllocatedStringPtr> outputNamesPtr;
    std::vector<char*> outputNames;
    for (size_t i = 0; i < outputCount; i++)
    {
        auto outputName = session.GetOutputNameAllocated(i, allocator);
        std::cout << "Output " << i << " : name = " << outputName.get() << std::endl;
        outputNames.push_back(outputName.get());
        outputNamesPtr.push_back(std::move(outputName));
    }

    std::vector<int64_t> inputShape = session.GetInputTypeInfo(0).GetTensorTypeAndShapeInfo().GetShape();
    size_t inputTensorSize(inputShape[1] * inputShape[2] * inputShape[3]);

    cv::Mat floatImg;
    image.convertTo(floatImg, CV_32F);
    cv::normalize(floatImg, floatImg, 0, 255, cv::NORM_MINMAX);

    // Create a vector to hold the data
    std::vector<float> imgData(floatImg.begin<float>(), floatImg.end<float>());

    Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    Ort::Value inputTensor = Ort::Value::CreateTensor<float>(memoryInfo, imgData.data(), imgData.size(), inputShape.data(), inputShape.size());

    std::vector<Ort::Value> outputTensors = session.Run(Ort::RunOptions {nullptr}, inputNames.data(), &inputTensor, 1, outputNames.data(), 1);

    std::vector<cv::Mat> outputImages;

    for (auto& tensor : outputTensors)
    {
        // Ensure the Ort::Value is a tensor
        if (tensor.IsTensor())
        {
            // Get the dimensions of the tensor
            std::vector<int64_t> dimensions = tensor.GetTensorTypeAndShapeInfo().GetShape();
            // Ensure the tensor has the expected dimensions (e.g., 4D tensor)
            float* data = tensor.GetTensorMutableData<float>();

            // Create a vector of cv::Mat objects for each channel (B, G, and R)
            std::vector<cv::Mat> channels;
            int channel_size = dimensions[2] * dimensions[3];
            for (int i = 0; i < dimensions[1]; ++i)
            {
                channels.push_back(cv::Mat(cv::Size(dimensions[3], dimensions[2]), CV_32F, data + i * channel_size));
            }

            // Merge the separate channels into a single BGR image
            cv::Mat mat;
            cv::merge(channels, mat);
            outputImages.push_back(mat);
        }
    }
    return outputImages;
}

StyleTransfer::StyleTransfer(const std::string& modelPath) : session(env, modelPath.c_str(), Ort::SessionOptions {})
{
}
