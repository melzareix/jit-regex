//
// Created by Mohamed El Zarei on 05/02/2022.
//

#ifndef ZREGEXSTANDALONE_UTF8_H
#define ZREGEXSTANDALONE_UTF8_H
#include <cstdint>

namespace ZRegex {
  class Utf8 {
  public:
    static const uint32_t MAX_TRANS = 0xFFFFFFF;
    static uint32_t ReadMultiByteCase(const std::string& reader, char firstByte, unsigned byteLen) {
      switch (byteLen) {
        case 2:
          return ((firstByte & 0x1F) << 6) | (reader[1] & 0x3F);
        case 3:
          return ((firstByte & 0xF) << 12) | ((reader[1] & 0x3F) << 6) | (reader[2] & 0x3F);
        case 4:
          return ((firstByte & 0x7) << 18) | ((reader[1] & 0x3F) << 12) | ((reader[2] & 0x3F) << 6)
                 | (reader[3] & 0x3F);
        default:
          return firstByte;
      }
    }


    static constexpr unsigned clz(uint8_t a) noexcept {
      assert(a);
      return __builtin_clz(a) - (8 * (sizeof(unsigned) - sizeof(uint8_t)));
    }
    //---------------------------------------------------------------------------
    static inline unsigned multiByteSequenceLength(char firstByte)
    // Compute the length of a multi-byte utf8 sequence from the header byte
    {
      // The header has the form 1...10<bits>, where the number of 1s is the number of bytes.
      unsigned len = Utf8::clz(~firstByte);
      return len ? len : 1;
    }
  };
}  // namespace ZRegex
#endif  // ZREGEXSTANDALONE_UTF8_H
