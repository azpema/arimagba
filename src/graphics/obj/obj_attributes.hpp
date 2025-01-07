#ifndef _OBJ_ATTRIBUTES_ 
#define _OBJ_ATTRIBUTES_ 

#include <cstdint>
#include "obj_attribute_0.hpp"
#include "obj_attribute_1.hpp"
#include "obj_attribute_2.hpp"

enum class ObjMode {NORMAL = 0b00, AFFINE = 0b01, DISABLED = 0b10, AFFINE_DOUBLE = 0b11};

class ObjAttributes {
    public:
        ObjAttributes(uint64_t rawVal);
        ~ObjAttributes() = default;

        uint8_t getYCoord() const;
        ObjMode getObjMode() const;
        uint8_t getGfxMode() const;
        bool getMosaicEnabled() const;
        uint8_t getColorMode() const;
        uint8_t getSpriteShape() const;

        uint16_t getXCoord() const;
        uint8_t getAffineIndex() const;
        bool getHorizontalFlip() const;
        bool getVerticalFlip() const;
        uint8_t getSpriteSize() const;

        uint16_t getTileIndex() const;
        uint8_t getPriority() const;
        uint8_t getPaletteBank() const;

        // Calc
        uint8_t getWidth() const;
        uint8_t getHeight() const;

        uint32_t getPaletteIndex(const uint8_t coordX, const uint8_t coordY, const bool mapping1D) const;

        
        
    private:
        ObjAttribute0 objAttr0;
        ObjAttribute1 objAttr1;
        ObjAttribute2 objAttr2;

        struct WidthHeight{
            uint8_t width;
            uint8_t height;
        };

        constexpr static WidthHeight widthHeight[3][4] = { { {8,8},  {16,16}, {32,32}, {64,64} },
                                                           { {16,8}, {32,8},  {32,16}, {64,32} },
                                                           { {8,16}, {8,32},  {16,32}, {32,64} } };

        const static uint32_t LO_ATTRIBUTES_0_MASK  = 0b00000000000000001111111111111111;
        const static uint8_t LO_ATTRIBUTES_0_SHIFT  = 0x0;

        const static uint32_t LO_ATTRIBUTES_1_MASK  = 0b11111111111111110000000000000000;
        const static uint8_t LO_ATTRIBUTES_1_SHIFT  = 0x10;

        const static uint32_t HI_ATTRIBUTES_2_MASK  = 0b00000000000000001111111111111111;
        const static uint8_t HI_ATTRIBUTES_2_SHIFT  = 0x0;
};

#endif