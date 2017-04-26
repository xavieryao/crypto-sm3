//
// Created by 沛然 姚 on 2017/4/19.
//

#ifndef SM3_SM3_H
#define SM3_SM3_H

#include <cstdint>
#include <vector>

typedef uint32_t Word;
typedef uint8_t Byte;

class SM3 {
private:
    static void CF(Word* V, Byte* B);
    static Word P0(Word X);
    static Word P1(Word X);
    static Word FF(int j, Word X, Word Y, Word Z);
    static Word GG(int j, Word X, Word Y, Word Z);
    static Word T(int j);
public:
    static Word* hash(Byte* input, uint64_t inputLen);
    static void word2byte(Word word, Byte *byte);
    static Word byte2word(Byte* byte);
};


#endif //SM3_SM3_H
