#include "BPNeuralNet.h"

/*
void winit(double w[],int n) //权值初始化  
{  
  for(int i=0;i<n;i++)  
    w[i]=(2.0*(double)rand()/RAND_MAX)-1;  
} 
*/

void initial(vector<double> &a, int n) {
    a.clear();
    for (int i = 0; i < n; ++i) 
        a.push_back((2.0*(double)rand()/RAND_MAX)-1);
}

void initial(vector<vector<double> > &a, int n, int m) {
    a.clear();
    vector<double> b;
    for (int i = 0; i < n; ++i) {
        b.clear();
        for (int j = 0; j < m; j++)
            b.push_back((2.0*(double)rand()/RAND_MAX)-1);
        a.push_back(b);
    }
    
}

BPNeuralNet::BPNeuralNet() {  
    error=100;  
    e=0.0;
    rate_w=0.9;  //权值学习率（输入层--隐含层)  
    rate_w1=0.9; //权值学习率 (隐含层--输出层)  
    rate_b1=0.9; //隐含层阀值学习率  
    rate_b2=0.9; //输出层阀值学习率 
    initial(w, innode, hidenode);
    initial(w1, hidenode, outnode);
    initial(b1, hidenode);
    initial(b2, outnode); 
    /*
    winit((double*)w,innode*hidenode);  
    winit((double*)w1,hidenode*outnode);  
    winit(b1,hidenode);  
    winit(b2,outnode);  
    */  
}  
  
BPNeuralNet::~BPNeuralNet() {  
}  
  
void BPNeuralNet::train(vector<vector<double> > p,vector<vector<double> > t)  
{  
    double pp[hidenode];//隐含结点的校正误差  
    double qq[outnode];//希望输出值与实际输出值的偏差  
    double yd[outnode];//希望输出值  
  
    double x[innode]; //输入向量  
    double x1[hidenode];//隐含结点状态值  
    double x2[outnode];//输出结点状态值  
    double o1[hidenode];//隐含层激活值  
    double o2[hidenode];//输出层激活值  
  
    for(int isamp=0;isamp<trainsample;isamp++)//循环训练一次样品  
    {  
        for(int i=0;i<innode;i++)  
            x[i]=p[isamp][i]; //输入的样本  
        for(int i=0;i<outnode;i++)  
            yd[i]=t[isamp][i]; //期望输出的样本  
  
        //构造每个样品的输入和输出标准  
        for(int j=0;j<hidenode;j++)  
        {  
            o1[j]=0.0;  
            for(int i=0;i<innode;i++)  
                o1[j]=o1[j]+w[i][j]*x[i];//隐含层各单元输入激活值  
            x1[j]=1.0/(1+exp(-o1[j]-b1[j]));//隐含层各单元的输出  
            //    if(o1[j]+b1[j]>0) x1[j]=1;  
            //else x1[j]=0;  
        }  
  
        for(int k=0;k<outnode;k++)  
        {  
            o2[k]=0.0;  
            for(int j=0;j<hidenode;j++)  
                o2[k]=o2[k]+w1[j][k]*x1[j]; //输出层各单元输入激活值  
            x2[k]=1.0/(1.0+exp(-o2[k]-b2[k])); //输出层各单元输出  
            //    if(o2[k]+b2[k]>0) x2[k]=1;  
            //    else x2[k]=0;  
        }  
  
        for(int k=0;k<outnode;k++)  
        {  
            qq[k]=(yd[k]-x2[k])*x2[k]*(1-x2[k]); //希望输出与实际输出的偏差  
            for(int j=0;j<hidenode;j++)  
                w1[j][k]+=rate_w1*qq[k]*x1[j];  //下一次的隐含层和输出层之间的新连接权  
        }  
  
        for(int j=0;j<hidenode;j++)  
        {  
            pp[j]=0.0;  
            for(int k=0;k<outnode;k++)  
                pp[j]=pp[j]+qq[k]*w1[j][k];  
            pp[j]=pp[j]*x1[j]*(1-x1[j]); //隐含层的校正误差  
  
            for(int i=0;i<innode;i++)  
                w[i][j]+=rate_w*pp[j]*x[i]; //下一次的输入层和隐含层之间的新连接权  
        }  
  
        for(int k=0;k<outnode;k++)  
        {  
            e+=fabs(yd[k]-x2[k])*fabs(yd[k]-x2[k]); //计算均方差  
        }  
        error=e/2.0;  
  
        for(int k=0;k<outnode;k++)  
            b2[k]=b2[k]+rate_b2*qq[k]; //下一次的隐含层和输出层之间的新阈值  
        for(int j=0;j<hidenode;j++)  
            b1[j]=b1[j]+rate_b1*pp[j]; //下一次的输入层和隐含层之间的新阈值  
    }  
}  

void BPNeuralNet::recognize(vector<double> &p, vector<double> &result)  
{  
    double x[innode]; //输入向量  
    double x1[hidenode]; //隐含结点状态值  
    double x2[outnode]; //输出结点状态值  
    double o1[hidenode]; //隐含层激活值  
    double o2[hidenode]; //输出层激活值  
  
    for(int i=0;i<innode;i++)  
        x[i]=p[i];  
  
    for(int j=0;j<hidenode;j++)  
    {  
        o1[j]=0.0;  
        for(int i=0;i<innode;i++)  
            o1[j]=o1[j]+w[i][j]*x[i]; //隐含层各单元激活值  
        x1[j]=1.0/(1.0+exp(-o1[j]-b1[j])); //隐含层各单元输出  
        //if(o1[j]+b1[j]>0) x1[j]=1;  
        //    else x1[j]=0;  
    }  
  
    for(int k=0;k<outnode;k++)  
    {  
        o2[k]=0.0;  
        for(int j=0;j<hidenode;j++)  
            o2[k]=o2[k]+w1[j][k]*x1[j];//输出层各单元激活值  
        x2[k]=1.0/(1.0+exp(-o2[k]-b2[k]));//输出层各单元输出  
        //if(o2[k]+b2[k]>0) x2[k]=1;  
        //else x2[k]=0;  
    }  
  
    for(int k=0;k<outnode;k++)  
    {  
        result[k]=x2[k];  
    }
}

void BPNeuralNet::SaveWeightAndThreshold() {
    FILE *fp;
    fp = fopen("IOFiles/WeightAndThreshold", "w");
    fprintf(fp, "%d %d %d\n", innode, hidenode, outnode);
    for (int i = 0; i < innode; ++i) {
        fprintf(fp, "%lf", w[i][0]);
        for (int j = 1; j < hidenode; ++j) {
            fprintf(fp, " %lf", w[i][j]);
        }
        fprintf(fp, "\n");
    }
    for (int i = 0; i < hidenode; ++i) {
        fprintf(fp, "%lf", w1[i][0]);
        for (int j = 1; j < outnode; ++j) {
            fprintf(fp, " %lf", w1[i][j]);
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "%lf", b1[0]);
    for (int i = 1; i < hidenode; ++i){
        fprintf(fp, " %lf", b1[i]);
    }
    fprintf(fp, "\n");
    fprintf(fp, "%lf", b2[0]);
    for (int i = 1; i < outnode; ++i){
        fprintf(fp, " %lf", b2[i]);
    }
    fprintf(fp, "\n");

    fclose(fp);
}


void BPNeuralNet::GetWeightAndThreshold() {
    FILE *fp;
    fp = fopen("IOFiles/WeightAndThreshold", "r");
    int t1, t2, t3;
    fscanf(fp, "%d %d %d\n", &t1, &t2, &t3);
    for (int i = 0; i < innode; ++i) {
        for (int j = 0; j < hidenode; ++j) {
            fscanf(fp, " %lf", &w[i][j]);
        }
    }
    for (int i = 0; i < hidenode; ++i) {
        for (int j = 0; j < outnode; ++j) {
            fscanf(fp, " %lf", &w1[i][j]);
        }
    }
    for (int i = 0; i < hidenode; ++i){
        fscanf(fp, " %lf", &b1[i]);
    }
    for (int i = 0; i < outnode; ++i){
        fscanf(fp, " %lf", &b2[i]);
    }
    fclose(fp);
}