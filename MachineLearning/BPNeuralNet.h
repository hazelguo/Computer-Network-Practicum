#ifndef BP_NEURAL_NET_H
#define BP_NEURAL_NET_H

#include <iostream>  
#include <cmath>
#include <vector>
#include <cstdio>

using namespace std;  
  
//#define innode 7  //输入结点数  
//#define hidenode 10//隐含结点数  
//#define outnode 1 //输出结点数  
const int innode = 10;
const int hidenode = 10;
const int outnode = 1;

//#define trainsample 6//BP训练样本数  

class BPNeuralNet {  
public:  
    int trainsample;

    void train(vector<vector<double> > p,vector<vector<double> > t);//Bp训练  
    //double p[trainsample][innode];     //输入的样本  
    //double t[trainsample][outnode];    //样本要输出的  
   // vector<vector<double> > p;
   // vector<vector<double> > t;
  
    void recognize(vector<double> &p, vector<double> &result);//Bp识别
    void SaveWeightAndThreshold();
    void GetWeightAndThreshold();
  
    BPNeuralNet();  
    virtual ~BPNeuralNet();  
  
public:  
    void init();  
    //double w[innode][hidenode];//隐含结点权值  
    //double w1[hidenode][outnode];//输出结点权值  
    //double b1[hidenode];//隐含结点阀值  
    //double b2[outnode];//输出结点阀值  
    vector<vector<double> > w;
    vector<vector<double> > w1;
    vector<double> b1;
    vector<double> b2;
  
    double rate_w; //权值学习率（输入层-隐含层)  
    double rate_w1;//权值学习率 (隐含层-输出层)  
    double rate_b1;//隐含层阀值学习率  
    double rate_b2;//输出层阀值学习率  
  
    double e;//误差计算  
    double error;//允许的最大误差
};

#endif /* BP_NEURAL_NET_H */