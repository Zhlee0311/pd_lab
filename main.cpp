#include<iostream>
#include<fstream>
#include<algorithm>
#include<vector>


using namespace std;
struct Emotion {
    double t[62]{};     //62维度的数据
    double ID{};        //实际的情绪种类
    double id = -1;        //预测的情绪种类（-1表示未进行预测）
};
struct Distance {
    double distance;   //测试样本与训练样本的欧式距离
    int index;         //训练样本在训练集中的编号
};
int K;              //KNN算法中的K

const int DataTrain = 350;      //训练集的大小为350
const int DataTest = 150;       //测试集的大小150
const int Data = 500;           //样本总数为500

Emotion emo[Data];            //所有的样本


bool Input() {
    char fname1[256];
    char fname2[256];
    cout << "请输入存放训练集数据的文件:\n";
    cin >> fname1;
    cout << "请输入存放测试集数据的文件:\n";
    cin >> fname2;
    cout << "请输入K的大小:\n";
    cin >> K;
    ifstream inputFile1(fname1, ios::binary);
    if (!inputFile1.is_open()) {
        cerr << "不能打开文件！\n";
        return false;
    } else {
        for (int i = 0; i < DataTrain; i++) {
            for (double &j: emo[i].t) {
                inputFile1 >> j;
            }
            inputFile1 >> emo[i].ID;
        }
        inputFile1.close();
    }

    ifstream inputFile2(fname2, ios::binary);
    if (!inputFile2.is_open()) {
        cerr << "不能打开文件！\n";
        return false;
    } else {
        for (int i = DataTrain; i < Data; i++) {
            for (double &j: emo[i].t) {
                inputFile2 >> j;
            }
            inputFile2 >> emo[i].ID;
        }
        inputFile2.close();
    }
    cout << "读取文件、初始化成功！" << endl;
    return true;
}

bool cmp(Distance a, Distance b) {
    return a.distance < b.distance;
}

void Classification() {
    for (int i = DataTrain; i < Data; i++) {
        vector<Distance> DIS;//存储测试样本与各个训练样本的距离
        for (int j = 0; j <= i - 1; j++) {
            double dis = 0;
            for (int k = 0; k < 62; k++) {
                dis += sqrt(pow(emo[i].t[k] - emo[j].t[k], 2));
            }
            Distance demo = {dis, j};
            DIS.push_back(demo);
        }
        sort(DIS.begin(), DIS.end(), cmp);
        int cnt[3] = {0};//记录最近的K个邻居中的情绪种类数目
        for (int k = 0; k < K; k++) {
            if (DIS[k].index >= DataTrain) {
                cnt[(int) emo[DIS[k].index].id]++;
            } else {
                cnt[(int) emo[DIS[k].index].ID]++;
            }
        }
        int max = cnt[0];
        int kind = 0;    //暂定预测的情绪种类为0;
        for (int p = 1; p < 3; p++) {
            if (cnt[p] > max) {
                max = cnt[p];
                kind = p;
            }
        }
        emo[i].id = (double) kind;
        printf("测试样本%d：", (i - 349));
        printf("%d个邻居中三种情绪（0.0，1.0，2.0）的个数分别为%d %d %d 所以预测种类=%.1lf,而实际种类=%.1lf\n",
               K, cnt[0], cnt[1], cnt[2],
               emo[i].id, emo[i].ID);
        printf("\n");
    }
}//分类

void Test() {
    for (int i = DataTrain; i < Data; i++) {
        if (emo[i].id == -1) {
            cout << "暂未完成分类,请先分类！" << endl;
            return;
        }
    }
    int count = 0;
    for (int i = DataTrain; i < Data; i++) {
        if (emo[i].id == emo[i].ID) {
            count++;
        }
    }
    double result = (double) count / DataTest;
    printf("K=%d时预测准确度为:%lf\n", K, result);
    printf("\n");
}//分类准确度计算

void ResetK() {
    int exit = 1;
    while (exit) {
        cout << "请输入你想要重新设定的K值:" << endl;
        if (cin >> K && K > 0) {
            cout << "设定成功！" << endl;
            exit = 0;
        } else {
            cout << "输入不合法，请重新输入！" << endl;
        }
    }
    Classification();
    Test();
}//重新设定K


void search1(double x) {
    for (auto &i: emo) {
        for (double j: i.t) {
            if (fabs(x - j) < 1e-15) {
                printf("%.1lf ", i.ID);
                if (i.ID == 2.0) printf("positive\n");
                else if (i.ID == 1.0) printf("neutral\n");
                else printf("negative\n");
            }
        }
    }
    printf("\n");
}


void search2(double x) {
    cout << "以下是这种情绪的所有样本:" << endl << endl;
    for (auto &i: emo) {
        if (fabs(x - i.ID) < 1e-15) {
            for (double j: i.t) {
                printf("%lf ", j);
            }
            printf("\n");
        }
    }
    printf("\n");
}


int main() {
    int Exit = 1;
    cout << "欢迎使用情绪检测分类系统！" << endl;
    while (!Input()) {
        cout << "读取文件错误，请检查文件名并重新输入！" << endl;
    }
    int option;
    while (Exit) {
        printf("1.情绪分类\n2.显示分类准确度\n3.重新设定K\n4.根据样本某一维度的元素推测其情绪种类\n5.列出某一种情绪种类的所有样本\n6.退出\n");
        cin >> option;
        switch (option) {
            case 1:
                Classification();
                break;
            case 2:
                Test();
                break;
            case 3:
                ResetK();
                break;
            case 4:
                cout << "请输入某一维度的元素:" << endl;
                double x;
                cin >> x;
                search1(x);
                break;
            case 5:
                cout << "请输入一种情绪:" << endl;
                double y;
                cin >> y;
                search2(y);
                break;
            case 6:
                Exit = 0;
                break;
            default:
                cout<<"输入错误，请重新输入！"<<endl;
                break;
        }
    }
    return 0;
}