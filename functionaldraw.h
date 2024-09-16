#pragma once
#include "atlas.h"
#include <stdio.h>
#include <graphics.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include <string>
using namespace std;

#pragma comment(lib, "MSIMG32.LIB")
inline void putimage_alpha(int x, int y, IMAGE* img) {
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

//翻转图片，dst目标图片，src源图片
void FlipImage(IMAGE* dst, IMAGE* src) {
	int w = src->getwidth();
	int h = src->getheight();
	Resize(dst, w, h);
	DWORD* src_buffer = GetImageBuffer(src);
	DWORD* dst_buffer = GetImageBuffer(dst);
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			int idx_src = y * w + x;
			int idx_dst = y * w + w - x - 1;//由于x，y从0开始计数，这里需要-1
			dst_buffer[idx_dst] = src_buffer[idx_src];
		}
	}
}
//翻转图集，dst目标图集，src源图集
void FlipAtlas(Atlas& dst, Atlas& src) {
	dst.Clear();
	for (int i = 0; i < src.GetSize(); i++) {
		IMAGE img_flpipped;
		FlipImage(&img_flpipped, src.GetImage(i));
		dst.AddImage(img_flpipped);
	}
}

//绘制图片，可覆盖透明度
//x，y为绘图位置
//image绘制的图片
//AA覆盖的透明度
void DrawAlpha( int x, int y,IMAGE* image, double AA = 1, double size = 1)
{
	// 变量初始化
	int WindowWidth = GetSystemMetrics(SM_CXFULLSCREEN), WindowHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	DWORD* dst = GetImageBuffer();			// GetImageBuffer() 函数，用于获取绘图设备的显存指针， EasyX 自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(image);
	src = GetImageBuffer(image);
	src = GetImageBuffer(image);// 获取 picture 的显存指针
	int srcWidth = image->getwidth();		    // 获取图片宽度
	int srcHeight = image->getheight();	    // 获取图片宽度
	int dstWidth = srcWidth * size;
	int dstHeight = srcHeight * size;
	int dstX = 0;							// 在 绘图区域 显存里像素的角标
	int srcX = 0;							// 在 image 显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < dstHeight; iy++){
		for (int ix = 0; ix < dstWidth; ix++){
			// 防止越界
			if (ix >= 0 && ix < dstWidth && ix/size < srcWidth && iy >= 0 && iy < dstHeight &&
				ix + x >= 0 && ix + x < WindowWidth && iy + y >= 0 && iy + y < WindowHeight){
				// 获取像素角标
				srcX = (int)(ix/size) + (int)(iy/size)*srcWidth;
				dstX = (ix + x) + (iy + y) * WindowWidth;

				int sa = ((src[srcX] & 0xff000000) >> 24) * AA;			// 0xAArrggbb; AA 是透明度
				int sr = ((src[srcX] & 0xff0000) >> 16);				// 获取 RGB 里的 R
				int sg = ((src[srcX] & 0xff00) >> 8);					// G
				int sb = src[srcX] & 0xff;								// B

				// 设置对应的绘图区域像素信息
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
						| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
						| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
				
			}
		}
	}
}

// 旋转图像（保留透明信息，自适应大小）
// pImg 原图像
// radian 旋转弧度,为正时逆时针旋转
// bkcolor 背景填充颜色
// 返回旋转后的图像
IMAGE RotateImageAlpha(IMAGE* pImg, double radian, COLORREF bkcolor = BLACK)
{
	radian = -radian;														// 由于 y 轴翻转，旋转角度需要变负
	float fSin = (float)sin(radian), fCos = (float)cos(radian);				// 存储三角函数值
	float fNSin = (float)sin(-radian), fNCos = (float)cos(-radian);
	int left = 0, top = 0, right = 0, bottom = 0;							// 旋转后图像顶点
	int w = pImg->getwidth(), h = pImg->getheight();
	DWORD* pBuf = GetImageBuffer(pImg);
	POINT points[4] = { {0, 0}, {w, 0}, {0, h}, {w, h} };					// 存储图像顶点
	for (int j = 0; j < 4; j++)												// 旋转图像顶点，搜索旋转后的图像边界
	{
		points[j] = {
			(int)(points[j].x * fCos - points[j].y * fSin),
			(int)(points[j].x * fSin + points[j].y * fCos)
		};
		if (points[j].x < points[left].x)	left = j;
		if (points[j].y > points[top].y)	top = j;
		if (points[j].x > points[right].x)	right = j;
		if (points[j].y < points[bottom].y)	bottom = j;
	}

	int nw = points[right].x - points[left].x;								// 旋转后的图像尺寸
	int nh = points[top].y - points[bottom].y;
	int nSize = nw * nh;
	int offset_x = points[left].x < 0 ? points[left].x : 0;					// 旋转后图像超出第一象限的位移（据此调整图像位置）
	int offset_y = points[bottom].y < 0 ? points[bottom].y : 0;

	IMAGE img(nw, nh);
	DWORD* pNewBuf = GetImageBuffer(&img);
	if (bkcolor != BLACK)													// 设置图像背景色
		for (int i = 0; i < nSize; i++)
			pNewBuf[i] = BGR(bkcolor);

	for (int i = offset_x, ni = 0; ni < nw; i++, ni++)						// i 用于映射原图像坐标，ni 用于定位旋转后图像坐标
	{
		for (int j = offset_y, nj = 0; nj < nh; j++, nj++)
		{
			int nx = (int)(i * fNCos - j * fNSin);							// 从旋转后的图像坐标向原图像坐标映射
			int ny = (int)(i * fNSin + j * fNCos);
			if (nx >= 0 && nx < w && ny >= 0 && ny < h)						// 若目标映射在原图像范围内，则拷贝色值
				pNewBuf[nj * nw + ni] = pBuf[ny * w + nx];
		}
	}

	return img;
}

// 绘制图片，支撑透明信息、覆盖透明度、旋转图片
// x，y 绘制位置的左上角坐标
// image 原图像
// AA 覆盖透明度，默认为1(不透明)，0为完全透明
// rad 旋转弧度,为正时逆时针旋转
// bkcolor 背景填充颜色，默认为BLACK
//图片缩放比例
void DrawImage(int x, int y, IMAGE* image, double AA = 1, double rad = 0, COLORREF bkcolor = BLACK, double size = 1) {
	IMAGE new_image;
	new_image = RotateImageAlpha(image, rad);
	DrawAlpha(x, y, &new_image, AA, size);
}
