#include <iostream>
#include <cstring>
#include "utils.h"

using namespace std;

/*
将信息转化为数字
*/
void convertCharToNum(char *text, int x[]) {
    int i = 0;
    while (*text != '\0') {
        x[i] = int(*text);
        cout << *text << "->" << x[i] << '\t';
        ++i;
        ++text;
    }
}

//将密文转换成数字
void convertTextToNum(int decodedNum[], const int textLen, const char sendText[]) {
    for (int i = 0; i < textLen; ++i) {
        int result = 0;
        for (int j = 0; j < 5; ++j) {
            char tmp = sendText[5 * i + j];//取得字符
            if (tmp == '%') continue;
            result *= 52;
            if (tmp >= 'a' && tmp <= 'z') {
                result += int(tmp - 'a');
                //cout<<int(tmp-'a')<<' ';
            } else if (tmp >= 'A' && tmp <= 'Z') {
                result += int(tmp - 'A' + 26);
                //cout<<int(tmp-'A'+26)<<' ';
            }
        }
        decodedNum[i] = result;
    }
}

//将单个数字转换成字母
void convertNumToChar(int decodedNum[], int length) {
    for (int i = 0; i < length; ++i) {
        cout << char(decodedNum[i]);
    }
}

//将数字转换成密文，encodedNum是加密后的int数组，textLen是明文长度，sendText是密文
void convertNumToText(const int encodedNum[], const int textLen, char sendText[]) {
    for (int i = 0; i < textLen; ++i) {
        int tmp = encodedNum[i], j = 4;
        while (tmp) {
            int mod = tmp % 52;
            if (mod >= 0 && mod < 26) {
                //cout<<mod<<' ';
                sendText[i * 5 + j] = char('a' + mod);
                //cout<<sendText[i*5+j];
            } else if (mod >= 26 && mod <= 51) {
                //cout<<mod<<' ';
                sendText[i * 5 + j] = char('A' + mod - 26);
                //cout<<sendText[i*5+j];
            }
            tmp = tmp / 52;
            --j;//倒序输出
        }
    }
    cout << "\n加密成功，密文为:\n";
    cout << sendText << endl;
}

/*
欧几里得算法（贝祖等式），用在生成私钥d步骤
*/
int Euclid(long long int m, long long int n) {
    long long int q, r[2], s[2], t[2], stmp, ttmp, rtmp;
    if (m >= n) {
        r[0] = n;
        r[1] = m;
    } else {
        r[1] = n;
        r[0] = m;
    }
    s[0] = 0;
    s[1] = 1;
    t[0] = 1;
    t[1] = 0;
    q = r[1] / r[0];
    rtmp = r[0];
    r[0] = r[1] % r[0];
    r[1] = rtmp;
    while (r[0] != 0) {
        stmp = s[0];
        s[0] = -q * s[0] + s[1];
        s[1] = stmp;
        ttmp = t[0];
        t[0] = -q * t[0] + t[1];
        t[1] = ttmp;
        q = r[1] / r[0];
        rtmp = r[0];
        r[0] = r[1] % r[0];
        r[1] = rtmp;
        //cout<<"s:"<<s[0]<<" t:"<<t[0]<<endl;
    }
    //cout<<"s:"<<s[0]<<" t:"<<t[0]<<endl;
    return t[0];
}

//运用广义欧几里得除法检验两数是否互素
bool isCoprime(long long m, long long n) {
    long long a, b, remainder;
    if (m >= n) {
        a = m;
        b = n;
    } else {
        a = n;
        b = m;
    }
    while (b != 0) {
        remainder = a % b;
        //cout<<"quotient:"<<quotient<<endl;
        //cout<<"remainder:"<<remainder<<endl;
        a = b;
        b = remainder;
    }
    return a == 1;
}

//随机生成14-bit的奇数，然后进行素性检验，安全参数为30
long long generatePrime() {
    //srand(time(0));//随机数种子
    long long a = 8192, b = 32786; //限制位数在14-bitsrand((unsigned)time(NULL));
    long long num;
    bool flag = true;//生成素数的标志
    while (flag) {
        num = (rand() % (b - a)) + a;
        while (num % 2 == 0) {
            num = (rand() % (b - a)) + a;
        }
        cout << endl << "得到随机奇数：" << num << "，开始素性检验！" << endl;
        int i;//用于标志num是否为素数
        for (i = 0; i < 30; ++i) //安全参数30
        {
            //if((i+1)%10==0) cout<<"\n第"<<i+1<<"次检验！\n";;
            long long b = (rand() % (num - 1)) + 1;
            //if((i+1)%10==0) cout<<"检验数为 "<<b<<endl;
            if (!isCoprime(b, num)) {
                //if((i+1)%10==0) cout<<num<<"不是素数，重新生成随机数！\n";
                break;
            }
            //if((i+1)%10==0) cout<<num<<"与"<<b<<"互素，开始Fermat素性检验！\n";
            long long yu = repeatMod(b, num - 1, num);
            if (yu != 1) {
                //cout<<"同余于"<<yu<<endl;
                //cout<<num<<"不是素数，重新生成随机数！\n";
                break;
            }
            //else if((i+1)%10==0) cout<<"同余于"<<yu<<"，满足条件！"<<endl;
        }
        //cout<<i<<endl;
        if (i == 30) {
            //cout<<num<<"通过Fremat素性检验，得到素数！\n";
            flag = false;
        }
    }
    return num;
}

/*
模重复平方计算法，用于加密和解密过程求模运算，形式为m^e(mod n)
*/
int repeatMod(long long int m, int e, long long int n) {
    int a = 1;
    int b[100], i = e, j = 0;
    while (i) {
        b[j] = i % 2;
        i = i / 2;
        ++j;
    }
    for (int k = 0; k < j; ++k) {
        if (b[k] != 0) {
            a = (a * m) % n;
        }
        //cout<<a<<endl;
        m = (m * m) % n;
    }
    return a;
}

void RSA(char *text) {
    long long p, q, n, phi, e, d;
    cout << "要加密的信息为：\n" << text << endl;
    cout << "\n尝试生成素数p和q...";
    //生成素数p,q以及n和phi
    p = generatePrime();
    q = generatePrime();
    while (p == q) {
        p = generatePrime();
        q = generatePrime();
    }
    n = p * q;
    phi = (p - 1) * (q - 1);

    //d<0，则重新生成e
    do {
        e = (rand() % (phi - 1)) + 2;
        while (!isCoprime(e, phi)) {
            //生成与phi互素的e
            e = (rand() % (phi - 1)) + 2;
        }

        //调用贝祖等式生成d
        d = Euclid(e, phi);
    } while (d < 0);

    cout << "\n成功生成素数p和q！\n";
    cout << "p: " << p << "\nq: " << q << endl;
    cout << "\n成功算得公钥对(n,e)，私钥d和phi！\n";
    //输出信息
    cout << "n: " << n << endl << "phi: " << phi << endl;
    cout << "e: " << e << endl;
    cout << "d: " << d << endl << endl;

    cout << "开始将明文转换成数字...\n";
    int textLen = strlen(text);
    int *num = new int[textLen], *encodedNum = new int[textLen], *decodedNum = new int[textLen];

    //将明文转换成数字
    convertCharToNum(text, num);

    char sendText[textLen * 5 + 1];//这是要发送的加密后的文本

    //初始化密文，防止每个字母加密后位数不一样出现乱码
    for (int i = 0; i < textLen * 5; ++i) {
        sendText[i] = '%';//用%作为占位符
    }
    sendText[textLen * 5] = '\0';

    cout << "\n\n开始加密...\n";

    //利用模重复平方计算法加密
    for (int i = 0; i < textLen; ++i) {
        encodedNum[i] = repeatMod(num[i], e, n);
    }

    cout << "\n加密成功！加密过后的数字：\n";
    for (int i = 0; i < textLen; ++i) {
        cout << encodedNum[i];
    }

    cout << "\n\n开始将加密后的数字转换成密文...\n";
    convertNumToText(encodedNum, textLen, sendText);

    //解密过程
    cout << "\n\n开始将密文转换为数字...\n";
    convertTextToNum(decodedNum, textLen, sendText);

    cout << "\n转换成功，数字为:\n";
    for (int i = 0; i < textLen; ++i) {
        cout << decodedNum[i];
    }

    cout << "\n\n开始解密...\n";
    //运用模重复平方计算法算得原文
    for (int i = 0; i < textLen; ++i) {
        decodedNum[i] = repeatMod(encodedNum[i], d, n);
        //cout<<decodedNum[i]<<'\t';
    }
    cout << "\n\n解密成功！解密后的文字内容如下：\n";
    convertNumToChar(decodedNum, textLen);
    cout << endl << endl;
}
