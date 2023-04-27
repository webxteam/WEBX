#pragma once

#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <string>

namespace eosiosystem {
   class system_contract;
}

namespace eosio {

   using std::string;

   class [[eosio::contract("burn")]] burn : public contract {
      public:
         using contract::contract;

         [[eosio::action]]
         void daoburn(const name& miner);    


         static asset get_balance( const name& token_contract_account, const name& owner, const symbol_code& sym_code )
         {
            accounts accountstable( token_contract_account, owner.value );
            const auto& ac = accountstable.get( sym_code.raw() );
            return ac.balance;
         }

          using daoburn_action = eosio::action_wrapper<"burn"_n, &burn::daoburn>;

      private:
         struct  account {
            asset    balance;

            uint64_t primary_key()const { return balance.symbol.code().raw(); }
         };

      typedef eosio::multi_index< "accounts"_n, account > accounts;
  

   };
} 