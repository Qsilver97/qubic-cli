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
                    uint32_t scheduledTickOffset);

void qpoolgetnumberofenableToken(const char* nodeIp, int nodePort,
                    const char* seed,
                    uint32_t scheduledTickOffset);

void qpoolgetenableToken(const char* nodeIp, int nodePort,
                    const char* seed,
                    uint32_t tokenID,
                    uint32_t scheduledTickOffset);
                    

void qpoolswap(char* nodeIp, int nodePort,
                    const char* seed,
                    uint64_t Amountoftoken1,
                    uint16_t indexOfToken1,
                    uint16_t indexOfToken2,
                    uint8_t NumberOfPool,
                    uint32_t scheduledTickOffset);