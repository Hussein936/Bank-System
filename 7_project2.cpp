#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>

using namespace std;
const string FileName = "Bank2.txt";

void showMainMenue();

struct sClient {
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete;
};

vector<string> SplitString(string line,string delim) {
    string temp;
    vector<string> result;
    size_t pos;
    while ((pos = line.find(delim))!= std::string::npos) {
        temp = line.substr(0,pos);
        if (temp != "") result.push_back(temp);
        line.erase(0,pos+delim.length());
    }
    if (!line.empty()) result.push_back(line);
    return result;
}

string convertRecordToLine(sClient data,string delim = "#//#"){
    return data.AccountNumber+delim+data.PinCode+delim+data.Name+
    delim+data.Phone+delim+to_string(data.AccountBalance);
}

sClient convertLineToRecord(string line,string delim = "#//#"){
    vector<string> clientData = SplitString(line,delim);
    sClient client;
    client.MarkForDelete = false;
    client.AccountNumber = clientData[0];
    client.PinCode = clientData[1];
    client.Name = clientData[2];
    client.Phone = clientData[3];
    client.AccountBalance = stod(clientData[4]);

    return client;
}

bool ClientExistByAccountNumber(string accountNumber,string fileName){
    fstream file;
    file.open(fileName,ios::in);
    if(file.is_open()){
        string line;
        sClient client;
        while(getline(file,line)){
            client = convertLineToRecord(line);
            if(client.AccountNumber == accountNumber){
                file.close();
                return true;
            }
        }
    } else {
        cerr << "Error: can not open the file!\n";
    }
    
    return false;
}

sClient readNewClient(){
    sClient client;
    cout << "enter the account number!\n";
    getline(cin >> ws,client.AccountNumber);
    while(ClientExistByAccountNumber(client.AccountNumber,FileName)){
        cout << "\nClient whit account number[" << client.AccountNumber 
        <<"]already exists, enter another account number!";
        getline(cin >> ws,client.AccountNumber);
    }
    
    cout<< "enter PinCode!\n";
    cin>>client.PinCode;
    cout<<"enter Name!\n";
    getline(cin >> ws,client.Name);
    cout<<"enter Phone Number!\n";
    cin>>client.Phone;
    cout<<"enter account balance!\n";
    cin>>client.AccountBalance;

    return client;
}

vector<sClient> loadAClientsDataFromFile(const string fileName){
    vector<sClient> allClients;
    fstream file;
    file.open(fileName,ios::in);
    if(file.is_open()){
        string line;
        while(getline(file,line)){
            allClients.push_back(convertLineToRecord(line));
        }
    } else {
        cerr << "Error: can not open the file!\n";
    }
    
    return allClients;
}

void printClientRecordLine(sClient client){
    cout << "| " << setw(15) << left <<client.AccountNumber;
    cout << "| " << setw(10) << left <<client.PinCode;
    cout << "| " << setw(25) << left <<client.Name;
    cout << "| " << setw(12) << left <<client.Phone;
    cout << "| " << setw(12) << left <<client.AccountBalance;
}

void ShowAllClientsScreen(){
    vector<sClient> allClients = loadAClientsDataFromFile(FileName);
    cout << "\n\t\t\t\t\tClient List (" << allClients.size() << ") Client(s).";
    cout << "\n--------------------------------------------";
    cout << "----------------------------------------------\n" << endl;

    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(25) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";

    cout << "\n--------------------------------------------";
    cout << "----------------------------------------------\n" << endl;
    if(allClients.size() == 0)
        cout << "No Clients Available in the System!\n";
    else
        for(const sClient &client: allClients){
            printClientRecordLine(client);
            cout << endl;
        }

    cout << "\n--------------------------------------------";
    cout << "----------------------------------------------\n" << endl;
}

void printClientCard(sClient client){
    cout<<"\n Client Data:\n";
    cout<<string(25,'-');
    cout<<"\nAccount Number  : " << client.AccountNumber;
    cout<<"\nPin code        : " << client.PinCode;
    cout<<"\nName            : " << client.Name;
    cout<<"\nPhone           : " << client.Phone;
    cout<<"\nAccount Balance : " << client.AccountBalance << endl;
    cout << string(25,'-') << endl;
}

bool findClientByAccountNumber(const string &accountNumber,const vector<sClient> &allClients,sClient &Client){
    for(const sClient &client:allClients){
        if(client.AccountNumber == accountNumber){
            Client = client;
            return true;
        }
    }
    return false;
}

sClient changeClientRecord(string accountNumber){
    sClient client;

    client.MarkForDelete = false;
    client.AccountNumber = accountNumber;

    cout<<"Enter PIN code!\n";
    cin>>client.PinCode;

    cout<<"Enter the Name!\n";
    getline(cin >> ws,client.Name);

    cout<<"Enter Phone number!\n";
    cin>>client.Phone;

    cout<<"Enter the account balance!\n";
    cin>>client.AccountBalance;

    return client;
}

bool markClientForDelete(const string &accountNumber,vector<sClient> &allClients){
    for(sClient &client:allClients){
        if(client.AccountNumber == accountNumber){
            client.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

vector<sClient> saveClientsDataToFile(const string &fileName,vector<sClient> &allClients){
    vector<sClient> NewClientsData;
    fstream file;
    file.open(fileName,ios::out);

    if(file.is_open()){
        for(sClient &client: allClients){
            string line;
            if(client.MarkForDelete == false){
                line = convertRecordToLine(client);
                file << line << endl;
            }
        }
        file.close();
    } else {
        cerr << "Error: can not open the file!\n";
    }
    
    NewClientsData = loadAClientsDataFromFile(fileName);
    return NewClientsData;
}

void addLineToFile(const string &fileName,const string &line){
    fstream file;
    file.open(fileName,ios::out | ios::app);
    
    if(file.is_open()){
        file << line << endl;
        file.close();
    } else {
        cerr << "Error: can not open the file!\n";
    }
}

void addNewClient(){
    sClient client;
    client = readNewClient();
    addLineToFile(FileName,convertRecordToLine(client));
}

void addNewClients(){
    char addMore = 'Y';
    do{
        cout<<"Adding New Client\n\n";
        addNewClient();
        cout << "\nClient added successfully, do want to add more clients? Y/N?\n";
        cin >> addMore;
    } while(toupper(addMore) == 'Y');
}

bool deleteClientByAccountNumber(string accountNumber,vector<sClient> &allClients){
    char del = 'n';
    sClient client;
    if(findClientByAccountNumber(accountNumber,allClients,client)){
    printClientCard(client);
    cout<<"Are you sure that you want to delete this client? Y/N?\n";
    cin >> del;
        if(toupper(del) == 'Y'){
            markClientForDelete(accountNumber,allClients);
            allClients = saveClientsDataToFile(FileName,allClients);
            cout << "n\n Client Deleted Successfully.\n";
            return true;
        }

    } else {
        cout << "Client with Account Number (" << accountNumber << ") does not exist!\n";
        return false;
    }

}

bool updateClientByAccountNumber(string accountNumber,vector<sClient> &allClients){
    char answer = 'n';
    sClient client;

    if(findClientByAccountNumber(accountNumber,allClients,client)){
        printClientCard(client);
        cout<<"Are you sure that you want to change this client data? Y/N\n";
        cin >> answer;
        if(toupper(answer) == 'Y'){
            for(sClient &client : allClients){
                if(client.AccountNumber == accountNumber){
                    client = changeClientRecord(accountNumber);
                    break;
                }
            }
            saveClientsDataToFile(FileName,allClients);
            cout << "\nclient updated succussfully!\n";
            return true;
        }
    } else {
        cout << "the client with account number(" << accountNumber
        << ") does not exist!\n";
        return false;
    }
}

string readClientAccountNumber(){
    string accountNumber;
    cout << "\nPlease enter the account number!\n";
    cin >> accountNumber;
    return accountNumber;
}

void showDeletedClientScreen(){
    cout<<"\n---------------------------------\n";
    cout << "\tDelete Client Screen";
    cout<<"\n---------------------------------\n";

    vector<sClient> allClients = loadAClientsDataFromFile(FileName);
    string accountNumber = readClientAccountNumber();
    deleteClientByAccountNumber(accountNumber,allClients);
}

void showUpdatedClientScreen(){
    cout<<"\n---------------------------------\n";
    cout << "\t Update Client Screen";
    cout<<"\n---------------------------------\n"; 

    vector<sClient> allClients = loadAClientsDataFromFile(FileName);
    string accountNumber = readClientAccountNumber();
    updateClientByAccountNumber(accountNumber,allClients);
}

void showAddNewClientScreen(){
    cout<<"\n---------------------------------\n";
    cout << "\t Add New Client Screen";
    cout<<"\n---------------------------------\n";

    addNewClient();
}

void showFindClientScreen(){
    cout<<"\n---------------------------------\n";
    cout << "\t Find Client Screen";
    cout<<"\n---------------------------------\n";

    vector<sClient> allClients = loadAClientsDataFromFile(FileName);
    sClient client;
    string accountNumber = readClientAccountNumber();
    if(findClientByAccountNumber(accountNumber,allClients,client))
        printClientCard(client);
    else 
        cout << "\n Client with account number (" << accountNumber
        << ") does not exist!\n";
}

void showEndScreen(){
    cout<<"\n---------------------------------\n";
    cout << "\tProgram Ends";
    cout<<"\n---------------------------------\n";
}

void goBackToMainMenue(){
    cout << "\n\nPress any key to go back to the main menue...\n";
    system("pause>0");
    showMainMenue();
}

enum enTransactionOptions {
    eDeposite = 1, eWithdraw = 2,
    eTotalBalances = 3, eMainMenue = 4
};

double ReadAmountOfMoney() {
    double  AmountOfMoney;

    cout << "Please enter the amount of money: \n";
    cin >> AmountOfMoney;

    return AmountOfMoney;
}

void DepositeByAccountNumber(const string& accountNumber, vector<sClient> &allClients) {
    char answer = 'n';
    sClient client;
    double moneyToDeposite = 0;
 
    if (findClientByAccountNumber(accountNumber, allClients, client)) {

        printClientCard(client);
        moneyToDeposite = ReadAmountOfMoney();
        double newBalance = client.AccountBalance + moneyToDeposite;

        cout << "Are you sure that you want to do this transaction your account balance will be: " 
             << newBalance << " y/n?\n";
        cin >> answer;

        if (tolower(answer) == 'y') {
            for (sClient& Client : allClients) {
                if (Client.AccountNumber == accountNumber) {
                    Client.AccountBalance += moneyToDeposite;
                    break;
                }
            }
            saveClientsDataToFile(FileName, allClients);
            cout << "the transaction is done successfully! your balance now is: " << newBalance << endl;
        }
    }
    else {
        cout << "the account number (" << accountNumber << ") does not exist!\n";
    }
}

void WithdrawByAccountNumber(const string& accountNumber, vector<sClient>& allClients) {
    char answer = 'n';
    sClient client;
    double moneyToWithdraw = 0;

    if (findClientByAccountNumber(accountNumber, allClients, client)) {

        printClientCard(client);
        moneyToWithdraw = ReadAmountOfMoney();
        if (moneyToWithdraw >= client.AccountBalance) {
            cout << "your account balance is not enough for this transaction!\n";
            return ;
        }
        double newBalance = client.AccountBalance - moneyToWithdraw;

        cout << "Are you sure that you want to do this transaction your account balance will be: "
            << newBalance << " y/n?\n";
        cin >> answer;

        if (tolower(answer) == 'y') {
            for (sClient& Client : allClients) {
                if (Client.AccountNumber == accountNumber) {
                    Client.AccountBalance -= moneyToWithdraw;
                    break;
                }
            }
            saveClientsDataToFile(FileName, allClients);
            cout << "the transaction is done successfully! your balance now is: " << newBalance << endl;
        }
    }
    else {
        cout << "the account number (" << accountNumber << ") does not exist!\n";
    }
}

void printClientBalance(sClient client) {
    cout << "|" << setw(15) << left << client.AccountNumber;
    cout << "|" << setw(25) << left << client.Name;
    cout << "|" << setw(15) << left << client.AccountBalance;
    cout << endl;
}

void printClientsBalances(const vector<sClient>& allClients) {
    cout << string(75,'-') << endl;
    cout << "|" << left << setw(15) << "Account Number";
    cout << "|" << left << setw(25) << "Name";
    cout << "|" << left << setw(15) << "Account Balance" << endl;
    cout << string(75,'-') << endl;

    if (allClients.size() == 0) {
        cout << "there are no clients \n";
        return;
    }
    else {
        double TotalBalances = 0;
        for (const sClient& client : allClients) {
            printClientBalance(client);
            TotalBalances += client.AccountBalance;
        }
        cout << "\n Total Balances is: " << TotalBalances << endl;
    }
}

short readTransactionMenue() {
    short choice;

    cout << "Choose Your Transaction! [1] to [4]\n";
    cin >> choice;

    return choice;
}

void showDepositeScreen() {
    cout << "\n---------------------------------\n";
    cout << "\tDeposite Screen";
    cout << "\n---------------------------------\n";

    vector<sClient> allClients = loadAClientsDataFromFile(FileName);
    string AccountNumber = readClientAccountNumber();
    DepositeByAccountNumber(AccountNumber, allClients);
}

void showWithdrawScreen() {
    cout << "\n---------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n---------------------------------\n";

    vector<sClient> allCliens = loadAClientsDataFromFile(FileName);
    string AccountNumber = readClientAccountNumber();
    WithdrawByAccountNumber(AccountNumber, allCliens);
}

void showAllBalancesScreen() {
    vector<sClient> allClients = loadAClientsDataFromFile(FileName);

    cout << "\t\tAll Balances Screen of " << allClients.size() << " Client(s).\n";

    printClientsBalances(allClients);
}

void performTransactionScreen(enTransactionOptions TransactionChoice) {
    switch (TransactionChoice) {
    case enTransactionOptions::eDeposite: {
        system("cls");
        showDepositeScreen();
        goBackToMainMenue();
        break;
    }
    case enTransactionOptions::eWithdraw: {
        system("cls");
        showWithdrawScreen();
        goBackToMainMenue();
        break;
    }
    case enTransactionOptions::eTotalBalances: {
        system("cls");
        showAllBalancesScreen();
        goBackToMainMenue();
        break;
    }
    case enTransactionOptions::eMainMenue: {
        goBackToMainMenue();
        break;
    }

    }
}

void showTransactionScreen() {
    system("cls");
    cout << "\n---------------------------------\n";
    cout << "\tTransaction Menue Screen";
    cout << "\n---------------------------------\n";

    cout << "[1] Deposite \n";
    cout << "[2] Withdraw \n";
    cout << "[3] Total Balances \n";
    cout << "[4] Main Menue \n";

    cout << "---------------------------------\n";
    cout << "---------------------------------\n";

    performTransactionScreen(enTransactionOptions(readTransactionMenue()));
}


enum enMainMenueOptions{
    eShowClientsList = 1, eAddNewClient = 2,
    eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eTransactions =6,
    eExit = 7
};

short readMainMenueOption(){
    short choice = 0;
    cout <<  "Choose the transaction that you want! [1 to 7]\n";
    cin >> choice;
    return choice;
}

void perfomMainMenueOption(enMainMenueOptions mainMenueOption){
    switch (mainMenueOption){
        case enMainMenueOptions::eShowClientsList: {
            system("cls");
            ShowAllClientsScreen();
            goBackToMainMenue();
            break;
        }

        case enMainMenueOptions::eAddNewClient: {
            system("cls");
            showAddNewClientScreen();
            goBackToMainMenue();
            break;
        }

        case enMainMenueOptions::eDeleteClient: {
            system("cls");
            showDeletedClientScreen();
            goBackToMainMenue();
            break;
        }

        case enMainMenueOptions::eUpdateClient: {
            system("cls");
            showUpdatedClientScreen();
            goBackToMainMenue();
            break;
        }

        case enMainMenueOptions::eFindClient: {
            system("cls");
            showFindClientScreen();
            goBackToMainMenue();
            break;
        }

        case enMainMenueOptions::eTransactions: {
            system("cls");
            showTransactionScreen();
            break;
        }

        case enMainMenueOptions::eExit: {
            system("cls");
            showEndScreen();
            break;
        }
    }
}

void showMainMenue(){
    system("cls");
    cout << string(35,'=') << endl;
    cout << "\t\tMain Menue Screen\n";
    cout << string(35,'=') << endl;

    cout<<"\t[1] Show Client List. \n";
    cout<<"\t[2] Add New Client. \n";
    cout<<"\t[3] Delete Client. \n";
    cout<<"\t[4] Update Client Data. \n";
    cout<<"\t[5] Find Client. \n";
    cout<<"\t[6] Transactions. \n";
    cout<<"\t[7] Exit. \n";

    cout << string(35,'=') << endl;
    perfomMainMenueOption(enMainMenueOptions(readMainMenueOption()));
}

int main(){
    showMainMenue();
    system("pause>0");
    return 0;
}