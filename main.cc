

#include<iostream>
#include<fstream>
#include<regex>
#include <codecvt>
#include<string>
#include "cppjieba/Jieba.hpp"
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
      for (auto& word : words) {
        cout << word << "/";
    }
    cout<<endl<<endl;
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

double func(string filePath1,string filePath2 ){
    string str1 = getFileData(filePath1);
    string str2 = getFileData(filePath2);

    vector<string> word1 = cutWord(str1);
    vector<string> word2 = cutWord(str2);


  double similarity = Similarity_Cal(word1,word2);
    // cout<<"相似度："<<similarity;
  return similarity;
}

int main(int argc, char** argv) {
    // string str1 = getFileData("test/orig.txt");
    // string str2 = getFileData("test/orig_0.8_add.txt");
    string str1 = getFileData(argv[1]);
    string str2 = getFileData(argv[2]);
    vector<string> word1 = cutWord(str1);
    vector<string> word2 = cutWord(str2);


  double similarity = Similarity_Cal(word1,word2);
    cout<<"相似度："<<similarity;
    return 0;
}