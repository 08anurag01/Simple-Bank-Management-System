#ifndef BANK_H
#define BANK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ACCOUNT_FILE "C:/Apache24/bank-data/accounts.txt"
#define TRANSACTION_FILE "C:/Apache24/bank-data/transactions.txt"

typedef struct {
    char account[21], name[61], phone[16], address[101], pin[5];
    double balance;
} Account;

static char post_data[8192];

static void html_header(const char *title) {
    printf("Content-Type: text/html; charset=UTF-8\r\n\r\n");
    printf("<!doctype html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1'>");
    printf("<title>%s</title><link rel='stylesheet' href='/bank/style.css'></head><body>", title);
    printf("<header class='topbar'><a class='brand' href='/bank/index.html'>Secure<span>Bank</span></a><a class='navlink' href='/bank/index.html'>Home</a></header>");
    printf("<main class='narrow'><section class='card'>");
}
static void html_footer(void) {
    printf("<p><a class='button secondary' href='/bank/index.html'>Back to home</a></p>");
    printf("</section></main><footer>Educational demonstration only · Not a real banking service</footer></body></html>");
}
static void read_post(void) {
    char *len = getenv("CONTENT_LENGTH");
    int n = len ? atoi(len) : 0;
    if (n < 0) n = 0;
    if (n > (int)sizeof(post_data)-1) n = (int)sizeof(post_data)-1;
    if (n > 0) fread(post_data, 1, (size_t)n, stdin);
    post_data[n] = '\0';
}
static void url_decode(char *s) {
    char *src=s, *dst=s;
    while (*src) {
        if (*src=='+') { *dst++=' '; src++; }
        else if (*src=='%' && isxdigit((unsigned char)src[1]) && isxdigit((unsigned char)src[2])) {
            char hex[3]={src[1],src[2],0}; *dst++=(char)strtol(hex,NULL,16); src+=3;
        } else { *dst++=*src++; }
    }
    *dst='\0';
}
static int get_value(const char *key, char *out, size_t cap) {
    char copy[sizeof(post_data)];
    strncpy(copy, post_data, sizeof(copy)-1); copy[sizeof(copy)-1]='\0';
    char *pair=strtok(copy, "&");
    while(pair) {
        char *eq=strchr(pair,'=');
        if(eq) {
            *eq='\0'; url_decode(pair); url_decode(eq+1);
            if(strcmp(pair,key)==0) {
                strncpy(out,eq+1,cap-1); out[cap-1]='\0'; return 1;
            }
        }
        pair=strtok(NULL,"&");
    }
    if (out && cap) out[0]='\0';
    return 0;
}
static int valid_text(const char *s) {
    return s && *s && !strchr(s,'|') && !strchr(s,'\n') && !strchr(s,'\r');
}
static int find_account(const char *number, Account *found) {
    FILE *f=fopen(ACCOUNT_FILE,"r"); if(!f) return 0;
    char line[512]; int ok=0;
    while(fgets(line,sizeof(line),f)) {
        Account a;
        if(sscanf(line,"%20[^|]|%60[^|]|%15[^|]|%100[^|]|%4[^|]|%lf",
          a.account,a.name,a.phone,a.address,a.pin,&a.balance)==6 &&
          strcmp(a.account,number)==0) { *found=a; ok=1; break; }
    }
    fclose(f); return ok;
}
static int save_account(const Account *target) {
    FILE *in=fopen(ACCOUNT_FILE,"r");
    FILE *out=fopen("C:/Apache24/bank-data/accounts.tmp","w");
    if(!out) { if(in) fclose(in); return 0; }
    int replaced=0; char line[512];
    if(in) {
        while(fgets(line,sizeof(line),in)) {
            Account a;
            if(sscanf(line,"%20[^|]|%60[^|]|%15[^|]|%100[^|]|%4[^|]|%lf",
              a.account,a.name,a.phone,a.address,a.pin,&a.balance)==6 &&
              strcmp(a.account,target->account)==0) {
                fprintf(out,"%s|%s|%s|%s|%s|%.2f\n",target->account,target->name,target->phone,target->address,target->pin,target->balance);
                replaced=1;
            } else fputs(line,out);
        }
        fclose(in);
    }
    if(!replaced) fprintf(out,"%s|%s|%s|%s|%s|%.2f\n",target->account,target->name,target->phone,target->address,target->pin,target->balance);
    fclose(out);
    remove(ACCOUNT_FILE);
    return rename("C:/Apache24/bank-data/accounts.tmp",ACCOUNT_FILE)==0;
}
static void log_transaction(const char *account,const char *type,double amount,double balance) {
    FILE *f=fopen(TRANSACTION_FILE,"a");
    if(f) { fprintf(f,"%s|%s|%.2f|%.2f\n",account,type,amount,balance); fclose(f); }
}
static int authenticate(const char *number,const char *pin,Account *a) {
    return find_account(number,a) && strcmp(a->pin,pin)==0;
}
static void show_error(const char *message) {
    html_header("Bank Management System");
    printf("<p class='eyebrow'>REQUEST STATUS</p><h1>Unable to complete</h1><p class='muted'>%s</p>",message);
    html_footer();
}
static void show_dashboard(const Account *a) {
    printf("<p class='eyebrow'>ACCOUNT DASHBOARD</p><h1>Hello, %s</h1>",a->name);
    printf("<p class='muted'>Account number: %s</p><h2>Available balance: &#8377; %.2f</h2>",a->account,a->balance);
    printf("<div class='feature-list'><a href='/bank/index.html'>Log out / return to login</a></div>");
    printf("<hr><h2>Deposit money</h2><form action='/cgi-bin/deposit.exe' method='post'>");
    printf("<input type='hidden' name='account' value='%s'><input type='hidden' name='pin' value='%s'>",a->account,a->pin);
    printf("<label>Amount (₹)</label><input type='number' name='amount' min='0.01' step='0.01' required><button>Deposit</button></form>");
    printf("<hr><h2>Withdraw money</h2><form action='/cgi-bin/withdraw.exe' method='post'>");
    printf("<input type='hidden' name='account' value='%s'><input type='hidden' name='pin' value='%s'>",a->account,a->pin);
    printf("<label>Amount (₹)</label><input type='number' name='amount' min='0.01' step='0.01' required><button>Withdraw</button></form>");
    printf("<hr><form action='/cgi-bin/balance.exe' method='post'><input type='hidden' name='account' value='%s'><input type='hidden' name='pin' value='%s'><button>Check balance</button></form>",a->account,a->pin);
    printf("<form action='/cgi-bin/history.exe' method='post'><input type='hidden' name='account' value='%s'><input type='hidden' name='pin' value='%s'><button>Transaction history</button></form>",a->account,a->pin);
}
#endif
