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

void towrite(vector<vector<vector<int>>> &values, int rule, double alpha, vector<int> &iterations, int  k, int Km){
    ofstream myfile;
    int nos=0;
    myfile.open("table5_all.txt");
    myfile<< "\tRule = " << rule << "\tAlpha = " << alpha << "\tSeed_Key = " << k << "\t\tMixer_Key = " << Km;
    
    //cout <<"\t\tIterations or Ciphertext = [ ";
    //for(int j=0; j<iterations.size()-1; j++) myfile << iterations[j] << ", ";
    //myfile<< iterations[iterations.size()-1] <<" ]";
    
    myfile <<"\n\nTable shows is it possible to go from starting value (row number) to target value (column number) [list of iterations for which it's possible = content of cell of original 3d matrix]\n 'N' for NO and '_' for YES.\n\n   ";
    for(int j=0; j<values.size(); j++){
        if(j<10) myfile << "  ";
        else if(j<100) myfile << " ";
        myfile << j << "  ";
    }
    for(int i=0; i<values.size(); i++){
        myfile << endl;
        if(i<10) myfile << "  ";
        else if(i<100) myfile << " ";
        myfile << i;
        for(int j=0; j<values[i].size(); j++){
            if(values[i][j].size()==0){
                nos++;
                myfile << "  " << "N" << "  ";
            }
            else myfile << "  " << "_" << "  ";
        
        }
    }
    myfile<<"\n\n\t\tTotal Number of NOs Cells = "<<nos;
    myfile.close();
}

vector<vector<int>> show_async(int rule, double alpha, int x, int Ks, bool &flg) {
    int tans;
    srand(Ks);
    set<int> nos;
    vector<vector<int>> x_gets_to_tans_at_iters (pow(2,size));
    vector<int> arr(size, 0);
    int_to_binary(x, arr, size);
    vector<int> row(8, 0);
    int_to_binary(rule, row, 8);
    //for (int i = 0; i < int(pow(pow(2,size),2)); i++) {
    for (int i = 0; i < int(1e3); i++) {
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
        x_gets_to_tans_at_iters[tans].push_back(i);
    }
    if(nos.size()!=pow(2,size)) flg=false;
    return x_gets_to_tans_at_iters;
}

void cryption2(int rule, double alpha, vector<int> &ctxt, int  Ks, int Km, int &characterr, string& pltxt){
    string pltxt2 = mixer(pltxt, Km);
    vector<vector<vector<int>>> r_to_c;
    characterr = rand()%(int(pow(2,size)));
    for (int x = 0; x < pow(2, size); x++) {
        bool flg = true;
        r_to_c.push_back(show_async(rule, alpha, x, Ks, flg));
        if(!flg){
            characterr = -1;
            return;
        }
    }
    for (int i=0; i<pltxt2.size(); i++) {
        ctxt.push_back(r_to_c[characterr][int(pltxt2[i])][rand()%(r_to_c[characterr][int(pltxt2[i])].size())]);
    }
    towrite(r_to_c, rule, alpha, ctxt, Ks, Km);//writing table into txt file

}

void encryption_file(string &filename){
    ifstream infile;
    infile.open(filename.c_str());
    string pltxt;
    if(infile.is_open()){
        while(true){
            char c = infile.get();
            if(c==EOF) break;
            if(int(c)<0){
                while(int(c)<0){
                    c = infile.get();
                }
                pltxt.push_back('-');
            }
            pltxt.push_back(char(c%int(pow(2,size))));
        }
        infile.close();
    }
    else{
        ex<<"Wrong Plaintext Filename !\n";
        return;
    }
    vector<int> rules = {19, 27, 39, 43, 53, 55, 57, 83, 99, 113};
    int rule;//key
    double alpha;//key
    int Ks;//key
    int Km;//key
    int characterr;//key
    vector<int> ctxt;//list of iterations to be sent as ciphertext
    while(true){
        srand(time(0));
        rule = rules[rand()%rules.size()];
        alpha = 0.4 + ((rand()%RAND_MAX)*(0.7-0.4) / (double(RAND_MAX))) ;
        Ks = int(rand());
        Km = rand() % (int)pow(2, size);
        ctxt.clear();
        cryption2(rule, alpha, ctxt, Ks, Km, characterr, pltxt);
        if(characterr != -1)    break;
    }
    ofstream key, cipher;
    key.open("key.txt");
    key << rule << endl << alpha << endl << Ks << endl << Km << endl << characterr;
    key.close();
    cipher.open("ciphertext.txt");
    for(int i=0; i<ctxt.size()-1; i++){
        cipher<<ctxt[i]<<endl;
    }
    cipher<<ctxt[ctxt.size()-1];
    cipher.close();

}

int main(){
    string plaintxt = "plaintext.txt";
    encryption_file(plaintxt);
    ex<<"Encryption Done"<<endl;
    return 0;
}