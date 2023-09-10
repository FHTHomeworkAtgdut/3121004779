/*
 * @Author: SkylarShadow 851428179@qq.com
 * @Date: 2023-09-10 23:41:13
 * @LastEditors: SkylarShadow 851428179@qq.com
 * @LastEditTime: 2023-09-11 00:26:33
 * @FilePath: /vscode/homework/3121004779_se/time_cal.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include "cppjieba/Jieba.hpp"
#include<iostream>
#include<fstream>
#include<regex>
#include <codecvt>
#include<chrono>
#include<string>

using namespace std;

wstring_convert<codecvt_utf8<wchar_t>> conv;



string getFileData(string filePath){
  ifstream file;
  file.open(filePath);


  string line = "";
  string buf;
  while(getline(file,buf)){
    wstring ws = conv.from_bytes(buf);
    wstring nws;
    //过滤每一行中的标点和空格
    for (wchar_t ch : ws){
        if (!iswpunct(ch) && !iswspace(ch)){
          if(ch!=L'，'&&ch!=L'。'&&ch!=L'？'&&ch!=L'！'&&ch!=L'“'&&ch!=L'”'&&ch!=L'：'&&ch!=L'、')
            nws.push_back(ch);
        }

    }
    string ns = conv.to_bytes(nws);
  
    line += ns;
  }
  file.close();
  return line;
}

vector<string> cutWord(string src){
  cppjieba::Jieba jieba("dict/jieba.dict.utf8",
                      "dict/hmm_model.utf8",
                      "dict/user.dict.utf8",
                      "dict/idf.utf8",
                      "dict/stop_words.utf8");
  vector<string> words;
  jieba.Cut(src, words, true);
  return words;
}

double Similarity_Cal(vector<string> word1,vector<string> word2){
    int m = word1.size();
    int n = word2.size();

    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    for (int i = 0; i <= m; ++i) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= n; ++j) {
        dp[0][j] = j;
    }

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            int insertion = dp[i][j - 1] + 1;
            int deletion = dp[i - 1][j] + 1;
            int substitution = dp[i - 1][j - 1] + (word1[i - 1] != word2[j - 1]);

            dp[i][j] = min({insertion, deletion, substitution});
        }
    }

    double similarity = 1.0 - static_cast<double>(dp[m][n]) / max(m, n);

    return similarity;
}

int main(int argc, char** argv) {
    auto begin = std::chrono::high_resolution_clock::now();
    string str1 = getFileData("test/orig.txt");
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    string str2 = getFileData("test/orig_0.8_add.txt");

    // string str1 = getFileData(argv[1]);

    // string str2 = getFileData(argv[2]);
    begin = std::chrono::high_resolution_clock::now();
    vector<string> word1 = cutWord(str1);
    end = std::chrono::high_resolution_clock::now();
    auto elapsed2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    vector<string> word2 = cutWord(str2);

    begin = std::chrono::high_resolution_clock::now();
    double similarity = Similarity_Cal(word1,word2);
    end = std::chrono::high_resolution_clock::now();
    auto elapsed3 = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

    cout<<"getFileData: "<<elapsed1.count()*1e-9<<endl;
    
    cout<<"cutWord: "<<elapsed2.count()*1e-9<<endl;

    cout<<"Similarity_Cal: "<<elapsed3.count()*1e-9<<endl;
    return 0;
}