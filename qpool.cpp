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
#define QPOOL_SWAP 4
#define QPOOL_BIGNUMBERTO_STRING 10
#define QPOOL_BIGSTRINGTO_NUMBER 11
#define QPOOL_BIGPLUS 12
#define QPOOL_BIGMINUS 13
#define QPOOL_BIGMULTI 14
#define QPOOL_BIGDIV 15
#define QPOOL_BIGMODULUS 16
#define QPOOL_BIGOREQUALCOMPARSIN 17
#define QPOOL_SMALLOREQUALCOMPARISON 18
#define QPOOL_BIGCOMPARISON 19
#define QPOOL_SMALLCOMPARISON 20
#define FEE_CREATE_POOL 100000000LL
#define TOKEN_TRANSER_FEE 1000LL // Amount of qus

constexpr int QPOOL_CONTRACT_ID = 7;

enum qPoolFunctionId{
    GetNumberOfEnableToken = 1,
    GetEnableToken = 2,
    PoolList = 3,
    GetValueOfToken = 4,
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

struct Swap_input {
    uint64_t AmountOfToken1;

    uint16_t IndexOfToken1;
    uint16_t IndexOfToken2;

    uint16_t Poolnum;
};

struct Swap_output {
    uint64_t AmountOfToken2;
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


struct GetValueOfToken_input {
    uint16_t IndexOfToken;
    uint16_t Poolnum;
};

struct GetValueOfToken_output {
    uint64_t ValueOfToken;
};

struct BIGNumberToString_input {
    int64_t a;
};

struct BIGNumberToString_output {
    uint8_t len;
    uint8_t result[128];
};

struct GetBIGStatus_input {

};

struct GetBIGStatus_output {
    int64_t BIGTest;
    uint8_t BIGStringNumberLen;
    uint8_t BIGStringNumber[128];
    bool BIGTestComparisonResult;
};

struct BIGStringToNumber_input {
    uint8_t len;
    uint8_t a[128];
};

struct BIGStringToNumber_output {
    int64_t result;
};

struct BIGPlus_input {
    uint8_t alen;
    uint8_t blen;
    uint8_t a[128];
    uint8_t b[128];
};

struct BIGPlus_output {
    uint8_t resultlen;
    uint8_t result[128];
};

struct BIGMinus_input {
    uint8_t alen;
    uint8_t blen;
    uint8_t a[128];
    uint8_t b[128];
};

struct BIGMinus_output {
    uint8_t resultlen;
    uint8_t result[128];
};

struct BIGMultiple_input {
    uint8_t alen;
    uint8_t blen;
    uint8_t a[128];
    uint8_t b[128];
};

struct BIGMultiple_output {
    uint8_t resultlen;
    uint8_t result[128];
};

struct BIGDiv_input {
    uint8_t alen;
    uint8_t blen;
    uint8_t a[128];
    uint8_t b[128];
};

struct BIGDiv_output {
    uint8_t resultlen;
    uint8_t result[128];
};

struct BIGModulus_input {
    uint8_t alen;
    uint8_t blen;
    uint8_t a[128];
    uint8_t b[128];
};

struct BIGModulus_output {
    uint8_t resultlen;
    uint8_t result[128];
};

struct BIGBigOrEqualComparison_input {
    uint8_t alen;
    uint8_t blen;
    uint8_t a[128];
    uint8_t b[128];
};

struct BIGBigOrEqualComparison_output {
    bool result;
};

struct BIGSmallOrEqualComparison_input {
    uint8_t alen;
    uint8_t blen;
    uint8_t a[128];
    uint8_t b[128];
};

struct BIGSmallOrEqualComparison_output {
    bool result;
};

struct BIGBigComparison_input {
    uint8_t alen;
    uint8_t blen;
    uint8_t a[128];
    uint8_t b[128];
};

struct BIGBigComparison_output {
    bool result;
};
struct BIGSmallComparison_input {
    uint8_t alen;
    uint8_t blen;
    uint8_t a[128];
    uint8_t b[128];
};

struct BIGSmallComparison_output {
    bool result;
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
    printf("4 %llu\n", packet.transaction.amount);
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
    printf("%lu\n", packet.ia.swapFee);
    printf("%lu %u %u\n", packet.ia.initialAmount1, packet.ia.IndexOfToken1, packet.ia.Weight1);
    printf("%lu %u %u\n", packet.ia.initialAmount2, packet.ia.IndexOfToken2, packet.ia.Weight2);
    printf("%lu %u %u\n", packet.ia.initialAmount3, packet.ia.IndexOfToken3, packet.ia.Weight3);
    printf("%lu %u %u\n", packet.ia.initialAmount4, packet.ia.IndexOfToken4, packet.ia.Weight4);
    
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

GetValueOfToken_output QpoolGetValueOfTokenByQu(const char* nodeIp, int nodePort,
                    uint16_t indexOfToken,
                    uint16_t Poolnum
                    )
{
    auto qc = make_qc(nodeIp, nodePort);

    struct {
        RequestResponseHeader header;
        RequestContractFunction rcf;
        GetValueOfToken_input input;
    } packet;

    packet.header.setSize(sizeof(packet));
    packet.header.randomizeDejavu();
    packet.header.setType(RequestContractFunction::type());
    packet.rcf.inputSize = sizeof(GetValueOfToken_input);
    packet.rcf.inputType = qPoolFunctionId::GetValueOfToken;
    packet.rcf.contractIndex = QPOOL_CONTRACT_ID;
    packet.input.IndexOfToken = indexOfToken;
    packet.input.Poolnum = Poolnum;
    qc->sendData((uint8_t *) &packet, packet.header.size());
    std::vector<uint8_t> buffer;
    qc->receiveDataAll(buffer);
    uint8_t* data = buffer.data();
    int recvByte = buffer.size();
    int ptr = 0;
    GetValueOfToken_output result;
    while (ptr < recvByte)
    {
        auto header = (RequestResponseHeader*)(data+ptr);
        if (header->type() == RespondContractFunction::type()){
            auto oup = (GetValueOfToken_output*)(data + ptr + sizeof(RequestResponseHeader));
            result = *oup;
        }
        ptr+= header->size();
    }

    return result;
}



void qpoolswap(char* nodeIp, int nodePort,
                    const char* seed,
                    uint64_t Amountoftoken1,
                    uint16_t indexOfToken1,
                    uint16_t indexOfToken2,
                    uint16_t NumberOfPool,
                    uint32_t scheduledTickOffset)
{
    auto qc = make_qc(nodeIp, nodePort);
    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};
    uint8_t destPublicKey[32] = {0};
    uint8_t subSeed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    uint8_t TokenIssuerPublicKey[32] = {0};
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
        Swap_input ia;
        uint8_t sig[SIGNATURE_SIZE];
    } packet;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    GetValueOfToken_output Token1 = QpoolGetValueOfTokenByQu(nodeIp, nodePort, indexOfToken1, NumberOfPool);
    packet.transaction.amount = Token1.ValueOfToken * Amountoftoken1 / 70;  //    Swap fee is 1% but should provide enough amount. it will be refunded the rest amount excluding 1% for swap fee
    uint32_t scheduledTick = 0;
    if (scheduledTickOffset < 50000){
        uint32_t currentTick = getTickNumberFromNode(qc);
        scheduledTick = currentTick + scheduledTickOffset;
    } else {
        scheduledTick = scheduledTickOffset;
    }
    packet.transaction.tick = scheduledTick;
    packet.transaction.inputType = QPOOL_SWAP;
    packet.transaction.inputSize = sizeof(Swap_input);

    // fill the input
    packet.ia.AmountOfToken1 = Amountoftoken1;
    packet.ia.IndexOfToken1 = indexOfToken1;
    packet.ia.IndexOfToken2 = indexOfToken2;
    packet.ia.Poolnum = NumberOfPool;
    // sign the packet
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction) + sizeof(Swap_input),
                   digest,
                   32);
    sign(subSeed, sourcePublicKey, digest, signature);
    memcpy(packet.sig, signature, SIGNATURE_SIZE);
    // set header
    packet.header.setSize(sizeof(packet.header)+sizeof(Transaction)+sizeof(Swap_input)+ SIGNATURE_SIZE);
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);

    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction)+sizeof(Swap_input)+ SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("Transaction has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.ia));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", scheduledTick, txHash);
    LOG("to check your tx confirmation status\n");
}


void qpoolbignumberTostring(char* nodeIp, int nodePort,
                    const char* seed,
                    int64_t number,
                    uint32_t scheduledTickOffset)
{
    auto qc = make_qc(nodeIp, nodePort);
    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};
    uint8_t destPublicKey[32] = {0};
    uint8_t subSeed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    uint8_t TokenIssuerPublicKey[32] = {0};
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
        BIGNumberToString_input ia;
        uint8_t sig[SIGNATURE_SIZE];
    } packet;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    packet.transaction.amount = 30;  
    uint32_t scheduledTick = 0;
    if (scheduledTickOffset < 50000){
        uint32_t currentTick = getTickNumberFromNode(qc);
        scheduledTick = currentTick + scheduledTickOffset;
    } else {
        scheduledTick = scheduledTickOffset;
    }
    packet.transaction.tick = scheduledTick;
    packet.transaction.inputType = QPOOL_BIGNUMBERTO_STRING;
    packet.transaction.inputSize = sizeof(BIGNumberToString_input);

    // fill the input
    packet.ia.a = number;
    // sign the packet
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction) + sizeof(BIGNumberToString_input),
                   digest,
                   32);
    sign(subSeed, sourcePublicKey, digest, signature);
    memcpy(packet.sig, signature, SIGNATURE_SIZE);
    // set header
    packet.header.setSize(sizeof(packet.header)+sizeof(Transaction)+sizeof(BIGNumberToString_input)+ SIGNATURE_SIZE);
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);

    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction)+sizeof(BIGNumberToString_input)+ SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("Transaction has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.ia));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", scheduledTick, txHash);
    LOG("to check your tx confirmation status\n");
}

void qpoolbignumbergetstatus(char* nodeIp, int nodePort,
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
    packet.rcf.inputType = 5;
    packet.rcf.contractIndex = QPOOL_CONTRACT_ID;
    qc->sendData((uint8_t *) &packet, packet.header.size());
    std::vector<uint8_t> buffer;
    qc->receiveDataAll(buffer);
    uint8_t* data = buffer.data();
    int recvByte = buffer.size();
    int ptr = 0;
    GetBIGStatus_output result;
    while (ptr < recvByte)
    {
        auto header = (RequestResponseHeader*)(data+ptr);
        if (header->type() == RespondContractFunction::type()){
            auto oup = (GetBIGStatus_output*)(data + ptr + sizeof(RequestResponseHeader));
            result = *oup;
        }
        ptr+= header->size();
    }
    
    printf("BIGNumber: %lld\n length of string number: %u\n", result.BIGTest, result.BIGStringNumberLen);
    if(result.BIGStringNumber[0] == 45) printf("- ");
    for(uint8_t i = 0 ; i < result.BIGStringNumberLen; i++) {
        if(i == 0 && result.BIGStringNumber[0] == '-') continue;
        printf("%u ", result.BIGStringNumber[i] - '0');
    }
    printf("\nstatus of comparison: %d", result.BIGTestComparisonResult);
}


void qpoolbigstringTonumber(char* nodeIp, int nodePort,
                    const char* seed,
                    uint8_t lengthOfNumber,
                    uint8_t* stringnumber,
                    uint32_t scheduledTickOffset)
{
    auto qc = make_qc(nodeIp, nodePort);
    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};
    uint8_t destPublicKey[32] = {0};
    uint8_t subSeed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    uint8_t TokenIssuerPublicKey[32] = {0};
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
        BIGStringToNumber_input ia;
        uint8_t sig[SIGNATURE_SIZE];
    } packet;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    packet.transaction.amount = 30;  
    uint32_t scheduledTick = 0;
    if (scheduledTickOffset < 50000){
        uint32_t currentTick = getTickNumberFromNode(qc);
        scheduledTick = currentTick + scheduledTickOffset;
    } else {
        scheduledTick = scheduledTickOffset;
    }
    packet.transaction.tick = scheduledTick;
    packet.transaction.inputType = QPOOL_BIGSTRINGTO_NUMBER;
    packet.transaction.inputSize = sizeof(BIGStringToNumber_input);

    // fill the input
    packet.ia.len = lengthOfNumber;
    if(stringnumber[0] == 45) {     /// if '-'
        packet.ia.a[0] = 45;
        for(uint8_t i = 1 ; i < lengthOfNumber; i++) packet.ia.a[i] = stringnumber[i] + '0';
    }
    else {
        for(uint8_t i = 0 ; i < lengthOfNumber; i++) packet.ia.a[i] = stringnumber[i] + '0';
    }
    // sign the packet
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction) + sizeof(BIGStringToNumber_input),
                   digest,
                   32);
    sign(subSeed, sourcePublicKey, digest, signature);
    memcpy(packet.sig, signature, SIGNATURE_SIZE);
    // set header
    packet.header.setSize(sizeof(packet.header)+sizeof(Transaction)+sizeof(BIGStringToNumber_input)+ SIGNATURE_SIZE);
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);

    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction)+sizeof(BIGStringToNumber_input)+ SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("Transaction has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.ia));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", scheduledTick, txHash);
    LOG("to check your tx confirmation status\n");
}


void qpoolbigplus(char* nodeIp, int nodePort,
                    const char* seed,
                    uint8_t alen,
                    uint8_t* a,
                    uint8_t blen,
                    uint8_t* b,
                    uint32_t scheduledTickOffset)
{
    auto qc = make_qc(nodeIp, nodePort);
    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};
    uint8_t destPublicKey[32] = {0};
    uint8_t subSeed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    uint8_t TokenIssuerPublicKey[32] = {0};
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
        BIGPlus_input ia;
        uint8_t sig[SIGNATURE_SIZE];
    } packet;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    packet.transaction.amount = 30;  
    uint32_t scheduledTick = 0;
    if (scheduledTickOffset < 50000){
        uint32_t currentTick = getTickNumberFromNode(qc);
        scheduledTick = currentTick + scheduledTickOffset;
    } else {
        scheduledTick = scheduledTickOffset;
    }
    packet.transaction.tick = scheduledTick;
    packet.transaction.inputType = QPOOL_BIGPLUS;
    packet.transaction.inputSize = sizeof(BIGPlus_input);

    // fill the input
    packet.ia.alen = alen;
    packet.ia.blen = blen;
    for(uint8_t i = 0 ; i < alen; i++) packet.ia.a[i] = a[i] + '0';
    for(uint8_t i = 0 ; i < blen; i++) packet.ia.b[i] = b[i] + '0';
    // sign the packet
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction) + sizeof(BIGPlus_input),
                   digest,
                   32);
    sign(subSeed, sourcePublicKey, digest, signature);
    memcpy(packet.sig, signature, SIGNATURE_SIZE);
    // set header
    packet.header.setSize(sizeof(packet.header)+sizeof(Transaction)+sizeof(BIGPlus_input)+ SIGNATURE_SIZE);
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);

    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction)+sizeof(BIGPlus_input)+ SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("Transaction has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.ia));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", scheduledTick, txHash);
    LOG("to check your tx confirmation status\n");
}

void qpoolbigminus(char* nodeIp, int nodePort,
                    const char* seed,
                    uint8_t alen,
                    uint8_t* a,
                    uint8_t blen,
                    uint8_t* b,
                    uint32_t scheduledTickOffset)
{
    auto qc = make_qc(nodeIp, nodePort);
    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};
    uint8_t destPublicKey[32] = {0};
    uint8_t subSeed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    uint8_t TokenIssuerPublicKey[32] = {0};
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
        BIGMinus_input ia;
        uint8_t sig[SIGNATURE_SIZE];
    } packet;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    packet.transaction.amount = 30;  
    uint32_t scheduledTick = 0;
    if (scheduledTickOffset < 50000){
        uint32_t currentTick = getTickNumberFromNode(qc);
        scheduledTick = currentTick + scheduledTickOffset;
    } else {
        scheduledTick = scheduledTickOffset;
    }
    packet.transaction.tick = scheduledTick;
    packet.transaction.inputType = QPOOL_BIGMINUS;
    packet.transaction.inputSize = sizeof(BIGMinus_input);

    // fill the input
    packet.ia.alen = alen;
    packet.ia.blen = blen;
    for(uint8_t i = 0 ; i < alen; i++) packet.ia.a[i] = a[i] + '0';
    for(uint8_t i = 0 ; i < blen; i++) packet.ia.b[i] = b[i] + '0';
    // sign the packet
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction) + sizeof(BIGMinus_input),
                   digest,
                   32);
    sign(subSeed, sourcePublicKey, digest, signature);
    memcpy(packet.sig, signature, SIGNATURE_SIZE);
    // set header
    packet.header.setSize(sizeof(packet.header)+sizeof(Transaction)+sizeof(BIGMinus_input)+ SIGNATURE_SIZE);
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);

    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction)+sizeof(BIGMinus_input)+ SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("Transaction has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.ia));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", scheduledTick, txHash);
    LOG("to check your tx confirmation status\n");
}

void qpoolbigmulti(char* nodeIp, int nodePort,
                    const char* seed,
                    uint8_t alen,
                    uint8_t* a,
                    uint8_t blen,
                    uint8_t* b,
                    uint32_t scheduledTickOffset)
{
    auto qc = make_qc(nodeIp, nodePort);
    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};
    uint8_t destPublicKey[32] = {0};
    uint8_t subSeed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    uint8_t TokenIssuerPublicKey[32] = {0};
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
        BIGMultiple_input ia;
        uint8_t sig[SIGNATURE_SIZE];
    } packet;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    packet.transaction.amount = 30;  
    uint32_t scheduledTick = 0;
    if (scheduledTickOffset < 50000){
        uint32_t currentTick = getTickNumberFromNode(qc);
        scheduledTick = currentTick + scheduledTickOffset;
    } else {
        scheduledTick = scheduledTickOffset;
    }
    packet.transaction.tick = scheduledTick;
    packet.transaction.inputType = QPOOL_BIGMULTI;
    packet.transaction.inputSize = sizeof(BIGMultiple_input);

    // fill the input
    packet.ia.alen = alen;
    packet.ia.blen = blen;
    if(a[0] == 45) {
        packet.ia.a[0] = 45;
        for(uint8_t i = 1 ; i < alen; i++) packet.ia.a[i] = a[i] + '0';
    }
    else {
        for(uint8_t i = 0 ; i < alen; i++) packet.ia.a[i] = a[i] + '0';
    }
    if(b[0] == 45) {
        packet.ia.b[0] = 45;
        for(uint8_t i = 1 ; i < blen; i++) packet.ia.b[i] = b[i] + '0';
    }
    else {
        for(uint8_t i = 0 ; i < blen; i++) packet.ia.b[i] = b[i] + '0';
    }
    // sign the packet
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction) + sizeof(BIGMultiple_input),
                   digest,
                   32);
    sign(subSeed, sourcePublicKey, digest, signature);
    memcpy(packet.sig, signature, SIGNATURE_SIZE);
    // set header
    packet.header.setSize(sizeof(packet.header)+sizeof(Transaction)+sizeof(BIGMultiple_input)+ SIGNATURE_SIZE);
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);

    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction)+sizeof(BIGMultiple_input)+ SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("Transaction has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.ia));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", scheduledTick, txHash);
    LOG("to check your tx confirmation status\n");
}

void qpoolbigdiv(char* nodeIp, int nodePort,
                    const char* seed,
                    uint8_t alen,
                    uint8_t* a,
                    uint8_t blen,
                    uint8_t* b,
                    uint32_t scheduledTickOffset)
{
    auto qc = make_qc(nodeIp, nodePort);
    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};
    uint8_t destPublicKey[32] = {0};
    uint8_t subSeed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    uint8_t TokenIssuerPublicKey[32] = {0};
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
        BIGDiv_input ia;
        uint8_t sig[SIGNATURE_SIZE];
    } packet;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    packet.transaction.amount = 30;  
    uint32_t scheduledTick = 0;
    if (scheduledTickOffset < 50000){
        uint32_t currentTick = getTickNumberFromNode(qc);
        scheduledTick = currentTick + scheduledTickOffset;
    } else {
        scheduledTick = scheduledTickOffset;
    }
    packet.transaction.tick = scheduledTick;
    packet.transaction.inputType = QPOOL_BIGDIV;
    packet.transaction.inputSize = sizeof(BIGDiv_input);

    // fill the input
    packet.ia.alen = alen;
    packet.ia.blen = blen;
    if(a[0] == 45) {
        packet.ia.a[0] = 45;
        for(uint8_t i = 1 ; i < alen; i++) packet.ia.a[i] = a[i] + '0';
    }
    else {
        for(uint8_t i = 0 ; i < alen; i++) packet.ia.a[i] = a[i] + '0';
    }
    if(b[0] == 45) {
        packet.ia.b[0] = 45;
        for(uint8_t i = 1 ; i < blen; i++) packet.ia.b[i] = b[i] + '0';
    }
    else {
        for(uint8_t i = 0 ; i < blen; i++) packet.ia.b[i] = b[i] + '0';
    }
    // sign the packet
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction) + sizeof(BIGDiv_input),
                   digest,
                   32);
    sign(subSeed, sourcePublicKey, digest, signature);
    memcpy(packet.sig, signature, SIGNATURE_SIZE);
    // set header
    packet.header.setSize(sizeof(packet.header)+sizeof(Transaction)+sizeof(BIGDiv_input)+ SIGNATURE_SIZE);
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);

    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction)+sizeof(BIGDiv_input)+ SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("Transaction has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.ia));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", scheduledTick, txHash);
    LOG("to check your tx confirmation status\n");
}

void qpoolbigorequalcomparison(char* nodeIp, int nodePort,
                    const char* seed,
                    uint8_t alen,
                    uint8_t* a,
                    uint8_t blen,
                    uint8_t* b,
                    uint32_t scheduledTickOffset)
{
    auto qc = make_qc(nodeIp, nodePort);
    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};
    uint8_t destPublicKey[32] = {0};
    uint8_t subSeed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    uint8_t TokenIssuerPublicKey[32] = {0};
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
        BIGBigOrEqualComparison_input ia;
        uint8_t sig[SIGNATURE_SIZE];
    } packet;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    packet.transaction.amount = 30;  
    uint32_t scheduledTick = 0;
    if (scheduledTickOffset < 50000){
        uint32_t currentTick = getTickNumberFromNode(qc);
        scheduledTick = currentTick + scheduledTickOffset;
    } else {
        scheduledTick = scheduledTickOffset;
    }
    packet.transaction.tick = scheduledTick;
    packet.transaction.inputType = QPOOL_BIGOREQUALCOMPARSIN;
    packet.transaction.inputSize = sizeof(BIGBigOrEqualComparison_input);

    // fill the input
    packet.ia.alen = alen;
    packet.ia.blen = blen;
    for(uint8_t i = 0 ; i < alen; i++) packet.ia.a[i] = a[i] + '0';
    for(uint8_t i = 0 ; i < blen; i++) packet.ia.b[i] = b[i] + '0';
    // sign the packet
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction) + sizeof(BIGBigOrEqualComparison_input),
                   digest,
                   32);
    sign(subSeed, sourcePublicKey, digest, signature);
    memcpy(packet.sig, signature, SIGNATURE_SIZE);
    // set header
    packet.header.setSize(sizeof(packet.header)+sizeof(Transaction)+sizeof(BIGBigOrEqualComparison_input)+ SIGNATURE_SIZE);
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);

    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction)+sizeof(BIGBigOrEqualComparison_input)+ SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("Transaction has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.ia));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", scheduledTick, txHash);
    LOG("to check your tx confirmation status\n");
}


void qpoolsmallorequalcomparison(char* nodeIp, int nodePort,
                    const char* seed,
                    uint8_t alen,
                    uint8_t* a,
                    uint8_t blen,
                    uint8_t* b,
                    uint32_t scheduledTickOffset)
{
    auto qc = make_qc(nodeIp, nodePort);
    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};
    uint8_t destPublicKey[32] = {0};
    uint8_t subSeed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    uint8_t TokenIssuerPublicKey[32] = {0};
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
        BIGSmallOrEqualComparison_input ia;
        uint8_t sig[SIGNATURE_SIZE];
    } packet;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    packet.transaction.amount = 30;  
    uint32_t scheduledTick = 0;
    if (scheduledTickOffset < 50000){
        uint32_t currentTick = getTickNumberFromNode(qc);
        scheduledTick = currentTick + scheduledTickOffset;
    } else {
        scheduledTick = scheduledTickOffset;
    }
    packet.transaction.tick = scheduledTick;
    packet.transaction.inputType = QPOOL_SMALLOREQUALCOMPARISON;
    packet.transaction.inputSize = sizeof(BIGSmallOrEqualComparison_input);

    // fill the input
    packet.ia.alen = alen;
    packet.ia.blen = blen;
    for(uint8_t i = 0 ; i < alen; i++) packet.ia.a[i] = a[i] + '0';
    for(uint8_t i = 0 ; i < blen; i++) packet.ia.b[i] = b[i] + '0';
    // sign the packet
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction) + sizeof(BIGSmallOrEqualComparison_input),
                   digest,
                   32);
    sign(subSeed, sourcePublicKey, digest, signature);
    memcpy(packet.sig, signature, SIGNATURE_SIZE);
    // set header
    packet.header.setSize(sizeof(packet.header)+sizeof(Transaction)+sizeof(BIGSmallOrEqualComparison_input)+ SIGNATURE_SIZE);
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);

    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction)+sizeof(BIGSmallOrEqualComparison_input)+ SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("Transaction has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.ia));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", scheduledTick, txHash);
    LOG("to check your tx confirmation status\n");
}


void qpoolbigcomparison(char* nodeIp, int nodePort,
                    const char* seed,
                    uint8_t alen,
                    uint8_t* a,
                    uint8_t blen,
                    uint8_t* b,
                    uint32_t scheduledTickOffset)
{
    auto qc = make_qc(nodeIp, nodePort);
    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};
    uint8_t destPublicKey[32] = {0};
    uint8_t subSeed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    uint8_t TokenIssuerPublicKey[32] = {0};
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
        BIGBigComparison_input ia;
        uint8_t sig[SIGNATURE_SIZE];
    } packet;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    packet.transaction.amount = 30;  
    uint32_t scheduledTick = 0;
    if (scheduledTickOffset < 50000){
        uint32_t currentTick = getTickNumberFromNode(qc);
        scheduledTick = currentTick + scheduledTickOffset;
    } else {
        scheduledTick = scheduledTickOffset;
    }
    packet.transaction.tick = scheduledTick;
    packet.transaction.inputType = QPOOL_BIGCOMPARISON;
    packet.transaction.inputSize = sizeof(BIGBigComparison_input);

    // fill the input
    packet.ia.alen = alen;
    packet.ia.blen = blen;
    for(uint8_t i = 0 ; i < alen; i++) packet.ia.a[i] = a[i] + '0';
    for(uint8_t i = 0 ; i < blen; i++) packet.ia.b[i] = b[i] + '0';
    // sign the packet
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction) + sizeof(BIGBigComparison_input),
                   digest,
                   32);
    sign(subSeed, sourcePublicKey, digest, signature);
    memcpy(packet.sig, signature, SIGNATURE_SIZE);
    // set header
    packet.header.setSize(sizeof(packet.header)+sizeof(Transaction)+sizeof(BIGBigComparison_input)+ SIGNATURE_SIZE);
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);

    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction)+sizeof(BIGBigComparison_input)+ SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("Transaction has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.ia));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", scheduledTick, txHash);
    LOG("to check your tx confirmation status\n");
}

void qpoolsmallcomparison(char* nodeIp, int nodePort,
                    const char* seed,
                    uint8_t alen,
                    uint8_t* a,
                    uint8_t blen,
                    uint8_t* b,
                    uint32_t scheduledTickOffset)
{
    auto qc = make_qc(nodeIp, nodePort);
    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};
    uint8_t destPublicKey[32] = {0};
    uint8_t subSeed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    uint8_t TokenIssuerPublicKey[32] = {0};
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
        BIGBigComparison_input ia;
        uint8_t sig[SIGNATURE_SIZE];
    } packet;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    packet.transaction.amount = 30; 
    uint32_t scheduledTick = 0;
    if (scheduledTickOffset < 50000){
        uint32_t currentTick = getTickNumberFromNode(qc);
        scheduledTick = currentTick + scheduledTickOffset;
    } else {
        scheduledTick = scheduledTickOffset;
    }
    packet.transaction.tick = scheduledTick;
    packet.transaction.inputType = QPOOL_SMALLCOMPARISON;
    packet.transaction.inputSize = sizeof(BIGBigComparison_input);

    // fill the input
    packet.ia.alen = alen;
    packet.ia.blen = blen;
    for(uint8_t i = 0 ; i < alen; i++) packet.ia.a[i] = a[i] + '0';
    for(uint8_t i = 0 ; i < blen; i++) packet.ia.b[i] = b[i] + '0';
    // sign the packet
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction) + sizeof(BIGBigComparison_input),
                   digest,
                   32);
    sign(subSeed, sourcePublicKey, digest, signature);
    memcpy(packet.sig, signature, SIGNATURE_SIZE);
    // set header
    packet.header.setSize(sizeof(packet.header)+sizeof(Transaction)+sizeof(BIGBigComparison_input)+ SIGNATURE_SIZE);
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);

    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction)+sizeof(BIGBigComparison_input)+ SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("Transaction has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.ia));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", scheduledTick, txHash);
    LOG("to check your tx confirmation status\n");
}


void qpoolbigmodulus(char* nodeIp, int nodePort,
                    const char* seed,
                    uint8_t alen,
                    uint8_t* a,
                    uint8_t blen,
                    uint8_t* b,
                    uint32_t scheduledTickOffset)
{
    auto qc = make_qc(nodeIp, nodePort);
    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};
    uint8_t destPublicKey[32] = {0};
    uint8_t subSeed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    uint8_t TokenIssuerPublicKey[32] = {0};
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
        BIGModulus_input ia;
        uint8_t sig[SIGNATURE_SIZE];
    } packet;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    packet.transaction.amount = 30;  
    uint32_t scheduledTick = 0;
    if (scheduledTickOffset < 50000){
        uint32_t currentTick = getTickNumberFromNode(qc);
        scheduledTick = currentTick + scheduledTickOffset;
    } else {
        scheduledTick = scheduledTickOffset;
    }
    packet.transaction.tick = scheduledTick;
    packet.transaction.inputType = QPOOL_BIGMODULUS;
    packet.transaction.inputSize = sizeof(BIGModulus_input);

    // fill the input
    packet.ia.alen = alen;
    packet.ia.blen = blen;
    if(a[0] == 45) {
        packet.ia.a[0] = 45;
        for(uint8_t i = 1 ; i < alen; i++) packet.ia.a[i] = a[i] + '0';
    }
    else {
        for(uint8_t i = 0 ; i < alen; i++) packet.ia.a[i] = a[i] + '0';
    }
    if(b[0] == 45) {
        packet.ia.b[0] = 45;
        for(uint8_t i = 1 ; i < blen; i++) packet.ia.b[i] = b[i] + '0';
    }
    else {
        for(uint8_t i = 0 ; i < blen; i++) packet.ia.b[i] = b[i] + '0';
    }
    // sign the packet
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction) + sizeof(BIGModulus_input),
                   digest,
                   32);
    sign(subSeed, sourcePublicKey, digest, signature);
    memcpy(packet.sig, signature, SIGNATURE_SIZE);
    // set header
    packet.header.setSize(sizeof(packet.header)+sizeof(Transaction)+sizeof(BIGModulus_input)+ SIGNATURE_SIZE);
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);

    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(Transaction)+sizeof(BIGModulus_input)+ SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("Transaction has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.ia));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", scheduledTick, txHash);
    LOG("to check your tx confirmation status\n");
}