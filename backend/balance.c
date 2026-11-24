#include "bank.h"
int main(void) {
    char account[21],pin[5]; Account a;
    read_post(); get_value("account",account,sizeof account); get_value("pin",pin,sizeof pin);
    if(!authenticate(account,pin,&a)) { show_error("Account number or PIN is incorrect."); return 0; }
    html_header("Balance"); printf("<p class='eyebrow'>ACCOUNT SUMMARY</p><h1>Available balance</h1><h2>&#8377; %.2f</h2><p class='muted'>Account: %s</p>",a.balance,a.account); show_dashboard(&a); html_footer(); return 0;
}