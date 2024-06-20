#include "stdio.h"
#include "structs.h"
#include "global.h"
#include "argparser.h"
#include "walletUtils.h"
#include "nodeUtils.h"
#include "assetUtil.h"
#include "keyUtils.h"
#include "sanityCheck.h"
#include "SCUtils.h"
#include "quottery.h"
#include "qutil.h"
#include "qx.h"
#include "qairdrop.h"
#include "qpool.h"

int run(int argc, char* argv[])
{
    parseArgument(argc, argv);
    switch (g_cmd){
        case SHOW_KEYS:
            sanityCheckSeed(g_seed);
            printWalletInfo(g_seed);
            break;
        case GET_CURRENT_TICK:
            sanityCheckNode(g_nodeIp, g_nodePort);
            printTickInfoFromNode(g_nodeIp, g_nodePort);
            break;
        case GET_SYSTEM_INFO:
            sanityCheckNode(g_nodeIp, g_nodePort);
            printSystemInfoFromNode(g_nodeIp, g_nodePort);
            break;
        case GET_BALANCE:
            sanityCheckIdentity(g_requestedIdentity);
            sanityCheckNode(g_nodeIp, g_nodePort);
            printBalance(g_requestedIdentity, g_nodeIp, g_nodePort);
            break;
        case GET_ASSET:
            sanityCheckIdentity(g_requestedIdentity);
            sanityCheckNode(g_nodeIp, g_nodePort);
            printOwnedAsset(g_nodeIp, g_nodePort, g_requestedIdentity);
            printPossessionAsset(g_nodeIp, g_nodePort, g_requestedIdentity);
            break;
        case SEND_COIN:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckSeed(g_seed);
            sanityCheckIdentity(g_targetIdentity);
            sanityCheckTxAmount(g_TxAmount);
            makeStandardTransaction(g_nodeIp, g_nodePort, g_seed, g_targetIdentity, g_TxAmount, g_offsetScheduledTick, g_waitUntilFinish);
            break;
        case SEND_COIN_IN_TICK:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckSeed(g_seed);
            sanityCheckIdentity(g_targetIdentity);
            sanityCheckTxAmount(g_TxAmount);
            makeStandardTransactionInTick(g_nodeIp, g_nodePort, g_seed, g_targetIdentity, g_TxAmount, g_TxTick, g_waitUntilFinish);
            break;
        case SEND_CUSTOM_TX:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckSeed(g_seed);
            sanityCheckTxType(g_TxType);
            sanityCheckIdentity(g_targetIdentity);
            sanityCheckTxAmount(g_TxAmount);
            sanityCheckExtraDataSize(g_txExtraDataSize);
            makeCustomTransaction(g_nodeIp, g_nodePort, g_seed, g_targetIdentity,
                                  g_TxType, g_TxAmount, g_txExtraDataSize,
                                  g_txExtraData, g_offsetScheduledTick);

            break;
        case CHECK_TX_ON_TICK:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckTxHash(g_requestedTxId);
            checkTxOnTick(g_nodeIp, g_nodePort, g_requestedTxId, g_requestedTickNumber);
            break;
        case SEND_RAW_PACKET:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckRawPacketSize(g_rawPacketSize);
            sendRawPacket(g_nodeIp, g_nodePort, g_rawPacketSize, g_rawPacket);
            break;
        case GET_TICK_DATA:
            sanityCheckNode(g_nodeIp, g_nodePort);
            getTickDataToFile(g_nodeIp, g_nodePort, g_requestedTickNumber, g_requestedFileName);
            break;
        case GET_QUORUM_TICK:
            sanityCheckNode(g_nodeIp, g_nodePort);
            getQuorumTick(g_nodeIp, g_nodePort, g_requestedTickNumber, g_requestedFileName);
            break;
        case READ_TICK_DATA:
            sanityFileExist(g_requestedFileName);
            sanityFileExist(g_requestedFileName2);
            printTickDataFromFile(g_requestedFileName, g_requestedFileName2);
            break;
        case CHECK_TX_ON_FILE:
            sanityFileExist(g_requestedFileName);
            sanityCheckTxHash(g_requestedTxId);
            checkTxOnFile(g_requestedTxId, g_requestedFileName);
            break;
        case PUBLISH_PROPOSAL:
            printf("On development. Come back later\n");
            break;
        case VOTE_PROPOSAL:
            printf("On development. Come back later\n");
            break;
        case QX_ISSUE_ASSET:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckSeed(g_seed);
            sanityCheckNumberOfUnit(g_qx_issue_asset_number_of_unit);
            sanityCheckValidString(g_qx_issue_asset_name);
            sanityCheckValidString(g_qx_issue_unit_of_measurement);
            sanityCheckNumberOfDecimal(g_qx_issue_asset_num_decimal);
            qxIssueAsset(g_nodeIp, g_nodePort, g_seed,
                         g_qx_issue_asset_name,
                         g_qx_issue_unit_of_measurement,
                         g_qx_issue_asset_number_of_unit,
                         g_qx_issue_asset_num_decimal,
                         g_offsetScheduledTick);
            break;
        case QX_TRANSFER_ASSET:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckSeed(g_seed);
            sanityCheckNumberOfUnit(g_qx_asset_transfer_amount);
            sanityCheckValidString(g_qx_asset_transfer_asset_name);
            sanityCheckValidString(g_qx_asset_transfer_issuer_in_hex);
            sanityCheckIdentity(g_qx_asset_transfer_new_owner_identity);
            qxTransferAsset(g_nodeIp, g_nodePort, g_seed,
                            g_qx_asset_transfer_asset_name,
                            g_qx_asset_transfer_issuer_in_hex,
                            g_qx_asset_transfer_new_owner_identity,
                            g_qx_asset_transfer_amount,
                            g_offsetScheduledTick);
            break;
        case QX_ORDER:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckSeed(g_seed);
            sanityCheckValidString(g_qx_command_1);
            sanityCheckValidString(g_qx_command_2);
            if (strcmp(g_qx_command_1, "add") == 0){
                if (strcmp(g_qx_command_2, "bid") == 0){
                    qxAddToBidOrder(g_nodeIp, g_nodePort, g_seed, g_qx_asset_name, g_qx_issuer, g_qx_price, g_qx_number_of_share, g_offsetScheduledTick);
                } else if (strcmp(g_qx_command_2, "ask") == 0){
                    qxAddToAskOrder(g_nodeIp, g_nodePort, g_seed, g_qx_asset_name, g_qx_issuer, g_qx_price, g_qx_number_of_share, g_offsetScheduledTick);
                }
            } else if (strcmp(g_qx_command_1, "remove") == 0){
                if (strcmp(g_qx_command_2, "bid") == 0){
                    qxRemoveToBidOrder(g_nodeIp, g_nodePort, g_seed, g_qx_asset_name, g_qx_issuer, g_qx_price, g_qx_number_of_share, g_offsetScheduledTick);
                } else if (strcmp(g_qx_command_2, "ask") == 0){
                    qxRemoveToAskOrder(g_nodeIp, g_nodePort, g_seed, g_qx_asset_name, g_qx_issuer, g_qx_price, g_qx_number_of_share, g_offsetScheduledTick);
                }
            }
            break;
        case QX_GET_ORDER:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckValidString(g_qx_command_1);
            sanityCheckValidString(g_qx_command_2);
            if (strcmp(g_qx_command_1, "entity") == 0){
                if (strcmp(g_qx_command_2, "bid") == 0){
                    qxGetEntityBidOrder(g_nodeIp, g_nodePort, g_qx_issuer, g_qx_offset);
                } else if (strcmp(g_qx_command_2, "ask") == 0){
                    qxGetEntityAskOrder(g_nodeIp, g_nodePort, g_qx_issuer, g_qx_offset);
                }
            } else if (strcmp(g_qx_command_1, "asset") == 0){
                if (strcmp(g_qx_command_2, "bid") == 0){
                    qxGetAssetBidOrder(g_nodeIp, g_nodePort, g_qx_asset_name, g_qx_issuer, g_qx_offset);
                } else if (strcmp(g_qx_command_2, "ask") == 0){
                    qxGetAssetAskOrder(g_nodeIp, g_nodePort, g_qx_asset_name, g_qx_issuer, g_qx_offset);
                }
            }
            break;
        case GET_COMP_LIST:
            sanityCheckNode(g_nodeIp, g_nodePort);
            getComputorListToFile(g_nodeIp, g_nodePort, g_requestedFileName);
            break;
        case GET_NODE_IP_LIST:
            sanityCheckNode(g_nodeIp, g_nodePort);
            getNodeIpList(g_nodeIp, g_nodePort);
            break;
        case GET_LOG_FROM_NODE:
            sanityCheckNode(g_nodeIp, g_nodePort);
            getLogFromNode(g_nodeIp, g_nodePort, g_get_log_passcode);
            break;
        case DUMP_SPECTRUM_FILE:
            sanityFileExist(g_dump_binary_file_input);
            sanityCheckValidString(g_dump_binary_file_output);
            dumpSpectrumToCSV(g_dump_binary_file_input, g_dump_binary_file_output);
            break;
        case DUMP_UNIVERSE_FILE:
            sanityFileExist(g_dump_binary_file_input);
            sanityCheckValidString(g_dump_binary_file_output);
            dumpUniverseToCSV(g_dump_binary_file_input, g_dump_binary_file_output);
            break;
        case PRINT_QX_FEE:
            sanityCheckNode(g_nodeIp, g_nodePort);
            printQxFee(g_nodeIp, g_nodePort);
            break;
        case MAKE_IPO_BID:
            sanityCheckNode(g_nodeIp, g_nodePort);
            makeIPOBid(g_nodeIp, g_nodePort, g_seed, g_ipo_contract_index, g_make_ipo_bid_price_per_share, g_make_ipo_bid_number_of_share, g_offsetScheduledTick);
            break;
        case GET_IPO_STATUS:
            sanityCheckNode(g_nodeIp, g_nodePort);
            printIPOStatus(g_nodeIp, g_nodePort, g_ipo_contract_index);
            break;
        case QUOTTERY_ISSUE_BET:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckSeed(g_seed);
            quotteryIssueBet(g_nodeIp, g_nodePort, g_seed, g_offsetScheduledTick);
            break;
        case QUOTTERY_GET_BET_INFO:
            sanityCheckNode(g_nodeIp, g_nodePort);
            quotteryPrintBetInfo(g_nodeIp, g_nodePort, g_quottery_bet_id);
            break;
        case QUOTTERY_JOIN_BET:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckSeed(g_seed);
            quotteryJoinBet(g_nodeIp, g_nodePort, g_seed, g_quottery_bet_id, g_quottery_number_bet_slot, g_quottery_amount_per_bet_slot, g_quottery_picked_option, g_offsetScheduledTick);
            break;
        case QUOTTERY_GET_BET_DETAIL:
            sanityCheckNode(g_nodeIp, g_nodePort);
            quotteryPrintBetOptionDetail(g_nodeIp, g_nodePort, g_quottery_bet_id, g_quottery_option_id);
            break;
        case QUOTTERY_GET_ACTIVE_BET:
            sanityCheckNode(g_nodeIp, g_nodePort);
            quotteryPrintActiveBet(g_nodeIp, g_nodePort);
            break;
        case QUOTTERY_GET_ACTIVE_BET_BY_CREATOR:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckIdentity(g_quottery_creator_id);
            quotteryPrintActiveBetByCreator(g_nodeIp, g_nodePort, g_quottery_creator_id);
            break;
        case QUOTTERY_GET_BET_FEE:
            sanityCheckNode(g_nodeIp, g_nodePort);
            quotteryPrintBetFees(g_nodeIp, g_nodePort);
            break;
        case QUOTTERY_PUBLISH_RESULT:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckSeed(g_seed);
            quotteryPublishResult(g_nodeIp, g_nodePort, g_seed, g_quottery_bet_id, g_quottery_option_id, g_offsetScheduledTick);
            break;
        case QUOTTERY_CANCEL_BET:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckSeed(g_seed);
            quotteryCancelBet(g_nodeIp, g_nodePort, g_seed, g_quottery_bet_id, g_offsetScheduledTick);
            break;
        case TOOGLE_MAIN_AUX:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckSeed(g_seed);
            sanityCheckMainAuxStatus(g_toogle_main_aux_0);
            sanityCheckMainAuxStatus(g_toogle_main_aux_1);
            toogleMainAux(g_nodeIp, g_nodePort, g_seed, g_requestedSpecialCommand, g_toogle_main_aux_0, g_toogle_main_aux_1);
            break;
        case SET_SOLUTION_THRESHOLD:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckSeed(g_seed);
            checkValidEpoch(g_set_solution_threshold_epoch);
            checkValidSolutionThreshold(g_set_solution_threshold_value);
            setSolutionThreshold(g_nodeIp, g_nodePort, g_seed,
                                 g_requestedSpecialCommand, g_set_solution_threshold_epoch, g_set_solution_threshold_value);
            break;
        case SEND_SPECIAL_COMMAND:
        case REFRESH_PEER_LIST:
        case FORCE_NEXT_TICK:
        case REISSUE_VOTE:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckSeed(g_seed);
            sanityCheckSpecialCommand(g_requestedSpecialCommand);
            sendSpecialCommand(g_nodeIp, g_nodePort, g_seed, g_requestedSpecialCommand);
            break;
        case GET_MINING_SCORE_RANKING:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckSeed(g_seed);
            sanityCheckSpecialCommand(g_requestedSpecialCommand);
            sendSpecialCommandGetMiningScoreRanking(g_nodeIp, g_nodePort, g_seed, g_requestedSpecialCommand);
            break;
        case SYNC_TIME:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckSeed(g_seed);
            syncTime(g_nodeIp, g_nodePort, g_seed);
            break;
        case QUTIL_SEND_TO_MANY_V1:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckSeed(g_seed);
            sanityFileExist(g_qutil_sendtomanyv1_payout_list_file);
            qutilSendToManyV1(g_nodeIp, g_nodePort, g_seed, g_qutil_sendtomanyv1_payout_list_file, g_offsetScheduledTick);
            break;
            case QAIRDROP_START_AIRDROP:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckSeed(g_seed);
            sanityCheckNumberOfUnit(g_airdrop_numberOfShares);
            sanityCheckValidString(g_airdrop_assetName);
            sanityCheckValidString(g_airdrop_unitOfMeasurement);
            sanityCheckNumberOfDecimal(g_airdrop_numberOfDecimalPlaces);
            qairdropStartAirdrop(g_nodeIp, g_nodePort, g_seed,
                         g_airdrop_assetName,
                         g_airdrop_unitOfMeasurement,
                         g_airdrop_numberOfShares,
                         g_airdrop_numberOfDecimalPlaces,
                         g_offsetScheduledTick);
            break;
        case QARIDROP_TRANSFER_ASSET:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckSeed(g_seed);
            sanityCheckValidString(g_qairdrop_asset_transfer_asset_name);
            DistributeToken(g_nodeIp, g_nodePort, g_seed,
                            g_qairdrop_asset_transfer_asset_name,
                            g_offsetScheduledTick,
                            g_qairdrop_asset_issuer
                            );
            break;
        case QARIDROP_ONE_TRANSFER_ASSET:
        sanityCheckNode(g_nodeIp, g_nodePort);
        sanityCheckSeed(g_seed);
        sanityCheckNumberOfUnit(g_qairdrop_one_asset_transfer_amount);
        sanityCheckValidString(g_qairdrop_one_asset_transfer_asset_name);
        sanityCheckValidString(g_qairdrop_one_asset_transfer_issuer_in_hex);
        sanityCheckIdentity(g_qairdrop_one_asset_transfer_new_owner_identity);
        AirdropOneTransferAsset(g_nodeIp, g_nodePort, g_seed,
                        g_qairdrop_one_asset_transfer_asset_name,
                        g_qairdrop_one_asset_transfer_issuer_in_hex,
                        g_qairdrop_one_asset_transfer_new_owner_identity,
                        g_qairdrop_one_asset_transfer_amount,
                        g_offsetScheduledTick);
        case QPOOL_CREATE:
        sanityCheckNode(g_nodeIp, g_nodePort);
        sanityCheckSeed(g_seed);
        QpoolCreate(g_nodeIp, g_nodePort, g_seed,
                        g_qpool_LPname,
                        g_qpool_create_number_of_token,
                        g_qpool_amount_of_qwallet,
                        g_qpool_weight_of_qwallet,
                        g_qpool_amount_of_qu,
                        g_qpool_index_of_token,
                        g_qpool_amount_of_token,
                        g_qpool_weight_of_token,
                        g_qpool_amount_of_microtoken,
                        g_qpool_type_of_token,
                        g_qpool_swap_fee,
                        g_offsetScheduledTick);
        break;
        case QPOOL_ISSUE_ASSET:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckSeed(g_seed);
            sanityCheckNumberOfUnit(g_pool_numberOfShares);
            sanityCheckValidString(g_pool_assetName);
            sanityCheckValidString(g_pool_unitOfMeasurement);
            sanityCheckNumberOfDecimal(g_pool_numberOfDecimalPlaces);
            qpoolIssueAsset(g_nodeIp, g_nodePort, g_seed,
                         g_pool_assetName,
                         g_pool_unitOfMeasurement,
                         g_pool_numberOfShares,
                         g_pool_numberOfDecimalPlaces,
                         g_offsetScheduledTick);
            break;

        case QPOOL_GET_INFOR:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckSeed(g_seed);
            qpoolgetInfor(g_nodeIp, g_nodePort, g_seed,
                         g_qpool_get_infor_number_of_pool,
                         g_offsetScheduledTick);
            break;
        case QPOOL_ENABLE_TOKEN:
            sanityCheckNode(g_nodeIp, g_nodePort);
            sanityCheckSeed(g_seed);
            qpoolenableToken(g_nodeIp, g_nodePort, g_seed,
                         g_qpool_enable_asset_name,
                         g_qpool_enable_asset_issur,
                         g_qpool_enable_contract_index,
                         g_qpool_enable_type_of_token,
                         g_offsetScheduledTick);
            break;
        case QPOOL_GET_NUMBER_OF_ENABLE_TOKEN:
        sanityCheckNode(g_nodeIp, g_nodePort);
        sanityCheckSeed(g_seed);
        qpoolgetnumberofenableToken(g_nodeIp, g_nodePort, g_seed,
                        g_offsetScheduledTick);
        break;
        case QPOOL_GET_ENABLE_TOKEN:
        sanityCheckNode(g_nodeIp, g_nodePort);
        sanityCheckSeed(g_seed);
        qpoolgetenableToken(g_nodeIp, g_nodePort, g_seed,
                        g_qool_tokenID_in_pool,
                        g_qool_get_eneable_token_type_of_token,
                        g_offsetScheduledTick);
        break;

        case QPOOL_SWAP:
        sanityCheckNode(g_nodeIp, g_nodePort);
        sanityCheckSeed(g_seed);
        qpoolswap(g_nodeIp, g_nodePort, g_seed,
                        g_qpool_swap_amount_of_token1,
                        g_qpool_swap_index_of_token1,
                        g_qpool_swap_index_of_token2,
                        g_qpool_swap_number_of_pool,
                        g_qpool_swap_type_of_token1,
                        g_qpool_swap_type_of_token2,
                        g_offsetScheduledTick);
        break;
        case QPOOL_DEPOSIT_EXPENSIVE_TOKEN:
        sanityCheckNode(g_nodeIp, g_nodePort);
        sanityCheckSeed(g_seed);
        qpooldepositexpensivetoken(g_nodeIp, g_nodePort, g_seed,
                        g_qool_deposit_expensive_token_amountoftoken,
                        g_qool_deposit_expensive_token_indexoftoken,
                        g_offsetScheduledTick);
        break;
        case QPOOL_WITHDRAW_EXPENSIVE_TOKEN:
        sanityCheckNode(g_nodeIp, g_nodePort);
        sanityCheckSeed(g_seed);
        qpoolwithdrawexpensivetoken(g_nodeIp, g_nodePort, g_seed,
                        g_qool_deposit_expensive_token_amountoftoken,
                        g_qool_deposit_expensive_token_indexoftoken,
                        g_offsetScheduledTick);
        break;
        case QPOOL_GET_AMOUNT_OF_EXPENSIVE_TOKEN_USER_DEPOSITED:
        sanityCheckNode(g_nodeIp, g_nodePort);
        sanityCheckSeed(g_seed);
        qpoolgetamountofexpensivetokenuserdeposited(g_nodeIp, g_nodePort, g_seed,
                        g_qpool_get_amount_of_expensive_token_user,
                        g_qool_get_amount_of_expensive_token_indexoftoken,
                        g_offsetScheduledTick);
        break;
        case MAKE_PROPOSAL:
        sanityCheckNode(g_nodeIp, g_nodePort);
        sanityCheckSeed(g_seed);
        makeproposal(g_nodeIp, g_nodePort, g_seed,
                        g_proposal_computor,
                        g_proposal_uri);
        break;
        case QPOOL_BIGNUMBER_TO_STRING:
        sanityCheckNode(g_nodeIp, g_nodePort);
        sanityCheckSeed(g_seed);
        qpoolbignumberTostring(g_nodeIp, g_nodePort, g_seed,
                        g_qpool_bignumber_to_string,
                        g_offsetScheduledTick);
        break;
        case QPOOL_BIGNUMBER_GET_STATUS:
        sanityCheckNode(g_nodeIp, g_nodePort);
        sanityCheckSeed(g_seed);
        qpoolbignumbergetstatus(g_nodeIp, g_nodePort, g_seed,
                        g_offsetScheduledTick);
        break;
        case QPOOL_BIGSTRING_TO_NUMBER:
        sanityCheckNode(g_nodeIp, g_nodePort);
        sanityCheckSeed(g_seed);
        qpoolbigstringTonumber(g_nodeIp, g_nodePort, g_seed,
                        g_qpool_bigstring_to_number_length,
                        g_qpool_bigstring_to_numbers,
                        g_offsetScheduledTick);
        break;
        case QPOOL_BIGPLUS:
        sanityCheckNode(g_nodeIp, g_nodePort);
        sanityCheckSeed(g_seed);
        qpoolbigplus(g_nodeIp, g_nodePort, g_seed,
                        g_qpool_bigoperation_alen,
                        g_qpool_bigoperation_a,
                        g_qpool_bigoperation_blen,
                        g_qpool_bigoperation_b,
                        g_offsetScheduledTick);
        break;
        case QPOOL_BIGMINUS:
        sanityCheckNode(g_nodeIp, g_nodePort);
        sanityCheckSeed(g_seed);
        qpoolbigminus(g_nodeIp, g_nodePort, g_seed,
                        g_qpool_bigoperation_alen,
                        g_qpool_bigoperation_a,
                        g_qpool_bigoperation_blen,
                        g_qpool_bigoperation_b,
                        g_offsetScheduledTick);
        break;
        case QPOOL_BIGMULTIPLE:
        sanityCheckNode(g_nodeIp, g_nodePort);
        sanityCheckSeed(g_seed);
        qpoolbigmulti(g_nodeIp, g_nodePort, g_seed,
                        g_qpool_bigoperation_alen,
                        g_qpool_bigoperation_a,
                        g_qpool_bigoperation_blen,
                        g_qpool_bigoperation_b,
                        g_offsetScheduledTick);
        break;
        case QPOOL_BIGDIV:
        sanityCheckNode(g_nodeIp, g_nodePort);
        sanityCheckSeed(g_seed);
        qpoolbigdiv(g_nodeIp, g_nodePort, g_seed,
                        g_qpool_bigoperation_alen,
                        g_qpool_bigoperation_a,
                        g_qpool_bigoperation_blen,
                        g_qpool_bigoperation_b,
                        g_offsetScheduledTick);
        break;
        case QPOOL_BIGDMODULUS:
        sanityCheckNode(g_nodeIp, g_nodePort);
        sanityCheckSeed(g_seed);
        qpoolbigmodulus(g_nodeIp, g_nodePort, g_seed,
                        g_qpool_bigoperation_alen,
                        g_qpool_bigoperation_a,
                        g_qpool_bigoperation_blen,
                        g_qpool_bigoperation_b,
                        g_offsetScheduledTick);
        break;
        case QPOOL_BIGOREQUALCOMPARISON:
        sanityCheckNode(g_nodeIp, g_nodePort);
        sanityCheckSeed(g_seed);
        qpoolbigorequalcomparison(g_nodeIp, g_nodePort, g_seed,
                        g_qpool_bigoperation_alen,
                        g_qpool_bigoperation_a,
                        g_qpool_bigoperation_blen,
                        g_qpool_bigoperation_b,
                        g_offsetScheduledTick);
        break;
        case QPOOL_SMALLOREQUALCOMPARISON:
        sanityCheckNode(g_nodeIp, g_nodePort);
        sanityCheckSeed(g_seed);
        qpoolsmallorequalcomparison(g_nodeIp, g_nodePort, g_seed,
                        g_qpool_bigoperation_alen,
                        g_qpool_bigoperation_a,
                        g_qpool_bigoperation_blen,
                        g_qpool_bigoperation_b,
                        g_offsetScheduledTick);
        break;
        case QPOOL_BIGCOMPARISON:
        sanityCheckNode(g_nodeIp, g_nodePort);
        sanityCheckSeed(g_seed);
        qpoolbigcomparison(g_nodeIp, g_nodePort, g_seed,
                        g_qpool_bigoperation_alen,
                        g_qpool_bigoperation_a,
                        g_qpool_bigoperation_blen,
                        g_qpool_bigoperation_b,
                        g_offsetScheduledTick);
        break;
        case QPOOL_SMALLCOMPARISON:
        sanityCheckNode(g_nodeIp, g_nodePort);
        sanityCheckSeed(g_seed);
        qpoolsmallcomparison(g_nodeIp, g_nodePort, g_seed,
                        g_qpool_bigoperation_alen,
                        g_qpool_bigoperation_a,
                        g_qpool_bigoperation_blen,
                        g_qpool_bigoperation_b,
                        g_offsetScheduledTick);
        break;
        default:
            printf("Unexpected command!\n");
            break;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    try
    {
        return run(argc, argv);
    }
    catch (std::exception & ex)
    {
        printf("%s\n", ex.what());
        return -1;
    }
}
