#include "exception_handler.hpp"

ExceptionHandler::ExceptionHandler(ARM7TDMI &cpu) : cpu(cpu){

}

void ExceptionHandler::raiseException(Exception ex){
    if(ex != Exception::IRQ || (isMasterInterruptEnabled() && isSpecificInterruptEnabled(ex))){
        PSR::Mode newMode = except2Mode[ex];
        uint32_t newPC = exceptionVector[ex];
        uint32_t returnPC = cpu.getPC();
        if(cpu.getCPSR().getTFlag()){
            returnPC += 2;
        }else{
            //returnPC += 2;
        }
        // This is a copy
        PSR oldCPSR = cpu.getCPSR();

        cpu.getCPSR().setMode(newMode);
        cpu.getCPSR().setTFlag(false);
        cpu.getCPSR().setIFlag(true);
        if(ex == Exception::RESET || ex == Exception::FIQ)
            cpu.getCPSR().setFFlag(true);


        cpu.setReg(14, returnPC);
        cpu.setSPSR(oldCPSR.getValue());

        cpu.setPC(newPC);

        cpu.setMustFlushPipeline(true);

        switch(ex){
            case Exception::RESET:
                throw std::runtime_error("TODO: Unimplemented exception RESET in ExceptionHandler::raiseException");
                break;
            case Exception::UNDEF:
                throw std::runtime_error("TODO: Unimplemented exception UNDEF in ExceptionHandler::raiseException");
                break;
            case Exception::PREFETCH_ABORT:
                throw std::runtime_error("TODO: Unimplemented exception PREFETCH_ABORT in ExceptionHandler::raiseException");
                break;
            case Exception::DATA_ABORT:
                throw std::runtime_error("TODO: Unimplemented exception DATA_ABORT in ExceptionHandler::raiseException");
                break;
            case Exception::ADDRESS_EXCEED:
                throw std::runtime_error("TODO: Unimplemented exception ADDRESS_EXCEED in ExceptionHandler::raiseException");
                break;
            case Exception::FIQ:
                throw std::runtime_error("TODO: Unimplemented exception FIQ in ExceptionHandler::raiseException");
                break;
            case Exception::IRQ:
                break;
            default:
                break;
        }
            

    }
}

bool ExceptionHandler::isMasterInterruptEnabled(){
    return (cpu.getMemManager().readWord(REG_IME_ADDR) & 0x1) == 0x1;
}

bool ExceptionHandler::isSpecificInterruptEnabled(Exception ex){
    switch (ex)
    {
        case Exception::IRQ:
            return (cpu.getMemManager().readWord(REG_IE_ADDR) & 0x1) == 0x1;

    default:
        throw new std::runtime_error("TODO isSpecificInterruptEnabled");
        break;
    }
}

