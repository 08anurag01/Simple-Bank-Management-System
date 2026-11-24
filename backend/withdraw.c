#include "bank.h"
int main(void) {
    char account[21],pin[5],amount_s[40]; Account a;
    read_post(); get_value("account",account,sizeof account); get_value("pin",pin,sizeof pin); get_value("amount",amount_s,sizeof amount_s);
    double amount=strtod(amount_s,NULL);
    if(!authenticate(account,pin,&a)||amount<=0) { show_error("Invalid account credentials or withdrawal amount."); return 0; }
    if(amount>a.balance) { show_error("Insufficient balance. No money was withdrawn."); return 0; }
    a.balance-=amount;
    if(!save_account(&a)) { show_error("Could not update account data. Check Apache write permissions."); return 0; }
    log_transaction(account,"WITHDRAW",amount,a.balance);
    html_header("Withdrawal Successful"); printf("<p class='eyebrow'>TRANSACTION COMPLETE</p><h1>Withdrawal successful</h1><p class='muted'>Amount withdrawn: &#8377; %.2f</p><h2>Remaining balance: &#8377; %.2f</h2>",amount,a.balance); show_dashboard(&a); html_footer(); return 0;
}
