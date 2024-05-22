#ifndef _PPU_ 
#define _PPU_ 

#include <iostream>
#include <string>
#include <SDL2/SDL.h>

#include "../memory/memory_manager.hpp"

class PPU {
    public:
        PPU(std::string title, MemoryManager *memManager);
        ~PPU();
        void renderScanline();
        
        
    private:
        void renderScanlineMode3();
        void renderScanlineMode4();

        MemoryManager *mem;
        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;

        const uint32_t SCREEN_WIDTH = 240;
        const uint32_t SCREEN_HEIGHT = 160;
        
        volatile uint16_t* io;

        volatile uint16_t* DISPCNT;
        volatile uint16_t* GREEN_SWAP;
        volatile uint16_t* DISPSTAT;
        volatile uint16_t* VCOUNT;

        // ***************** VCOUNT ***************** 
        const uint8_t VCOUNT_START_VBLANK = 160;
        const uint8_t VCOUNT_END_VBLANK = 227;

        const uint16_t DCNT_MODE_MASK = 0b0000000000000111;
        const uint16_t DCNT_MODE_SHIFT = 0; 

        // ***************** DISPSTAT ***************** 
        /*
            Bit   Name  Expl.
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
        // VBlank status
        const uint16_t DSTAT_IN_VBL_MASK = 0b0000000000000001;
        const uint16_t DSTAT_IN_VBL_SHIFT = 0;

        // HBlank status
        const uint16_t DSTAT_IN_HBL_MASK = 0b0000000000000010;
        const uint16_t DSTAT_IN_HBL_SHIFT = 1;

        uint8_t getDCNT_MODE();
        void setVBlankFlag(bool val);

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