/**
* @file VecMath.h
*/
#ifndef VECMATH_H_INCLUDED
 #define VECMATH_H_INCLUDED
 #include <cmath>


// ��s�錾
struct vec2;
struct vec3;
struct vec4;
struct mat3;
struct mat4;
// �~����
//�~�����̌v�Z���y�ɂ��邽�߂ɒ萔�̒ǉ�
constexpr float pi = 3.1415926535f;


//�p�x�̕ϊ����y�ɂ��邽�߂ɁA�ϊ�����֐����`
// �x���@���ʓx�@�ɕϊ�
constexpr float radians(float deg)
{
	return deg * pi / 180.0f;	
}

// �ʓx�@��x���@�ɕϊ�
constexpr float degrees(float rad)
{
	return rad * 180.0f / pi;
}

struct vec2 {
	
	float x, y; 

	vec2() = default;
	explicit constexpr vec2(const vec3&);
	explicit constexpr vec2(const vec4&);

	//1��float�^�̒l���󂯎��A�S�Ẵ����o�[�ɃR�s�[����
	explicit constexpr vec2(float n) : x(n), y(n) {}
	//2��float�^�̒l�����A�Ή����郁���o�[�ɃR�s�[����
	constexpr vec2(float x, float y) : x(x), y(y) {}
	////vec3,vec4�^����vec2�^�ɕϊ�����
	//constexpr vec3(const vec2& v,float z) : x(v.x), y(v.y),z(z) {}
	//constexpr vec4(const vec2& v,float z,float w) : x(v.x), y(v.y),z(z),w(w) {}


	float operator[](size_t i) const { return *(&x + i); }
	float& operator[](size_t i) { return *(&x + i); }

};
struct vec3 
{ 
	float x, y, z; 

	
	vec3() = default;
	explicit constexpr vec3(const vec4&);

	explicit constexpr vec3(float n) : x(n), y(n), z(n) {}
	constexpr vec3(float x, float y, float z) : x(x), y(y), z(z) {}
	constexpr vec3(const vec2 & v, float z) : x(v.x), y(v.y), z(z) {}

	float operator[](size_t i) const { return *(&x + i); }
	float& operator[](size_t i) { return *(&x + i); }
};

struct vec4
{
	float x, y, z, w;
	vec4() = default;
	//explicit�͖����I�ɕ\�L����K�v�̂��钆�g�ɂ��邱��
	//vec3����vec4���̈Ⴄ�^�ϊ����s����Ƃ��́A�����I�ɂ���K�v�͂��邪
	//�����z�̏ꍇ�͒�`����K�v�͂Ȃ�
	constexpr vec4(const vec4&);

	//1��float�^�̒l���󂯎��A�S�Ẵ����o�[�ɃR�s�[����
	explicit constexpr vec4(float n) : x(n), y(n),z(n),w(n) {}
	//4��float�^�̒l�����A�Ή����郁���o�[�ɃR�s�[����
	constexpr vec4(float x, float y,float z, float w) : x(x), y(y),z(z),w(w) {}
	//1��vec2�^�ƁA2��float�^�̒l�̒l�����A�Ή����郁���o�[�ɃR�s�[����
	constexpr vec4(const vec2& v,float z,float w) : x(v.x), y(v.y),z(z),w(w) {}
	//1��vec3�^�ƁA1��float�^�̒l���󂯎��A�Ή����郁���o�ɃR�s�[����
	constexpr vec4(const vec3& v,float w) : x(v.x), y(v.y),z(v.z),w(w) {}

	float operator[](size_t i) const { return *(&x + i); }
	float& operator[](size_t i) { return *(&x + i); }
};

constexpr vec2::vec2(const vec3& v) : x(v.x), y(v.y) {}
constexpr vec2::vec2(const vec4& v) : x(v.x), y(v.y) {}
constexpr vec3::vec3(const vec4& v) : x(v.x), y(v.y), z(v.z) {}
constexpr vec4::vec4(const vec4& v) : x(v.x), y(v.y), z(v.z),w(v.w) {}



					//====================================//
					//			vec2�̌v�Z                //
					// ===================================//
// vec2�̒P��-���Z�q
inline vec2 operator-(const vec2& a) { return { -a.x, -a.y }; }

// vec2���m�̉��Z
inline vec2& operator+=(vec2& a, const vec2& b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}
inline vec2 operator+(const vec2& a, const vec2& b) { vec2 n = a; return n += b; }

// vec2��float�̉��Z
inline vec2& operator+=(vec2& a, float b) { return a += { b, b }; }
inline vec2 operator+(const vec2& a, float b) { vec2 n = a; return n += b; }


// vec2���m�̌��Z
inline vec2& operator-=(vec2& a, const vec2& b)
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}
inline vec2 operator-(const vec2& a, const vec2& b) { vec2 n = a; return n -= b; }

// vec2��float�̌��Z
inline vec2& operator-=(vec2& a, float b) { return a -= { b, b}; }
inline vec2 operator-(const vec2& a, float b) { vec2 n = a; return n -= b; }
inline vec2 operator-(float a, const vec2& b) { return vec2{ a, a } - b; }

// vec2���m�̏�Z
inline vec2& operator*=(vec2& a, const vec2& b)
{
	a.x *= b.x;
	a.y *= b.y;
	return a;
}
inline vec2 operator*(const vec2& a, const vec2& b)
{
	vec2 n = a;
	return n *= b;
}
// vec2��float�̏�Z
inline vec2& operator*=(vec2& a, float b) { return a *= { b, b }; }
inline vec2 operator*(const vec2& a, float b) { vec2 n = a; return n *= b; }
inline vec2 operator*(float a, const vec2& b) { return b * a; }

// vec2���m�̏��Z
inline vec2& operator/=(vec2& a, const vec2& b)
{
	a.x /= b.x;
	a.y /= b.y;
	return a;
}
inline vec2 operator/(const vec2& a, const vec2& b) { vec2 n = a; return n /= b; }

// vec2��float�̏��Z
inline vec2& operator/=(vec2& a, float b) { return a /= { b, b }; }
inline vec2 operator/(const vec2& a, float b) { vec2 n = a; return n /= b; }


									//====================================//
									//			vec3�̌v�Z				  //
									//====================================//


// vec3�̒P��-���Z�q
inline vec3 operator-(const vec3 & a) { return { -a.x, -a.y, -a.z }; }


// vec3���m�̉��Z
inline vec3 & operator+=(vec3 & a, const vec3 & b)
{	
	a.x += b.x;	
	a.y += b.y;	
	a.z += b.z;	
	return a;	
}
 inline vec3 operator+(const vec3 & a, const vec3 & b)
 {
	 vec3 n = a; 
	 return n += b; 
 }
 
 // vec3��float�̉��Z
 inline vec3 & operator+=(vec3 & a, float b) { return a += { b, b, b }; }
 inline vec3 operator+(const vec3 & a, float b) { vec3 n = a; return n += b; }
 inline vec3 operator+(float a, const vec3 & b) { return b + a; }

 
 // vec3���m�̌��Z
 inline vec3& operator-=(vec3& a, const vec3& b)
 {
	 a.x -= b.x;
	 a.y -= b.y;
	 a.z -= b.z;
	 return a;
 }
 inline vec3 operator-(const vec3 & a, const vec3 & b) { vec3 n = a; return n -= b; }
 
 // vec3��float�̌��Z
 inline vec3 & operator-=(vec3 & a, float b) { return a -= { b, b, b }; }
 inline vec3 operator-(const vec3 & a, float b) { vec3 n = a; return n -= b; }
 inline vec3 operator-(float a, const vec3 & b) { return vec3{ a, a, a } - b; }


 // vec3���m�̏�Z
 inline vec3 & operator*=(vec3 & a, const vec3 & b)
 {
 	a.x *= b.x;
 	a.y *= b.y;
 	a.z *= b.z;
 	return a;
 }
 inline vec3 operator*(const vec3 & a, const vec3 & b) 
 { 
	 vec3 n = a; 
	 return n *= b;
 }
 // vec3��float�̏�Z
 inline vec3 & operator*=(vec3 & a, float b) { return a *= { b, b, b }; }
 inline vec3 operator*(const vec3 & a, float b) { vec3 n = a; return n *= b; }
 inline vec3 operator*(float a, const vec3 & b) { return b * a; }

 // vec3���m�̏��Z
 inline vec3& operator/=(vec3& a, const vec3& b)
 {
	 a.x /= b.x;
	 a.y /= b.y;
	 a.z /= b.z;
	 return a;
 }
 inline vec3 operator/(const vec3& a, const vec3& b) { vec3 n = a; return n /= b; }

 // vec3��float�̏��Z
 inline vec3& operator/=(vec3& a, float b) { return a /= { b, b, b }; }
 inline vec3 operator/(const vec3& a, float b) { vec3 n = a; return n /= b; }

 // vec3�̓���
 inline float dot(const vec3& a, const vec3& b)
 {
	 return a.x * b.x + a.y * b.y + a.z * b.z;
 }
 
// vec3�̊O��
 inline vec3 cross(const vec3& a, const vec3& b)
 {
	 return {
	 a.y * b.z - a.z * b.y,
	 a.z * b.x - a.x * b.z,
	 a.x * b.y - a.y * b.x };
 }

 // vec3�̒���
 inline float length(const vec3 & a) { return sqrt(dot(a, a)); }
 
// vec3�̐��K��
inline vec3 normalize(const vec3 & a) { return a / length(a); }

									//====================================//
									//			vec4�̌v�Z				  //
									//====================================//
// vec4�̒P��-���Z�q
inline vec4 operator-(const vec4 & a) { return { -a.x, -a.y, -a.z, -a.w }; }

// vec4���m�̉��Z
inline vec4& operator+=(vec4& a, const vec4& b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	a.w += b.w;
	return a;
}
inline vec4 operator+(const vec4& a, const vec4& b)
{
	vec4 n = a;
	return n += b;
}

// vec4��float�̉��Z
inline vec4 & operator+=(vec4 & a, float b) { return a += { b, b, b, b }; }
 inline vec4 operator+(const vec4 & a, float b) { vec4 n = a; return n += b; }


 // vec4���m�̌��Z
 inline vec4& operator-=(vec4& a, const vec4& b)
 {
	 a.x -= b.x;
	 a.y -= b.y;
	 a.z -= b.z;
	 a.w -= b.w;
	 return a;
 }
 inline vec4 operator-(const vec4& a, const vec4& b) { vec4 n = a; return n -= b; }

 // vec4��float�̌��Z
 inline vec4& operator-=(vec4& a, float b) { return a -= { b, b, b, b }; }
 inline vec4 operator-(const vec4& a, float b) { vec4 n = a; return n -= b; }
 inline vec4 operator-(float a, const vec4& b) { return vec4{ a, a, a, a } - b; }

 // vec4���m�̏�Z
 inline vec4& operator*=(vec4& a, const vec4& b)
 {
	 a.x *= b.x;
	 a.y *= b.y;
	 a.z *= b.z;
	 a.w *= b.w;
	 return a;
 }
 inline vec4 operator*(const vec4& a, const vec4& b)
 {
	 vec4 n = a;
	 return n *= b;
 }
 // vec4��float�̏�Z
 inline vec4& operator*=(vec4& a, float b) { return a *= { b, b, b, b }; }
 inline vec4 operator*(const vec4& a, float b) { vec4 n = a; return n *= b; }
 inline vec4 operator*(float a, const vec4& b) { return b * a; }

 // vec4���m�̏��Z
 inline vec4& operator/=(vec4& a, const vec4& b)
 {
	 a.x /= b.x;
	 a.y /= b.y;
	 a.z /= b.z;
	 a.w /= b.w;
	 return a;
 }
 inline vec4 operator/(const vec4& a, const vec4& b) { vec4 n = a; return n /= b; }

 // vec4��float�̏��Z
 inline vec4& operator/=(vec4& a, float b) { return a /= { b, b, b, b }; }
 inline vec4 operator/(const vec4& a, float b) { vec4 n = a; return n /= b; }

 
// ���W�ϊ��x�N�g���̔z����v�Z����
inline void GetTransformVectors(vec4 * result,
const vec3 & scale, const vec3 & rotation, const vec3 & position)
 {
const float sinX = sin(rotation.x);
const float cosX = cos(rotation.x);
const float sinY = sin(rotation.y);
const float cosY = cos(rotation.y);
result[0] = { scale.x * cosY,  scale.y * sinX * sinY, scale.z * cosX * sinY, position.x };
result[1] = { 0,               scale.y * cosX,        scale.z * -sinX,       position.y };
result[2] = { scale.x * -sinY, scale.y * sinX * cosY, scale.z * cosX * cosY, position.z };
result[3] = { 0,               0,              0,                     1 };
}


// ���W�ϊ��x�N�g���̔z����������킹��
inline void MultiplyTransformVectors(vec4* result, const vec4* m, const vec4* n)
{
	//����result�̕��ɁA������Ă��܂��ƌ��̃f�[�^���󂵂Ă��܂��\����
	//�o���Ă��܂��̂ŁAmn�z��Ɉ�U�i�[
	vec4 mn[4];
	for (int i = 0; i < 4; ++i) {
		mn[i] = m[0] * n[i].x + m[1] * n[i].y + m[2] * n[i].z + m[3] * n[i].w;

	}
	for (int i = 0; i < 4; ++i) {
		result[i] = mn[i];

	}
}

//scale��position�̌v�Z�͂Ȃ����߁ARotation�݂̂�
// �@���ϊ��x�N�g���̔z����v�Z����
inline void GetRotationVectors(vec3 * result, const vec3 & rotation)
{
	const float sinX = sin(rotation.x);
	const float cosX = cos(rotation.x);
	const float sinY = sin(rotation.y);
	const float cosY = cos(rotation.y);
	result[0] = { cosY,  sinX * sinY, cosX * sinY };
	result[1] = { 0,     cosX,        -sinX };
	result[2] = { -sinY, sinX * cosY, cosX * cosY };
}

// �@���ϊ��x�N�g���̔z����������킹��
inline void MultiplyRotationVectors(vec3* result, const vec3* m, const vec3* n)
{
	//�v�Z���̃f�[�^�j���̉\�����l���āA��U�N�b�V�����u��
	vec3 mn[3];
	for (int i = 0; i < 3; ++i) {
		mn[i] = m[0] * n[i].x + m[1] * n[i].y + m[2] * n[i].z;

	}
	for (int i = 0; i < 3; ++i) {
		result[i] = mn[i];

	}
}

/**
* 3x3�s��
*/
struct mat3
 {	
	vec3 data[3];	
	
	// �f�t�H���g�R���X�g���N�^
	mat3() = default;
	
	// 3��vec3����mat3���\�z����R���X�g���N�^
	mat3(const vec3 & v0, const vec3 & v1, const vec3 & v2)
	 {
	data[0] = v0;
	data[1] = v1;
	data[2] = v2;
	}
	
	  // 1��float��Ίp���ɃR�s�[����R���X�g���N�^
	explicit mat3(float f)
	 {
	data[0] = { f, 0, 0 };
	data[1] = { 0, f, 0 };
	data[2] = { 0, 0, f };
	}
	
	  // mat4����mat3�ɕϊ�����R���X�g���N�^
	explicit mat3(const struct mat4& m);
	
	// �Y�����Z�q
	vec3 & operator[](size_t i) { return data[i]; }
	const vec3 & operator[](size_t i) const { return data[i]; }

};

/**
* 4x4�s��
*/
struct mat4
{
	vec4 data[4];
	
	  // �f�t�H���g�R���X�g���N�^
	mat4() = default;
	
	  // 4��vec3����mat4���\�z����R���X�g���N�^
	mat4(const vec4 & v0, const vec4 & v1, const vec4 & v2, const vec4 & v3)
	 {
	data[0] = v0;
	data[1] = v1;
	data[2] = v2;
	data[3] = v3;
	}
	
	  // 1��float��Ίp���ɃR�s�[����R���X�g���N�^
	explicit mat4(float f)
	 {
	data[0] = { f, 0, 0, 0 };
	data[1] = { 0, f, 0, 0 };
	data[2] = { 0, 0, f, 0 };
	data[3] = { 0, 0, 0, f };
	}
	
	  // mat3����mat4�ɕϊ�����R���X�g���N�^
	explicit mat4(const mat3 & m)
	 {
	data[0] = { m.data[0], 0 };
	data[1] = { m.data[1], 0 };
	data[2] = { m.data[2], 0 };
	data[3] = { 0, 0, 0, 1 };
	}
	
	  // �Y�����Z�q
	vec4 & operator[](size_t i) { return data[i]; }
	 const vec4 & operator[](size_t i) const { return data[i]; }

};

// mat4����mat3�ɕϊ�����R���X�g���N�^
inline mat3::mat3(const mat4& m)
{
	data[0] = vec3(m.data[0]);
	data[1] = vec3(m.data[1]);
	data[2] = vec3(m.data[2]);
}

// mat3��vec3�̏�Z
inline vec3 operator*(const mat3 & m, const vec3 & v)
{	return m.data[0] * v.x 
	+ m.data[1] * v.y 
	+ m.data[2] * v.z;	
}

// mat3���m�̏�Z
inline mat3 operator*(const mat3& a, const mat3& b)
{
	mat3 m;	m.data[0] = a * b.data[0];
	m.data[1] = a * b.data[1];
	m.data[2] = a * b.data[2];
	return m;
}
 inline mat3 & operator*=(mat3 & a, const mat3 & b) { a = a * b; return a; }

// mat4��vec4�̏�Z
inline vec4 operator*(const mat4 & m, const vec4 & v)
{	return m.data[0] * v.x 
	+ m.data[1] * v.y 
	+ m.data[2] * v.z 
	+ m.data[3] * v.w;	
}

// mat4���m�̏�Z
inline mat4 operator*(const mat4& a, const mat4& b)
{
	mat4 m;
	m.data[0] = a * b.data[0];
	m.data[1] = a * b.data[1];
	m.data[2] = a * b.data[2];
	m.data[3] = a * b.data[3];
	return m;
}
 inline mat4 & operator*=(mat4 & a, const mat4 & b) { a = a * b; return a; }
 //inline mat4 & operator*=(const mat4 & a, const mat4 & b) { a = a * b; return a; }




 
// ��]�p�x�����]�s����쐬
 inline mat3 GetRotationMatrix(const vec3& rotation)
 {
	 const float sinX = sin(rotation.x);
	 const float cosX = cos(rotation.x);
	 const float sinY = sin(rotation.y);
	 const float cosY = cos(rotation.y);
	 return { vec3{ cosY,        0,     -sinY },
	 vec3{ sinX * sinY, cosX,  sinX * cosY },
	 vec3{ cosX * sinY, -sinX, cosX * cosY } };
 }
 
// �g�嗦�A��]�p�x�A���s�ړ��ʂ�����W�ϊ��s����쐬
inline mat4 GetTransformMatrix(
	const vec3& scale, const vec3& rotation, const vec3& position)
{
	
	const mat3 m = GetRotationMatrix(rotation);
	return {
	vec4{ scale.x * m.data[0], 0 },
	vec4{ scale.y * m.data[1], 0 },
	vec4{ scale.z * m.data[2], 0 },
	vec4{ position, 1 } };
}


// 4x4�s�񂩂�g�嗦�𒊏o����
inline vec3 ExtractScale(const mat4 & transform)
{
	const float sx = length(vec3(transform[0]));
	const float sy = length(vec3(transform[1]));
	const float sz = length(vec3(transform[2]));

	return { sx, sy, sz };
}

//OBB�́u���S���W�v�u�O�̎��x�N�g���v�u�R�̊g�嗦�v�ȂǂȂ�
//�����̗v�f�������Ă��āA���ϊ����@���قȂ�
/**
* ���W�ϊ��s��𕽍s�ړ��A�g�嗦�A��]�s��̊e�����ɕ�������
*
* @param[in]  transform �������̍��W�ϊ��s��
* @param[out] translate ���s�ړ��̊i�[��ƂȂ�ϐ�
* @param[out] scale     �g�嗦�̊i�[��ƂȂ�ϐ�
* @param[out] rotation  ��]�s��̊i�[��ƂȂ�ϐ�
*/
inline void Decompose(const mat4& transform,
	vec3& translate, vec3& scale, mat3& rotation)
{
	// ���s�ړ��𕪉�
	translate = vec3(transform[3]);

	// �g�嗦�𕪉�
	scale = ExtractScale(transform);

	// ��]�s��𕪉�
	rotation = mat3(transform);
	rotation[0] /= scale.x;
	rotation[1] /= scale.y;
	rotation[2] /= scale.z;
}

//���[���h���W�n�ɂ����鎋�_���W�n�̎��̌������v�Z���A�u�ˉe�v����

/**
* �r���[�s����쐬����
*
* @param eye    ���_�̍��W
* @param target �����_�̍��W
* @param up     ���_�̏�������w�����̃x�N�g��
*
* @return eye, target, up����쐬�����r���[�s��
*/
inline mat4 LookAt(const vec3& eye, const vec3& target, const vec3& up)
{
	// ���[���h���W�n�ɂ����鎋�_���W�n��XYZ���̌������v�Z
	const vec3 axisZ = normalize(eye - target);
	const vec3 axisX = normalize(cross(up, axisZ));
	const vec3 axisY = normalize(cross(axisZ, axisX));

	// ���W�����x�N�g���Ɏˉe����悤�ɍs���ݒ�
	mat4 m;
	m[0] = { axisX.x, axisY.x, axisZ.x, 0 };
	m[1] = { axisX.y, axisY.y, axisZ.y, 0 };
	m[2] = { axisX.z, axisY.z, axisZ.z, 0 };

	// eye�����_�ƂȂ�悤�ɁAeye���e���Ɏˉe���ĕ��s�ړ��ʂ��v�Z
	m[3] = { -dot(axisX, eye), -dot(axisY, eye), -dot(axisZ, eye), 1 };

	return m;
}


//���s���e�Ƃ́A���e�ʂɐ����ȕ���������𓖂Ă��l�q
/**
* ���s���e�s����쐬����
*
* @param left   �`��͈͂̍��[�܂ł̋���
* @param right  �`��͈͂̉E�[�܂ł̋���
* @param bottom �`��͈͂̉��[�܂ł̋���
* @param top    �`��͈͂̏�[�܂ł̋���
* @param zNear  �`��͈͂Ɋ܂܂��ŏ�Z���W
* @param zFar   �`��͈͂Ɋ܂܂��ő�Z���W
*
* @return ���s���e�s��
*/
inline mat4 Orthogonal(float left, float right, float bottom, float top,
	float zNear, float zFar)
	 {
	  // �`��͈͂̃T�C�Y�ƒ��S���W���v�Z
	const vec3 size = { right - left, top - bottom, zFar - zNear };
	const vec3 center = { right + left, top + bottom, zFar + zNear };
	
	  // �`��͈͓��̍��W��-1�`+1�ɕϊ������悤�ɁA�g�嗦�ƕ��s�ړ���ݒ�
	//���̂��߂��ł��A�����ł��\����͕ω��Ȃ�
	//���_���W�n���E��n�ŁA�N���b�v���W�n�͍���n�Ȃ̂�Z�������}�C�i�X
	mat4 m;
	m[0] = { 2 / size.x,  0,          0, 0 };
	m[1] = { 0, 2 / size.y,           0, 0 };
	m[2] = { 0,          0, -2 / size.z, 0 };
	//���s���e�ł͋����͊֌W�Ȃ��̂ł��̒l�́A�P
	m[3] = vec4(-center / size, 1);
	return m;
}

/**
* X����]�s����쐬����
*/
inline mat4 RotateX(float angle)
 {
	const float c = cos(angle);
	const float s = sin(angle);
	mat4 m(1);
	  // y' = y * c - z * s
	  // z' = y * s + z * c
	m[1][1] = c;
	m[2][1] = -s;
	m[1][2] = s;
	m[2][2] = c;
	return m;
}

/**
* Y����]�s����쐬����
*/
inline mat4 RotateY(float angle)
{
	const float c = cos(angle);
	const float s = sin(angle);
	mat4 m(1);
	// x' = x * c + z * s
    // z' = x * -s + z * c
	m[0][0] = c;
	m[2][0] = s;
	m[0][2] = -s;
	m[2][2] = c;
	return m;
}
/**
* Z����]�s����쐬����
*/
inline mat4 RotateZ(float angle)
{
	const float c = cos(angle);
	const float s = sin(angle);
	mat4 m(1);
	// x' = x * c + y * s
    // y' = x * -s + y * c
	m[0][0] = c;
	m[0][1] = -s;
	m[1][0] = s;
	m[1][1] = c;
	return m;
}

#endif // VECMATH_H_INCLUDED