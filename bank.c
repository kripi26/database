/* Build a program for managing a bank.
   There should be a database of bank
   accounts and transactions. We should
   be able to add new accounts, 
   new transactions (credit/debit) and 
   compute the balance of a account     */
#include <stdio.h>
#include <string.h>

typedef enum AccountType {
  Savings,
  Current
} AccountType;

typedef enum TransactionType {
  Credit,
  Debit
} TransactionType;

typedef struct Transaction {
  TransactionType type;
  struct BankAccount* account;
  int amount;
} Transaction;

typedef struct BankAccount {
  char name[100];
  int pin;
  AccountType type;
  // passbook is an array of transactions
  // pointers to avoid taking too much memory
  struct Transaction* passbook[1000]; 
  int transactions_count;
} BankAccount;

typedef struct BankDatabase {
  BankAccount accounts[1000];
  Transaction transactions[10000];
  int accounts_count;
  int transactions_count;
} BankDatabase;

// compute the total amount of money 
// with the bank amoung all the accounts
int compute_money_with_bank(
                      BankDatabase* db) {
  int sum = 0;  
  for(int i = 0; 
        i < db->transactions_count; i++) {
    switch(db->transactions[i].type) {
      case Credit:
        sum += db->transactions[i].amount;
        break;
      case Debit:
        sum -= db->transactions[i].amount;
        break;
    }  
  }
  return sum;
}


int compute_balance(BankAccount* acc) {
  // Problem 2 a.) fill in the code to 
  // find the balance of the account 
  // 'acc'.(3 marks)
  int balance = 0;
  for (int i = 0; i < acc->transactions_count; i++) {
    switch (acc->passbook[i]->type) {
    case Credit:
      balance += acc->passbook[i]->amount;
      break;
    case Debit:
      balance -= acc->passbook[i]->amount;
      break;
    }
  }
  return balance;
}

BankAccount* add_bank_account(char* name, 
                    int pin, AccountType type,
                    BankDatabase* db) {
  // Problem 2 b.) fill in the code to add 
  // a new account 'acc' to the bank 
  // database 'db'. The function should 
  // also return a pointer to the bank 
  // account created in 'db'. (3 marks)
  BankAccount* acc = &(db->accounts[db->accounts_count++]);
  strcpy(acc->name, name);
  acc->pin = pin;
  acc->type = type;
  acc->transactions_count = 0;
  return acc;
}

Transaction* add_transaction(
            TransactionType type, 
            BankAccount *account, 
            int amount, BankDatabase* db) {
  // Problem 2 c.) Fill in the code for 
  // adding a transaction to the system. 
  // The logic should be written such 
  // that the all the other functions in
  //  this program continue to work 
  // correctly. (4 marks)
  Transaction* t = 
    &(db->transactions[db->transactions_count++]);
  t->account = account;
  t->amount = amount;
  t->type = type;
  account->passbook[account->transactions_count++]
   = t;
  return t;
}

int main() {

  BankDatabase db;
  db.accounts_count = db.transactions_count = 0;
  BankAccount acc = { .pin = 1234, 
                      .transactions_count = 0};
  strcpy(acc.name, "Ivan");
  BankAccount* acc_ptr = add_bank_account(
                 acc.name,acc.pin,acc.type, &db);
  add_transaction(Credit, acc_ptr, 10000, &db);
  add_transaction(Debit, acc_ptr, 2000, &db);
  add_transaction(Credit, acc_ptr, 5000, &db);

  // should print 13000
  printf("Account balance is %d\n", 
         compute_balance(acc_ptr));

  BankAccount acc2 = { .pin = 6897, 
                      .transactions_count = 0};
  strcpy(acc2.name, "Jake");
  BankAccount* acc_ptr2 = add_bank_account(
             acc2.name, acc2.pin,acc2.type, &db);
  add_transaction(Credit, acc_ptr2, 100000, &db);
  add_transaction(Debit, acc_ptr2, 20000, &db);
  add_transaction(Credit, acc_ptr2, 50000, &db);

    // should print 130000
  printf("Account balance is %d\n", 
         compute_balance(acc_ptr2));

  // should print 143000
  printf("Total Money with bank is %d\n", 
         compute_money_with_bank(&db)); 
}

