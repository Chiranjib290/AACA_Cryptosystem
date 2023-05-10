#include <bits/stdc++.h>
using namespace std;

vector<int> intersection(vector<int> v1, vector<int> v2){
    vector<int> v3;
    set_intersection(v1.begin(),v1.end(),v2.begin(),v2.end(),back_inserter(v3));
    return v3;
}

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

bool show_async(int size, int rule, double alpha, int cipher, int Ks) {
    int tans;
    srand(Ks);
    set<int> nos;
    vector<int> arr(size, 0);
    int_to_binary(cipher, arr, size);
    vector<int> row(8, 0);
    int_to_binary(rule, row, 8);
    
    for (int i = 0; i < int(1e4); i++) {
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
        if(nos.size()==pow(2,size)) return true;
    }
    return false; 
}

void checking(){
    vector<int> all_rev_rules;
    for(int i=0; i<256; i++) all_rev_rules.push_back(i);
    
    for(int size=10; size < 11; size++){
        vector<bool> rules (all_rev_rules.size(), false);
        
        for(int rnd=1; rnd<20; rnd++){
            srand(time(0)+rnd*10) ;
            double alpha = 0.4 + (((rand()+rnd*6153)%RAND_MAX)*(0.7-0.4) / (double(RAND_MAX))) ;
            //double alpha = 0.5;
            int Ks = int(rand()) ;
            ex << "\nFor Size = "<< size << ",     Round = " << rnd << ",     Seed Key = "<< Ks << ",     Alpha = "<< alpha << "\nNew Rev Rules :- [ ";
            
            for(int rule=0; rule<all_rev_rules.size(); rule++){
                if(!rules[rule]){
                    bool flg = true;
                    for(int x=0; x<pow(2,size); x++){
                        if(!show_async(size, rule, alpha, x, Ks)){
                            flg = false;
                            break;
                        }
                    }
                    if(flg){
                        rules[rule] = true;
                        ex<<rule<<" , ";
                    }
                }
            }
            ex<<"]\n";
        }
        vector<int> v; for(int x=0; x<all_rev_rules.size(); x++) if(rules[x]) v.push_back(x);
        all_rev_rules = intersection(v, all_rev_rules);
        ex<<"\nUnion :- [ "; for(int &rl: v) ex<<rl<<" , "; ex<<"]\n\n";
    }
    //cout<<"\nIntersection of Unions :- [ "; for(int &rl: all_rev_rules) cout<<rl<<", "; cout<<"]\n\n";
}

int main(){
    checking();
    // 1 = { 1 , 3 , 5 , 7 , 9 , 11 , 13 , 15 , 17 , 19 , 21 , 23 , 25 , 27 , 29 , 31 , 33 , 35 , 37 , 39 , 41 , 43 , 45 , 47 , 49 , 51 , 53 , 55 , 57 , 59 , 61 , 63 , 65 , 67 , 69 , 71 , 73 , 75 , 77 , 79 , 81 , 83 , 85 , 87 , 89 , 91 , 93 , 95 , 97 , 99 , 101 , 103 , 105 , 107 , 109 , 111 , 113 , 115 , 117 , 119 , 121 , 123 , 125 , 127 }
    // 2 = { 1 , 3 , 9 , 11 , 17 , 19 , 25 , 27 , 33 , 35 , 37 , 39 , 41 , 43 , 45 , 47 , 49 , 51 , 53 , 55 , 57 , 59 , 61 , 63 , 65 , 67 , 73 , 75 , 81 , 83 , 89 , 91 , 97 , 99 , 101 , 103 , 105 , 107 , 109 , 111 , 113 , 115 , 117 , 119 , 121 , 123 , 125 , 127 }
    // 3 = { 1 , 3 , 7 , 9 , 11 , 17 , 19 , 21 , 23 , 25 , 27 , 31 , 33 , 35 , 39 , 41 , 43 , 47 , 49 , 51 , 53 , 55 , 57 , 59 , 61 , 63 , 65 , 67 , 81 , 83 , 87 , 97 , 99 , 103 , 105 , 107 , 111 , 113 , 115 , 117 , 119 , 121 , 123 , 125 , 127 }
    // 4 = { 1 , 3 , 9 , 11 , 17 , 19 , 25 , 27 , 33 , 35 , 37 , 39 , 41 , 43 , 45 , 47 , 49 , 51 , 53 , 55 , 57 , 59 , 61 , 63 , 65 , 67 , 75 , 81 , 83 , 89 , 91 , 97 , 99 , 101 , 103 , 107 , 111 , 113 , 115 , 117 , 119 , 121 , 123 , 125 , 127 }
    // 5 = { 1 , 3 , 7 , 9 , 11 , 17 , 19 , 21 , 23 , 25 , 27 , 31 , 33 , 35 , 37 , 39 , 41 , 43 , 45 , 47 , 49 , 51 , 53 , 55 , 57 , 59 , 61 , 63 , 65 , 67 , 75 , 81 , 83 , 87 , 89 , 91 , 97 , 99 , 101 , 103 , 105 , 107 , 111 , 113 , 115 , 117 , 119 , 121 , 123 , 125 , 127 }
    // 6 = { 1 , 3 , 9 , 11 , 17 , 19 , 25 , 27 , 33 , 35 , 39 , 41 , 43 , 47 , 49 , 51 , 53 , 55 , 57 , 59 , 61 , 63 , 65 , 67 , 81 , 83 , 97 , 99 , 103 , 105 , 107 , 111 , 113 , 115 , 117 , 119 , 121 , 123 , 125 , 127 }
    // 7 = { 1 , 3 , 7 , 9 , 11 , 17 , 19 , 21 , 23 , 25 , 27 , 31 , 33 , 35 , 37 , 39 , 41 , 43 , 45 , 47 , 49 , 51 , 53 , 55 , 57 , 59 , 61 , 63 , 65 , 67 , 75 , 81 , 83 , 87 , 89 , 91 , 97 , 99 , 101 , 103 , 105 , 107 , 111 , 113 , 115 , 117 , 119 , 121 , 123 , 125 , 127 }
    // 8 = { 1 , 3 , 9 , 11 , 17 , 19 , 25 , 27 , 33 , 35 , 37 , 39 , 41 , 43 , 45 , 47 , 49 , 51 , 53 , 55 , 57 , 59 , 61 , 63 , 65 , 67 , 75 , 81 , 83 , 89 , 91 , 97 , 99 , 101 , 103 , 107 , 111 , 113 , 115 , 117 , 119 , 121 , 123 , 125 , 127 }
    // 9 = { 1 , 3 , 7 , 9 , 11 , 17 , 19 , 21 , 23 , 25 , 27 , 31 , 33 , 35 , 39 , 41 , 43 , 47 , 49 , 51 , 53 , 55 , 57 , 59 , 61 , 63 , 65 , 67 , 81 , 83 , 87 , 97 , 99 , 103 , 105 , 107 , 111 , 113 , 115 , 117 , 119 , 121 , 123 , 125 , 127 }
    //10 = { 1 , 3 , 9 , 11 , 17 , 19 , 25 , 27 , 33 , 35 , 37 , 39 , 41 , 45 , 47 , 49 , 51 , 53 , 55 , 57 , 59 , 61 , 63 , 65 , 67 , 75 , 81 , 83 , 89 , 91 , 97 , 99 , 101 , 103 , 105 , 107 , 111 , 115 , 117 , 119 , 121 , 123 , 125 , 127 }
    //44
    //10 e4 = [ 19 , 27 , 39 , 43 , 51 , 53 , 55 , 57 , 83 , 99 , 105 , 113 ]
    // intersection = {1, 3, 9, 11, 17, 19, 25, 27, 33, 35, 39, 41, 43, 47, 49, 51, 53, 55, 57, 59, 61, 63, 65, 67, 81, 83, 97, 99, 103, 107, 111, 113, 115, 117, 119, 121, 123, 125, 127}
    return 0;
}