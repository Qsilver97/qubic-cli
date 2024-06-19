#include "defines.h"
COMMAND g_cmd;
char* g_seed = DEFAULT_SEED;
char* g_nodeIp = DEFAULT_NODE_IP;
char* g_targetIdentity = nullptr;
char* g_configFile = nullptr;
char* g_requestedFileName = nullptr;
char* g_requestedFileName2 = nullptr;
char* g_requestedTxId  = nullptr;
char* g_requestedIdentity  = nullptr;
char* g_qx_share_transfer_possessed_identity = nullptr;
char* g_qx_share_transfer_new_owner_identity = nullptr;
int64_t g_qx_share_transfer_amount = 0;
int16_t g_proposal_computor;
char *g_proposal_uri;

int64_t g_TxAmount = 0;
uint16_t g_TxType = 0;
uint32_t g_TxTick = 0;
int g_nodePort = DEFAULT_NODE_PORT;
int g_txExtraDataSize = 0;
int g_rawPacketSize = 0;
int g_requestedSpecialCommand = -1;
char* g_toogle_main_aux_0 = nullptr;
char* g_toogle_main_aux_1 = nullptr;
int g_set_solution_threshold_epoch = -1;
int g_set_solution_threshold_value = -1;


uint32_t g_requestedTickNumber = 0;
uint32_t g_offsetScheduledTick = DEFAULT_SCHEDULED_TICK_OFFSET;
int g_waitUntilFinish = 0;
uint8_t g_txExtraData[1024] = {0};
uint8_t g_rawPacket[1024] = {0};

char* g_qx_issue_asset_name = nullptr;
char* g_qx_issue_unit_of_measurement = nullptr;
int64_t g_qx_issue_asset_number_of_unit = -1;
char g_qx_issue_asset_num_decimal = 0;

char* g_qx_command_1 = nullptr;
char* g_qx_command_2 = nullptr;
char* g_qx_issuer = nullptr;
char* g_qx_asset_name = nullptr;
long long g_qx_offset = -1;
long long g_qx_price = -1;
long long g_qx_number_of_share = -1;

char* g_qx_asset_transfer_possessed_identity = nullptr;
char* g_qx_asset_transfer_new_owner_identity = nullptr;
int64_t g_qx_asset_transfer_amount = -1;
char* g_qx_asset_transfer_asset_name;
char* g_qx_asset_transfer_issuer_in_hex;

char* g_dump_binary_file_input;
char* g_dump_binary_file_output;

//IPO bid
uint32_t g_ipo_contract_index = 0;
uint16_t g_make_ipo_bid_number_of_share = 0;
uint64_t g_make_ipo_bid_price_per_share = 0;
// quottery
uint32_t g_quottery_bet_id;
uint32_t g_quottery_option_id;
char* g_quottery_creator_id = nullptr;
uint64_t g_quottery_number_bet_slot;
uint64_t g_quottery_amount_per_bet_slot;
uint32_t g_quottery_picked_option;

char* g_qutil_sendtomanyv1_payout_list_file = nullptr;
char* g_qairdrop_qairdrop_payout_list_file = nullptr;

char* g_airdrop_assetName = nullptr;
char* g_airdrop_unitOfMeasurement = nullptr;
int64_t g_airdrop_numberOfShares = -1;
char g_airdrop_numberOfDecimalPlaces = 0;

char* g_qairdrop_asset_transfer_asset_name;
char* g_qairdrop_asset_issuer = nullptr;

char* g_qairdrop_one_asset_transfer_asset_name = nullptr;;
char* g_qairdrop_one_asset_transfer_issuer_in_hex;
char* g_qairdrop_one_asset_transfer_new_owner_identity = nullptr;
int64_t g_qairdrop_one_asset_transfer_amount = -1;

int64_t g_qpool_create_number_of_token = 0;
char* g_qpool_enable_asset_name;
char* g_qpool_enable_asset_issur;
int16_t g_qpool_enable_contract_index;
bool g_qpool_enable_type_of_token;
int64_t g_qpool_amount_of_qwallet;
int64_t g_qpool_amount_of_qu;
int8_t g_qpool_weight_of_qwallet;
int64_t g_qpool_amount_of_token[8];
int8_t g_qpool_weight_of_token[8];
int16_t g_qpool_index_of_token[8];
int32_t g_qpool_amount_of_microtoken[8];
bool g_qpool_type_of_token[8];
int64_t g_qpool_swap_fee;
char* g_qpool_LPname;
uint16_t g_qool_tokenID_in_pool;
bool g_qool_get_eneable_token_type_of_token;

char* g_pool_assetName = nullptr;
char* g_pool_unitOfMeasurement = nullptr;
int64_t g_pool_numberOfShares = -1;
char g_pool_numberOfDecimalPlaces = 0;

uint64_t g_qpool_swap_amount_of_token1;
uint16_t g_qpool_swap_index_of_token1;
uint16_t g_qpool_swap_index_of_token2;
uint16_t g_qpool_swap_number_of_pool;
bool g_qpool_swap_type_of_token1;
bool g_qpool_swap_type_of_token2;

int32_t g_qpool_get_infor_number_of_pool = 0;

uint64_t g_get_log_passcode[4] = {0};


int64_t g_qpool_bignumber_to_string;
uint8_t g_qpool_bigstring_to_number_length;
uint8_t g_qpool_bigstring_to_numbers[128];

uint8_t g_qpool_bigoperation_alen;
uint8_t g_qpool_bigoperation_blen;
uint8_t g_qpool_bigoperation_a[128];
uint8_t g_qpool_bigoperation_b[128];

uint64_t g_qool_deposit_expensive_token_amountoftoken;
uint16_t g_qool_deposit_expensive_token_indexoftoken;

uint16_t g_qool_get_amount_of_expensive_token_indexoftoken;
char* g_qpool_get_amount_of_expensive_token_user;