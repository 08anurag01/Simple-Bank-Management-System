#include "bank.h"
int main(void) {
    char account[21],pin[5],line[256],acc[21],type[30]; double amount,balance; Account a;
    read_post(); get_value("account",account,sizeof account); get_value("pin",pin,sizeof pin);
    if(!authenticate(account,pin,&a)) { show_error("Account number or PIN is incorrect."); return 0; }
    html_header("Transaction History"); printf("<p class='eyebrow'>ACCOUNT ACTIVITY</p><h1>Transaction history</h1><p class='muted'>Account: %s</p>",a.account);
    FILE *f=fopen(TRANSACTION_FILE,"r"); int found=0;
    printf("<table style='width:100%%;border-collapse:collapse;text-align:left'><tr><th>Type</th><th>Amount</th><th>Balance after</th></tr>");
    if(f) { while(fgets(line,sizeof line,f)) if(sscanf(line,"%20[^|]|%29[^|]|%lf|%lf",acc,type,&amount,&balance)==4 && strcmp(acc,account)==0) {
        printf("<tr><td>%s</td><td>&#8377; %.2f</td><td>&#8377; %.2f</td></tr>",type,amount,balance); found=1;
    } fclose(f); }
    printf("</table>");
    if(!found) printf("<p class='muted'>No transactions found yet.</p>");
    show_dashboard(&a); html_footer(); return 0;
}