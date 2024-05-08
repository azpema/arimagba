#include <iostream>
#include "arm_opcode.hpp"
#include "../fields/operand.hpp"
#include "../fields/shift_rm.hpp"
#include "../fields/imm.hpp"
#include "../fields/rotate_imm.hpp"
#include <unordered_map>

class DataProcessing : public ArmOpcode {
    public:
		DataProcessing(uint32_t op, ARM7TDMI &cpu);
        ~DataProcessing();
        std::string toString() override;
        void doExecute() override;
        void doDecode() override;
        uint32_t cyclesUsed() const override;

        std::string getOpCodeMnemonic();
        std::string getRdMnemonic();
        std::string getRnMnemonic();
        std::string getOperand2Mnemonic();
        uint32_t getOperand2();

    private:
        uint16_t i, s, rn, rd, dataOpCode;
        uint32_t op1, op2;
        Operand *operand2;

        const static uint32_t IMMEDIATE_OPERAND_MASK = 0b00000010000000000000000000000000; 
        const static uint32_t IMMEDIATE_OPERAND_SHIFT = 25; 

        const static uint32_t OPCODE_MASK = 0b00000001111000000000000000000000;
        const static uint32_t OPCODE_SHIFT = 21;

        const static uint32_t SET_CONDITION_MASK = 0b00000000000100000000000000000000;
        const static uint32_t SET_CONDITION_SHIFT = 20;

        const static uint32_t RN_MASK = 0b00000000000011110000000000000000;
        const static uint32_t RN_SHIFT = 16;

        const static uint32_t RD_MASK = 0b00000000000000001111000000000000;
        const static uint32_t RD_SHIFT = 12;

        const static uint32_t OPERAND2_MASK = 0b00000000000000000000111111111111;
        const static uint32_t OPERAND2_SHIFT = 0;


        const static uint16_t OPCODE_AND_VAL = 0x0;
        const static uint16_t OPCODE_EOR_VAL = 0x1;
        const static uint16_t OPCODE_SUB_VAL = 0x2;
        const static uint16_t OPCODE_RSB_VAL = 0x3;
        const static uint16_t OPCODE_ADD_VAL = 0x4;
        const static uint16_t OPCODE_ADC_VAL = 0x5;
        const static uint16_t OPCODE_SBC_VAL = 0x6;
        const static uint16_t OPCODE_RSC_VAL = 0x7;
        const static uint16_t OPCODE_TST_VAL = 0x8;
        const static uint16_t OPCODE_TEQ_VAL = 0x9;
        const static uint16_t OPCODE_CMP_VAL = 0xA;
        const static uint16_t OPCODE_CMN_VAL = 0xB;
        const static uint16_t OPCODE_ORR_VAL = 0xC;
        const static uint16_t OPCODE_MOV_VAL = 0xD;
        const static uint16_t OPCODE_BIC_VAL = 0xE;
        const static uint16_t OPCODE_MVN_VAL = 0xF;

        std::unordered_map<uint16_t, std::string> dataOpCode2Mnemonic = {
            {0x0, "and"},
            {0x1, "eor"},
            {0x2, "sub"},
            {0x3, "rsb"},
            {0x4, "add"},
            {0x5, "adc"},
            {0x6, "sbc"},
            {0x7, "rsc"},
            {0x8, "tst"},
            {0x9, "teq"},
            {0xA, "cmp"},
            {0xB, "cmn"},
            {0xC, "orr"},
            {0xD, "mov"},
            {0xE, "bic"},
            {0xF, "mvn"}
        }; 

        std::unordered_map<uint16_t, std::string> SFlag2Mnemonic = {
            {0, ""},
            {1, "s"},
        }; 

        // https://alisdair.mcdiarmid.org/arm-immediate-value-encoding/
        uint32_t getOperand2Rm();

        //void DataProcessing::doExecuteCmp(ARM7TDMI &cpu);
        void doExecuteCmp(ARM7TDMI &cpu);
        void doExecuteMov(ARM7TDMI &cpu);
        void doExecuteAdd(ARM7TDMI &cpu);
        void doExecuteOrr(ARM7TDMI &cpu);
};