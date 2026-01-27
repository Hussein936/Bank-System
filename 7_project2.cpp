#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>

using namespace std;
const string FileName = "Bank2.txt";

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
    while ((pos = line.find(delim)!= std::string::npos)) {
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
    getline(cin,client.Name);
    cout<<"enter Phone Number!\n";
    cin>>client.Phone;
    cout<<"enter account balance!\n";
    cin>>client.AccountBalance;

    return client;
}