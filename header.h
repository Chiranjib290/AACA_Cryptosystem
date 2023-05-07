#include <bits/stdc++.h>
const int size = 10;

void int_to_binary(int n, std::vector<int>& row, int length) {
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

std::vector<int> mixer(std::string &txt, int k) {
    std::vector<int> ctxt;
    int v = int(pow(2,5));
    for (int i=0; i<txt.size(); i+=2) {
        int x = int(txt[i]);
        if(i+1<txt.size()) x = int(txt[i+1]) + x*v;
        k = k ^ x;
        ctxt.push_back(k);
    }
    return ctxt;
}

void writeTable(std::vector<std::vector<int>> &values, int rule, double alpha, int  k, int Km, int cipher){
    std::ofstream myfile;
    int nos=0;
    myfile.open("table10.txt");
    myfile<< "\tRule = " << rule << "\tAlpha = " << alpha << "\tSeed_Key = " << k << "\t\tMixer_Key = " << Km << "\t\tCipher = "<< cipher;
    
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

bool enc_async(int rule, double alpha, int cipher, int Ks, std::vector<std::vector<int>>& cipher_gets_to_tans_at_iters) {
    bool flg = true;
    int tans;
    srand(Ks);
    std::set<int> nos;
    std::vector<int> arr(size, 0);
    int_to_binary(cipher, arr, size);
    std::vector<int> row(8, 0);
    int_to_binary(rule, row, 8);
    for (int i = 0; i < int(1e4); i++) { //iterations
        tans = 0;
        int mul = pow(2,size-1);
        std::vector<int> new_arr(size, 0);
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

void encryption_file(std::string &filename){
    std::string pltxt;
    std::ifstream infile;
    infile.open(filename.c_str());
    if(infile.is_open()){
        while(true){
            char c = infile.get();
            if(c==EOF) break;
            if(int(c)<0){
                while(int(c)<0){
                    c = infile.get();
                }
                pltxt.push_back(char(27));
            }
            if(c>='A' && c<='Z') c = c - 'A' + 'a';
            if(c>='a' && c<='z') c = c-'a'+1;
            else if(c=='\n') c = char(28);//'\n'==28
            else c = char(27);//' '==27
            pltxt.push_back(c);
        }
        infile.close();
    }
    else{
        std::cout<<"Wrong Plaintext Filename !\n";
        return;
    }
    std::vector<int> pltxt2;
    std::vector<int> rules = { 19 , 27 , 39 , 43 , 51 , 53 , 55 , 57 , 83 , 99 , 105 , 113 };
    int rule;//key
    double alpha;//key
    int Ks;//key
    int characterr;//key
    std::vector<std::vector<int>> cipher_to_cell;
    while(true){
        cipher_to_cell.clear();
        cipher_to_cell.resize(pow(2,size));
        srand(time(0));
        rule = rules[rand()%rules.size()];
        alpha = 0.4 + ((rand()%RAND_MAX)*(0.7-0.4) / (double(RAND_MAX))) ;
        Ks = int(rand());
        characterr = rand()%(int(pow(2,size)));
        if(enc_async(rule, alpha, characterr, Ks, cipher_to_cell)) break;
    }
    int Km = rand() % (int)pow(2, size);
    pltxt2 = mixer(pltxt, Km);
    std::ofstream cipher;
    cipher.open("ciphertext.txt");
    for (int i=0; i<pltxt2.size(); i++) {
        int x = pltxt2[i];
        cipher << (cipher_to_cell[x][rand()%(cipher_to_cell[x].size())]);
        if(i+1<pltxt2.size()) cipher << '\n';
    }
    cipher.close();
    std::ofstream key;
    key.open("key.txt");
    key << rule << '\n' << alpha << '\n' << Ks << '\n' << Km << '\n' << characterr;
    key.close();
    writeTable(cipher_to_cell,rule,alpha,Ks,Km,characterr);
}



std::string demix(std::vector<int> &txt, int k) {
    std::string ptxt = "";
    int v = int(pow(2,5));
    for (int i=0; i<txt.size(); i++) {
        int x = k ^ txt[i];
        char c;
        if(x>v){
            c = char(x/v)+'a'-1;
            if(c-'a'+1==char(27)) c = ' ';
            if(c-'a'+1==char(28)) c = '\n';
            ptxt.push_back(c);
        }
        c = char(x%v)+'a'-1;
        if(c-'a'+1==char(27)) c = ' ';
        if(c-'a'+1==char(28)) c = '\n';
        ptxt.push_back(c);
        k = txt[i];
    }
    return ptxt;
}

void dec_async(int rule, double alpha, int x, int iterations, int Ks, std::vector<int>& vals) {
    int ans;
    srand(Ks);
    std::vector<int> arr(size, 0);
    int_to_binary(x, arr, size);
    std::vector<int> row(8, 0);
    int_to_binary(rule, row, 8);
    for (int i = 0; i < iterations; i++) {
        std::vector<int> new_arr(size, 0);
        int mul = int(pow(2, size - 1));
        ans = 0;
        for (int j = 0; j < size; j++) {
            if ((double)rand() / RAND_MAX <= alpha) {
                new_arr[j] = row[7 - (4 * arr[(j - 1 + size) % size] + 2 * arr[j] + arr[(j + 1) % size])];
            } else {
                new_arr[j] = arr[j];
            }
            ans += new_arr[j]*mul;
            mul = mul/2;
        }
        arr = new_arr;
        vals[i]=ans;
    }
}

void decryption_file(std::string &encname, std::string &keys){
    std::ifstream kfile, cfile;
    std::vector<int> ctxt;
    int rule, Ks, Km, characterr;
    double alpha;
    int mx = -1;
    kfile.open(keys.c_str());
    if(kfile.is_open()){
        kfile >> rule;
        kfile >> alpha;
        kfile >> Ks;
        kfile >> Km;
        kfile >> characterr;
        kfile.close();
    }else{
        std::cout << "Wrong Key File name!\n";
        return;
    }
    cfile.open(encname.c_str());
    if(cfile.is_open()){
        int x;
        while(cfile>>x){
            if(mx<x) mx = x;
            ctxt.push_back(x);
        }
        cfile.close();
    }else{
        std::cout << "Wrong Ciphertext File name!\n";
        return;
    }
    mx++;
    std::vector<int> p2;
    std::vector<int> characterr_at_i_iters (mx);
    dec_async(rule, alpha, characterr, mx, Ks, characterr_at_i_iters);
    for (int &i : ctxt) {
        p2.push_back(characterr_at_i_iters[i]);
    }
    std::string decryption = demix(p2,Km);
    std::ofstream dec;
    dec.open("Decrypted.txt");
    dec << decryption;
    dec.close();
}