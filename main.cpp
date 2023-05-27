#include <iostream>
#include <graphics.h>
#include "conio.h"
#include "matrix.h"

#define cmd 0

int main()
{
	// 正方体初始位置顶点坐标
	// 顺序从右上角开始 顺时针
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

	// x y z轴
	const double x[3] = {25 , 0  , 0 };
	const double y[3] = {0  , 25 , 0 };
	const double z[3] = {0  , 0  , 25};

	double transXYZ[3][2] = {};
	initXYZ_trans(x , y , z , transXYZ);


    // 显示器屏幕大小为1920x1080 , 以50%大小作为窗口
	#if cmd
	initgraph(960, 540 , EX_SHOWCONSOLE);
	#else
	initgraph(960, 540);
	#endif

	// 设置坐标轴 原点为左下角  x朝右 y朝上
	setorigin(0, 540);
	setaspectratio(1 , -1);



	int posO[2] = {430 , 230};
	update(pos , posO , transXYZ , 15);


	_getch();				// 按任意键继续
	closegraph();			// 关闭绘图窗口
	return 0;
}