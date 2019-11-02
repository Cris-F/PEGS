https://github.com/Mixa84/komodo/wiki/Pegs-CC

      Pegs CC

      Mihailo Milenkovic edited this page on Jul 15
      ·
      
        6 revisions
        

Pegs CC

1. What is Pegs CC
Pegs CC is a complex module using several other CCs. It allows 
creating a coin that is pegged (bound) to some other currency. In 
essence a mechanism for creating a decentralized stable coin. Using 
Tokens CC and Gateways CC other crypto currencies can be deposited on 
chain in form of tokens and locked in an account so it can back a coin 
with existing value. With Prices CC that locked deposit can be 
calculated in any other currency like fiat (USD, EUR ...) or crypto 
(BTC, ETH ...) and coins can be issued in correspondent ratio. For the 
sake of further explanation of Pegs in this article, I will use the term
 USDK that is a chain in Komodo ecosystem in which the USDK coin is 
pegged to US Dollar and backed by KMD.



2. Intro into Tokens CC, Gateways CC and Prices CC
These CCs won't be explained here as it assumed that you are familiar
 with other CC, but we'll cover just some basic explanation of what is 
each one for so that you can better understand Pegs CC if you have not 
yet read about these CCs.
Tokens CC allows you to create a token from the coins 
on chain up to the satoshi. So each satoshi can represent one token or 
you can choose whatever scaling you like by the amount of tokens you 
create for you need. Gateways CC will give you tokens 
upon deposits on external chain. For example create BTCT token on Komodo
 asset chain, lock all tokens in gateways CC and deposit funds in BTC 
chain to get BTCT. Last but not least is the Prices CC 
which is a decentralized oracle for getting crypto prices. It has some 
native prices submitted like BTC/USD, BTC/EUR, EUR/USD, ETH/BTC, 
KMD/BTC, and for every other a synthetic can be created from these 
prices.



3. How Pegs CC work
Core functionality is based on accounts that track deposited value 
vs. issued coins on that deposit. It is not easy to grasp the essence on
 how it works from the beginning but if you think of it as a model of 
"deposit and debt" it will be much easier. You deposit the value on 
chain and take a "loan" on that deposit in form of USDK coins. Initially
 the USDK chain will start with 0 coins and they will be issued for 
account holders on request. To get yourself an account you must deposit 
some KMD first through the gateway and get KMDT tokens. With this tokens
 you can fund your account with pegsfund command and issue USDK coins up to 80% of the deposited value with pegsget. Now you are equipped with USDK coins and you can do with them like with every other coin. At any point, the pegsredeem
 command can be done to basically close your account by repaying the 
whole "debt" (burning coins) and get the deposited KMDT back. If you are
 a holder of USDK without an account, got them on exchange or sent by 
someone, you can exchange them with pegsexchange and 
you will get KMDT for the coins you gave. This kind of exchange is done 
by taking an account that is in bad shape, so called "yellow zone" - the
 debt on the deposit is pretty high and in this way the account ratio is
 improved by repaying the part of debt and taking that amount of 
deposit. The example would be an account: deposit 100KMDT, debt 80USDK 
(at KMD price 1$) - debt ratio 80%, so by exchanging 40USDK you are 
getting account: deposit 60KMDT, debt 40USDK - debt ratio 66.66%.


So how does Pegs CC manage to maintain a coin to be stable and pegged
 to some other currency? When looking at the problems of maintaining a 
stable coin there are two aspects. When the price of backed assets goes 
up and when price goes down. As the coin is backed by crypto which can 
have its price very volatile it will be necessary to solve these 
problems very often. The first case is pretty easy to solve, you can 
issue more coins to reach the value of the deposit (but not necessary 
needed as the backed value stays there). The problem comes when price 
goes down. One way would be to burn coins and in that way maintain the 
ratio with the deposited value. But as this is decentralized stable 
coin, no body has the control of the supply of the coins. So to address 
this problem there is a method of liquidating worst accounts. The whole 
pegs CC system tracks all accounts and calculates the global ratio of 
the system by summing all deposits and debts. When the ratio of the 
system reaches some high value (let's say 60%) then it is possible that 
other users liquidate accounts that have account ratio very high that we
 will call "red zone" (for example over 90%). They can repay the whole 
debt of that account to get 5% gain and what is left will go to the 
system to improve the global ratio. By liquidating worst accounts you 
are lowering the overall debt of the system and prevent getting in the 
situation that you have more coins than backed assets - so the system 
keeps every satoshi of the coin backed with some existing value.


Let's now see how this works on a proper example. Initial state of 
the system is normal - global ratio is 35% and price of KMD is 1$, and 
one of the accounts is: 100KMDT, 46USDK - ratio 46%. Now let's see what 
happens if price of KMD suddenly drops to 0.5$. The global ratio now is 
70% and account is: 100KMDT, 46USDK - ratio 92%. Liquidator will now 
give 46USDK to get 46$+5% worth of KMDT that equals to 96.6KMDT and 
3.4KMDT what is left will go to pegs CC global reserve. In this way, the
 account holder that did not maintain good ratio lost 10%, liquidator 
gained 5% on his funds and the rest went to the system.



4. RPC commands

pegsaccounthistory pegstxid
History of all transactions for account.
komodo-cli -ac_name=KUSDTEST pegsaccounthistory a130861d422655cb28ffbf788c3b4a07748ba67eb7c413c9440b7cbbfcb9d296


{ "result": "success", "name": "pegsaccounthistory", "account 
history": [ { "action": "fund", "amount": 1000000, "accounttxid": 
"11de1dfdd5a061be45072b758feb5d1ef34b698b609a758ca1c6b45b87cf2492", 
"token": "KMD", "deposit": 1000000, "debt": 0 }, { "action": "get", 
"amount": 1100000, "accounttxid": 
"193c90bb479a70c7a0d56f18ab2ea004aaa64ad53c410fe45761e431e09fe959", 
"token": "KMD", "deposit": 1000000, "debt": 1100000 } ] }



pegsaccountinfo pegstxid
Current info for account.
komodo-cli -ac_name=KUSDTEST pegsaccountinfo a130861d422655cb28ffbf788c3b4a07748ba67eb7c413c9440b7cbbfcb9d296


{ "result": "success", "name": "pegsaccountinfo", "account 
info": [ { "token": "KMD", "deposit": 0.01, "debt": 0.013, "ratio": 
"93.85%" } ] }



pegssaddress [pubkey]
Addresses of Pegs CC contract.
komodo-cli -ac_name=KUSDTEST pegsaddress


{ "result": "success", "PegsCCAddress": 
"RHnkVb7vHuHnjEjhkCF1bS6xxLLNZPv5fd", "PegsCCBalance": 9.99920000, 
"PegsNormalAddress": "RMcCZtX6dHf1fz3gpLQhUEMQ8cVZ6Rzaro", 
"PegsNormalBalance": 0.00000000, "PegsCCTokensAddress": 
"RHG4K84bPP9h9KKqvpYbUzocaZ3LSUHxLa", "myCCAddress(Pegs)": 
"RWvHzZy4caP1jv3yVoa7Pps6Wsqm8TsM7q", "myCCbalance(Pegs)": 0.00000000, 
"myaddress": "RLMV9iDe8iPLf82xEniz2R8rr9EKEHTmiP", "mybalance": 
0.00060000 }



pegscreate amount N bindtxid1 [bindtxid2 ...]
Create peg contract for given gateways. Amount parameter is for funds
 that will be added to pegs CC global address for tx fees and markers of
 pegs transactions. N is for number of following gateways bind 
transactions. This tx is done once per chain and put in -ac_earlytxid 
parameter so that after block height 100 it cannot be changed and only 
one pegs txid is the trusted one.



pegsexchange pegstxid tokenid amount
Exchange given amount of coins for deposited token (tokenid). This 
rpc is used when user does not have an account and wants to exchange 
coins for other crypto.



pegsfund pegstxid tokenid amount
Funds the account with given token and amount. If user does not have an account yet it will open a new one.



pegsget pegstxid tokenid amount
Issues coins for given amount on account of given token.



pegsinfo pegstxid
Info about total deposit and debt per token and global ratio of whole pegs CC.


komodo-cli -ac_name=KUSDTEST pegsinfo a130861d422655cb28ffbf788c3b4a07748ba67eb7c413c9440b7cbbfcb9d296


{ "result": "success", "name": "pegsinfo", "info": [ { "token":
 "KMD", "total deposit": 1022885, "total debt": 1300000, "total ratio": 
"86.63%" } ], "global ratio": "86.63%" } 



pegsliquidate pegstxid tokenid accounttxid
Liquidate account for given txid if the account ratio is over defined "red zone" threshold.



pegsredem pegstxid tokenid
Withdraw deposited tokens on account by covering the whole debt in coins.



pegsworstaccounts pegstxid
Get info on accounts that can be liquidated (ratio in red zone).
komodo-cli -ac_name=KUSDTEST pegsworstaccounts a130861d422655cb28ffbf788c3b4a07748ba67eb7c413c9440b7cbbfcb9d296


{ "result": "success", "name": "pegsworstaccounts", "KMD": [ { 
"accounttxid": 
"a9f6d273e916b74831d7ee5822bd47f32e5e2f6c6c6f97a7f8ebcd25cab747a6", 
"deposit": 1000000, "debt": 1300000, "ratio": "91.18%" } ] }



5. Examples
----------------------------------
