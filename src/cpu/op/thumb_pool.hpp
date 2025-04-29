#ifndef _THUMB_POOL_
#define _THUMB_POOL_

class ARM7TDMI;

#include "thumb/thumb_opcode.hpp"
#include <memory>
#include <unordered_map>

class ThumbPool {
  public:
    ThumbPool(ARM7TDMI& cpu);
    ThumbOpCode* getThumbInstance(ThumbOpCode::OpCodeEnum key) const;

  private:
    std::unordered_map<ThumbOpCode::OpCodeEnum, std::unique_ptr<ThumbOpCode>> thumbPool{};
};

#endif