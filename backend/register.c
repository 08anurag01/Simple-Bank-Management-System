#include "bank.h"
int main(void) {
    char name[61], account[21], phone[16], address[101], pin[5], amount_s[40];
    read_post();
    get_value("name",name,sizeof name); get_value("account",account,sizeof account);
    get_value("phone",phone,sizeof phone); get_value("address",address,sizeof address);
    get_value("pin",pin,sizeof pin); get_value("amount",amount_s,sizeof amount_s);
    double amount=strtod(amount_s,NULL); Account existing;
    if(!valid_text(name)||!valid_text(account)||!valid_text(phone)||!valid_text(address)||
       strlen(pin)!=4||!isdigit((unsigned char)pin[0])||!isdigit((unsigned char)pin[1])||
       !isdigit((unsigned char)pin[2])||!isdigit((unsigned char)pin[3])||amount<0) {
        show_error("Please enter valid details. The PIN must contain exactly four digits."); return 0;
    }
    if(find_account(account,&existing)) { show_error("That account number is already registered."); return 0; }
    Account a; strcpy(a.name,name); strcpy(a.account,account); strcpy(a.phone,phone);
    strcpy(a.address,address); strcpy(a.pin,pin); a.balance=amount;
    if(!save_account(&a)) { show_error("Could not save the account. Check the database folder and Apache write permissions."); return 0; }
    log_transaction(account,"OPENING",amount,amount);
    html_header("Account Created");
    printf("<p class='eyebrow'>REGISTRATION SUCCESSFUL</p><h1>Account created!</h1><p class='muted'>Your demo account number is <b>%s</b>.</p><h2>Opening balance: &#8377; %.2f</h2>",a.account,a.balance);
    html_footer(); return 0;
}