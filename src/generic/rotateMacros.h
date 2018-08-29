
#define rotateLeft(variableName, bitsToRotate, numberOfBits) \
    variableName = (variableName << bitsToRotate) | (variableName >> (numberOfBits - bitsToRotate));

#define rotateRight(variableName, bitsToRotate, numberOfBits) \
    variableName = (variableName >> bitsToRotate) | (variableName << (numberOfBits - bitsToRotate));

#define rotateLeft128(variableNameLow, variableNameHigh, bitsToRotate, tempVariable) \
    if (bitsToRotate < 64) { \
        tempVariable = variableNameLow >> (64 - bitsToRotate); \
        variableNameLow = (variableNameLow << bitsToRotate) | (variableNameHigh >> (64 - bitsToRotate)); \
        variableNameHigh = (variableNameHigh << bitsToRotate) | tempVariable; \
    } else if (bitsToRotate > 64) { \
        tempVariable = variableNameLow << (bitsToRotate % 64); \
        variableNameLow = (variableNameLow >> (128 - bitsToRotate)) | (variableNameHigh << (bitsToRotate % 64)); \
        variableNameHigh = variableNameHigh >> (128 - bitsToRotate) | tempVariable; \
    } else { \
        tempVariable = variableNameHigh; \
        variableNameHigh = variableNameLow; \
        variableNameLow = tempVariable; \
    }

#define rotateRight128(variableNameLow, variableNameHigh, bitsToRotate, tempVariable) \
    if (bitsToRotate < 64) { \
        tempVariable = variableNameLow << (64 - bitsToRotate); \
        variableNameLow = (variableNameLow >> bitsToRotate) | (variableNameHigh << (64 - bitsToRotate); \
        variableNameHigh = (variableNameHigh >> bitsToRotate) | tempVariable; \
    } else if (bitsToRotate > 64) { \
        tempVariable = variableNameLow >> (bitsToRotate % 64); \
        variableNameLow = (variableNameLow << (128 - bitsToRotate)) | (variableNameHigh >> (bitsToRotate % 64)); \
        variableNameHigh = variableNameHigh << (128 - bitsToRotate) | tempVariable; \
    } else { \
        tempVariable = variableNameHigh; \
        variableNameHigh = variableNameLow; \
        variableNameLow = tempVariable; \
    }
