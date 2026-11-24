# Bank Management System (HTML + CSS + C)

Beginner-friendly educational micro-project for Windows 11, VS Code, GCC (MSYS2 UCRT64), and Apache Lounge.
It uses **HTML forms + C CGI programs**. No JavaScript is used.

http://localhost/bank/index.html

## Project structure

* `frontend/` — HTML pages and CSS
* `backend/` — C CGI handlers and shared helper header
* `database/` — flat-file data storage (created/used by the C programs)

## Important educational-use notice

This is a local classroom demonstration, **not real banking software**. It stores the demo PIN in a plain-text data file and has no production-grade authentication, encryption, audit controls, or concurrency protection. Use fake sample data only. Never use real bank details or passwords.

## 1\. Prerequisites

* Apache Lounge installed at `C:\\\\\\\\Apache24`
* http://localhost/bank/index.html
* GCC installed through MSYS2 UCRT64
* Apache's `cgi\\\\\\\_module` enabled and `/cgi-bin/` configured in `httpd.conf`
* The project folder can be placed anywhere, but the steps below assume it is at `C:\\\\\\\\Bank\\\\\\\_Management\\\\\\\_System`

## 2\. Copy frontend files

Copy the contents of `frontend` into:

`C:\\\\\\\\Apache24\\\\\\\\htdocs\\\\\\\\bank`

Create that folder if it does not exist. The resulting path should include:

`C:\\\\\\\\Apache24\\\\\\\\htdocs\\\\\\\\bank\\\\\\\\index.html`

## 3\. Copy database folder

Copy the project's `database` folder to:

`C:\\\\\\\\Apache24\\\\\\\\bank-data`

The C programs use this fixed Windows path for simplicity. Ensure the Windows account running Apache has permission to write to this folder.

## 4\. Compile the C CGI programs

Open the MSYS2 UCRT64 terminal, change to the backend folder (adjust the path if needed):

```bash
cd /c/Bank\\\\\\\_Management\\\\\\\_System/backend
gcc register.c -o /c/Apache24/cgi-bin/register.exe
gcc login.c -o /c/Apache24/cgi-bin/login.exe
gcc deposit.c -o /c/Apache24/cgi-bin/deposit.exe
gcc withdraw.c -o /c/Apache24/cgi-bin/withdraw.exe
gcc balance.c -o /c/Apache24/cgi-bin/balance.exe
gcc history.c -o /c/Apache24/cgi-bin/history.exe
```

If Apache is installed elsewhere, change the output paths accordingly.

## 5\. Run Apache

In an Administrator Command Prompt:

```cmd
cd C:\\\\\\\\Apache24\\\\\\\\bin
httpd.exe -t
httpd.exe
```

Open:

`http://localhost/bank/`

If Apache is already running as a service, restart it after configuration changes rather than starting a second copy.

## 6\. Test the project

1. Open the registration page and create a fake demo account.
2. Log in using that account number and PIN.
3. Use deposit, withdrawal, balance, and transaction history.
4. Check `C:\\\\\\\\Apache24\\\\\\\\bank-data\\\\\\\\accounts.txt` and `transactions.txt` to see the stored demo records.

## Notes / troubleshooting

* CGI programs must be in Apache's configured `cgi-bin` directory and compiled as `.exe`.
* If the browser shows `500 Internal Server Error`, inspect `C:\\\\\\\\Apache24\\\\\\\\logs\\\\\\\\error.log`.
* If Apache cannot write data, check folder permissions on `C:\\\\\\\\Apache24\\\\\\\\bank-data`.
* If port 80 is occupied, Apache may fail to start. Check the error log or configure another port consistently.
* The C programs return HTML directly, so no JavaScript is needed.

