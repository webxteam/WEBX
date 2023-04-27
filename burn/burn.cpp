#include <burn.hpp>
#define DAO_ACCOUNT "webxdaopoolx" 
#define BURN_ACCOUNT "eosio.null"
#define TOKEN_CONTRACT "webxtokenacc"
#define TOKEN_SYMBOL "WEBX"

namespace eosio {
 void burn::daoburn (const name& miner)
 {
   require_auth(miner);
   asset balance = get_balance(name(TOKEN_CONTRACT),get_self(),symbol_code(TOKEN_SYMBOL));
   check (balance.amount >= 10000000000,"Only when the WEBX balance reaches 100 can it be destroyed");

     action{
          permission_level{get_self(), "active"_n},
          name(TOKEN_CONTRACT),
          "transfer"_n,
          std::make_tuple(get_self(), name(BURN_ACCOUNT), balance/2, string("Burn tokens"))    
      }.send();
      
      balance -= balance/2;

      action{
          permission_level{get_self(), "active"_n},
          name(TOKEN_CONTRACT),
          "transfer"_n,
          std::make_tuple(get_self(), name(DAO_ACCOUNT), balance, string("To DAO pool"))    
      }.send();
 }

}
