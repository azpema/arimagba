#include "exception_handler.hpp"

//ExceptionHandler::ExceptionHandler(ARM7TDMI &cpu) : cpu(cpu), mustHandleException(false){
ExceptionHandler::ExceptionHandler(ARM7TDMI &cpu) : cpu(cpu),
                                                    io(cpu.getMemManager().getIOregisters()), 
                                                    IE(reinterpret_cast<uint16_t*>(io + (REG_ADDR::IE - MemoryManager::IO_REGISTERS_OFFSET_START))),
                                                    IF(reinterpret_cast<uint16_t*>(io + (REG_ADDR::IF - MemoryManager::IO_REGISTERS_OFFSET_START))),
                                                    IME(reinterpret_cast<uint16_t*>(io + (REG_ADDR::IME - MemoryManager::IO_REGISTERS_OFFSET_START)))
                                                    {}

void ExceptionHandler::raiseException(Exception ex, Interrupt inter){
    this->ex = ex;
    //this->mustHandleException = true;
    //this->inter = inter;

    if(ex == Exception::IRQ){
        //if(isMasterInterruptEnabled() && isSpecificInterruptEnabled(inter)){
            setInterruptIF(inter, true);
        //}
    }else if(ex == Exception::SWI){
        doHandleException();
    }else{
        throw std::runtime_error("Unimplemented raiseException exception type");
    }
}

void ExceptionHandler::doHandleException(){
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
            returnPC -= 2;  // OK
    
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

void ExceptionHandler::handleException(){
    if((*IF & *IE) != 0){
        // TODO check if CPSR irq enabled!!¿?¿¿?
        //if(ex != Exception::IRQ || (isMasterInterruptEnabled() && isSpecificInterruptEnabled(inter))){
        if(ex != Exception::IRQ || (isMasterInterruptEnabled())){
            doHandleException();
        }
        //mustHandleException = false;
    }

}

bool ExceptionHandler::isMasterInterruptEnabled(){
    uint16_t nose = *IME;
    return (nose & 0x1) == 0x1;
}

bool ExceptionHandler::isSpecificInterruptEnabled(Interrupt inter){
    switch (inter)
    {
        case Interrupt::VBLANK:
            return (*IE & 0b1) == 0x1;

        case Interrupt::HBLANK:
            return ((*IE & 0b10) >> 1) == 0x1;

        case Interrupt::VCOUNT:
            return ((*IE & 0b100) >> 2) == 0x1;

    default:
        throw std::runtime_error("TODO isSpecificInterruptEnabled");
        break;
    }
}

void ExceptionHandler::setInterruptIF(Interrupt inter, bool raise){
    uint32_t newVal = *IF;
    switch(inter){
        case Interrupt::VBLANK:
            newVal = Utils::setRegBits(newVal, 0b0000000000000001, (raise ? 1 : 0));
            break;
        case Interrupt::HBLANK:
            newVal = Utils::setRegBits(newVal, 0b0000000000000010, (raise ? 1 : 0) << 1);
            break;
        case Interrupt::VCOUNT:
            newVal = Utils::setRegBits(newVal, 0b0000000000000100, (raise ? 1 : 0) << 2);
            break;
        default:
            throw std::runtime_error("TODO setInterruptIF");
            break;
    }
    *IF = newVal;
}

