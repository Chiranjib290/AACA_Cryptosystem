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

string demix(string &txt, int k) {
    string ptxt = "";
    for (char& c : txt) {
        ptxt.push_back((char)(k ^ c));
        k = c;
    }
    return ptxt;
}

void show_async(int rule, double alpha, int x, int iterations, int Ks, vector<char>& vals) {
    int ans;
    srand(Ks);
    vector<int> arr(size, 0);
    int_to_binary(x, arr, size);
    vector<int> row(8, 0);
    int_to_binary(rule, row, 8);
    for (int i = 0; i < iterations; i++) {
        vector<int> new_arr(size, 0);
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
        vals[i]=char(ans);
    }
}

void decryption_file(string &encname, string &keys){
    ifstream kfile, cfile;
    vector<int> ctxt;
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
        ex << "Wrong Key File name!\n";
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
        ex << "Wrong Ciphertext File name!\n";
        return;
    }
    mx++;
    string p2 = "";
    vector<char> characterr_at_i_iters (mx);
    show_async(rule, alpha, characterr, mx, Ks, characterr_at_i_iters);
    for (int &i : ctxt) {
        p2.push_back(characterr_at_i_iters[i]);
    }
    string decryption = demix(p2,Km);
    ofstream dec;
    dec.open("Decrypted.txt");
    dec << decryption;
    dec.close();
}

int main(){
    string encname = "ciphertext.txt";
    string keys = "key.txt";
    decryption_file(encname,keys);
    ex<<"Decryption Done"<<endl;
    return 0;
}