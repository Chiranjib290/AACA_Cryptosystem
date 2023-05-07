#include <bits/stdc++.h>
using namespace std;
const int size = 7;

void int_to_binary(int n, vector<int>& row, int length) {
    int mask = 1;
    for (int i = 0; i < length; i++) {
        if ((n & mask) == 0) {
            row[length - 1 - i] = 0;
        } else {
            row[length - 1 - i] = 1;
        }
        mask <<= 1;
    }
}

string mixer(string &txt, int k) {
    string ctxt = "";
    for (char& c : txt) {
        k = k ^ c;
        ctxt.push_back((char)k);
    }
    return ctxt;
}

void writeTable(vector<vector<int>> &values, int rule, double alpha, int  k, int Km, int cipher){
    ofstream myfile;
    int nos=0;
    myfile.open("table5.txt");
    myfile<< "\tRule = " << rule << "\tAlpha = " << alpha << "\tSeed_Key = " << k << "\t\tMixer_Key = " << Km << "\t\tCipher [char]= "<< cipher << " [ "<< char(cipher) <<" ]";
    
    myfile <<"\n\nShowing target values (index) and list of number of iterations after which cipher becomes target value\n(Starting from 1st iteration, then 2nd and so on)\n\n";
    for(int j=0; j<values.size(); j++){
        if(j<10) myfile << "  ";
        else if(j<100) myfile << " ";
        myfile << j << "  -  [ ";
        if(values[j].size()==0) nos++;
        for(int i=0; i<values[j].size(); i++){
            myfile << values[j][i]+1 << ", ";
        }
        myfile << "]\n";
    }
    myfile<<"\n\n\t\tTotal Number of Unreachable Target Values when starting value is Cipher = "<<nos;
    myfile.close();

}

bool enc_async(int rule, double alpha, int cipher, int Ks, vector<vector<int>>& cipher_gets_to_tans_at_iters) {
    bool flg = true;
    int tans;
    srand(Ks);
    set<int> nos;
    vector<int> arr(size, 0);
    int_to_binary(cipher, arr, size);
    vector<int> row(8, 0);
    int_to_binary(rule, row, 8);
    for (int i = 0; i < int(1e3); i++) { ///////////////////////////////////////////////////////////////////////////////////////////////
        tans = 0;
        int mul = pow(2,size-1);
        vector<int> new_arr(size, 0);
        for (int j = 0; j < size; j++) {
            if ((double)rand() / RAND_MAX <= alpha) {
                new_arr[j] = row[7 - (4 * arr[(j - 1 + size) % size] + 2 * arr[j] + arr[(j + 1) % size])];
            } else {
                new_arr[j] = arr[j];
            }
            tans += new_arr[j]*mul;
            mul /= 2;
        }
        arr = new_arr;
        nos.insert(tans);
        cipher_gets_to_tans_at_iters[tans].push_back(i);
    }
    if(nos.size()!=pow(2,size)) flg=false;
    return flg; 
}

void encryption_file(string &filename){
    string pltxt;
    ifstream infile;
    infile.open(filename.c_str());
    if(infile.is_open()){
        while(true){
            char c = infile.get();
            if(c==EOF) break;
            if(int(c)<0){
                while(int(c)<0){
                    c = infile.get();
                }
                pltxt.push_back(' ');
            }
            pltxt.push_back(char(c%int(pow(2,size))));
        }
        infile.close();
    }
    else{
        cout<<"Wrong Plaintext Filename !\n";
        return;
    }
    string pltxt2;
    vector<int> rules = {3, 17, 19, 23, 27, 35, 39, 43, 45, 49, 51, 53, 55, 57, 59, 63, 75, 83, 99, 105, 113, 115, 119};
    int rule;//key
    double alpha;//key
    int Ks;//key
    int Km;//key
    int characterr;//key
    vector<vector<int>> cipher_to_cell;
    while(true){
        cipher_to_cell.clear();
        cipher_to_cell.resize(pow(2,size));
        srand(time(0));
        rule = rules[rand()%rules.size()];
        alpha = 0.4 + ((rand()%RAND_MAX)*(0.7-0.4) / (double(RAND_MAX))) ;
        Ks = int(rand());
        Km = rand() % (int)pow(2, size);
        pltxt2 = mixer(pltxt, Km);
        characterr = rand()%(int(pow(2,size)));
        if(enc_async(rule, alpha, characterr, Ks, cipher_to_cell)) break;
    }
    ofstream cipher;
    cipher.open("ciphertext.txt");
    for (int i=0; i<pltxt2.size()-1; i++) {
        int x = int(pltxt2[i]);
        cipher << (cipher_to_cell[x][rand()%(cipher_to_cell[x].size())]) << endl;
    }
    int x = int(pltxt2[pltxt2.size()-1]);
    cipher << (cipher_to_cell[x][rand()%(cipher_to_cell[x].size())]);
    cipher.close();
    ofstream key;
    key.open("key.txt");
    key << rule << endl << alpha << endl << Ks << endl << Km << endl << characterr;
    key.close();
    writeTable(cipher_to_cell,rule,alpha,Ks,Km,characterr);
}

int main(){
    string plaintxt = "plaintext.txt";
    encryption_file(plaintxt);
    cout<<"Encryption Done"<<endl;
    return 0;
}