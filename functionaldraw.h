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

//��תͼƬ��dstĿ��ͼƬ��srcԴͼƬ
void FlipImage(IMAGE* dst, IMAGE* src) {
	int w = src->getwidth();
	int h = src->getheight();
	Resize(dst, w, h);
	DWORD* src_buffer = GetImageBuffer(src);
	DWORD* dst_buffer = GetImageBuffer(dst);
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			int idx_src = y * w + x;
			int idx_dst = y * w + w - x - 1;//����x��y��0��ʼ������������Ҫ-1
			dst_buffer[idx_dst] = src_buffer[idx_src];
		}
	}
}
//��תͼ����dstĿ��ͼ����srcԴͼ��
void FlipAtlas(Atlas& dst, Atlas& src) {
	dst.Clear();
	for (int i = 0; i < src.GetSize(); i++) {
		IMAGE img_flpipped;
		FlipImage(&img_flpipped, src.GetImage(i));
		dst.AddImage(img_flpipped);
	}
}

//����ͼƬ���ɸ���͸����
//x��yΪ��ͼλ��
//image���Ƶ�ͼƬ
//AA���ǵ�͸����
void DrawAlpha( int x, int y,IMAGE* image, double AA = 1, double size = 1)
{
	// ������ʼ��
	int WindowWidth = GetSystemMetrics(SM_CXFULLSCREEN), WindowHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	DWORD* dst = GetImageBuffer();			// GetImageBuffer() ���������ڻ�ȡ��ͼ�豸���Դ�ָ�룬 EasyX �Դ�
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(image);
	src = GetImageBuffer(image);
	src = GetImageBuffer(image);// ��ȡ picture ���Դ�ָ��
	int srcWidth = image->getwidth();		    // ��ȡͼƬ���
	int srcHeight = image->getheight();	    // ��ȡͼƬ���
	int dstWidth = srcWidth * size;
	int dstHeight = srcHeight * size;
	int dstX = 0;							// �� ��ͼ���� �Դ������صĽǱ�
	int srcX = 0;							// �� image �Դ������صĽǱ�

	// ʵ��͸����ͼ ��ʽ�� Cp=��p*FP+(1-��p)*BP �� ��Ҷ˹���������е���ɫ�ĸ��ʼ���
	for (int iy = 0; iy < dstHeight; iy++){
		for (int ix = 0; ix < dstWidth; ix++){
			// ��ֹԽ��
			if (ix >= 0 && ix < dstWidth && ix/size < srcWidth && iy >= 0 && iy < dstHeight &&
				ix + x >= 0 && ix + x < WindowWidth && iy + y >= 0 && iy + y < WindowHeight){
				// ��ȡ���ؽǱ�
				srcX = (int)(ix/size) + (int)(iy/size)*srcWidth;
				dstX = (ix + x) + (iy + y) * WindowWidth;

				int sa = ((src[srcX] & 0xff000000) >> 24) * AA;			// 0xAArrggbb; AA ��͸����
				int sr = ((src[srcX] & 0xff0000) >> 16);				// ��ȡ RGB ��� R
				int sg = ((src[srcX] & 0xff00) >> 8);					// G
				int sb = src[srcX] & 0xff;								// B

				// ���ö�Ӧ�Ļ�ͼ����������Ϣ
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //��ʽ�� Cp=��p*FP+(1-��p)*BP  �� ��p=sa/255 , FP=sr , BP=dr
						| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //��p=sa/255 , FP=sg , BP=dg
						| (sb * sa / 255 + db * (255 - sa) / 255);              //��p=sa/255 , FP=sb , BP=db
				
			}
		}
	}
}

// ��תͼ�񣨱���͸����Ϣ������Ӧ��С��
// pImg ԭͼ��
// radian ��ת����,Ϊ��ʱ��ʱ����ת
// bkcolor ���������ɫ
// ������ת���ͼ��
IMAGE RotateImageAlpha(IMAGE* pImg, double radian, COLORREF bkcolor = BLACK)
{
	radian = -radian;														// ���� y �ᷭת����ת�Ƕ���Ҫ�为
	float fSin = (float)sin(radian), fCos = (float)cos(radian);				// �洢���Ǻ���ֵ
	float fNSin = (float)sin(-radian), fNCos = (float)cos(-radian);
	int left = 0, top = 0, right = 0, bottom = 0;							// ��ת��ͼ�񶥵�
	int w = pImg->getwidth(), h = pImg->getheight();
	DWORD* pBuf = GetImageBuffer(pImg);
	POINT points[4] = { {0, 0}, {w, 0}, {0, h}, {w, h} };					// �洢ͼ�񶥵�
	for (int j = 0; j < 4; j++)												// ��תͼ�񶥵㣬������ת���ͼ��߽�
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

	int nw = points[right].x - points[left].x;								// ��ת���ͼ��ߴ�
	int nh = points[top].y - points[bottom].y;
	int nSize = nw * nh;
	int offset_x = points[left].x < 0 ? points[left].x : 0;					// ��ת��ͼ�񳬳���һ���޵�λ�ƣ��ݴ˵���ͼ��λ�ã�
	int offset_y = points[bottom].y < 0 ? points[bottom].y : 0;

	IMAGE img(nw, nh);
	DWORD* pNewBuf = GetImageBuffer(&img);
	if (bkcolor != BLACK)													// ����ͼ�񱳾�ɫ
		for (int i = 0; i < nSize; i++)
			pNewBuf[i] = BGR(bkcolor);

	for (int i = offset_x, ni = 0; ni < nw; i++, ni++)						// i ����ӳ��ԭͼ�����꣬ni ���ڶ�λ��ת��ͼ������
	{
		for (int j = offset_y, nj = 0; nj < nh; j++, nj++)
		{
			int nx = (int)(i * fNCos - j * fNSin);							// ����ת���ͼ��������ԭͼ������ӳ��
			int ny = (int)(i * fNSin + j * fNCos);
			if (nx >= 0 && nx < w && ny >= 0 && ny < h)						// ��Ŀ��ӳ����ԭͼ��Χ�ڣ��򿽱�ɫֵ
				pNewBuf[nj * nw + ni] = pBuf[ny * w + nx];
		}
	}

	return img;
}

// ����ͼƬ��֧��͸����Ϣ������͸���ȡ���תͼƬ
// x��y ����λ�õ����Ͻ�����
// image ԭͼ��
// AA ����͸���ȣ�Ĭ��Ϊ1(��͸��)��0Ϊ��ȫ͸��
// rad ��ת����,Ϊ��ʱ��ʱ����ת
// bkcolor ���������ɫ��Ĭ��ΪBLACK
//ͼƬ���ű���
void DrawImage(int x, int y, IMAGE* image, double AA = 1, double rad = 0, COLORREF bkcolor = BLACK, double size = 1) {
	IMAGE new_image;
	new_image = RotateImageAlpha(image, rad);
	DrawAlpha(x, y, &new_image, AA, size);
}
