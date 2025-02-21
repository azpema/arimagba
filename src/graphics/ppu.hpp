#ifndef _PPU_ 
#define _PPU_ 

#include <iostream>
#include <string>

#include "../cpu/arm7tdmi.hpp"
#include "../memory/memory_manager.hpp"
#include "renderer.hpp"

class Renderer;

class PPU {
    public:
        PPU(const std::string &title, ARM7TDMI &cpu, MemoryManager *memManager);
        ~PPU();
        void renderScanline();

        uint8_t *getBgPaletteRAM() const;
        uint8_t *getObjPaletteRAM() const;
        uint8_t *getVRAM() const;
        uint8_t *getOVRAM() const;
        uint8_t *getOAM() const;
        const static uint8_t BG_NUM = 4;
        const static uint8_t MAX_SPRITE_NUM = 128;
        const static uint8_t TILE_SIZE = 0x20;

        uint32_t getPageFlipOffset() const;
        uint8_t getBgCharacterBaseBlock(const uint8_t bgNum) const;
        uint8_t getBgScreenBaseBlock(const uint8_t bgNum) const;
        uint8_t getBgColorMode(const uint8_t bgNum) const;
        uint32_t getTileSetVramOffset(const uint8_t bgNum) const;
        uint32_t getTileMapVramOffset(const uint8_t bgNum) const;
        uint8_t getBgSize(const uint8_t bgNum) const;
        uint8_t getBgPriority(const uint8_t bgNum) const;
        std::vector<uint8_t> getBgsWithPriorityX(const uint8_t prio) const;
        bool getBgEnabled(const uint8_t bgNum) const;
        std::vector<uint8_t> getBgBlendOrder() const;

        bool getObjMapping1D() const;
        bool getObjEnabled() const;

        uint32_t getVcount() const;
        uint32_t getBgOffsetH(const uint8_t bg) const;
        uint32_t getBgOffsetV(const uint8_t bg) const;

        const static uint32_t SCREEN_WIDTH = 240;
        const static uint32_t SCREEN_HEIGHT = 160;
        const static uint32_t PAGE_FLIP_SECOND_OFFSET = 0xA000;
        const static uint32_t TILE_WIDTH_HEIGHT = 8;

        const static uint8_t PALETTE_BANK_SIZE = 0x20;
        
    private:
        ARM7TDMI &cpu;
        MemoryManager *mem;
        std::unique_ptr<Renderer> rend;
        
        uint8_t* io;

        uint16_t* DISPCNT;
        uint16_t* GREEN_SWAP;
        uint16_t* DISPSTAT;
        uint16_t* VCOUNT;

        uint16_t* BGxCNT[BG_NUM];
        uint16_t* BGxHOFS[BG_NUM];
        uint16_t* BGxVOFS[BG_NUM];

        // ***************** VCOUNT ***************** 
        const static uint8_t VCOUNT_START_VBLANK = 160;
        const static uint8_t VCOUNT_END_VBLANK = 227;
        const static uint32_t VCOUNT_INITIAL_VALUE = 0x7e;

        void setDCNT_MODE(uint8_t mode);
        uint8_t getDCNT_MODE() const;
        void setVBlankFlag(bool val);
        void setHBlankFlag(bool val);
        void setVcountFlag(bool val);
        uint8_t getDCNT_PAGE() const;
        bool vBlankIrqEnabled() const;
        bool hBlankIrqEnabled() const;
        bool vCountIrqEnabled() const;
        uint8_t getVcountTrigger() const;

        bool getObj1DMapping() const;

        // ***************** DISPCNT ***************** 
        /*
            Bit   Name             Expl.
            0-2	  DCNT_MODEx.      Sets video mode. 0, 1, 2 are tiled modes; 3, 4, 5 are bitmap modes.
                DCNT_MODE#	   
            3     DCNT_GB	       Is set if cartridge is a GBC game. Read-only.
            4	  DCNT_PAGE	       Page select. Modes 4 and 5 can use page flipping for smoother animation.
                                    This bit selects the displayed page (and allowing the other one to be drawn on without artifacts).
            5	  DCNT_OAM_HBL	   Allows access to OAM in an HBlank. OAM is normally locked in VDraw.
                                    Will reduce the amount of sprite pixels rendered per line.
            6	  DCNT_OBJ_1D	   Object mapping mode. Tile memory can be seen as a 32x32 matrix of tiles.
                                    When sprites are composed of multiple tiles high, this bit tells whether the next row of tiles lies beneath the previous,
                                    in correspondence with the matrix structure (2D mapping, OM=0), or right next to it, so that memory is arranged as an array of sprites (1D mapping OM=1).
            7	  DCNT_BLANK	   Force a screen blank.
            8-B	  DCNT_BGx,        Enables rendering of the corresponding background and sprites.
                DCNT_OBJ.
                DCNT_LAYER#	
            D-F	  DCNT_WINx,       Enables the use of windows 0, 1 and Object window, respectively.
                DCNT_WINOBJ      Windows can be used to mask out certain areas (like the lamp did in Zelda:LTTP).
                        
        */

        struct REG_DISPCNT {
            const static uint16_t DCNT_MODE_MASK        = 0b0000000000000111;
            const static uint16_t DCNT_MODE_SHIFT       = 0;

            const static uint16_t DCNT_GB_MASK          = 0b0000000000001000;
            const static uint16_t DCNT_GB_SHIFT         = 3;

            const static uint16_t DCNT_PAGE_MASK        = 0b0000000000010000;
            const static uint16_t DCNT_PAGE_SHIFT       = 4;

            const static uint16_t DCNT_OAM_HBL_MASK     = 0b0000000000100000;
            const static uint16_t DCNT_OAM_HBL_SHIFT    = 5;

            const static uint16_t DCNT_OBJ_1D_MASK      = 0b0000000001000000;
            const static uint16_t DCNT_OBJ_1D_SHIFT     = 6;

            const static uint16_t DCNT_BLANK_MASK       = 0b0000000010000000;
            const static uint16_t DCNT_BLANK_SHIFT      = 7;

            const static uint16_t DCNT_BG0_MASK         = 0b0000000100000000;
            const static uint16_t DCNT_BG0_SHIFT        = 8;

            const static uint16_t DCNT_BG1_MASK         = 0b0000001000000000;
            const static uint16_t DCNT_BG1_SHIFT        = 9;

            const static uint16_t DCNT_BG2_MASK         = 0b0000010000000000;
            const static uint16_t DCNT_BG2_SHIFT        = 0xA;

            const static uint16_t DCNT_BG3_MASK         = 0b0000100000000000;
            const static uint16_t DCNT_BG3_SHIFT        = 0xB;

            const static uint16_t DCNT_OBJ_MASK         = 0b0001000000000000;
            const static uint16_t DCNT_OBJ_SHIFT        = 0xC;

            const static uint16_t DCNT_WIN0_MASK        = 0b0010000000000000;
            const static uint16_t DCNT_WIN0_SHIFT       = 0xD;

            const static uint16_t DCNT_WIN1_MASK        = 0b0100000000000000;
            const static uint16_t DCNT_WIN1_SHIFT       = 0xE;

            const static uint16_t DCNT_WINOBJ_MASK      = 0b1000000000000000;
            const static uint16_t DCNT_WINOBJ_SHIFT     = 0xF;
        };

        // ***************** DISPSTAT ***************** 
        /*
            Bit   Name              Expl.
            0     DSTAT_IN_VBL      V-Blank flag   (Read only) (1=VBlank) (set in line 160..226; not 227)
            1     DSTAT_IN_HBL      H-Blank flag   (Read only) (1=HBlank) (toggled in all lines, 0..227)
            2     DSTAT_IN_VCT      V-Counter flag (Read only) (1=Match)  (set in selected line)     (R)
            3     DSTAT_VBL_IRQ     V-Blank IRQ Enable         (1=Enable)                          (R/W)
            4     DSTAT_HBL_IRQ     H-Blank IRQ Enable         (1=Enable)                          (R/W)
            5     DSTAT_VCT_IRQ     V-Counter IRQ Enable       (1=Enable)                          (R/W)
            6                       Not used (0) / DSi: LCD Initialization Ready (0=Busy, 1=Ready)   (R)
            7                       Not used (0) / NDS: MSB of V-Vcount Setting (LYC.Bit8) (0..262)(R/W)
            8-15  DSTAT_VCT#        V-Count Setting (LYC)      (0..227)                            (R/W)
        */
        struct REG_DISPSTAT {
            // VBlank status
            const static uint16_t DSTAT_IN_VBL_MASK = 0b0000000000000001;
            const static uint16_t DSTAT_IN_VBL_SHIFT = 0;

            // HBlank status
            const static uint16_t DSTAT_IN_HBL_MASK = 0b0000000000000010;
            const static uint16_t DSTAT_IN_HBL_SHIFT = 1;

            // Vcount trigger status
            const static uint16_t DSTAT_IN_VCT_MASK = 0b0000000000000100;
            const static uint16_t DSTAT_IN_VCT_SHIFT = 2;

            // VBlank Interrupt request
            const static uint16_t DSTAT_VBL_IRQ_MASK = 0b0000000000001000;
            const static uint16_t DSTAT_VBL_IRQ_SHIFT = 3;

            // HBlank Interrupt request
            const static uint16_t DSTAT_HBL_IRQ_MASK = 0b0000000000010000;
            const static uint16_t DSTAT_HBL_IRQ_SHIFT = 4;

            // VCount Interrupt request
            const static uint16_t DSTAT_VCT_IRQ_MASK = 0b0000000000100000;
            const static uint16_t DSTAT_VCT_IRQ_SHIFT = 5;

            // VCount trigger value
            const static uint16_t DSTAT_VCT_NUM_MASK = 0b1111111100000000;
            const static uint16_t DSTAT_VCT_NUM_SHIFT = 8;
        };

        struct REG_BGxCNT {
            const static uint16_t BG_PRIO_NUM_MASK      = 0b0000000000000011;
            const static uint16_t BG_PRIO_NUM_SHIFT     = 0;

            const static uint16_t BG_CBB_NUM_MASK       = 0b0000000000001100;
            const static uint16_t BG_CBB_NUM_SHIFT      = 2;

            const static uint16_t BG_MOSAIC_MASK        = 0b0000000001000000;
            const static uint16_t BG_MOSAIC_SHIFT       = 6;

            const static uint16_t BG_8BPP_MASK          = 0b0000000010000000;
            const static uint16_t BG_8BPP_SHIFT         = 7;

            const static uint16_t BG_SBB_NUM_MASK       = 0b0001111100000000;
            const static uint16_t BG_SBB_NUM_SHIFT      = 8;

            const static uint16_t BG_WRAP_MASK          = 0b0010000000000000;
            const static uint16_t BG_WRAP_SHIFT         = 0xD;

            const static uint16_t BG_SIZE_NUM_MASK      = 0b1100000000000000;
            const static uint16_t BG_SIZE_NUM_SHIFT     = 0xE;
        };

        /*
        4000000h  2    R/W  DISPCNT   LCD Control
        4000002h  2    R/W  -         Undocumented - Green Swap
        4000004h  2    R/W  DISPSTAT  General LCD Status (STAT,LYC)
        4000006h  2    R    VCOUNT    Vertical Counter (LY)
        4000008h  2    R/W  BG0CNT    BG0 Control
        400000Ah  2    R/W  BG1CNT    BG1 Control
        400000Ch  2    R/W  BG2CNT    BG2 Control
        400000Eh  2    R/W  BG3CNT    BG3 Control
        4000010h  2    W    BG0HOFS   BG0 X-Offset
        4000012h  2    W    BG0VOFS   BG0 Y-Offset
        4000014h  2    W    BG1HOFS   BG1 X-Offset
        4000016h  2    W    BG1VOFS   BG1 Y-Offset
        4000018h  2    W    BG2HOFS   BG2 X-Offset
        400001Ah  2    W    BG2VOFS   BG2 Y-Offset
        400001Ch  2    W    BG3HOFS   BG3 X-Offset
        400001Eh  2    W    BG3VOFS   BG3 Y-Offset
        4000020h  2    W    BG2PA     BG2 Rotation/Scaling Parameter A (dx)
        4000022h  2    W    BG2PB     BG2 Rotation/Scaling Parameter B (dmx)
        4000024h  2    W    BG2PC     BG2 Rotation/Scaling Parameter C (dy)
        4000026h  2    W    BG2PD     BG2 Rotation/Scaling Parameter D (dmy)
        4000028h  4    W    BG2X      BG2 Reference Point X-Coordinate
        400002Ch  4    W    BG2Y      BG2 Reference Point Y-Coordinate
        4000030h  2    W    BG3PA     BG3 Rotation/Scaling Parameter A (dx)
        4000032h  2    W    BG3PB     BG3 Rotation/Scaling Parameter B (dmx)
        4000034h  2    W    BG3PC     BG3 Rotation/Scaling Parameter C (dy)
        4000036h  2    W    BG3PD     BG3 Rotation/Scaling Parameter D (dmy)
        4000038h  4    W    BG3X      BG3 Reference Point X-Coordinate
        400003Ch  4    W    BG3Y      BG3 Reference Point Y-Coordinate
        4000040h  2    W    WIN0H     Window 0 Horizontal Dimensions
        4000042h  2    W    WIN1H     Window 1 Horizontal Dimensions
        4000044h  2    W    WIN0V     Window 0 Vertical Dimensions
        4000046h  2    W    WIN1V     Window 1 Vertical Dimensions
        4000048h  2    R/W  WININ     Inside of Window 0 and 1
        400004Ah  2    R/W  WINOUT    Inside of OBJ Window & Outside of Windows
        400004Ch  2    W    MOSAIC    Mosaic Size
        400004Eh       -    -         Not used
        4000050h  2    R/W  BLDCNT    Color Special Effects Selection
        4000052h  2    R/W  BLDALPHA  Alpha Blending Coefficients
        4000054h  2    W    BLDY      Brightness (Fade-In/Out) Coefficient
        4000056h       -    -         Not used
        */

       
};

#endif