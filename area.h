#pragma once
#include "platform.h"
#include "flybot.h"
#include "playerA.h"

extern Camera camera;
extern Player* Player_A;

struct Area {
public:
	Area(int player_pos){
		position = player_pos;
		num = rand() % area_num;
		sprintf_s(file_path,"resource/area/area_%d.txt",num);
		FILE *file = fopen(file_path, "r");
		fgets(line, sizeof(line), file);
		if (!sscanf(line, "Platform:%d", &platform_num))
			cout << "文件解析失败" << endl;
		for (int i = 0; i < platform_num; i++)
		{
			fgets(line, sizeof(line), file);
			sscanf(line, "%d %d %d %d %d %d",
				&pos.x, &pos.y, &size.x, &size.y, &type,&draw_side);
			pos.y = -pos.y;
			Platform temp(size.x, size.y, pos,position, draw_side);
			platform_list.push_back(temp);
			cout << "平台添加成功" << endl;
		}
		fgets(line, sizeof(line), file);
		sscanf(line, "Enemy:%d", &enemy_num);
		for (int i = 0; i < enemy_num; i++)
		{
			fgets(line, sizeof(line), file);
			sscanf(line, "%d %d %d %d %d",
				&enemy_pos_l.x, &enemy_pos_l.y, &enemy_pos_r.x, &enemy_pos_r.y, &num);
			enemy_pos_l.y = -enemy_pos_l.y;
			enemy_pos_r.y = -enemy_pos_r.y;
			for (int j = 0; j < num; j++)
			{
				Vector2 spawn = { (enemy_pos_l.x + rand() % (enemy_pos_r.x - enemy_pos_l.x)) * (float)64 + position*2048,(enemy_pos_l.y + rand() % (enemy_pos_r.y - enemy_pos_l.y)) * (float)64 };
				Enemy* temp = new Flybot(spawn);
				enemy_list.push_back(temp);
				cout << "敌人添加成功" << endl;
			}
		}
			
		
		fclose(file);
	}
	~Area() {
		platform_list.clear();
		enemy_list.clear();
		num_list.clear();
	}
	void OnUpdate(clock_t delta) {
		enemy_list.erase(remove_if(enemy_list.begin(), enemy_list.end(), [](Enemy*& e) {
			return e->CheckAlive();
			}),enemy_list.end());
		for (Enemy* &e : enemy_list) {
			int damage = Player_A->BulletCollide(e->GetPos(), e->GetWidth(), e->GetHeight());
			e->OnUpdate(delta, Player_A->GetPos());
			e->ShowFrame(show_frame);
			e->Hurt(damage);
			if (e->CheckHurt()) {
				FloatNum fnum(e->CheckHurt(), e->GetPos());
				num_list.push_back(fnum);
			}
		}
		num_list.erase(remove_if(num_list.begin(), num_list.end(), [](FloatNum &f) {
			return !f.CheckValid();
			}),num_list.end());
		for (FloatNum &f : num_list) {
			f.OnUpdate(delta);
		}

		for (Platform &p : platform_list) {
			p.ShowFrame(show_frame);
		}
		Player_A->PlatformTopCollide(platform_list);
		Player_A->Hurt(enemy_list);
	}
	void OnDraw() {
		for (Platform &p : platform_list) {
			p.OnDraw();
		}
		for (Enemy* &e : enemy_list) {
			e->OnDraw();
		}
		for (FloatNum &f : num_list) {
			f.OnDraw();
		}
		if (show_frame) {
			setlinecolor(WHITE);
			rectangle(position * 32 * 64 - camera.GetPos().x, -64 *16 - camera.GetPos().y, position * 32 * 64 + 32 * 64 - camera.GetPos().x, 64 * 16 - camera.GetPos().y);
		}
	}
	void ShowFrame(bool flag) {
		show_frame = flag;
	}
private:
	int area_num = 3,num = 0,position, platform_num, enemy_num,type;
	POINT pos, size;
	POINT enemy_pos_l, enemy_pos_r;
	vector<Platform> platform_list;
	vector<Enemy*> enemy_list;
	vector<FloatNum> num_list;
	char file_path[256], line[256];
	bool show_frame = false;
	bool draw_side = false;
};