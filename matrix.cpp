#include "matrix.h"
#include <cmath>
#include <graphics.h>
#include "conio.h"
#include <windows.h>
#include <iostream>

#define output 0

const double sqrt2 = sqrt(2.0);
const double sqrt6 = sqrt(6.0);
const double projection[2][3] = { { (-1)*sqrt2/2 , sqrt2/2      , 0      },
                                  { (-1)*sqrt6/6 , (-1)*sqrt6/6 , sqrt6/3}};


// c语言三角函数用的弧度
#define pi 3.1415926535898
#define DEC (pi/180)

// 绕z轴旋转变换
const double trans_span_z[3][3] = { {cos(DEC * 0.5) ,-sin(DEC * 0.5) , 0} ,
                                    {sin(DEC * 0.5) , cos(DEC * 0.5) , 0} ,
                                    {0            , 0            , 1}};



// 2x3矩阵运算 主要用于投影变换
void compute_matrix_2(const double trans_matrix[2][3] , const double vector[3] , double res[2])
{
    res[0] = trans_matrix[0][0] * vector[0] + \
                trans_matrix[0][1] * vector[1] + \
                trans_matrix[0][2] * vector[2] ;

    res[1] = trans_matrix[1][0] * vector[0] + \
                trans_matrix[1][1] * vector[1] + \
                trans_matrix[1][2] * vector[2] ;
}

// 三阶矩阵运算 主要用于旋转变换
void compute_matrix_3(const double trans_matrix[3][3] , const double vector[3] , double res[3])
{
    res[0] = trans_matrix[0][0] * vector[0] + \
                trans_matrix[0][1] * vector[1] + \
                trans_matrix[0][2] * vector[2] ;

    res[1] = trans_matrix[1][0] * vector[0] + \
                trans_matrix[1][1] * vector[1] + \
                trans_matrix[1][2] * vector[2] ;

    res[2] = trans_matrix[2][0] * vector[0] + \
                trans_matrix[2][1] * vector[1] + \
                trans_matrix[2][2] * vector[2] ;
}

#define POSX(x) (posO[0] + x)
#define POSY(x) (posO[1] + x)

// posO 是原点坐标，用于坐标计算
// 传参     相对原点坐标             八个顶点坐标           xyz轴坐标                 缩放
void draw(const int posO[2] , const double pos[8][2] , const double XYZ[3][2], const int scale = 1)
{
    // 绘制坐标系
    // x轴
    for (int i = 0; i < 3; i++)
    {
        line(POSX(0) , POSY(0) , POSX(scale * XYZ[i][0]) , POSY(scale * XYZ[i][1]));
    }



    // 绘制八个点
    // 上层四个点
    for (int i = 0; i < 3; i++)
    {
        line(POSX(scale * pos[i][0]) , POSY(scale * pos[i][1]) , POSX(scale * pos[i+1][0]) , POSY(scale * pos[i+1][1]));
    }
        line(POSX(scale * pos[3][0]) , POSY(scale * pos[3][1]) , POSX(scale * pos[0][0]) , POSY(scale * pos[0][1]));
    // 下层4点
    for (int i = 4; i < 7; i++)
    {
        line(POSX(scale * pos[i][0]) , POSY(scale * pos[i][1]) , POSX(scale * pos[i+1][0]) , POSY(scale * pos[i+1][1]));
    }
        line(POSX(scale * pos[7][0]) , POSY(scale * pos[7][1]) , POSX(scale * pos[4][0]) , POSY(scale * pos[4][1]));
    // 上下连接线
    for (int i = 0; i < 4; i++)
    {
        line(POSX(scale * pos[i][0]) , POSY(scale * pos[i][1]) , POSX(scale * pos[i+4][0]) , POSY(scale * pos[i+4][1]));
    }
    
}

void initXYZ_trans(const double x[3] , const double y[3] , const double z[3] , double XYZ[3][2])
{
    // 投影x y z 三轴
    compute_matrix_2(projection , x , XYZ[0]);
    compute_matrix_2(projection , y , XYZ[1]);
    compute_matrix_2(projection , z , XYZ[2]);
    
}


#define output2 0
void fix( double _pos[8][3])
{
    static int sign = 0;

    double pos[8][3] = {// 上层
						{0 , 10 , 10} ,
						{10, 10 , 10} ,
						{10, 0  , 10} ,
						{0 , 0  , 10} ,
						
						// 下层
						{0 , 10 , 0 } ,
						{10, 10 , 0 } ,
						{10, 0  , 0 } ,
						{0 , 0  , 0 }};

    // 3号点位到达y正半轴
    double pos2[8][3] =  {// 上层
						{-10 , 0 , 10} ,
						{-10, 10 , 10} ,
						{0, 10  , 10} ,
						{0 , 0  , 10} ,
						
						// 下层
						{-10 , 0 , 0 } ,
						{-10, 10 , 0 } ,
						{0, 10  , 0 } ,
						{0 , 0  , 0 }};
    
    // 1号点位到达y负半轴
    double pos3[8][3] =  {// 上层
						{0 , -10 , 10} ,
						{-10, -10 , 10} ,
						{-10, 0  , 10} ,
						{0 , 0  , 10} ,
						
						// 下层
						{0 , -10 , 0 } ,
						{-10, -10 , 0 } ,
						{-10, 0  , 0 } ,
						{0 , 0  , 0 }};

    // 3号点到达y负半轴
    double pos4[8][3] =  {// 上层
						{10 , 0 , 10} ,
						{10, -10 , 10} ,
						{0, -10  , 10} ,
						{0 , 0  , 10} ,
						
						// 下层
						{10 , 0 , 0 } ,
						{10, -10 , 0 } ,
						{0, -10  , 0 } ,
						{0 , 0  , 0 }};
    
    // 误差校准
        // 使用5号点位校准误差
        // 初始点  5在y正半轴
        if ( fabs(_pos[4][0]) < 0.1 && fabs(_pos[4][1] - 10) < 1 && sign == 0 && _pos[4][1] > 5)
        {
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    _pos[i][j] = pos[i][j];
                }
            }
            #if output2
            std::cout << "get it 1!\n";
            #endif
            sign = 3;
        }
        // 3 +y
        else if (fabs(_pos[2][0]) < 0.1 && fabs(_pos[2][1] - 10) < 1 && sign == 0 && _pos[2][1] > 5)
        {
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    _pos[i][j] = pos2[i][j];
                }
            }
            #if output2
            std::cout << "get it 2!\n";
            #endif
            
            sign = 3;   
        }
        // 5 -y
        else if (fabs(_pos[4][0]) < 0.1 && fabs(_pos[4][1] + 10) < 1 && sign == 0 && _pos[4][1] < -5)
        {
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    _pos[i][j] = pos3[i][j];
                }
            }
            #if output2
            std::cout << "get it 3!\n";
            #endif
            sign = 3;
        }
        // 3 -y
        else if (fabs(_pos[2][0]) < 0.1 && fabs(_pos[2][1] + 10) < 1 && sign == 0 && _pos[2][1] < -5)
        {
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    _pos[i][j] = pos4[i][j];
                }
            }
            #if output2
            std::cout << "get it 4!\n";
            #endif
            sign = 3;
        }
        else if (sign > 0)
        {
            sign--;
        }
}

void update(const double pos[8][3] , const int posO[2] , const double XYZ[3][2] , const int scale)
{
    // 备份
    double _pos[8][3] = {};
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            _pos[i][j] = pos[i][j];
        }
    }

    

    
    while (1)
    {    
        BeginBatchDraw();
        // 旋转变换
        for (int i = 0; i < 8; i++)
        {
            compute_matrix_3(trans_span_z , _pos[i] , _pos[i]);
        }
        

        // 投影变换
        double projected[8][2] = {};
        for (int i = 0; i < 8; i++)
        {
            compute_matrix_2(projection , _pos[i] , projected[i]);
        }
        


        // 绘制
        cleardevice();
        draw(posO , projected , XYZ , scale);
        FlushBatchDraw();
        
        fix(_pos);
        

#if output

        for (int i = 0; i < 8; i++)
        {
            if (i != 4) continue;
            std::cout << "点位：" << i << '\n';
            for (int j = 0; j < 3; j++)
            {
                std::cout << _pos[i][j] << '\t';
            }
            std::cout << "\n===";
        }
#endif
        Sleep(5);
    }

}