
    AREA BnFunctions, CODE, READONLY

    GBLS BITS
    GBLA WORDS
    GBLA BYTES
    
WORDS SETA    16
BYTES SETA    64
BITS  SETS  "512"

#include "bn-asm.inc"
    
WORDS SETA    32
BYTES SETA   128
BITS  SETS "1024"

#include "bn-asm.inc"

WORDS SETA    64
BYTES SETA   256
BITS  SETS "2048"

#include "bn-asm.inc"

    END