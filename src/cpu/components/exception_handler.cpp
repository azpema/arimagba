#include "exception_handler.hpp"

ExceptionHandler::ExceptionHandler(ARM7TDMI &cpu) : cpu(cpu), mustHandleException(false){

}

void ExceptionHandler::raiseException(Exception ex, Interrupt inter){
    this->ex = ex;
    this->mustHandleException = true;
    this->inter = inter;

    if(ex == Exception::IRQ){
        setInterruptIF(inter, true);
    }
}

void ExceptionHandler::handleException(){
    if(mustHandleException){
        // TODO check if CPSR irq enabled!!¿?¿¿?
        if(ex != Exception::IRQ || (isMasterInterruptEnabled() && isSpecificInterruptEnabled(inter))){
            PSR::Mode newMode = except2Mode[ex];
            uint32_t newPC = exceptionVector[ex];
            uint32_t returnPC = cpu.getPC();
            /*if(cpu.getCPSR().getTFlag()){
                returnPC += 2;
            }else{
                //returnPC += 2;
            }*/
           if(cpu.getCPSR().getTFlag()){
                if(ex == Exception::SWI)
                    returnPC -= 4;  // OK
            
            }else{
                if(ex == Exception::SWI)
                    returnPC -= 4;  // OK
                else
                    returnPC -= 4;  // OK
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
        mustHandleException = false;
    }

}

bool ExceptionHandler::isMasterInterruptEnabled(){
    return (cpu.getMemManager().readWord(REG_IME_ADDR) & 0x1) == 0x1;
}

bool ExceptionHandler::isSpecificInterruptEnabled(Interrupt inter){
    switch (inter)
    {
        case Interrupt::VBLANK:
            return (cpu.getMemManager().readWord(REG_IE_ADDR) & 0b1) == 0x1;

        case Interrupt::HBLANK:
            return ((cpu.getMemManager().readWord(REG_IE_ADDR) & 0b10) >> 1) == 0x1;

        case Interrupt::VCOUNT:
            return ((cpu.getMemManager().readWord(REG_IE_ADDR) & 0b100) >> 2) == 0x1;

    default:
        throw new std::runtime_error("TODO isSpecificInterruptEnabled");
        break;
    }
}

void ExceptionHandler::setInterruptIF(Interrupt inter, bool raise){
    uint32_t regIf = cpu.getMemManager().readHalfWord(REG_IF_ADDR);
    switch(inter){
        case Interrupt::VBLANK:
            regIf = Utils::setRegBits(regIf, 0, raise ? 1 : 0);
            cpu.getMemManager().store(REG_IF_ADDR, static_cast<uint16_t>(regIf), 2);
            break;
        case Interrupt::HBLANK:
            regIf = Utils::setRegBits(regIf, 1, raise ? 1 : 0);
            cpu.getMemManager().store(REG_IF_ADDR, static_cast<uint16_t>(regIf), 2);
            break;
        default:
            throw new std::runtime_error("TODO setInterruptIF");
            break;
    }
}

