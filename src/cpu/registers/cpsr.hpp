#ifndef _CPSR_ 
#define _CPSR_ 

#include <iostream>
#include <unordered_map>

class PSR {
	public:
		PSR();
        enum Flag {N, Z, C, V, I, F, T};

        const static uint32_t MODE_FLAG_USR_VAL = 0b00000000000000000000000000010000;
        const static uint32_t MODE_FLAG_FIQ_VAL  = 0b00000000000000000000000000010001;
        const static uint32_t MODE_FLAG_IRQ_VAL  = 0b00000000000000000000000000010010;
        const static uint32_t MODE_FLAG_SVC_VAL  = 0b00000000000000000000000000010011;
        const static uint32_t MODE_FLAG_ABT_VAL  = 0b00000000000000000000000000010111;
        const static uint32_t MODE_FLAG_UND_VAL  = 0b00000000000000000000000000011011;
        const static uint32_t MODE_FLAG_SYS_VAL  = 0b00000000000000000000000000011111;

        enum Mode { User = MODE_FLAG_USR_VAL,
                    FIQ = MODE_FLAG_FIQ_VAL, 
                    IRQ = MODE_FLAG_IRQ_VAL, 
                    Supervisor = MODE_FLAG_SVC_VAL,
                    Abort = MODE_FLAG_ABT_VAL,
                    Undefined = MODE_FLAG_UND_VAL, 
                    System = MODE_FLAG_SYS_VAL };

        std::unordered_map<uint32_t, std::string> mode2String = {
            {0b10000, "User"},
            {0b10001, "FIQ"},
            {0b10010, "IRQ"},
            {0b10011, "Supervisor"},
            {0b10111, "Abort"},
            {0b11011, "Undefined"},
            {0b11111, "System"},
        }; 

        uint32_t getValue();
        void setValue(uint32_t val);
        Mode getMode();
        uint32_t getModeUInt();
        std::string getModeString();

	    bool getNFlag();
		bool getZFlag();
		bool getCFlag();
		bool getVFlag();
        bool getIFlag();
        bool getFFlag();
        bool getTFlag();

		void setNFlag(bool val);
		void setZFlag(bool val);
		void setCFlag(bool val);
		void setVFlag(bool val);
        void setIFlag(bool val);
		void setFFlag(bool val);

		void setTFlag(bool val);
        bool isThumbMode();

        void setMode(Mode mode);

        void reset();
    
	private:
		// Program Status Register
		uint32_t value = 0;

        // Condition code flags
        const static uint32_t N_FLAG_MASK = 0b10000000000000000000000000000000;
        const static uint32_t N_FLAG_SHIFT = 31;
        const static uint32_t Z_FLAG_MASK = 0b01000000000000000000000000000000;
        const static uint32_t Z_FLAG_SHIFT = 30;
        const static uint32_t C_FLAG_MASK = 0b00100000000000000000000000000000;
        const static uint32_t C_FLAG_SHIFT = 29;
        const static uint32_t V_FLAG_MASK = 0b00010000000000000000000000000000;
        const static uint32_t V_FLAG_SHIFT = 28;

        // Control bits
        // IRQ disable
        const static uint32_t I_FLAG_MASK = 0b00000000000000000000000010000000;
        const static uint32_t I_FLAG_SHIFT = 7;
        // FIQ disable
        const static uint32_t F_FLAG_MASK = 0b00000000000000000000000001000000;
        const static uint32_t F_FLAG_SHIFT = 6;
        // State bit (Thumb mode)
        const static uint32_t T_FLAG_MASK = 0b00000000000000000000000000100000;
        const static uint32_t T_FLAG_SHIFT = 5;
        // Mode bits
        const static uint32_t MODE_FLAG_MASK = 0b00000000000000000000000000011111;
        const static uint32_t MODE_FLAG_SHIFT = 0;

        void setCPSRFlags(Flag flag, bool val);
};

#endif