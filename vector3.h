// 用途: 向量类,带计算功能

#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>

class vector3
{
public:
	union
	{
		struct
		{
			float x, y, z;
		};
		float v[3];
	};
	// 构造
	vector3(void)
	{
	}
	vector3(float x_, float y_, float z_)
	{
		x = x_;
		y = y_;
		z = z_;
	}
	~vector3(void)
	{
	}
	// 设定值
	void set(float x_, float y_, float z_)
	{
		x = x_;
		y = y_;
		z = z_;
	}
	void zero(void)
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
	// 求模
	float length(void)
	{
		return((float)sqrt(x * x + y * y + z * z));
	}
	// 求单位向量
	void normalize(void)
	{
		float fLength = length();
		x = x / fLength;
		y = y / fLength;
		z = z / fLength;
	}

	void exchange(void)
	{
		float tmp;
		tmp = y;
		y = z;
		z = tmp;
	}
	// 内积
	float operator * (const vector3 &other)
	{
		return(x * other.x + y * other.y + z * other.z);
	}
	// 向量积
	vector3 operator ^ (const vector3 &other)
	{
		return vector3(y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x);
	}
	// 数乘
	vector3 operator * (const float scalar)
	{
		return vector3(x * scalar, y * scalar, z * scalar);
	}
	vector3 operator / (const float scalar)
	{
		return vector3(x / scalar, y / scalar, z / scalar);
	}
	// 赋值
	vector3& operator = (const vector3 &other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	vector3& operator += (const vector3 &other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	vector3& operator -= (const vector3 &other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	vector3& operator *= (const float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	vector3& operator /= (const float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}
	// 加减
	vector3 operator + (const vector3 &other)
	{
		return vector3(x + other.x, y + other.y, z + other.z);
	}

	vector3 operator + (void) const
	{
		return *this;
	}

	vector3 operator - (const vector3 &other)
	{
		return vector3(x - other.x, y - other.y, z - other.z);
	}

	vector3 operator - (void) const
	{
		return vector3(-x, -y, -z);
	}
	// 获取三角形单位法向量
	static vector3 GetTriangleNormal(vector3 &v1, vector3 &v2, vector3 &v3)
	{
		vector3 normal = (v1 - v2) ^ (v1 - v3);
		normal.normalize();
		return normal;
	}
};

#endif // VECTOR3_H