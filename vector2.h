#pragma once
#include <math.h>

struct Vector2 {
public:
	float x = 0, y = 0;
	Vector2(float newx = 0, float newy = 0) {
		x = newx;
		y = newy;
	}
	~Vector2() {}

	Vector2 operator+ (const Vector2 vec) {
		return Vector2(x + vec.x, y + vec.y);
	}
	Vector2 operator- (const Vector2 vec) {
		return Vector2(x - vec.x, y - vec.y);
	}
	void operator+= (const Vector2 vec) {
		x += vec.x;
		y += vec.y;
	}
	void operator-= (const Vector2 vec) {
		x += vec.x;
		y += vec.y;
	}
	float operator* (const Vector2 vec) {
		return x * vec.x + y * vec.y;
	}
	Vector2 operator* (const float vec) {
		return Vector2(x * vec, y * vec);
	}
	void operator*=(const float vec) {
		x *= vec;
		y *= vec;
	}

	float Length() {
		return sqrt(x * x + y * y);
	}

	Vector2 Normalize() {
		float len = Length();
		if (len != 0)
			return Vector2(x / len, y / len);
		else return Vector2(0, 0);
	}
private:
};