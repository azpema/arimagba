#ifndef _OBJ_ATTRIBUTE_1_ 
#define _OBJ_ATTRIBUTE_1_ 

#include <cstdint>

class ObjAttribute1 {
    public:
        ObjAttribute1(uint16_t rawVal);
        ~ObjAttribute1() = default;

        uint16_t getXCoord() const;
        uint8_t getAffineIndex() const;
        bool getHorizontalFlip() const;
        bool getVerticalFlip() const;
        uint8_t getSpriteSize() const;
        
    private:
        uint16_t rawVal;
        const static uint16_t X_COORD_MASK              = 0b0000000111111111;
        const static uint16_t X_COORD_SHIFT             = 0x0;

        const static uint16_t AFFINE_INDEX_MASK         = 0b0011111000000000;
        const static uint16_t AFFINE_INDEX_SHIFT        = 0x9;

        const static uint16_t HORIZONTAL_FLIP_MASK      = 0b0001000000000000;
        const static uint16_t HORIZONTAL_FLIP_SHIFT     = 0xC;

        const static uint16_t VERTICAL_FLIP_MASK        = 0b0010000000000000;
        const static uint16_t VERTICAL_FLIP_SHIFT       = 0xD;

        const static uint16_t SPRITE_SIZE_MASK          = 0b1100000000000000;
        const static uint16_t SPRITE_SIZE_SHIFT         = 0xE;
};

#endif