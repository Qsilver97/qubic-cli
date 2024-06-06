#include "qpool.h"
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

#define QPOOL_ADDRESS "GAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQGNM"
#define QPOOL_CREATE_FN 1
#define QPOOL_GET_LIST_OF_POOL_FN 1
#define QPOOL_ISSUE_ASSET 2
#define QPOOL_ENABLE_TOKEN 3
#define FEE_CREATE_POOL 100000000LL
#define TOKEN_TRANSER_FEE 1000LL // Amount of qus

constexpr int QPOOL_CONTRACT_ID = 7;

enum qPoolFunctionId{
    GetNumberOfEnableToken = 1,
    GetEnableToken = 2,
    PoolList = 3,
};

struct CreateLiquidityPool_input {
    uint64_t NameOfLPToken;       // Name of LP token
    uint64_t swapFee;              // Swap fee in a Pool

    uint64_t initialAmountOfQWALLET;
    uint64_t initialAmountOfQU; 
    uint64_t initialAmount1;
    uint64_t initialAmount2;
    uint64_t initialAmount3;
    uint64_t initialAmount4;

    uint16_t IndexOfToken1;
    uint16_t IndexOfToken2;
    uint16_t IndexOfToken3;
    uint16_t IndexOfToken4;

    uint8_t NumberOfToken;        // Number(maximum 5) of token in a pool

    uint8_t WeightOfQWALLET;
    uint8_t Weight1;
    uint8_t Weight2;
    uint8_t Weight3;
    uint8_t Weight4;
};

struct CreateLiquidityPool_output {
    uint64_t poolAddress;;               // created pool address
};

struct IssueAsset_input {
    uint64_t name;
    int64_t numberOfUnits;
    uint64_t unitOfMeasurement;
    char numberOfDecimalPlaces;
};

struct IssueAsset_output
{
    uint64_t issuedNumberOfShares;
};

struct PoolList_input {
    uint32_t NumberOfPool;
};

struct EnableToken_input
{
    uint64_t assetName;
    uint8_t issuer[32];
};

struct qpoolenableToken_output
{
    uint16_t TokenID;
};

struct GetNumberOfEnableToken_input
{
};

struct GetNumberOfEnableToken_output
{
    uint16_t NumberOfEnableToken;
};

struct GetEnableToken_input 
{
    uint32_t NumberOfToken;
};

struct GetEnableToken_output
{
    uint64_t assetName;
    uint8_t issuer[32];
};

struct PoolList_output {
    uint64_t NameOfLPToken;       // Name of LP token
    uint64_t swapFee;              // Swap fee in a Pool

    uint64_t liquidityOfQWALLET;
    uint64_t liquidityOfQU; 
    uint64_t liquidity1;
    uint64_t liquidity2;
    uint64_t liquidity3;
    uint64_t liquidity4;
    uint64_t totalAmountOfQPT;
    uint64_t totalSupplyByQU;

    uint16_t IndexOfToken1;
    uint16_t IndexOfToken2;
    uint16_t IndexOfToken3;
    uint16_t IndexOfToken4;

    uint8_t NumberOfToken;        // Number(maximum 5) of token in a pool

    uint8_t WeightOfQWALLET;
    uint8_t Weight1;
    uint8_t Weight2;
    uint8_t Weight3;
    uint8_t Weight4;
};

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
                    uint32_t offsetScheduledTick)
{
    char nameOfLPS1[8][8] = {0};
    auto qc = make_qc(nodeIp, nodePort);
    memcpy(nameOfLPS1, nameOfLP, strlen(nameOfLP));
    printf("\n1\n");
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
    getPublicKeyFromIdentity(QPOOL_ADDRESS, destPublicKey);
    ((uint64_t*)destPublicKey)[0] = QPOOL_CONTRACT_ID;
    ((uint64_t*)destPublicKey)[1] = 0;
    ((uint64_t*)destPublicKey)[2] = 0;
    ((uint64_t*)destPublicKey)[3] = 0;
    printf("2\n");
    struct {
        RequestResponseHeader header;
        Transaction transaction;
        CreateLiquidityPool_input ia;
        uint8_t sig[SIGNATURE_SIZE];
    } packet;
    printf("3\n");
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    packet.transaction.amount = amountOfQu + TOKEN_TRANSER_FEE * (number_of_token - 1) + FEE_CREATE_POOL;
    printf("4 %u\n", packet.transaction.amount);
    uint32_t scheduledTick = 0;
    if (offsetScheduledTick < 50000){
        uint32_t currentTick = getTickNumberFromNode(qc);
        scheduledTick = currentTick + offsetScheduledTick;
    } else {
        scheduledTick = offsetScheduledTick;
    }
    printf("5\n");
    packet.transaction.tick = scheduledTick;
    packet.transaction.inputType = QPOOL_CREATE_FN;
    packet.transaction.inputSize = sizeof(CreateLiquidityPool_input);
    printf("6\n");
    // fill the input
    memcpy(&packet.ia.NameOfLPToken, nameOfLPS1, 8);
    packet.ia.NumberOfToken = number_of_token;
    printf("\n%u\n", packet.ia.NumberOfToken);

    packet.ia.initialAmountOfQWALLET = amountOfQwallet;
    packet.ia.WeightOfQWALLET = weightOfQwallet;
    packet.ia.initialAmountOfQU = amountOfQu;

    if(number_of_token > 2) {
        packet.ia.IndexOfToken1 = index_of_token[0];
        packet.ia.initialAmount1 = amount_of_token[0];
        packet.ia.Weight1 = weight_of_token[0];
    }
    if(number_of_token > 3) {
        packet.ia.IndexOfToken2 = index_of_token[1];
        packet.ia.initialAmount2 = amount_of_token[1];
        packet.ia.Weight2 = weight_of_token[1];
    }
    if(number_of_token > 4) {
        packet.ia.IndexOfToken3 = index_of_token[2];
        packet.ia.initialAmount3 = amount_of_token[2];
        packet.ia.Weight3 = weight_of_token[2];
    }
    if(number_of_token > 5) {
        packet.ia.IndexOfToken4 = index_of_token[3];
        packet.ia.initialAmount4 = amount_of_token[3];
        packet.ia.Weight4 = weight_of_token[3];
    }
    packet.ia.swapFee = swap_fee;
    printf("%u\n", packet.ia.swapFee);
    printf("%u %u %u\n", packet.ia.initialAmount1, packet.ia.IndexOfToken1, packet.ia.Weight1);
    printf("%u %u %u\n", packet.ia.initialAmount2, packet.ia.IndexOfToken2, packet.ia.Weight2);
    printf("%u %u %u\n", packet.ia.initialAmount3, packet.ia.IndexOfToken3, packet.ia.Weight3);
    printf("%u %u %u\n", packet.ia.initialAmount4, packet.ia.IndexOfToken4, packet.ia.Weight4);
    
    // sign the packet
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction) + sizeof(CreateLiquidityPool_input),
                   digest,
                   32);
    sign(subSeed, sourcePublicKey, digest, signature);
    memcpy(packet.sig, signature, SIGNATURE_SIZE);
    // set header
    packet.header.setSize(sizeof(packet.header)+sizeof(Transaction)+sizeof(CreateLiquidityPool_input)+ SIGNATURE_SIZE);
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);

    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction)+sizeof(CreateLiquidityPool_input)+ SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("Transaction has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.ia));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", scheduledTick, txHash);
    LOG("to check your tx confirmation status\n");
}

void qpoolIssueAsset(const char* nodeIp, int nodePort,
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
    getPublicKeyFromIdentity(QPOOL_ADDRESS, destPublicKey);
    ((uint64_t*)destPublicKey)[0] = QPOOL_CONTRACT_ID;
    ((uint64_t*)destPublicKey)[1] = 0;
    ((uint64_t*)destPublicKey)[2] = 0;
    ((uint64_t*)destPublicKey)[3] = 0;

    struct {
        RequestResponseHeader header;
        Transaction transaction;
        IssueAsset_input ia;
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
    packet.transaction.inputType = QPOOL_ISSUE_ASSET;
    packet.transaction.inputSize = sizeof(IssueAsset_input);

    // fill the input
    memcpy(&packet.ia.name, assetNameS1, 8);
    memcpy(&packet.ia.unitOfMeasurement, UoMS1, 8);
    packet.ia.numberOfUnits = numberOfUnits;
    packet.ia.numberOfDecimalPlaces = numberOfDecimalPlaces;
    // sign the packet
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction) + sizeof(IssueAsset_input),
                   digest,
                   32);
    sign(subSeed, sourcePublicKey, digest, signature);
    memcpy(packet.sig, signature, SIGNATURE_SIZE);
    // set header
    packet.header.setSize(sizeof(packet.header)+sizeof(Transaction)+sizeof(IssueAsset_input)+ SIGNATURE_SIZE);
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);

    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction)+sizeof(IssueAsset_input)+ SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("Transaction has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.ia));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", scheduledTick, txHash);
    LOG("to check your tx confirmation status\n");
}

void getpool(QCPtr qc, uint32_t numberOfPool, PoolList_output& result)
{
    struct {
        RequestResponseHeader header;
        RequestContractFunction rcf;
        PoolList_input input;
    } packet;
    packet.header.setSize(sizeof(packet));
    packet.header.randomizeDejavu();
    packet.header.setType(RequestContractFunction::type());
    packet.rcf.inputSize = sizeof(PoolList_input);
    packet.rcf.inputType = qPoolFunctionId::PoolList;
    packet.rcf.contractIndex = QPOOL_CONTRACT_ID;
    packet.input.NumberOfPool = numberOfPool;
    qc->sendData((uint8_t *) &packet, packet.header.size());
    std::vector<uint8_t> buffer;
    qc->receiveDataAll(buffer);
    uint8_t* data = buffer.data();
    int recvByte = buffer.size();
    int ptr = 0;
    while (ptr < recvByte)
    {
        auto header = (RequestResponseHeader*)(data+ptr);
        if (header->type() == RespondContractFunction::type()){
            auto oup = (PoolList_output*)(data + ptr + sizeof(RequestResponseHeader));
            result = *oup;
        }
        ptr+= header->size();
    }
}

void qpoolgetInfor(const char* nodeIp, int nodePort,
                  const char* seed,
                  uint32_t number_of_pool,
                  uint32_t scheduledTickOffset
) {
    auto qc = make_qc(nodeIp, nodePort);
    PoolList_output pool;
    memset(&pool, 0, sizeof(PoolList_output));
    getpool(qc, number_of_pool, pool);

    LOG("number of token: %llu\n", pool.NameOfLPToken);
    LOG("number of token: %u\n", pool.NumberOfToken);
    char issueroftoken[128] = {0};

    LOG("liquidity of QWALLET: %llu\n",  pool.liquidityOfQWALLET);
    LOG("Weight of QWALLET: %u\n",  pool.WeightOfQWALLET);
    LOG("liquidity of QU: %llu\n",  pool.liquidityOfQU);

    if(pool.NumberOfToken > 2) {
        LOG("Address of token1: %u ", pool.IndexOfToken1);
        LOG("Weight of token1: %u ", pool.Weight1);
        LOG("liquidity of token1: %llu\n",  pool.liquidity1);
    }

    if(pool.NumberOfToken > 3) {
        LOG("Address of token2: %u ", pool.IndexOfToken2);
        LOG("Weight of token2: %u ", pool.Weight2);
        LOG("liquidity of token2: %llu\n",  pool.liquidity2);
    }

    if(pool.NumberOfToken > 4) {
        LOG("Address of token3: %u ", pool.IndexOfToken3);
        LOG("Weight of token3: %u ", pool.Weight3);
        LOG("liquidity of token3: %llu\n",  pool.liquidity3);
    }

    if(pool.NumberOfToken > 5) {    
        LOG("Address of token4: %u ", pool.IndexOfToken4);
        LOG("Weight of token4: %u ", pool.Weight4);
        LOG("liquidity of token4: %llu\n",  pool.liquidity4);
    }

    LOG("Swap fee in pool%u: %llu\n",number_of_pool+1, pool.swapFee);
    LOG("Total Amount Of QPT in pool%u: %llu\n", number_of_pool+1 ,pool.totalAmountOfQPT);
    LOG("Total value of tokens in pool%u: %llu\n", number_of_pool+1, pool.totalSupplyByQU);
}

void qpoolenableToken(const char* nodeIp, int nodePort,
                    const char* seed,
                    char* assetname,
                    char* issuerOfAsset,
                    uint32_t scheduledTickOffset)
{
    auto qc = make_qc(nodeIp, nodePort);
    char assetNameS1[8] = {0};
    memcpy(assetNameS1, assetname, strlen(assetname));
    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};
    uint8_t destPublicKey[32] = {0};
    uint8_t subSeed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    uint8_t TokenIssuerPublicKey[32] = {0};
    char txHash[128] = {0};

    if (strlen(issuerOfAsset) != 60){
        LOG("WARNING: Stop supporting hex format, please use qubic format 60-char length addresses\n");
        exit(0);
    }
    getPublicKeyFromIdentity(issuerOfAsset, TokenIssuerPublicKey);
    getSubseedFromSeed((uint8_t*)seed, subSeed);
    getPrivateKeyFromSubSeed(subSeed, privateKey);
    getPublicKeyFromPrivateKey(privateKey, sourcePublicKey);
    getPublicKeyFromIdentity(QPOOL_ADDRESS, destPublicKey);
    ((uint64_t*)destPublicKey)[0] = QPOOL_CONTRACT_ID;
    ((uint64_t*)destPublicKey)[1] = 0;
    ((uint64_t*)destPublicKey)[2] = 0;
    ((uint64_t*)destPublicKey)[3] = 0;
    struct {
        RequestResponseHeader header;
        Transaction transaction;
        EnableToken_input ia;
        uint8_t sig[SIGNATURE_SIZE];
    } packet;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    packet.transaction.amount = 100000000;
    uint32_t scheduledTick = 0;
    if (scheduledTickOffset < 50000){
        uint32_t currentTick = getTickNumberFromNode(qc);
        scheduledTick = currentTick + scheduledTickOffset;
    } else {
        scheduledTick = scheduledTickOffset;
    }
    packet.transaction.tick = scheduledTick;
    packet.transaction.inputType = QPOOL_ENABLE_TOKEN;
    packet.transaction.inputSize = sizeof(EnableToken_input);

    // fill the input
    memcpy(&packet.ia.assetName, assetNameS1, 8);
    memcpy(packet.ia.issuer , TokenIssuerPublicKey, 32);
    for(int16_t i = 0 ; i < 32 ; i++) printf("%u ", packet.ia.issuer[i]);
    // sign the packet
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction) + sizeof(EnableToken_input),
                   digest,
                   32);
    sign(subSeed, sourcePublicKey, digest, signature);
    memcpy(packet.sig, signature, SIGNATURE_SIZE);
    // set header
    packet.header.setSize(sizeof(packet.header)+sizeof(Transaction)+sizeof(EnableToken_input)+ SIGNATURE_SIZE);
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);

    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction)+sizeof(EnableToken_input)+ SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("Transaction has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.ia));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", scheduledTick, txHash);
    LOG("to check your tx confirmation status\n");
}

void qpoolgetnumberofenableToken(const char* nodeIp, int nodePort,
                  const char* seed,
                  uint32_t scheduledTickOffset
) {
    auto qc = make_qc(nodeIp, nodePort);

    struct {
        RequestResponseHeader header;
        RequestContractFunction rcf;
    } packet;


    packet.header.setSize(sizeof(packet));
    packet.header.randomizeDejavu();
    packet.header.setType(RequestContractFunction::type());
    packet.rcf.inputSize = 0;
    packet.rcf.inputType = qPoolFunctionId::GetNumberOfEnableToken;
    packet.rcf.contractIndex = QPOOL_CONTRACT_ID;
    qc->sendData((uint8_t *) &packet, packet.header.size());
    std::vector<uint8_t> buffer;
    qc->receiveDataAll(buffer);
    uint8_t* data = buffer.data();
    int recvByte = buffer.size();
    int ptr = 0;
    GetNumberOfEnableToken_output result;
    while (ptr < recvByte)
    {
        auto header = (RequestResponseHeader*)(data+ptr);
        if (header->type() == RespondContractFunction::type()){
            auto oup = (GetNumberOfEnableToken_output*)(data + ptr + sizeof(RequestResponseHeader));
            result = *oup;
        }
        ptr+= header->size();
    }
    LOG("Number of enable token in pool:  %u\n", result.NumberOfEnableToken);

}


void qpoolgetenableToken(const char* nodeIp, int nodePort,
                    const char* seed,
                    uint32_t tokenID,
                    uint32_t scheduledTickOffset)
{
    auto qc = make_qc(nodeIp, nodePort);

    struct {
        RequestResponseHeader header;
        RequestContractFunction rcf;
        GetEnableToken_input input;
    } packet;


    packet.header.setSize(sizeof(packet));
    packet.header.randomizeDejavu();
    packet.header.setType(RequestContractFunction::type());
    packet.rcf.inputSize = sizeof(GetEnableToken_input);
    packet.rcf.inputType = qPoolFunctionId::GetEnableToken;
    packet.rcf.contractIndex = QPOOL_CONTRACT_ID;
    packet.input.NumberOfToken = tokenID;
    qc->sendData((uint8_t *) &packet, packet.header.size());
    std::vector<uint8_t> buffer;
    qc->receiveDataAll(buffer);
    uint8_t* data = buffer.data();
    int recvByte = buffer.size();
    int ptr = 0;
    GetEnableToken_output result;
    while (ptr < recvByte)
    {
        auto header = (RequestResponseHeader*)(data+ptr);
        if (header->type() == RespondContractFunction::type()){
            auto oup = (GetEnableToken_output*)(data + ptr + sizeof(RequestResponseHeader));
            result = *oup;
        }
        ptr+= header->size();
    }
    LOG("Name of enable token:  %llu\n", result.assetName);
    for(int i = 0 ; i < 32; i++) LOG("%u ", result.issuer[i]);
}