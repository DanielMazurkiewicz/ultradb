#define rotateLeft(variableName, bitsToRotate, numberOfBits) \
    ((variableName << bitsToRotate) | (variableName >> (numberOfBits - bitsToRotate)))

#define rotateRight(variableName, bitsToRotate, numberOfBits) \
    ((variableName >> bitsToRotate) | (variableName << (numberOfBits - bitsToRotate)))

#define encodeWithBase(number, baseA, baseB, baseC, baseAll) \
    (number < baseAll ? \
        (number/(baseB*baseC) + ((number/baseC)%baseB)*baseA + (number%baseC)*baseA*baseB) \
    : number)

#define decodeWithBase(number, baseA, baseB, baseC, baseAll) \
    (number < baseAll ? \
        (number/(baseA*baseB) + ((number/baseA)%baseB)*baseC + (number%baseA)*baseB*baseC) \
    : number)

#define reverseLowest8Bits(number) \
    ((number & 0xffffffffffffff00ULL) | ((((number & 0xff) * 0x0202020202ULL & 0x010884422010ULL) % 1023) & 0xffULL))

#define inverseUpper(number, bound, mask) \
    ((number >= bound) ? (mask - number) + bound : number)


// minimum 2 bits for each space
#define getThreeRandomBitSpaces(resultA, resultB, resultC, bitsTotal, randomTemp) \
    resultA = (randomTemp % (bitsTotal - 5)) + 2; \
    randomTemp /= (bitsTotal - 5); \
    resultB = randomTemp % (bitsTotal - resultA - 3) + 2; \
    randomTemp /= (bitsTotal - resultA - 3); \
    resultC = bitsTotal - resultA - resultB; \
    if (randomTemp & 1) { \
        randomTemp = resultA; \
        resultA = resultB; \
        resultB = randomTemp; \
    } \
    if (randomTemp & 2) { \
        randomTemp = resultA; \
        resultA = resultC; \
        resultC = randomTemp; \
    } \


#define getAdjustedBitspaceValues(resultA, resultB, resultC, mask, maskHighestBit, randomTemp, tempSpace) \
    while (resultA * resultB * resultC < maskHighestBit) { \
        tempSpace = (mask / (resultA * resultB * resultC)) - 1; \
        switch (randomTemp % 3) { \
            case 0: \
                randomTemp /= 3; \
                resultA *= (randomTemp % tempSpace) + 2; \
                break; \
            case 1: \
                randomTemp /= 3; \
                resultB *= (randomTemp % tempSpace) + 2; \
                break; \
            case 2: \
                randomTemp /= 3; \
                resultC *= (randomTemp % tempSpace) + 2; \
                break; \
        } \
        randomTemp /= tempSpace; \
    } \


#define bitSpaceValue(bitSpace, randomValue) \
    ((randomValue & ((0x1ULL << bitSpace) - 1)) | (0x1ULL << (bitSpace - 1))) \

#define getThreeBitSpaceValues(resultA, resultB, resultC, tempRandom) \
    resultA = bitSpaceValue(resultA, tempRandom); \
    tempRandom >>= resultA; \
    resultB = bitSpaceValue(resultB, tempRandom); \
    tempRandom >>= resultB; \
    resultC = bitSpaceValue(resultC, tempRandom); \


#define xorHigherWithLower(value, maskHalf, bitsHalfRemaining) \
    (value ^ (( value & maskHalf ) << bitsHalfRemaining))

#define moveNumberUp(number, shiftValue, mask) \
    ((documentId + shiftValue) & mask)

#define moveNumberDown(number, shiftValue, mask) \
    ((documentId - shiftValue) & mask)


#define headerLength 14
#define headerRandoms 8
#define instructionsNumber 4

function (docIdGenerateKey) { // (bits, rounds, randoms)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);

    getLocalData(thisJS, localData, result, status);

    U64 bits;
    n_getU64(bits, args[0]);
    if (bits < 6 || bits > 64) { // not supported
      n_setUndefined(result);
      return result;
    }

    U64* randoms;
    size_t randomsBytes;
    n_getBuffer(randoms, randomsBytes, args[2], status);
    U64 randomsLength = randomsBytes / sizeOfU64;
    if (randomsLength < headerRandoms) {
      n_setUndefined(result);
      return result;
    }

    U64 rounds, roundNumber;
    n_getU64(rounds, args[1]);

    // precalculate length of key data
    U64 randomsPosition = headerRandoms;    //randoms passed before rounds
    U64 length = headerLength + rounds;
    //printf("length pre: %llu \n", length );
    U64 instruction = 0;
    randomsPosition %= randomsLength;
    for (roundNumber = 0; roundNumber < rounds; roundNumber++) {
        instruction += (randoms[randomsPosition++] % (instructionsNumber - 1)) + 1; // prevent same instructions one after another
        instruction %= instructionsNumber;
        switch (instruction) {
            case 0:
                randomsPosition++;
                length++;
                break;
            case 1:
                randomsPosition++;
                length++;
                break;
            case 2:
                randomsPosition+=3;
                length+=4;
                break;
            case 3:
                randomsPosition+=2;
                length+=2;
                break;
        }

        randomsPosition %= randomsLength;
    }
    // end of length calculations

    U64 keyData[length];

    randomsPosition = 0;
    U64 bitsM1 = bits - 1;
    U64 mask = (1ULL << bits) - 1;
    U64 maskHighestBit = 1ULL << (bits - 1);

    // header start
    keyData[0] = rounds;
    keyData[1] = length;
    keyData[2] = mask;
    keyData[3] = bits;

    U64 resultA, resultB, resultC, tempAdditional, 

    temp = randoms[randomsPosition++];
    getThreeRandomBitSpaces(resultA, resultB, resultC, bits, temp);
    temp = randoms[randomsPosition++];
    getThreeBitSpaceValues(resultA, resultB, resultC, temp);
    temp = randoms[randomsPosition++];
    getAdjustedBitspaceValues(resultA, resultB, resultC, mask, maskHighestBit, temp, tempAdditional);

    keyData[4] = resultA;
    keyData[5] = resultB;
    keyData[6] = resultC;
    keyData[7] = resultA * resultB * resultC;


    temp = randoms[randomsPosition++];
    getThreeRandomBitSpaces(resultA, resultB, resultC, bits, temp);
    temp = randoms[randomsPosition++];
    getThreeBitSpaceValues(resultA, resultB, resultC, temp);
    temp = randoms[randomsPosition++];
    getAdjustedBitspaceValues(resultA, resultB, resultC, mask, maskHighestBit, temp, tempAdditional);

    keyData[8] = resultA;
    keyData[9] = resultB;
    keyData[10] = resultC;
    keyData[11] = resultA * resultB * resultC;

    keyData[12] = randoms[randomsPosition++] & mask; //xor
    keyData[13] = randoms[randomsPosition++] & mask; //+
    // end of header

    randomsPosition %= randomsLength;
    U64 i = headerLength; //instruction
    U64 d = i + rounds;   //data
    instruction = 0;
    for (roundNumber = 0; roundNumber < rounds; roundNumber++) {
        instruction += (randoms[randomsPosition++] % (instructionsNumber - 1)) + 1; // prevent same instructions one after another
        instruction %= instructionsNumber; 

        keyData[i++] = instruction;
        randomsPosition %= randomsLength;
        switch (instruction) {
            case 0:
                keyData[d++] = (randoms[randomsPosition++] % bitsM1) + 1;
                randomsPosition %= randomsLength;
                break;

            case 1:
                keyData[d++] = randoms[randomsPosition++];
                randomsPosition %= randomsLength;
                break;

            case 2:
                temp = randoms[randomsPosition++];
                randomsPosition %= randomsLength;
                getThreeRandomBitSpaces(resultA, resultB, resultC, bits, temp);
                temp = randoms[randomsPosition++];
                randomsPosition %= randomsLength;
                getThreeBitSpaceValues(resultA, resultB, resultC, temp);
                temp = randoms[randomsPosition++];
                randomsPosition %= randomsLength;
                getAdjustedBitspaceValues(resultA, resultB, resultC, mask, maskHighestBit, temp, tempAdditional);

                keyData[d++] = resultA;
                keyData[d++] = resultB;
                keyData[d++] = resultC;
                keyData[d++] = resultA * resultB * resultC;
                break;

            case 3:
                keyData[d++] = randoms[randomsPosition++] & mask;
                randomsPosition %= randomsLength;
                keyData[d++] = randoms[randomsPosition++] & mask;
                randomsPosition %= randomsLength;
                break;
        }
    }

    n_newBuffer(result, &keyData, length * sizeOfU64);
    return result;
}

function (docIdEncrypt) { // (documentId, key)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor documentId;
    n_getU64(documentId, args[0]);

    U64* key;
    size_t keyBytes;
    n_getBuffer(key, keyBytes, args[1], status);
    if ( keyBytes <  sizeOfU64 * 5) {
      n_setUndefined(result);
      return result;
    }

    U64 rounds = key[0];
    if (rounds > 1024) { //in case of unreasonable value is provided
      n_setUndefined(result);
      return result;
    }

    U64 length = key[1];
    if (length > 2048) { //in case of unreasonable value is provided
      n_setUndefined(result);
      return result;
    }


    U64 mask = key[2];
    if ( documentId > mask ) {
      n_setUndefined(result);
      return result;
    } 


    U64 bits = key[3];
    U64 bitsHalf = bits >> 1;
    U64 bitsHalfRemaining = bits - bitsHalf;
    U64 maskHalf = (0x1ULL << bitsHalf) - 1;
    U64 valueA = key[4], valueB = key[5], valueC = key[6], valueD = key[7];
    documentId = encodeWithBase(documentId, valueA, valueB, valueC, valueD);
    documentId = moveNumberDown(documentId, valueD, mask);
    valueA = key[8], valueB = key[9], valueC = key[10], valueD = key[11];
    documentId = encodeWithBase(documentId, valueA, valueB, valueC, valueD);
    documentId ^= key[12]; //random
    documentId = xorHigherWithLower(documentId, maskHalf, bitsHalfRemaining);
    documentId = moveNumberUp(documentId, key[13], mask);


    U64 i = headerLength; //instruction
    U64 d = i + rounds;   //data
    while ( rounds-- ) {
        switch ( key[i++] ) {
            case 0:
                valueA = key[d++];
                documentId = rotateLeft(documentId, valueA, bits);
                documentId &= mask;
                break;

            case 1:
                documentId = moveNumberUp(documentId, key[d++], mask);
                documentId = reverseLowest8Bits(documentId);
                break;

            case 2:
                valueA = key[d++];
                valueB = key[d++];
                valueC = key[d++];
                documentId = encodeWithBase(documentId, valueA, valueB, valueC, key[d++]);
                break;

            case 3:
                valueA = key[d++];
                documentId = inverseUpper(documentId, valueA, mask);
                documentId = moveNumberUp(documentId, key[d++], mask);
                break;

            default:
                n_setUndefined(result);
                return result;
                break;
        }
    }

    n_newU64(result, documentId, status);
    return result;
}


function (docIdDecrypt) { // (documentIdEncrypted, key)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor documentId;
    n_getU64(documentId, args[0]);


    U64* key;
    size_t keyBytes;
    n_getBuffer(key, keyBytes, args[1], status);
    if ( keyBytes <  sizeOfU64 * 5) {
      n_setUndefined(result);
      return result;
    }

    U64 rounds = key[0];
    if (rounds > 1024) { //in case of unreasonable value is provided
      n_setUndefined(result);
      return result;
    }

    U64 d = key[1]; // length - data end
    if (d > 4096) { //in case of unreasonable value is provided
      n_setUndefined(result);
      return result;
    }

    U64 mask = key[2];
    if ( documentId > mask ) {
      n_setUndefined(result);
      return result;
    } 

    U64 i = headerLength + rounds; //instructions end
    U64 bits = key[3];
    U64 bitsHalf = bits >> 1;
    U64 bitsHalfRemaining = bits - bitsHalf;
    U64 maskHalf = (0x1ULL << bitsHalf) - 1;

    U64 valueA, valueB, valueC, valueD;

    while ( rounds-- ) {
        switch (key[--i]) {
            case 0:
                valueA = key[--d];
                documentId = rotateRight(documentId, valueA, bits);
                documentId &= mask;
                break;

            case 1:
                documentId = reverseLowest8Bits(documentId);
                documentId = moveNumberDown(documentId, key[--d], mask);
                break;

            case 2:
                valueD = key[--d];
                valueC = key[--d];
                valueB = key[--d];
                valueA = key[--d];
                documentId = decodeWithBase(documentId, valueA, valueB, valueC, valueD);
                break;

            case 3:
                documentId = moveNumberDown(documentId, key[--d], mask);
                valueA = key[--d];
                documentId = inverseUpper(documentId, valueA, mask);
                break;

            default:
                n_setUndefined(result);
                return result;
                break;
        }
    }

    documentId = moveNumberDown(documentId, key[13], mask);
    documentId = xorHigherWithLower(documentId, maskHalf, bitsHalfRemaining);
    documentId ^= key[12]; //random

    valueA = key[8]; valueB = key[9]; valueC = key[10]; valueD = key[11];
    documentId = decodeWithBase(documentId, valueA, valueB, valueC, valueD);

    valueA = key[4]; valueB = key[5]; valueC = key[6]; valueD = key[7];
    documentId = moveNumberUp(documentId, valueD, mask);
    documentId = decodeWithBase(documentId, valueA, valueB, valueC, valueD);

    n_newU64(result, documentId, status);
    return result;
}




#define methodsDocIdObfuscation(obj, methodFunction, status) \
  n_objAssignFunction(obj, methodFunction, docIdEncrypt, status); \
  n_objAssignFunction(obj, methodFunction, docIdDecrypt, status); \
  n_objAssignFunction(obj, methodFunction, docIdGenerateKey, status); \


  


