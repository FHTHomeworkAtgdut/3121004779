#include<iostream>
#include<vector>
#include<fstream>
#include <algorithm>
#include <sstream>
using namespace std;


using namespace std;

struct Sentence{
    string sentence;	
    int num;	//句子中单词数	
};  


// 记录比对信息
struct sameSentence{
    string origSentence;    
	string orig_addSentence;    
    int orig_addNum;	//对比的句子中的单词数
	int sameWordNum; //公共子句的词数	    
};

vector<sameSentence> save(1000);  //保存对比好的信息
vector<Sentence> origSentence(1000);  //orig文件中的内容

vector<vector<int>> wordSimilarity;
vector<vector<double>> score;
vector<string> str1, str2;

// 句子总数
int origStcNum = 0;  
int orig_addStcNum = 0;

//算出的每句相同单词总值
int sum = 0;

void origFile2Sentence(string filePath){
    ifstream file;
    file.open(filePath.data());

    if(file.fail()){
        string error = "Unable to open " + filePath;
		throw invalid_argument(error);
    }


    string tempStc;
    Sentence temp;
    char cur;
    int stcnum = 0;

    file >> noskipws;  //cin读取空白符

    while ( !file.eof() ){

        if((( cur >='a' )&&( cur <='z' ))||(( cur >='A' )&&( cur <='Z' ))){
            tempStc += cur;

        }else if(( cur =='.' )||( cur =='?' )||( cur =='!' )){
            transform(tempStc.begin(), tempStc.end(), tempStc.begin(), ::tolower);  // 改大写为小写
            temp.num = stcnum + 1;
            temp.sentence = tempStc;
            origSentence[origStcNum] = temp;     //每一句存到一维数组中
            stcnum = 0;
            tempStc = "";
            origStcNum++;
        }else{    //匹配到空格
            if( tempStc==""){
                continue;
            }
            else if(tempStc[tempStc.size()==1?1:tempStc.size()-1] != ' '){  //匹配到空格且当前句末不为空格，添加空格
                tempStc += " ";
                stcnum += 1;    //单词个数加1
            }
        }
    }
    file.close();

}

void orig_addFile2SentenceAndCheck(string filePath)    // 读取text文本的内容并使用动态规划进行查重
{
    ifstream file; 
    file.open( filePath.data() );   //将文件流对象与文件连接起来 
    
    if(file.fail()){
        string error = "Unable to open " + filePath;
		throw invalid_argument(error);
    }

    string tempStc;
    Sentence temp;  // 读取的当前句子
    char cur;
    int stcnum = 0;
 
    file >> noskipws;
 
    while ( !file.eof() ){
        file>>cur;
 
		if((( cur >='a' )&&( cur <='z' ))||(( cur >='A' )&&( cur <='Z' ))){
			tempStc += cur;
 
		}else if(( cur =='.' )||( cur =='?' )||( cur =='!' )){   //完成一句读取，进行动态规划匹配
		
			transform(tempStc.begin(), tempStc.end(), tempStc.begin(), ::tolower);
			temp.num = stcnum+1;            
			temp.sentence = tempStc; 
			stcnum = 0;
			tempStc = "";
 
			for(int i=0;i<origStcNum-1;i++){   //orig_add读取后，每一句都与orig进行对比
				string str;
				int n = 0;
 
				for(istringstream is(temp.sentence); is>>str;) {   //保存由orig_add句子分割出的单词
					str1[n++] = str;
				}
 
				n = 0;
				for(istringstream is1(origSentence[i].sentence); is1>>str;) {   //保存由orig句子分割出的单词
					str2[n++] = str;
				}
 
				for(int i=0;i<temp.num;i++){   //orig_add当前句子个数
				
					for(int j=0;j<origSentence[i].num;j++){  //与orig文件的句子的单词对比
						if(str1[i]==str2[j])
							wordSimilarity[i][j] = 1; //单词完全一样，相似度记为1
						else
							wordSimilarity[i][j] = 0;
					}
				}
				
				for(int i=0; i<temp.num;i++){  
					score[i][0] = max(score[i-1][0]-0.5, wordSimilarity[i][0]-0.5*(i-1) );
				} 

				for(int i=0; i<origSentence[i].num;i++){  
					score[0][i] = max(score[0][i-1]-0.5, wordSimilarity[0][i]-0.5*(i-1) );
				}

				for(int i=1;i<temp.num;i++){
					for(int j=1;j<origSentence[i].num;j++){
						score[i][j] = max( score[i-1][j]-0.5, score[i][j-1]-0.5, score[i-1][j-1]+wordSimilarity[i][j] );
					}
				}

				sum = score[temp.num-1][origSentence[i].num-1];  
				

			}
			orig_addStcNum++;   //test文件的句子的个数

		}else{
			if( tempStc=="" ){
				continue;
			}
			else if(tempStc[tempStc.size()==1?1:tempStc.size()-1] != ' ') {
				tempStc += " ";
				stcnum += 1;
			}
		}
    }
    file.close();             //关闭文件输入流 
}

int main(int argc, char *argv[]){
    string origFile = argv[1], orig_addFile = argv[2];
    
	try{
        origFile2Sentence(origFile);
        orig_addFile2SentenceAndCheck(orig_addFile);
    }
	catch(const exception& e){
		cerr << e.what() << endl;
	}



    return 0;
}