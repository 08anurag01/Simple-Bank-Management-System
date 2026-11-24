#include "bank.h"
int main(void) {
    char account[21],pin[5]; Account a;
    read_post(); get_value("account",account,sizeof account); get_value("pin",pin,sizeof pin);
    if(!authenticate(account,pin,&a)) { show_error("Account number or PIN is incorrect. Please try again."); return 0; }
    html_header("Account Dashboard"); show_dashboard(&a); html_footer(); return 0;
}