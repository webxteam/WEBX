#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

#define TOKEN_SYMBOL "WEBX"  
#define TOKEN_PRECISION 8       
#define TOKEN_ACCOUNT "webxtokenacc" 

using namespace eosio;
using namespace std;

class [[eosio::contract]] airdrop : public eosio::contract {
  public:
    airdrop(name self, name code, datastream<const char*> ds) : contract(self, code, ds) {}

    [[eosio::action]]
    void adduser(name user, asset amount) {
      require_auth(get_self());
      whitelist_t whitelist(get_self(), get_self().value);
      whitelist.emplace(get_self(), [&](auto& row) {
        row.user = user;
        row.airdrop_amount = amount;
      });
    }

    [[eosio::action]]
    void claim(name user) {
      require_auth(user);
      
      auto sym_code_raw = symbol(TOKEN_SYMBOL,TOKEN_PRECISION).code().raw();
      accounts acnts( name(TOKEN_ACCOUNT), user.value );
      auto it = acnts.find( sym_code_raw );
      check ( it != acnts.end(),"Register miner first please");

      whitelist_t whitelist(get_self(), get_self().value);
      auto itr = whitelist.find(user.value);
      check(itr != whitelist.end(), "User is not in the whitelist");

      asset quantity = itr->airdrop_amount;
      action(
        permission_level{get_self(),"active"_n},
        name(TOKEN_ACCOUNT),
        "transfer"_n,
        std::make_tuple(get_self(), user, quantity, std::string("WEBX Airdrop"))
      ).send();

      whitelist.erase(itr);
    }
  public:
   struct account {
            asset    balance;
            uint64_t primary_key()const { return balance.symbol.code().raw(); }
         };
    typedef eosio::multi_index< "accounts"_n, account > accounts;

  private:
    struct [[eosio::table]] whitelist {
      name user;
      asset airdrop_amount;
      uint64_t primary_key() const { return user.value; }
    };
    typedef eosio::multi_index<"whitelist"_n, whitelist> whitelist_t;
};

EOSIO_DISPATCH(airdrop, (adduser)(claim))