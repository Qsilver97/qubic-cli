#include "qairdrop.h"
#include "keyUtils.h"
#include "structs.h"
#include "logger.h"
#include "nodeUtils.h"
#include "K12AndKeyUtil.h"
#include "connection.h"
#include "walletUtils.h"
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include "stdio.h"
#define QAIRDROP_ADDRESS "FAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAYWJB"
#define QAIRDROP_ISSUE_ASSET_FN 1
#define QAIRDROP_TRANSFER_SHARE_FN 2
#define QAIRDROP_ONE_TRANSFER_SHARE_FN 3

constexpr int QAIRDROP_CONTRACT_ID = 5;
enum qAirdropFunctionId{
    Fees = 1,
};
struct StartAirdrop_input {
    uint64_t name;
    int64_t numberOfUnits;
    char numberOfDecimalPlaces;
    uint64_t unitOfMeasurement;
};

struct DistributeToken_input
{
    uint8_t issuer[32];
    uint64_t assetName;
};

Fees_output getAirdropFees(QCPtr qc)
{
    struct {
        RequestResponseHeader header;
        RequestContractFunction rcf;
    } packet;
    packet.header.setSize(sizeof(packet));
    packet.header.randomizeDejavu();
    packet.header.setType(RequestContractFunction::type());
    packet.rcf.inputSize = 0;
    packet.rcf.inputType = qAirdropFunctionId::Fees;
    packet.rcf.contractIndex = QAIRDROP_CONTRACT_ID;
    qc->sendData((uint8_t *) &packet, packet.header.size());
    auto fee = qc->receivePacketAs<Fees_output>();
    return fee;
}

struct TransferToken_input
{
    uint8_t issuer[32];
    unsigned long long assetName;
    long long numberOfUnits;
    uint8_t newOwnerAndPossessor[32];
};

void StartAirdrop_func(const char* nodeIp, int nodePort,
                  const char* seed,
                  const char* assetName,
                  const char* unitOfMeasurement,
                  int64_t numberOfUnits,
                  char numberOfDecimalPlaces,
                  uint32_t scheduledTickOffset)
{
    auto qc = make_qc(nodeIp, nodePort);
    char assetNameS1[8] = {0};
    char UoMS1[8] = {0};
    memcpy(assetNameS1, assetName, strlen(assetName));
    for (int i = 0; i < 7; i++) UoMS1[i] = unitOfMeasurement[i] - 48;
    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};
    uint8_t destPublicKey[32] = {0};
    uint8_t subSeed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    char txHash[128] = {0};
    getSubseedFromSeed((uint8_t*)seed, subSeed);
    getPrivateKeyFromSubSeed(subSeed, privateKey);
    getPublicKeyFromPrivateKey(privateKey, sourcePublicKey);
    getPublicKeyFromIdentity(QAIRDROP_ADDRESS, destPublicKey);
    ((uint64_t*)destPublicKey)[0] = QAIRDROP_CONTRACT_ID;
    ((uint64_t*)destPublicKey)[1] = 0;
    ((uint64_t*)destPublicKey)[2] = 0;
    ((uint64_t*)destPublicKey)[3] = 0;

    struct {
        RequestResponseHeader header;
        Transaction transaction;
        StartAirdrop_input ia;
        uint8_t sig[SIGNATURE_SIZE];
    } packet;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    packet.transaction.amount = 1000000000;
    uint32_t scheduledTick = 0;
    if (scheduledTickOffset < 50000){
        uint32_t currentTick = getTickNumberFromNode(qc);
        scheduledTick = currentTick + scheduledTickOffset;
    } else {
        scheduledTick = scheduledTickOffset;
    }
    packet.transaction.tick = scheduledTick;
    packet.transaction.inputType = QAIRDROP_ISSUE_ASSET_FN;
    packet.transaction.inputSize = sizeof(StartAirdrop_input);

    // fill the input
    memcpy(&packet.ia.name, assetNameS1, 8);
    memcpy(&packet.ia.unitOfMeasurement, UoMS1, 8);
    packet.ia.numberOfUnits = numberOfUnits;
    packet.ia.numberOfDecimalPlaces = numberOfDecimalPlaces;
    // sign the packet
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction) + sizeof(StartAirdrop_input),
                   digest,
                   32);
    sign(subSeed, sourcePublicKey, digest, signature);
    memcpy(packet.sig, signature, SIGNATURE_SIZE);
    // set header
    packet.header.setSize(sizeof(packet.header)+sizeof(Transaction)+sizeof(StartAirdrop_input)+ SIGNATURE_SIZE);
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);

    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction)+sizeof(StartAirdrop_input)+ SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("Transaction has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.ia));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", scheduledTick, txHash);
    LOG("to check your tx confirmation status\n");
}

void DistributeToken(const char* nodeIp, int nodePort,
                     const char* seed,
                     const char* pAssetName,
                     uint32_t scheduledTickOffset,
                     const char* token_issuer
                     )
{
    auto qc = make_qc(nodeIp, nodePort);
    uint8_t issuer[32] = {0};
    if (strlen(token_issuer) != 60){
        LOG("WARNING: Stop supporting hex format, please use qubic format 60-char length addresses\n");
        exit(0);
    }
    getPublicKeyFromIdentity(token_issuer, issuer);
    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};
    uint8_t destPublicKey[32] = {0};
    uint8_t subSeed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    char txHash[128] = {0};
    char assetNameU1[8] = {0};

    memcpy(assetNameU1, pAssetName, strlen(pAssetName));

    getSubseedFromSeed((uint8_t*)seed, subSeed);
    getPrivateKeyFromSubSeed(subSeed, privateKey);
    getPublicKeyFromPrivateKey(privateKey, sourcePublicKey);
    getPublicKeyFromIdentity(QAIRDROP_ADDRESS, destPublicKey);
    ((uint64_t*)destPublicKey)[0] = QAIRDROP_CONTRACT_ID;
    ((uint64_t*)destPublicKey)[1] = 0;
    ((uint64_t*)destPublicKey)[2] = 0;
    ((uint64_t*)destPublicKey)[3] = 0;
    
    struct {
        RequestResponseHeader header;
        Transaction transaction;
        DistributeToken_input ta;
        uint8_t sig[SIGNATURE_SIZE];
    } packet;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    packet.transaction.amount = 1000000;
    uint32_t currentTick = getTickNumberFromNode(qc);
    uint32_t scheduledTick = currentTick + scheduledTickOffset;
    packet.transaction.tick = scheduledTick;
    packet.transaction.inputType = QAIRDROP_TRANSFER_SHARE_FN;
    packet.transaction.inputSize = sizeof(DistributeToken_input);

    // fill the input
    memcpy(&packet.ta.assetName, assetNameU1, 8);
    memcpy(packet.ta.issuer, issuer, 32);
    // sign the packet
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction) + sizeof(DistributeToken_input),
                   digest,
                   32);
    sign(subSeed, sourcePublicKey, digest, signature);
    memcpy(packet.sig, signature, SIGNATURE_SIZE);
    // set header
    packet.header.setSize(sizeof(packet.header)+sizeof(Transaction)+sizeof(DistributeToken_input)+ SIGNATURE_SIZE);
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);
    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction)+sizeof(DistributeToken_input)+ SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("Transaction has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.ta));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", scheduledTick, txHash);
    LOG("to check your tx confirmation status\n");
}

void AirdropOneTransferAsset(const char* nodeIp, int nodePort,
                     const char* seed,
                     const char* pAssetName,
                     const char* pIssuerInQubicFormat,
                     const char* newOwnerIdentity,
                     long long numberOfUnits,
                     uint32_t scheduledTickOffset)
{
    auto qc = make_qc(nodeIp, nodePort);
    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};
    uint8_t destPublicKey[32] = {0};
    uint8_t subSeed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    uint8_t issuer[32] = {0};
    uint8_t newOwnerPublicKey[32] = {0};
    char txHash[128] = {0};
    char assetNameU1[8] = {0};

    memcpy(assetNameU1, pAssetName, strlen(pAssetName));
    if (strlen(pIssuerInQubicFormat) != 60){
        LOG("WARNING: Stop supporting hex format, please use qubic format 60-char length addresses\n");
        exit(0);
    }
    getPublicKeyFromIdentity(pIssuerInQubicFormat, issuer);

    getSubseedFromSeed((uint8_t*)seed, subSeed);
    getPrivateKeyFromSubSeed(subSeed, privateKey);
    getPublicKeyFromPrivateKey(privateKey, sourcePublicKey);
    getPublicKeyFromIdentity(QAIRDROP_ADDRESS, destPublicKey);
    getPublicKeyFromIdentity(newOwnerIdentity, newOwnerPublicKey);
    ((uint64_t*)destPublicKey)[0] = QAIRDROP_CONTRACT_ID;
    ((uint64_t*)destPublicKey)[1] = 0;
    ((uint64_t*)destPublicKey)[2] = 0;
    ((uint64_t*)destPublicKey)[3] = 0;
    
    struct {
        RequestResponseHeader header;
        Transaction transaction;
        TransferToken_input ta;
        uint8_t sig[SIGNATURE_SIZE];
    } packet;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    packet.transaction.amount = 1000000;
    uint32_t currentTick = getTickNumberFromNode(qc);
    uint32_t scheduledTick = currentTick + scheduledTickOffset;
    packet.transaction.tick = scheduledTick;
    packet.transaction.inputType = QAIRDROP_ONE_TRANSFER_SHARE_FN;
    packet.transaction.inputSize = sizeof(TransferToken_input);

    // fill the input
    memcpy(&packet.ta.assetName, assetNameU1, 8);
    memcpy(packet.ta.issuer, issuer, 32);
    memcpy(packet.ta.newOwnerAndPossessor, newOwnerPublicKey, 32);
    packet.ta.numberOfUnits = numberOfUnits;
    // sign the packet
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction) + sizeof(TransferToken_input),
                   digest,
                   32);
    sign(subSeed, sourcePublicKey, digest, signature);
    memcpy(packet.sig, signature, SIGNATURE_SIZE);
    // set header
    packet.header.setSize(sizeof(packet.header)+sizeof(Transaction)+sizeof(TransferToken_input)+ SIGNATURE_SIZE);
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);
    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction)+sizeof(TransferToken_input)+ SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("Transaction has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.ta));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", scheduledTick, txHash);
    LOG("to check your tx confirmation status\n");

}

void qairdropStartAirdrop(const char* nodeIp, int nodePort,
                  const char* seed,
                  const char* assetName,
                  const char* unitOfMeasurement,
                  int64_t numberOfUnits,
                  char numberOfDecimalPlaces,
                  uint32_t scheduledTickOffset) {
    auto qc = make_qc(nodeIp, nodePort);
    printf("airdroairdrop trigger!\n");

    Fees_output fees = getAirdropFees(qc);
    printf("airdropStartFee: %u\ntransferFee: %u\n", fees.airdropStartFee, fees.transferFee);
    
    StartAirdrop_func(nodeIp,nodePort,seed,assetName,unitOfMeasurement,numberOfUnits,numberOfDecimalPlaces,scheduledTickOffset);
}