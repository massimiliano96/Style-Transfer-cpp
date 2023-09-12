#ifndef _STYLE_TRANSFER_FACTORY_HPP_
#define _STYLE_TRANSFER_FACTORY_HPP_

#include <memory>
#include "StyleTransfer.hpp"

enum STYLE_TRANSFER_TYPE
{
    CANDY,
    MOSAIC,
    POINTILISM,
    RAIN_PRINCESS,
    UDNIE
};

class StyleTransferFactory
{
public:
    static void printStyleTransferTypes();
    static std::shared_ptr<StyleTransfer> getStyleTransfer(STYLE_TRANSFER_TYPE type);
};

#endif