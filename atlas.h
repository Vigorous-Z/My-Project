#pragma once

#include <vector>
#include <graphics.h>
using namespace std;

struct Atlas {
public:
	Atlas(){}
	~Atlas(){}

	void LoadFromFile(LPCTSTR path_file, int num) {
		image_list.clear();
		image_list.resize(num);

		TCHAR path[256];
		for (int i = 0; i < num; i++) {
			_stprintf_s(path, path_file,i);
			loadimage(&image_list[i], path);
		}
	}
	void Clear() {
		image_list.clear();
	}
	IMAGE* GetImage(int idx) {
		if (idx < 0 || idx > image_list.size())
			return NULL;
		else return &image_list[idx];
	}
	void AddImage(const IMAGE& img) {
		image_list.push_back(img);
	}
	int GetSize() {
		return (int)image_list.size();
	}

private:
	vector<IMAGE> image_list;
};