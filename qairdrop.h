#pragma once
#include "stdint.h"
void qairdropStartAirdrop(const char* nodeIp, int nodePort,
                  const char* seed,
                  const char* assetName,
                  const char* unitOfMeasurement,
                  int64_t numberOfUnits,
                  char numberOfDecimalPlaces,
                  uint32_t scheduledTickOffset);

void DistributeToken(const char* nodeIp, int nodePort,
                     const char* seed,
                     const char* pAssetName,
                     uint32_t scheduledTickOffset,
                     const char* token_issuer
                     );