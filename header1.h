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

std::vector<int> mixer(std::string &txt, int mix_key) {
    std::vector<int> ctxt, row(5,0), row1(10,0);
    int v = int(pow(2,5));
    for (int i=0; i<txt.size(); i+=2) {
        int x = int(txt[i]);
        if(i+1<txt.size()){
            x = int(txt[i+1]) + x*v;
        }
        mix_key = mix_key ^ x;
        ctxt.push_back(mix_key);
    }
    return ctxt;
}
std::string demix(std::vector<int> &mixed_text, int mix_key) {
    std::string plaintext = "";
    std::vector<int> row(5,0), row1(10,0);
    int v = int(pow(2,5));
    for (int i=0; i<mixed_text.size(); i++) {
        int x = mix_key ^ mixed_text[i];
        char c;
        if(x>v){
            c = char(x/v)+'a'-1;
            if(c-'a'+1==char(27)) c = ' ';
            if(c-'a'+1==char(28)) c = '\n';
            plaintext.push_back(c);
        }
        c = char(x%v)+'a'-1;
        if(c-'a'+1==char(27)) c = ' ';
        if(c-'a'+1==char(28)) c = '\n';
        plaintext.push_back(c);
        mix_key = mixed_text[i];
    }
    return plaintext;
}

bool enc_async(int rule, long double alpha, int start_value, unsigned long long int Ks, std::vector<std::vector<int>>& start_value_to_index_at_iterations) {
    bool flg = true;
    int tans;
    srand(Ks);
    std::set<int> nos;
    std::vector<int> arr(size, 0);
    int_to_binary(start_value, arr, size);
    std::vector<int> row(8, 0);
    int_to_binary(rule, row, 8);
    for (int i = 0; i < int(1e4); i++) { //iterations
        tans = 0;
        int mul = pow(2,size-1);
        std::vector<int> new_arr(size, 0);
        for (int j = 0; j < size; j++) {
            if ((long double)rand() / RAND_MAX <= alpha) {
                new_arr[j] = row[7 - (4 * arr[(j - 1 + size) % size] + 2 * arr[j] + arr[(j + 1) % size])];
            } else {
                new_arr[j] = arr[j];
            }
            tans += new_arr[j]*mul;
            mul /= 2;
        }
        arr = new_arr;
        nos.insert(tans);
        start_value_to_index_at_iterations[tans].push_back(i);
    }
    if(nos.size()!=pow(2,size)) flg=false;
    return flg; 
}

void encryption_file(std::string &filename){
    std::string plaintext;
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
                plaintext.push_back(char(27));
            }
            if(c>='A' && c<='Z') c = c - 'A' + 'a';
            if(c>='a' && c<='z') c = c-'a'+1;
            else if(c=='\n') c = char(28);//'\n'==28
            else c = char(27);//' '==27
            plaintext.push_back(c);
        }
        infile.close();
    }
    else{
        std::cout<<"Wrong Plaintext Filename !\n";
        return;
    }
    std::vector<int> Mixed_Plaintext;
    std::vector<int> rules = { 19 , 27 , 39 , 43 , 53 , 55 , 57 , 83 , 99 , 105 , 113 };
    int rule;//key
    long double alpha;//key
    unsigned long long int Seed_Key;//key
    int start_value;//key
    std::vector<std::vector<int>> start_value_to_index_at_iterations;
    while(true){
        start_value_to_index_at_iterations.clear();
        start_value_to_index_at_iterations.resize(pow(2,size));
        srand(time(0));
        rule = rules[rand()%rules.size()];
        alpha = 0.2 + ((rand()*rand()*rand()*rand()*rand()*rand())%RAND_MAX)*(0.9-0.2) / ((long double)(RAND_MAX)) ;
        Seed_Key = (unsigned long long int)(rand()*rand()*rand()*rand()*rand()*rand()*rand()*rand()*rand()*rand()*rand()*rand()*rand()*rand()*rand()*rand()*rand()*rand());
        start_value = rand()%(int(pow(2,size)));
        if(enc_async(rule, alpha, start_value, Seed_Key, start_value_to_index_at_iterations)) break;
    }
    int Mix_Key = rand() % (int)pow(2, size);//key
    Mixed_Plaintext = mixer(plaintext, Mix_Key);
    std::ofstream cipher;
    cipher.open("ciphertext.txt");
    for (int i=0; i<Mixed_Plaintext.size(); i++) {
        int x = Mixed_Plaintext[i];
        int y = start_value_to_index_at_iterations[x][rand()%(start_value_to_index_at_iterations[x].size())];
        cipher << y;
        if(i+1<Mixed_Plaintext.size()) cipher << '\n';
    }
    cipher.close();
    std::ofstream key;
    key.open("key.txt");
    key << rule << '\n' << alpha << '\n' << Seed_Key << '\n' << Mix_Key << '\n' << start_value;
    key.close();
}

void dec_async(int rule, long double alpha, int start_value, int max_iterations, unsigned long long int Seed_Key, std::vector<int>& start_value_at_i_iters) {
    int ans;
    srand(Seed_Key);
    std::vector<int> arr(size, 0);
    int_to_binary(start_value, arr, size);
    std::vector<int> row(8, 0);
    int_to_binary(rule, row, 8);
    for (int i = 0; i <= max_iterations; i++) {
        std::vector<int> new_arr(size, 0);
        int mul = int(pow(2, size - 1));
        ans = 0;
        for (int j = 0; j < size; j++) {
            if ((long double)rand() / RAND_MAX <= alpha) {
                new_arr[j] = row[7 - (4 * arr[(j - 1 + size) % size] + 2 * arr[j] + arr[(j + 1) % size])];
            } else {
                new_arr[j] = arr[j];
            }
            ans += new_arr[j]*mul;
            mul = mul/2;
        }
        arr = new_arr;
        start_value_at_i_iters[i]=ans;
    }
}

void decryption_file(std::string &encname, std::string &keys){
    std::ifstream kfile, cfile;
    std::vector<int> ciphertext;
    int rule, mix_key, start_value;
    unsigned long long int seed_key;
    long double alpha;
    int max_iterations = -1;
    kfile.open(keys.c_str());
    if(kfile.is_open()){
        kfile >> rule;
        kfile >> alpha;
        kfile >> seed_key;
        kfile >> mix_key;
        kfile >> start_value;
        kfile.close();
    }else{
        std::cout << "Wrong Key File name!\n";
        return;
    }
    cfile.open(encname.c_str());
    if(cfile.is_open()){
        int x;
        while(cfile>>x){
            if(max_iterations<x) max_iterations = x;
            ciphertext.push_back(x);
        }
        cfile.close();
    }else{
        std::cout << "Wrong Ciphertext File name!\n";
        return;
    }
    max_iterations++;
    std::vector<int> mixed_text;
    std::vector<int> start_value_at_i_iters (max_iterations);
    dec_async(rule, alpha, start_value, max_iterations, seed_key, start_value_at_i_iters);
    for (int &i : ciphertext) {
        int x = start_value_at_i_iters[i];
        mixed_text.push_back(x);
    }
    std::string decrypted_text = demix(mixed_text,mix_key);
    std::ofstream dec;
    dec.open("Decrypted.txt");
    dec << decrypted_text;
    dec.close();
}