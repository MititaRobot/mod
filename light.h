#include "TSL2561.h"


extern TSL2561 light;
/** 
 * luxの値を返す。
 *
 */
uint32_t get_lux();
/**
 * get_luxよりも詳細な値を返す
 * @param lux luxを代入する先のポインタ broadband ブロードバンド ir 赤外線
 *
 */
void get_light(uint32_t* lux,uint16_t* broadcast,uint16_t* ir);

/**
 *照度センサーを使う時に始めに呼ぶ
 *
 */
void light_init();
/**
 *照度センサーを使い終わったら呼ぶ。(mainの最後でいい)
 *
 */
void light_close();
