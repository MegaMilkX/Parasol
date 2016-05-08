#ifndef _MATH3F_H_
#define _MATH3F_H_

#include <math.h>
#include <assert.h>

#define PI 3.14159265359f

////////////////////////////////////////////
//Header only math lib
////////////////////////////////////////////

//
inline float qrsqrt(const float &n)
{
    long i;
    float x2, y;
    const float threehalves = 1.5f;

    x2 = n * 0.5f;
    y = n;
    i = *(long*)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float*)&i;

    y = y * (threehalves - (x2 * y * y));

    return y;
}

inline float qsqrt(const float &n)
{
    return n * qrsqrt(n);
}

///////////////////////////////////////////////
//Vectors
///////////////////////////////////////////////

//types
template<typename T>
struct vec4
{
    vec4() : x(0), y(0), z(0), w(0) {}
    vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w){}
    T length() const { return qsqrt(x*x + y*y + z*z + w*w); }
    union { T x, r; };
    union { T y, g; };
    union { T z, b; };
    union { T w, a; };
    T operator[](const int &i) const {
        return *((&x) + i);
    }
    T& operator[](const int &i){
        return *((&x) + i);
    }
};

template<typename T>
struct vec3
{
    vec3() : x(0), y(0), z(0){}
    vec3(T x, T y, T z) : x(x), y(y), z(z){}
    vec3(const vec4<T>& v) : x(v.x), y(v.y), z(v.z){}
    T length() const { return qsqrt(x*x + y*y + z*z); }
    union { T x, r; };
    union { T y, g; };
    union { T z, b; };
    T operator[](const int &i) const {
        return *((&x) + i);
    }
    T& operator[](const int &i){
        return *((&x) + i);
    }
};

template<typename T>
struct vec2
{
    vec2() : x(0), y(0){}
    vec2(T x, T y) : x(x), y(y){}
    vec2(const vec2<T>& v) : x(v.x), y(v.y){}
    union { T x, r; };
    union { T y, g; };
    T operator[](const int &i) const {
        return *((&x) + i);
    }
    T& operator[](const int &i){
        return *((&x) + i);
    }
};

typedef vec4<float> vec4f;
typedef vec3<float> vec3f;
typedef vec2<float> vec2f;
typedef vec4<int> vec4i;
typedef vec3<int> vec3i;
typedef vec2<int> vec2i;
typedef vec2<unsigned int> vec2ui;
typedef vec3<unsigned int> vec3ui;
typedef vec4<unsigned int> vec4ui;
typedef vec4<double> vec4d;
typedef vec3<double> vec3d;
typedef vec2<double> vec2d;

//Operators
//addition and substraction
inline vec3f operator+(const vec3f &a, const vec3f &b){
    return vec3f(a.x + b.x, a.y + b.y, a.z + b.z);
}
inline vec4f operator+(const vec4f &a, const vec4f &b){
    return vec4f(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

inline vec3f operator+=(vec3f &a, const vec3f &b){
    return a = a + b;
}
inline vec4f operator+=(vec4f &a, const vec4f &b){
    return a = a + b;
}

inline vec3f operator-(const vec3f &a, const vec3f &b){
    return vec3f(a.x - b.x, a.y - b.y, a.z - b.z);
}
inline vec4f operator-(const vec4f &a, const vec4f &b){
    return vec4f(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

inline vec3f operator-=(vec3f &a, const vec3f &b){
    return a = a - b;
}
inline vec4f operator-=(vec4f &a, const vec4f &b){
    return a = a - b;
}

inline vec3f operator-(const vec3f &a){
    return vec3f(-a.x, -a.y, -a.z);
}
inline vec4f operator-(const vec4f &a){
    return vec4f(-a.x, -a.y, -a.z, -a.w);
}

//multiplication and division
inline vec3f operator*(const vec3f &a, const float &f){
    return vec3f(a.x * f, a.y * f, a.z * f);
}
inline vec3f operator*(const float &f, const vec3f &a){
    return vec3f(a.x * f, a.y * f, a.z * f);
}
inline vec4f operator*(const vec4f &a, const float &f){
    return vec4f(a.x * f, a.y * f, a.z * f, a.w * f);
}
inline vec4f operator*(const float &f, const vec4f &a){
    return vec4f(a.x * f, a.y * f, a.z * f, a.w * f);
}

inline vec3f operator*=(vec3f &a, const float &f){
    return a = a*f;
}
inline vec4f operator*=(vec4f &a, const float &f){
    return a = a*f;
}

inline vec3f operator/(const vec3f &a, const float &f){
    return vec3f(a.x / f, a.y / f, a.z / f);
}
inline vec4f operator/(const vec4f &a, const float &f){
    return vec4f(a.x / f, a.y / f, a.z / f, a.w / f);
}

inline vec3f operator/=(vec3f &a, const float &f){
    return a / f;
}

//
inline float dot(const vec3f &a, const vec3f &b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline vec3f cross(const vec3f &a, const vec3f &b){
    return vec3f(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

inline vec3f normalize(const vec3f &a){
    return a / a.length();
}

inline vec4f normalize(const vec4f &a){
    return a / a.length();
}


///////////////////////////////////////////////
//Matrices
//Column-major, for easier use with opengl
///////////////////////////////////////////////

struct mat3f
{
    mat3f(){}
    explicit mat3f(const float &f)
    {
        v[0][0] = f;
        v[1][1] = f;
        v[2][2] = f;
    }
    mat3f(float xx, float xy, float xz,
        float yx, float yy, float yz,
        float zx, float zy, float zz)
    {
        v[0].x = xx; v[0].y = xy; v[0].z = xz;
        v[1].x = yx; v[1].y = yy; v[1].z = yz;
        v[2].x = zx; v[2].y = zy; v[2].z = zz;
    }
    vec3f operator[](const int &i) const {
        return v[i];
    }
    vec3f& operator[](const int &i){
        return v[i];
    }
private:
    vec3f v[3];
};

struct mat4f
{
    mat4f(){}
    explicit mat4f(const float &f)
    {
        v[0][0] = f;
        v[1][1] = f;
        v[2][2] = f;
        v[3][3] = f;
    }

    void operator=(const mat3f &m)
    {
        v[0][0] = m[0][0]; v[0][1] = m[0][1]; v[0][2] = m[0][2];
        v[1][0] = m[1][0]; v[1][1] = m[1][1]; v[1][2] = m[1][2];
        v[2][0] = m[2][0]; v[2][1] = m[2][1]; v[2][2] = m[2][2];
    }
    vec4f operator[](const int &i) const {
        return v[i];
    }
    vec4f& operator[](const int &i){
        return v[i];
    }
private:
    vec4f v[4];
};

inline mat4f operator+(const mat4f &m0, const mat4f &m1){
    mat4f m;
    for (int i = 0; i < 4; ++i)
        m[i] = m0[i] + m1[i];
    return m;
}

inline mat3f operator*(const mat3f &m0, const mat3f &m1){
    mat3f m;
    for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    for (int k = 0; k < 3; ++k)
        m[i][j] += m0[k][j] * m1[i][k];
    return m;
}

inline mat4f operator*(const mat4f &m0, const mat4f &m1){
    mat4f m;
    for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
    for (int k = 0; k < 4; ++k)
        m[i][j] += m0[k][j] * m1[i][k];
    return m;
}

//Rewrite these to be more cache-friendly
inline vec4f operator*(const mat4f &m, const vec4f &v)
{
    vec4f r;
    for (int i = 0; i < 4; ++i)
    for (int k = 0; k < 4; ++k)
        r[i] += m[k][i] * v[k];
    return r;
}

//Taking vec3 as vec4 and assuming v.w is zero
//so it transforms as a direction vector
inline vec3f operator*(const mat4f &m, const vec3f &v)
{
    vec3f r;
    for (int i = 0; i < 3; ++i)
    for (int k = 0; k < 3; ++k)
        r[i] += m[k][i] * v[k];
    return r;
}

inline vec3f operator*(const mat3f &m, const vec3f &v)
{
    vec3f r;
    for (int i = 0; i < 3; ++i)
    for (int k = 0; k < 3; ++k)
        r[i] += m[k][i] * v[k];
    return r;
}

//
inline mat3f transpose(const mat3f &m)
{
    //TODO
    return mat3f();
}

inline mat4f transpose(const mat4f &m)
{
    //TODO
    return mat4f();
}

inline mat4f scale(const mat4f &m, const vec3f &v)
{
    mat4f r = m;
    r[0] *= v[0];
    r[1] *= v[1];
    r[2] *= v[2];
    return r;
}

inline mat4f translate(const mat4f &m, const vec3f &v)
{
    mat4f r = m;
    r[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
    return r;
}

inline mat4f inverse(const mat4f &m)
{
    float det;
    mat4f inv;

    inv[0][0] = m[1][1] * m[2][2] * m[3][3] -
        m[1][1] * m[2][3] * m[3][2] -
        m[2][1] * m[1][2] * m[3][3] +
        m[2][1] * m[1][3] * m[3][2] +
        m[3][1] * m[1][2] * m[2][3] -
        m[3][1] * m[1][3] * m[2][2];

    inv[1][0] = -m[1][0] * m[2][2] * m[3][3] +
        m[1][0] * m[2][3] * m[3][2] +
        m[2][0] * m[1][2] * m[3][3] -
        m[2][0] * m[1][3] * m[3][2] -
        m[3][0] * m[1][2] * m[2][3] +
        m[3][0] * m[1][3] * m[2][2];

    inv[2][0] = m[1][0] * m[2][1] * m[3][3] -
        m[1][0] * m[2][3] * m[3][1] -
        m[2][0] * m[1][1] * m[3][3] +
        m[2][0] * m[1][3] * m[3][1] +
        m[3][0] * m[1][1] * m[2][3] -
        m[3][0] * m[1][3] * m[2][1];

    inv[3][0] = -m[1][0] * m[2][1] * m[3][2] +
        m[1][0] * m[2][2] * m[3][1] +
        m[2][0] * m[1][1] * m[3][2] -
        m[2][0] * m[1][2] * m[3][1] -
        m[3][0] * m[1][1] * m[2][2] +
        m[3][0] * m[1][2] * m[2][1];

    inv[0][1] = -m[0][1] * m[2][2] * m[3][3] +
        m[0][1] * m[2][3] * m[3][2] +
        m[2][1] * m[0][2] * m[3][3] -
        m[2][1] * m[0][3] * m[3][2] -
        m[3][1] * m[0][2] * m[2][3] +
        m[3][1] * m[0][3] * m[2][2];

    inv[1][1] = m[0][0] * m[2][2] * m[3][3] -
        m[0][0] * m[2][3] * m[3][2] -
        m[2][0] * m[0][2] * m[3][3] +
        m[2][0] * m[0][3] * m[3][2] +
        m[3][0] * m[0][2] * m[2][3] -
        m[3][0] * m[0][3] * m[2][2];

    inv[2][1] = -m[0][0] * m[2][1] * m[3][3] +
        m[0][0] * m[2][3] * m[3][1] +
        m[2][0] * m[0][1] * m[3][3] -
        m[2][0] * m[0][3] * m[3][1] -
        m[3][0] * m[0][1] * m[2][3] +
        m[3][0] * m[0][3] * m[2][1];

    inv[3][1] = m[0][0] * m[2][1] * m[3][2] -
        m[0][0] * m[2][2] * m[3][1] -
        m[2][0] * m[0][1] * m[3][2] +
        m[2][0] * m[0][2] * m[3][1] +
        m[3][0] * m[0][1] * m[2][2] -
        m[3][0] * m[0][2] * m[2][1];

    inv[0][2] = m[0][1] * m[1][2] * m[3][3] -
        m[0][1] * m[1][3] * m[3][2] -
        m[1][1] * m[0][2] * m[3][3] +
        m[1][1] * m[0][3] * m[3][2] +
        m[3][1] * m[0][2] * m[1][3] -
        m[3][1] * m[0][3] * m[1][2];

    inv[1][2] = -m[0][0] * m[1][2] * m[3][3] +
        m[0][0] * m[1][3] * m[3][2] +
        m[1][0] * m[0][2] * m[3][3] -
        m[1][0] * m[0][3] * m[3][2] -
        m[3][0] * m[0][2] * m[1][3] +
        m[3][0] * m[0][3] * m[1][2];

    inv[2][2] = m[0][0] * m[1][1] * m[3][3] -
        m[0][0] * m[1][3] * m[3][1] -
        m[1][0] * m[0][1] * m[3][3] +
        m[1][0] * m[0][3] * m[3][1] +
        m[3][0] * m[0][1] * m[1][3] -
        m[3][0] * m[0][3] * m[1][1];

    inv[3][2] = -m[0][0] * m[1][1] * m[3][2] +
        m[0][0] * m[1][2] * m[3][1] +
        m[1][0] * m[0][1] * m[3][2] -
        m[1][0] * m[0][2] * m[3][1] -
        m[3][0] * m[0][1] * m[1][2] +
        m[3][0] * m[0][2] * m[1][1];

    inv[0][3] = -m[0][1] * m[1][2] * m[2][3] +
        m[0][1] * m[1][3] * m[2][2] +
        m[1][1] * m[0][2] * m[2][3] -
        m[1][1] * m[0][3] * m[2][2] -
        m[2][1] * m[0][2] * m[1][3] +
        m[2][1] * m[0][3] * m[1][2];

    inv[1][3] = m[0][0] * m[1][2] * m[2][3] -
        m[0][0] * m[1][3] * m[2][2] -
        m[1][0] * m[0][2] * m[2][3] +
        m[1][0] * m[0][3] * m[2][2] +
        m[2][0] * m[0][2] * m[1][3] -
        m[2][0] * m[0][3] * m[1][2];

    inv[2][3] = -m[0][0] * m[1][1] * m[2][3] +
        m[0][0] * m[1][3] * m[2][1] +
        m[1][0] * m[0][1] * m[2][3] -
        m[1][0] * m[0][3] * m[2][1] -
        m[2][0] * m[0][1] * m[1][3] +
        m[2][0] * m[0][3] * m[1][1];

    inv[3][3] = m[0][0] * m[1][1] * m[2][2] -
        m[0][0] * m[1][2] * m[2][1] -
        m[1][0] * m[0][1] * m[2][2] +
        m[1][0] * m[0][2] * m[2][1] +
        m[2][0] * m[0][1] * m[1][2] -
        m[2][0] * m[0][2] * m[1][1];

    det = m[0][0] * inv[0][0] + m[0][1] * inv[1][0] + m[0][2] * inv[2][0] + m[0][3] * inv[3][0];

    assert(det != 0);

    det = 1.0f / det;

    for (int j = 0; j < 4; ++j)
    for (int i = 0; i < 4; ++i)
        inv[i][j] *= det;

    return inv;
}

inline mat4f perspective(float fov, float aspect, float znear, float zfar)
{
    assert(aspect != 0.0f);
    assert(zfar != znear);

    float tanHalfFovy = tan(fov / 2.0f);

    mat4f r(0);
    r[0][0] = 1.0f / (aspect * tanHalfFovy);
    r[1][1] = 1.0f / (tanHalfFovy);
    r[2][2] = -(zfar + znear) / (zfar - znear);
    r[2][3] = -1.0f;
    r[3][2] = -(2.0f * zfar * znear) / (zfar - znear);
    return r;
}

inline mat4f ortho(float left, float right, float bottom, float top, float znear, float zfar)
{
    mat4f r(1.0f);
    r[0][0] = 2.0f / (right - left);
    r[1][1] = 2.0f / (top - bottom);
    r[2][2] = -2.0f / (zfar - znear);
    r[3][0] = -(right + left) / (right - left);
    r[3][1] = -(top + bottom) / (top - bottom);
    r[3][2] = -(zfar + znear) / (zfar - znear);
    return r;
}

///////////////////////////////////////////////
//Quaternion
///////////////////////////////////////////////

typedef vec4f quat;

inline quat operator*(const quat &q0, const quat &q1)
{
    return quat((q0.w * q1.x + q1.w * q0.x) + (q0.y * q1.z - q1.y * q0.z),
        (q0.w * q1.y + q1.w * q0.y) + (q1.x * q0.z - q0.x * q1.z), //Inverted, since y axis rotation is inverted
        (q0.w * q1.z + q1.w * q0.z) + (q0.x * q1.y - q1.x * q0.y),
        (q1.w * q0.w) - (q1.x * q0.x) - (q1.y * q0.y) - (q1.z * q0.z));
}

inline vec3f rotate(const quat &q, const vec3f &v)
{
    //TODO
}

inline quat angleAxis(const float &a, const vec3f &axis)
{
    float s = sin(a * 0.5f);
    return quat(axis.x * s, axis.y * s, axis.z * s, cos(a*0.5f));
}

///////////////////////////////////////////////
//Conversion
///////////////////////////////////////////////
inline mat3f toMat3(const quat &q)
{
    mat3f m = { 1 - 2 * q.y * q.y - 2 * q.z * q.z, q.z * 2 * q.w + 2 * q.x * q.y, -q.y * 2 * q.w + 2 * q.x * q.z,

        -q.z * 2 * q.w + 2 * q.x * q.y, 1 - 2 * q.x * q.x - 2 * q.z * q.z, q.x * 2 * q.w + 2 * q.y * q.z,

        q.y * 2 * q.w + 2 * q.x * q.z, -q.x * 2 * q.w + 2 * q.y * q.z, 1 - 2 * q.x * q.x - 2 * q.y * q.y };
    return m;
}

inline mat4f toMat4(const quat &q)
{
    mat4f m(1.0f);
    m = toMat3(q);
    return m;
}

///////////////////////////////////////////////
//Interpolation
///////////////////////////////////////////////
inline float clamp(float f, float a, float b)
{
    f = f < a ? a : (f > b ? b : f);
    return f;
}

inline float smoothstep(float a, float b, float x)
{
    x = clamp((x - a) / (b - a), 0.0f, 1.0f);
    return x * x * (3 - 2 * x);
}

#endif