#pragma once
#include "atlas.h"
#include "functionaldraw.h"
#include <graphics.h>


struct Animation {
public:
	Animation() {}
	~Animation() {}

	void Reset() {
		count = 0;
		idx_frame = 0;
	}
	//���õ�ǰͼ��
	// new_atlas ͼ����
	// ms ֡����� 
	// loop �Ƿ�ѭ��
	void SetAtlas(Atlas* new_atlas, int ms = 45, bool loop = true) {
		Reset();
		interval = ms;
		atlas = new_atlas;
		is_loop = loop;
	}
	int GetIndex() {
		return idx_frame;
	}
	IMAGE* GetImage(int idx = 100) {
		if(idx > atlas->GetSize())
			return atlas->GetImage(idx_frame);
		else return atlas->GetImage(idx);
	}
	bool CheckFinished() {
		if (is_loop) return false;
		return (idx_frame == atlas->GetSize() - 1);
	}

	void OnUpdate(clock_t delta) {
		count += delta;
		if (count >= interval) {
			idx_frame += count/interval;
			count = 0;
			if (idx_frame >= atlas->GetSize()) {
				idx_frame = is_loop ? 0 : atlas->GetSize() - 1;
			}
		}
	}	
// ���Ƶ�ǰͼ����֧��͸����Ϣ������͸���ȡ���תͼƬ
// x,y ����λ�õ����Ͻ�����
// AA ����͸���ȣ�0Ϊ��ȫ͸��
// rad ��ת����,Ϊ��ʱ��ʱ����ת
// bkcolor ���������ɫ
	void OnDraw(int x, int y, double AA = 1, double rad = 0, COLORREF bkcolor = BLACK) {
		DrawImage(x, y, atlas->GetImage(idx_frame), AA, rad, bkcolor);
	}

private:
	clock_t count = 0;
	clock_t interval = 100;
	int idx_frame = 0;
	bool is_loop = true;
	Atlas* atlas = NULL;
};