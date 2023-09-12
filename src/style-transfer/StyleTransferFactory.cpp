#include "StyleTransferFactory.hpp"
#include <iostream>

void StyleTransferFactory::printStyleTransferTypes()
{
}

std::shared_ptr<StyleTransfer> StyleTransferFactory::getStyleTransfer(STYLE_TRANSFER_TYPE type)
{
    std::string modelPath = "./model/";
    switch (type)
    {
        case CANDY:
            modelPath += "candy-8.onnx";
            break;
        case MOSAIC:
            modelPath += "mosaic-9.onnx";
            break;
        case POINTILISM:
            modelPath += "pointilism.onnx";
            break;
        case RAIN_PRINCESS:
            modelPath += "rain-princess-9.onnx";
            break;
        case UDNIE:
            modelPath += "udnie-9.onnx";
            break;
        default:
            throw std::runtime_error("Style Transfer type not handled");
            break;
    }
    return std::make_shared<StyleTransfer>(modelPath);
}