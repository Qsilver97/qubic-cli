#pragma once
#include "stdint.h"

void QpoolCreate(const char* nodeIp, int nodePort,
                const char* seed,
                char* nameOfLP,
                int64_t number_of_token,
                int64_t amountOfQwallet,
                int8_t weightOfQwallet,
                int64_t amountOfQu,
                int16_t* index_of_token,
                int64_t* amount_of_token,
                int8_t* weight_of_token,
                int32_t* amount_of_microtoken,
                bool* type_of_token,
                int64_t swap_fee,
                uint32_t offsetScheduledTick);

void qpoolIssueAsset(const char* nodeIp, int nodePort,
                  const char* seed,
                  const char* assetName,
                  const char* unitOfMeasurement,
                  int64_t numberOfUnits,
                  char numberOfDecimalPlaces,
                  uint32_t scheduledTickOffset);

void qpoolgetInfor(const char* nodeIp, int nodePort,
                  const char* seed,
                  uint32_t number_of_pool,
                  uint32_t scheduledTickOffset
);

void qpoolenableToken(const char* nodeIp, int nodePort,
                    const char* seed,
                    char* assetname,
                    char* issuerOfAsset,
                    uint16_t contractIndex,
                    bool typeoftoken,
                    uint32_t scheduledTickOffset);

void qpoolgetnumberofenableToken(const char* nodeIp, int nodePort,
                    const char* seed,
                    uint32_t scheduledTickOffset);

void qpoolgetenableToken(const char* nodeIp, int nodePort,
                    const char* seed,
                    uint32_t tokenID,
                    bool typeoftoken,
                    uint32_t scheduledTickOffset);
                    

void qpoolswap(char* nodeIp, int nodePort,
                    const char* seed,
                    uint64_t Amountoftoken1,
                    uint16_t indexOfToken1,
                    uint16_t indexOfToken2,
                    uint16_t NumberOfPool,
                    bool typeoftoken1,
                    bool typeoftoken2,
                    uint32_t scheduledTickOffset);

void qpooldepositexpensivetoken(char* nodeIp, int nodePort,
                    const char* seed,
                    uint64_t Amountoftoken,
                    uint16_t indexOfToken,
                    uint32_t scheduledTickOffset);

void qpoolwithdrawexpensivetoken(char* nodeIp, int nodePort,
                    const char* seed,
                    uint64_t Amountoftoken,
                    uint16_t indexOfToken,
                    uint32_t scheduledTickOffset);

void qpoolgetamountofexpensivetokenuserdeposited(const char* nodeIp, int nodePort,
                    const char* seed,
                    char* user,
                    uint16_t indexOfToken,
                    uint32_t scheduledTickOffset);

void qpoolbignumberTostring(char* nodeIp, int nodePort,
                    const char* seed,
                    int64_t number,
                    uint32_t scheduledTickOffset);

void qpoolbignumbergetstatus(char* nodeIp, int nodePort,
                    const char* seed,
                    uint32_t scheduledTickOffset);

void qpoolbigstringTonumber(char* nodeIp, int nodePort,
                    const char* seed,
                    uint8_t lengthOfNumber,
                    uint8_t* stringnumber,
                    uint32_t scheduledTickOffset);

void qpoolbigplus(char* nodeIp, int nodePort,
                    const char* seed,
                    uint8_t alen,
                    uint8_t* a,
                    uint8_t blen,
                    uint8_t* b,
                    uint32_t scheduledTickOffset);

void qpoolbigminus(char* nodeIp, int nodePort,
                    const char* seed,
                    uint8_t alen,
                    uint8_t* a,
                    uint8_t blen,
                    uint8_t* b,
                    uint32_t scheduledTickOffset);

void qpoolbigmulti(char* nodeIp, int nodePort,
                    const char* seed,
                    uint8_t alen,
                    uint8_t* a,
                    uint8_t blen,
                    uint8_t* b,
                    uint32_t scheduledTickOffset);

void qpoolbigdiv(char* nodeIp, int nodePort,
                    const char* seed,
                    uint8_t alen,
                    uint8_t* a,
                    uint8_t blen,
                    uint8_t* b,
                    uint32_t scheduledTickOffset);

void qpoolbigorequalcomparison(char* nodeIp, int nodePort,
                    const char* seed,
                    uint8_t alen,
                    uint8_t* a,
                    uint8_t blen,
                    uint8_t* b,
                    uint32_t scheduledTickOffset);

void qpoolsmallorequalcomparison(char* nodeIp, int nodePort,
                    const char* seed,
                    uint8_t alen,
                    uint8_t* a,
                    uint8_t blen,
                    uint8_t* b,
                    uint32_t scheduledTickOffset);

void qpoolbigcomparison(char* nodeIp, int nodePort,
                    const char* seed,
                    uint8_t alen,
                    uint8_t* a,
                    uint8_t blen,
                    uint8_t* b,
                    uint32_t scheduledTickOffset);

void qpoolsmallcomparison(char* nodeIp, int nodePort,
                    const char* seed,
                    uint8_t alen,
                    uint8_t* a,
                    uint8_t blen,
                    uint8_t* b,
                    uint32_t scheduledTickOffset);

void qpoolbigmodulus(char* nodeIp, int nodePort,
                    const char* seed,
                    uint8_t alen,
                    uint8_t* a,
                    uint8_t blen,
                    uint8_t* b,
                    uint32_t scheduledTickOffset);