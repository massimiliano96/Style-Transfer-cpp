#ifndef _STYLE_TRANSFER_HPP_
#define _STYLE_TRANSFER_HPP_

#include <string>

#include <onnxruntime_cxx_api.h>
#include <opencv2/core/core.hpp>

class StyleTransfer
{
public:
    explicit StyleTransfer(const std::string& modelPath);

    cv::Mat process(cv::Mat);

private:
    Ort::Env env {ORT_LOGGING_LEVEL_FATAL, "style-transfer"};
    Ort::Session session;
};

#endif