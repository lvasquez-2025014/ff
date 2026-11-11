#pragma once

#include "lib.h"
// Variables de Selección
uint32_t selectedEntity = 0;
std::string selectedName = "";
float selectedDist = 0.0f;

struct Vector2
{
    union
    {
        struct
        {
            float x;
            float y;
        };
        float data[2];
    };

    /**
     * Constructors.
     */
    inline Vector2();
    inline Vector2(float data[]);
    inline Vector2(float value);
    inline Vector2(float x, float y);


    /**
     * Constants for common vectors.
     */
    static inline Vector2 Zero();
    static inline Vector2 One();
    static inline Vector2 Right();
    static inline Vector2 Left();
    static inline Vector2 Up();
    static inline Vector2 Down();


    /**
     * Returns the angle between two vectors in radians.
     * @param a: The first vector.
     * @param b: The second vector.
     * @return: A scalar value.
     */
    static inline float Angle(Vector2 a, Vector2 b);

    /**
     * Returns a vector with its magnitude clamped to maxLength.
     * @param vector: The target vector.
     * @param maxLength: The maximum length of the return vector.
     * @return: A new vector.
     */
    static inline Vector2 ClampMagnitude(Vector2 vector, float maxLength);

    /**
     * Returns the component of a in the direction of b (scalar projection).
     * @param a: The target vector.
     * @param b: The vector being compared against.
     * @return: A scalar value.
     */
    static inline float Component(Vector2 a, Vector2 b);

    /**
     * Returns the distance between a and b.
     * @param a: The first point.
     * @param b: The second point.
     * @return: A scalar value.
     */
    static inline float Distance(Vector2 a, Vector2 b);

    /**
     * Returns the dot product of two vectors.
     * @param lhs: The left side of the multiplication.
     * @param rhs: The right side of the multiplication.
     * @return: A scalar value.
     */
    static inline float Dot(Vector2 lhs, Vector2 rhs);

    /**
     * Converts a polar representation of a vector into cartesian
     * coordinates.
     * @param rad: The magnitude of the vector.
     * @param theta: The angle from the X axis.
     * @return: A new vector.
     */
    static inline Vector2 FromPolar(float rad, float theta);

    /**
     * Returns a vector linearly interpolated between a and b, moving along
     * a straight line. The vector is clamped to never go beyond the end points.
     * @param a: The starting point.
     * @param b: The ending point.
     * @param t: The interpolation value [0-1].
     * @return: A new vector.
     */
    static inline Vector2 Lerp(Vector2 a, Vector2 b, float t);

    /**
     * Returns a vector linearly interpolated between a and b, moving along
     * a straight line.
     * @param a: The starting point.
     * @param b: The ending point.
     * @param t: The interpolation value [0-1] (no actual bounds).
     * @return: A new vector.
     */
    static inline Vector2 LerpUnclamped(Vector2 a, Vector2 b, float t);

    /**
     * Returns the magnitude of a vector.
     * @param v: The vector in question.
     * @return: A scalar value.
     */
    static inline float Magnitude(Vector2 v);

    /**
     * Returns a vector made from the largest components of two other vectors.
     * @param a: The first vector.
     * @param b: The second vector.
     * @return: A new vector.
     */
    static inline Vector2 Max(Vector2 a, Vector2 b);

    /**
     * Returns a vector made from the smallest components of two other vectors.
     * @param a: The first vector.
     * @param b: The second vector.
     * @return: A new vector.
     */
    static inline Vector2 Min(Vector2 a, Vector2 b);

    /**
     * Returns a vector "maxDistanceDelta" units closer to the target. This
     * interpolation is in a straight line, and will not overshoot.
     * @param current: The current position.
     * @param target: The destination position.
     * @param maxDistanceDelta: The maximum distance to move.
     * @return: A new vector.
     */
    static inline Vector2 MoveTowards(Vector2 current, Vector2 target,
        float maxDistanceDelta);

    /**
     * Returns a new vector with magnitude of one.
     * @param v: The vector in question.
     * @return: A new vector.
     */
    static inline Vector2 Normalized(Vector2 v);

    /**
     * Creates a new coordinate system out of the two vectors.
     * Normalizes "normal" and normalizes "tangent" and makes it orthogonal to
     * "normal"..
     * @param normal: A reference to the first axis vector.
     * @param tangent: A reference to the second axis vector.
     */
    static inline void OrthoNormalize(Vector2& normal, Vector2& tangent);

    /**
     * Returns the vector projection of a onto b.
     * @param a: The target vector.
     * @param b: The vector being projected onto.
     * @return: A new vector.
     */
    static inline Vector2 Project(Vector2 a, Vector2 b);

    /**
     * Returns a vector reflected about the provided line.
     * This behaves as if there is a plane with the line as its normal, and the
     * vector comes in and bounces off this plane.
     * @param vector: The vector traveling inward at the imaginary plane.
     * @param line: The line about which to reflect.
     * @return: A new vector pointing outward from the imaginary plane.
     */
    static inline Vector2 Reflect(Vector2 vector, Vector2 line);

    /**
     * Returns the vector rejection of a on b.
     * @param a: The target vector.
     * @param b: The vector being projected onto.
     * @return: A new vector.
     */
    static inline Vector2 Reject(Vector2 a, Vector2 b);

    /**
     * Rotates vector "current" towards vector "target" by "maxRadiansDelta".
     * This treats the vectors as directions and will linearly interpolate
     * between their magnitudes by "maxMagnitudeDelta". This function does not
     * overshoot. If a negative delta is supplied, it will rotate away from
     * "target" until it is pointing the opposite direction, but will not
     * overshoot that either.
     * @param current: The starting direction.
     * @param target: The destination direction.
     * @param maxRadiansDelta: The maximum number of radians to rotate.
     * @param maxMagnitudeDelta: The maximum delta for magnitude interpolation.
     * @return: A new vector.
     */
    static inline Vector2 RotateTowards(Vector2 current, Vector2 target,
        float maxRadiansDelta,
        float maxMagnitudeDelta);

    /**
     * Multiplies two vectors component-wise.
     * @param a: The lhs of the multiplication.
     * @param b: The rhs of the multiplication.
     * @return: A new vector.
     */
    static inline Vector2 Scale(Vector2 a, Vector2 b);

    /**
     * Returns a vector rotated towards b from a by the percent t.
     * Since interpolation is done spherically, the vector moves at a constant
     * angular velocity. This rotation is clamped to 0 <= t <= 1.
     * @param a: The starting direction.
     * @param b: The ending direction.
     * @param t: The interpolation value [0-1].
     */
    static inline Vector2 Slerp(Vector2 a, Vector2 b, float t);

    /**
     * Returns a vector rotated towards b from a by the percent t.
     * Since interpolation is done spherically, the vector moves at a constant
     * angular velocity. This rotation is unclamped.
     * @param a: The starting direction.
     * @param b: The ending direction.
     * @param t: The interpolation value [0-1].
     */
    static inline Vector2 SlerpUnclamped(Vector2 a, Vector2 b, float t);

    /**
     * Returns the squared magnitude of a vector.
     * This is useful when comparing relative lengths, where the exact length
     * is not important, and much time can be saved by not calculating the
     * square root.
     * @param v: The vector in question.
     * @return: A scalar value.
     */
    static inline float SqrMagnitude(Vector2 v);

    /**
     * Calculates the polar coordinate space representation of a vector.
     * @param vector: The vector to convert.
     * @param rad: The magnitude of the vector.
     * @param theta: The angle from the X axis.
     */
    static inline void ToPolar(Vector2 vector, float& rad, float& theta);


    /**
     * Operator overloading.
     */
    inline struct Vector2& operator+=(const float rhs);
    inline struct Vector2& operator-=(const float rhs);
    inline struct Vector2& operator*=(const float rhs);
    inline struct Vector2& operator/=(const float rhs);
    inline struct Vector2& operator+=(const Vector2 rhs);
    inline struct Vector2& operator-=(const Vector2 rhs);
};

inline Vector2 operator-(Vector2 rhs);
inline Vector2 operator+(Vector2 lhs, const float rhs);
inline Vector2 operator-(Vector2 lhs, const float rhs);
inline Vector2 operator*(Vector2 lhs, const float rhs);
inline Vector2 operator/(Vector2 lhs, const float rhs);
inline Vector2 operator+(const float lhs, Vector2 rhs);
inline Vector2 operator-(const float lhs, Vector2 rhs);
inline Vector2 operator*(const float lhs, Vector2 rhs);
inline Vector2 operator/(const float lhs, Vector2 rhs);
inline Vector2 operator+(Vector2 lhs, const Vector2 rhs);
inline Vector2 operator-(Vector2 lhs, const Vector2 rhs);
inline bool operator==(const Vector2 lhs, const Vector2 rhs);
inline bool operator!=(const Vector2 lhs, const Vector2 rhs);

Vector2::Vector2() : x(0), y(0) {}
Vector2::Vector2(float data[]) : x(data[0]), y(data[1]) {}
Vector2::Vector2(float value) : x(value), y(value) {}
Vector2::Vector2(float x, float y) : x(x), y(y) {}

Vector2 Vector2::Zero() { return Vector2(0, 0); }
Vector2 Vector2::One() { return Vector2(1, 1); }
Vector2 Vector2::Right() { return Vector2(1, 0); }
Vector2 Vector2::Left() { return Vector2(-1, 0); }
Vector2 Vector2::Up() { return Vector2(0, 1); }
Vector2 Vector2::Down() { return Vector2(0, -1); }

float Vector2::Angle(Vector2 a, Vector2 b)
{
    float v = Dot(a, b) / (Magnitude(a) * Magnitude(b));
    v = fmax(v, -1.0);
    v = fmin(v, 1.0);
    return acos(v);
}

Vector2 Vector2::ClampMagnitude(Vector2 vector, float maxLength)
{
    float length = Magnitude(vector);
    if (length > maxLength)
        vector *= maxLength / length;
    return vector;
}

float Vector2::Component(Vector2 a, Vector2 b)
{
    return Dot(a, b) / Magnitude(b);
}

float Vector2::Distance(Vector2 a, Vector2 b)
{
    return Vector2::Magnitude(a - b);
}

float Vector2::Dot(Vector2 lhs, Vector2 rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

Vector2 Vector2::FromPolar(float rad, float theta)
{
    Vector2 v;
    v.x = rad * cos(theta);
    v.y = rad * sin(theta);
    return v;
}

Vector2 Vector2::Lerp(Vector2 a, Vector2 b, float t)
{
    if (t < 0) return a;
    else if (t > 1) return b;
    return LerpUnclamped(a, b, t);
}

Vector2 Vector2::LerpUnclamped(Vector2 a, Vector2 b, float t)
{
    return (b - a) * t + a;
}

float Vector2::Magnitude(Vector2 v)
{
    return sqrt(SqrMagnitude(v));
}

Vector2 Vector2::Max(Vector2 a, Vector2 b)
{
    float x = a.x > b.x ? a.x : b.x;
    float y = a.y > b.y ? a.y : b.y;
    return Vector2(x, y);
}

Vector2 Vector2::Min(Vector2 a, Vector2 b)
{
    float x = a.x > b.x ? b.x : a.x;
    float y = a.y > b.y ? b.y : a.y;
    return Vector2(x, y);
}

Vector2 Vector2::MoveTowards(Vector2 current, Vector2 target, float maxDistanceDelta)
{
    Vector2 d = target - current;
    float m = Magnitude(d);
    if (m < maxDistanceDelta || m == 0)
        return target;
    return current + (d * maxDistanceDelta / m);
}

Vector2 Vector2::Normalized(Vector2 v)
{
    float mag = Magnitude(v);
    if (mag == 0)
        return Vector2::Zero();
    return v / mag;
}

void Vector2::OrthoNormalize(Vector2& normal, Vector2& tangent)
{
    normal = Normalized(normal);
    tangent = Reject(tangent, normal);
    tangent = Normalized(tangent);
}

Vector2 Vector2::Project(Vector2 a, Vector2 b)
{
    float m = Magnitude(b);
    return Dot(a, b) / (m * m) * b;
}

Vector2 Vector2::Reflect(Vector2 vector, Vector2 planeNormal)
{
    return vector - 2 * Project(vector, planeNormal);
}

Vector2 Vector2::Reject(Vector2 a, Vector2 b)
{
    return a - Project(a, b);
}

Vector2 Vector2::RotateTowards(Vector2 current, Vector2 target, float maxRadiansDelta, float maxMagnitudeDelta)
{
    float magCur = Magnitude(current);
    float magTar = Magnitude(target);
    float newMag = magCur + maxMagnitudeDelta *
        ((magTar > magCur) - (magCur > magTar));
    newMag = fmin(newMag, fmax(magCur, magTar));
    newMag = fmax(newMag, fmin(magCur, magTar));

    float totalAngle = Angle(current, target) - maxRadiansDelta;
    if (totalAngle <= 0)
        return Normalized(target) * newMag;
    else if (totalAngle >= M_PI)
        return Normalized(-target) * newMag;

    float axis = current.x * target.y - current.y * target.x;
    axis = axis / fabs(axis);
    if (!(1 - fabs(axis) < 0.00001))
        axis = 1;
    current = Normalized(current);
    Vector2 newVector = current * cos(maxRadiansDelta) +
        Vector2(-current.y, current.x) * sin(maxRadiansDelta) * axis;
    return newVector * newMag;
}

Vector2 Vector2::Scale(Vector2 a, Vector2 b)
{
    return Vector2(a.x * b.x, a.y * b.y);
}

Vector2 Vector2::Slerp(Vector2 a, Vector2 b, float t)
{
    if (t < 0) return a;
    else if (t > 1) return b;
    return SlerpUnclamped(a, b, t);
}

Vector2 Vector2::SlerpUnclamped(Vector2 a, Vector2 b, float t)
{
    float magA = Magnitude(a);
    float magB = Magnitude(b);
    a /= magA;
    b /= magB;
    float dot = Dot(a, b);
    dot = fmax(dot, -1.0);
    dot = fmin(dot, 1.0);
    float theta = acos(dot) * t;
    Vector2 relativeVec = Normalized(b - a * dot);
    Vector2 newVec = a * cos(theta) + relativeVec * sin(theta);
    return newVec * (magA + (magB - magA) * t);
}

float Vector2::SqrMagnitude(Vector2 v)
{
    return v.x * v.x + v.y * v.y;
}

void Vector2::ToPolar(Vector2 vector, float& rad, float& theta)
{
    rad = Magnitude(vector);
    theta = atan2(vector.y, vector.x);
}

struct Vector2& Vector2::operator+=(const float rhs)
{
    x += rhs;
    y += rhs;
    return *this;
}

struct Vector2& Vector2::operator-=(const float rhs)
{
    x -= rhs;
    y -= rhs;
    return *this;
}

struct Vector2& Vector2::operator*=(const float rhs)
{
    x *= rhs;
    y *= rhs;
    return *this;
}

struct Vector2& Vector2::operator/=(const float rhs)
{
    x /= rhs;
    y /= rhs;
    return *this;
}

struct Vector2& Vector2::operator+=(const Vector2 rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

struct Vector2& Vector2::operator-=(const Vector2 rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

Vector2 operator-(Vector2 rhs) { return rhs * -1; }
Vector2 operator+(Vector2 lhs, const float rhs) { return lhs += rhs; }
Vector2 operator-(Vector2 lhs, const float rhs) { return lhs -= rhs; }
Vector2 operator*(Vector2 lhs, const float rhs) { return lhs *= rhs; }
Vector2 operator/(Vector2 lhs, const float rhs) { return lhs /= rhs; }
Vector2 operator+(const float lhs, Vector2 rhs) { return rhs += lhs; }
Vector2 operator-(const float lhs, Vector2 rhs) { return rhs -= lhs; }
Vector2 operator*(const float lhs, Vector2 rhs) { return rhs *= lhs; }
Vector2 operator/(const float lhs, Vector2 rhs) { return rhs /= lhs; }
Vector2 operator+(Vector2 lhs, const Vector2 rhs) { return lhs += rhs; }
Vector2 operator-(Vector2 lhs, const Vector2 rhs) { return lhs -= rhs; }

bool operator==(const Vector2 lhs, const Vector2 rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(const Vector2 lhs, const Vector2 rhs)
{
    return !(lhs == rhs);
}

struct Vector3
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
        };
        float data[3];
    };

    /**
     * Constructors.
     */
    inline Vector3();
    inline Vector3(float data[]);
    inline Vector3(float value);
    inline Vector3(float x, float y);
    inline Vector3(float x, float y, float z);

    /**
     * Constants for common vectors.
     */
    static inline Vector3 Zero();
    static inline Vector3 One();
    static inline Vector3 Right();
    static inline Vector3 Left();
    static inline Vector3 Up();
    static inline Vector3 Down();
    static inline Vector3 Forward();
    static inline Vector3 Backward();


    /**
     * Returns the angle between two vectors in radians.
     * @param a: The first vector.
     * @param b: The second vector.
     * @return: A scalar value.
     */
    static inline float Vector3::Angle(Vector3 a, Vector3 b)
    {
        float v = Vector3::Dot(a, b) / (Vector3::Magnitude(a) * Vector3::Magnitude(b));
        v = fmax(v, -1.0f);
        v = fmin(v, 1.0f);
        return acosf(v);
    }

    /**
     * Returns a vector with its magnitude clamped to maxLength.
     * @param vector: The target vector.
     * @param maxLength: The maximum length of the return vector.
     * @return: A new vector.
     */
    static inline Vector3 ClampMagnitude(Vector3 vector, float maxLength);
    // Verifica si el vector es cero
    bool IsZero() const {
        return x == 0.0f && y == 0.0f && z == 0.0f;
    }
    /**
     * Retorna o componente de a na dire  o de b (proje  o escalar).
     * @param a: O vetor de destino.
     * @param b: O vetor que est  sendo comparado.
     * @return: Um valor escalar.
     */
    static inline float Component(Vector3 a, Vector3 b);

    /**
     * Retorna o produto vetorial de dois vetores.
     * @param lhs: O lado esquerdo da multiplica  o.
     * @param rhs: O lado direito da multiplica  o.
     * @return: Um novo vetor.
     */
    static inline Vector3 Cross(Vector3 lhs, Vector3 rhs);

    /**
     * Retorna o produto vetorial de dois vetores.
     * @param lhs: O lado esquerdo da multiplica  o.
     * @param rhs: O lado direito da multiplica  o.
     * @return: Um novo vetor.
     */
    static inline float Distance(const Vector3& a, const Vector3& b) {
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        float dz = b.z - a.z;
        return sqrtf(dx * dx + dy * dy + dz * dz);
    }

    /**
     * Retorna o produto vetorial de dois vetores.
     * @param lhs: O lado esquerdo da multiplica  o.
     * @param rhs: O lado direito da multiplica  o.
     * @return: Um novo vetor.
     */
    static inline float SqrDistance(const Vector3& a, const Vector3& b) {
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        float dz = b.z - a.z;
        return dx * dx + dy * dy + dz * dz;
    }

    /**
     * Retorna o produto vetorial de dois vetores.
     * @param lhs: O lado esquerdo da multiplica  o.
     * @param rhs: O lado direito da multiplica  o.
     * @return: Um novo vetor.
     */
    static inline float Dot(const Vector3& lhs, const Vector3& rhs) {
        return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
    }

    /**
     * Converte uma representa  o esf rica de um vetor em cartesiano
     * coordenadas.
     * Isso usa a conven  o ISO (raio r, inclina  o theta, azimute phi).
     * @param rad: A magnitude do vetor.
     * @param theta: O  ngulo no plano XY do eixo X.
     * @param phi: O  ngulo do eixo Z positivo para o vetor.
     * @return: Um novo vetor.
     */
    static inline Vector3 FromSpherical(float rad, float theta, float phi);

    /**
     * Returns a vector linearly interpolated between a and b, moving along
     * a straight line. The vector is clamped to never go beyond the end points.
     * @param a: The starting point.
     * @param b: The ending point.
     * @param t: The interpolation value [0-1].
     * @return: A new vector.
     */
    static inline Vector3 Lerp(Vector3 a, Vector3 b, float t);

    /**
     * Returns a vector linearly interpolated between a and b, moving along
     * a straight line.
     * @param a: The starting point.
     * @param b: The ending point.
     * @param t: The interpolation value [0-1] (no actual bounds).
     * @return: A new vector.
     */
    static inline Vector3 LerpUnclamped(Vector3 a, Vector3 b, float t);

    /**
     * Returns the magnitude of a vector.
     * @param v: The vector in question.
     * @return: A scalar value.
     */
    static inline float Magnitude(Vector3 v);

    // Normalizes the vector to have a magnitude of one.
    inline void Normalize()
    {
        float mag = Vector3::Magnitude(*this);
        if (mag != 0)
        {
            x /= mag;
            y /= mag;
            z /= mag;
        }
    }

    /**
     * Returns a vector made from the largest components of two other vectors.
     * @param a: The first vector.
     * @param b: The second vector.
     * @return: A new vector.
     */
    static inline Vector3 Max(Vector3 a, Vector3 b);

    /**
     * Returns a vector made from the smallest components of two other vectors.
     * @param a: The first vector.
     * @param b: The second vector.
     * @return: A new vector.
     */
    static inline Vector3 Min(Vector3 a, Vector3 b);

    /**
     * Returns a vector "maxDistanceDelta" units closer to the target. This
     * interpolation is in a straight line, and will not overshoot.
     * @param current: The current position.
     * @param target: The destination position.
     * @param maxDistanceDelta: The maximum distance to move.
     * @return: A new vector.
     */
    static inline Vector3 MoveTowards(Vector3 current, Vector3 target,
        float maxDistanceDelta);

    /**
     * Returns a new vector with magnitude of one.
     * @param v: The vector in question.
     * @return: A new vector.
     */
    static inline Vector3 Normalized(Vector3 v);

    /**
     * Returns an arbitrary vector orthogonal to the input.
     * This vector is not normalized.
     * @param v: The input vector.
     * @return: A new vector.
     */
    static inline Vector3 Orthogonal(Vector3 v);

    /**
     * Creates a new coordinate system out of the three vectors.
     * Normalizes "normal", normalizes "tangent" and makes it orthogonal to
     * "normal" and normalizes "binormal" and makes it orthogonal to both
     * "normal" and "tangent".
     * @param normal: A reference to the first axis vector.
     * @param tangent: A reference to the second axis vector.
     * @param binormal: A reference to the third axis vector.
     */
    static inline void OrthoNormalize(Vector3& normal, Vector3& tangent,
        Vector3& binormal);

    /**
     * Returns the vector projection of a onto b.
     * @param a: The target vector.
     * @param b: The vector being projected onto.
     * @return: A new vector.
     */
    static inline Vector3 Project(Vector3 a, Vector3 b);

    /**
     * Returns a vector projected onto a plane orthogonal to "planeNormal".
     * This can be visualized as the shadow of the vector onto the plane, if
     * the light source were in the direction of the plane normal.
     * @param vector: The vector to project.
     * @param planeNormal: The normal of the plane onto which to project.
     * @param: A new vector.
     */
    static inline Vector3 ProjectOnPlane(Vector3 vector, Vector3 planeNormal);

    /**
     * Returns a vector reflected off the plane orthogonal to the normal.
     * The input vector is pointed inward, at the plane, and the return vector
     * is pointed outward from the plane, like a beam of light hitting and then
     * reflecting off a mirror.
     * @param vector: The vector traveling inward at the plane.
     * @param planeNormal: The normal of the plane off of which to reflect.
     * @return: A new vector pointing outward from the plane.
     */
    static inline Vector3 Reflect(Vector3 vector, Vector3 planeNormal);

    /**
     * Returns the vector rejection of a on b.
     * @param a: The target vector.
     * @param b: The vector being projected onto.
     * @return: A new vector.
     */
    static inline Vector3 Reject(Vector3 a, Vector3 b);

    /**
     * Rotates vector "current" towards vector "target" by "maxRadiansDelta".
     * This treats the vectors as directions and will linearly interpolate
     * between their magnitudes by "maxMagnitudeDelta". This function does not
     * overshoot. If a negative delta is supplied, it will rotate away from
     * "target" until it is pointing the opposite direction, but will not
     * overshoot that either.
     * @param current: The starting direction.
     * @param target: The destination direction.
     * @param maxRadiansDelta: The maximum number of radians to rotate.
     * @param maxMagnitudeDelta: The maximum delta for magnitude interpolation.
     * @return: A new vector.
     */
    static inline Vector3 RotateTowards(Vector3 current, Vector3 target,
        float maxRadiansDelta,
        float maxMagnitudeDelta);

    /**
     * Multiplies two vectors element-wise.
     * @param a: The lhs of the multiplication.
     * @param b: The rhs of the multiplication.
     * @return: A new vector.
     */
    static inline Vector3 Scale(Vector3 a, Vector3 b);

    /**
     * Returns a vector rotated towards b from a by the percent t.
     * Since interpolation is done spherically, the vector moves at a constant
     * angular velocity. This rotation is clamped to 0 <= t <= 1.
     * @param a: The starting direction.
     * @param b: The ending direction.
     * @param t: The interpolation value [0-1].
     */
    static inline Vector3 Slerp(Vector3 a, Vector3 b, float t);

    /**
     * Returns a vector rotated towards b from a by the percent t.
     * Since interpolation is done spherically, the vector moves at a constant
     * angular velocity. This rotation is unclamped.
     * @param a: The starting direction.
     * @param b: The ending direction.
     * @param t: The interpolation value [0-1].
     */
    static inline Vector3 SlerpUnclamped(Vector3 a, Vector3 b, float t);

    /**
     * Returns the squared magnitude of a vector.
     * This is useful when comparing relative lengths, where the exact length
     * is not important, and much time can be saved by not calculating the
     * square root.
     * @param v: The vector in question.
     * @return: A scalar value.
     */
    static inline float SqrMagnitude(Vector3 v);

    /**
     * Calculates the spherical coordinate space representation of a vector.
     * This uses the ISO convention (radius r, inclination theta, azimuth phi).
     * @param vector: The vector to convert.
     * @param rad: The magnitude of the vector.
     * @param theta: The angle in the XY plane from the X axis.
     * @param phi: The angle from the positive Z axis to the vector.
     */
    static inline void ToSpherical(Vector3 vector, float& rad, float& theta,
        float& phi);


    /**
     * Operator overloading.
     */
    inline struct Vector3& operator+=(const float rhs);
    inline struct Vector3& operator-=(const float rhs);
    inline struct Vector3& operator*=(const float rhs);
    inline struct Vector3& operator/=(const float rhs);
    inline struct Vector3& operator+=(const Vector3 rhs);
    inline struct Vector3& operator-=(const Vector3 rhs);
};

inline Vector3 operator-(Vector3 rhs);
inline Vector3 operator+(Vector3 lhs, const float rhs);
inline Vector3 operator-(Vector3 lhs, const float rhs);
inline Vector3 operator*(Vector3 lhs, const float rhs);
inline Vector3 operator/(Vector3 lhs, const float rhs);
inline Vector3 operator+(const float lhs, Vector3 rhs);
inline Vector3 operator-(const float lhs, Vector3 rhs);
inline Vector3 operator*(const float lhs, Vector3 rhs);
inline Vector3 operator/(const float lhs, Vector3 rhs);
inline Vector3 operator+(Vector3 lhs, const Vector3 rhs);
inline Vector3 operator-(Vector3 lhs, const Vector3 rhs);
inline bool operator==(const Vector3 lhs, const Vector3 rhs);
inline bool operator!=(const Vector3 lhs, const Vector3 rhs);

Vector3::Vector3() : x(0), y(0), z(0) {}
Vector3::Vector3(float data[]) : x(data[0]), y(data[1]), z(data[2]) {}
Vector3::Vector3(float value) : x(value), y(value), z(value) {}
Vector3::Vector3(float x, float y) : x(x), y(y), z(0) {}
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

Vector3 Vector3::Zero() { return Vector3(0, 0, 0); }
Vector3 Vector3::One() { return Vector3(1, 1, 1); }
Vector3 Vector3::Right() { return Vector3(1, 0, 0); }
Vector3 Vector3::Left() { return Vector3(-1, 0, 0); }
Vector3 Vector3::Up() { return Vector3(0, 1, 0); }
Vector3 Vector3::Down() { return Vector3(0, -1, 0); }
Vector3 Vector3::Forward() { return Vector3(0, 0, 1); }
Vector3 Vector3::Backward() { return Vector3(0, 0, -1); }

static inline float Angle(const Vector3& a, const Vector3& b) {
    float dot = a.x * b.x + a.y * b.y + a.z * b.z;

    float magA = sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
    float magB = sqrtf(b.x * b.x + b.y * b.y + b.z * b.z);

    if (magA == 0.0f || magB == 0.0f)
        return 0.0f;

    float v = dot / (magA * magB);
    v = std::fmaxf(-1.0f, std::fminf(1.0f, v));
    return acosf(v);
}

Vector3 Vector3::ClampMagnitude(Vector3 vector, float maxLength)
{
    float length = Magnitude(vector);
    if (length > maxLength)
        vector *= maxLength / length;
    return vector;
}

float Vector3::Component(Vector3 a, Vector3 b)
{
    return Dot(a, b) / Magnitude(b);
}

Vector3 Vector3::Cross(Vector3 lhs, Vector3 rhs)
{
    float x = lhs.y * rhs.z - lhs.z * rhs.y;
    float y = lhs.z * rhs.x - lhs.x * rhs.z;
    float z = lhs.x * rhs.y - lhs.y * rhs.x;
    return Vector3(x, y, z);
}

Vector3 Vector3::FromSpherical(float rad, float theta, float phi)
{
    Vector3 v;
    v.x = rad * sin(theta) * cos(phi);
    v.y = rad * sin(theta) * sin(phi);
    v.z = rad * cos(theta);
    return v;
}

Vector3 Vector3::Lerp(Vector3 a, Vector3 b, float t)
{
    if (t < 0) return a;
    else if (t > 1) return b;
    return LerpUnclamped(a, b, t);
}

Vector3 Vector3::LerpUnclamped(Vector3 a, Vector3 b, float t)
{
    return (b - a) * t + a;
}

float Vector3::Magnitude(Vector3 v)
{
    return sqrt(SqrMagnitude(v));
}

Vector3 Vector3::Max(Vector3 a, Vector3 b)
{
    float x = a.x > b.x ? a.x : b.x;
    float y = a.y > b.y ? a.y : b.y;
    float z = a.z > b.z ? a.z : b.z;
    return Vector3(x, y, z);
}

Vector3 Vector3::Min(Vector3 a, Vector3 b)
{
    float x = a.x > b.x ? b.x : a.x;
    float y = a.y > b.y ? b.y : a.y;
    float z = a.z > b.z ? b.z : a.z;
    return Vector3(x, y, z);
}

Vector3 Vector3::MoveTowards(Vector3 current, Vector3 target, float maxDistanceDelta)
{
    Vector3 d = target - current;
    float m = Magnitude(d);
    if (m < maxDistanceDelta || m == 0)
        return target;
    return current + (d * maxDistanceDelta / m);
}

Vector3 Vector3::Normalized(Vector3 v)
{
    float mag = Magnitude(v);
    if (mag == 0)
        return Vector3::Zero();
    return v / mag;
}

Vector3 Vector3::Orthogonal(Vector3 v)
{
    return v.z < v.x ? Vector3(v.y, -v.x, 0) : Vector3(0, -v.z, v.y);
}

void Vector3::OrthoNormalize(Vector3& normal, Vector3& tangent, Vector3& binormal)
{
    normal = Normalized(normal);
    tangent = ProjectOnPlane(tangent, normal);
    tangent = Normalized(tangent);
    binormal = ProjectOnPlane(binormal, tangent);
    binormal = ProjectOnPlane(binormal, normal);
    binormal = Normalized(binormal);
}

Vector3 Vector3::Project(Vector3 a, Vector3 b)
{
    float m = Magnitude(b);
    return Dot(a, b) / (m * m) * b;
}

Vector3 Vector3::ProjectOnPlane(Vector3 vector, Vector3 planeNormal)
{
    return Reject(vector, planeNormal);
}

Vector3 Vector3::Reflect(Vector3 vector, Vector3 planeNormal)
{
    return vector - 2 * Project(vector, planeNormal);
}

Vector3 Vector3::Reject(Vector3 a, Vector3 b)
{
    return a - Project(a, b);
}

Vector3 Vector3::RotateTowards(Vector3 current, Vector3 target, float maxRadiansDelta, float maxMagnitudeDelta)
{
    float magCur = Magnitude(current);
    float magTar = Magnitude(target);
    float newMag = magCur + maxMagnitudeDelta *
        ((magTar > magCur) - (magCur > magTar));
    newMag = fmin(newMag, fmax(magCur, magTar));
    newMag = fmax(newMag, fmin(magCur, magTar));

    float totalAngle = Angle(current, target) - maxRadiansDelta;
    if (totalAngle <= 0)
        return Normalized(target) * newMag;
    else if (totalAngle >= M_PI)
        return Normalized(-target) * newMag;

    Vector3 axis = Cross(current, target);
    float magAxis = Magnitude(axis);
    if (magAxis == 0)
        axis = Normalized(Cross(current, current + Vector3(3.95, 5.32, -4.24)));
    else
        axis /= magAxis;
    current = Normalized(current);
    Vector3 newVector = current * cos(maxRadiansDelta) +
        Cross(axis, current) * sin(maxRadiansDelta);
    return newVector * newMag;
}

Vector3 Vector3::Scale(Vector3 a, Vector3 b)
{
    return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
}

Vector3 Vector3::Slerp(Vector3 a, Vector3 b, float t)
{
    if (t < 0) return a;
    else if (t > 1) return b;
    return SlerpUnclamped(a, b, t);
}

Vector3 Vector3::SlerpUnclamped(Vector3 a, Vector3 b, float t)
{
    float magA = Magnitude(a);
    float magB = Magnitude(b);
    a /= magA;
    b /= magB;
    float dot = Dot(a, b);
    dot = fmax(dot, -1.0);
    dot = fmin(dot, 1.0);
    float theta = acos(dot) * t;
    Vector3 relativeVec = Normalized(b - a * dot);
    Vector3 newVec = a * cos(theta) + relativeVec * sin(theta);
    return newVec * (magA + (magB - magA) * t);
}

float Vector3::SqrMagnitude(Vector3 v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

void Vector3::ToSpherical(Vector3 vector, float& rad, float& theta, float& phi)
{
    rad = Magnitude(vector);
    float v = vector.z / rad;
    v = fmax(v, -1.0);
    v = fmin(v, 1.0);
    theta = acos(v);
    phi = atan2(vector.y, vector.x);
}

struct Vector3& Vector3::operator+=(const float rhs)
{
    x += rhs;
    y += rhs;
    z += rhs;
    return *this;
}

struct Vector3& Vector3::operator-=(const float rhs)
{
    x -= rhs;
    y -= rhs;
    z -= rhs;
    return *this;
}

struct Vector3& Vector3::operator*=(const float rhs)
{
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
}

struct Vector3& Vector3::operator/=(const float rhs)
{
    x /= rhs;
    y /= rhs;
    z /= rhs;
    return *this;
}

struct Vector3& Vector3::operator+=(const Vector3 rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

struct Vector3& Vector3::operator-=(const Vector3 rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

Vector3 operator-(Vector3 rhs) { return rhs * -1; }
Vector3 operator+(Vector3 lhs, const float rhs) { return lhs += rhs; }
Vector3 operator-(Vector3 lhs, const float rhs) { return lhs -= rhs; }
Vector3 operator*(Vector3 lhs, const float rhs) { return lhs *= rhs; }
Vector3 operator/(Vector3 lhs, const float rhs) { return lhs /= rhs; }
Vector3 operator+(const float lhs, Vector3 rhs) { return rhs += lhs; }
Vector3 operator-(const float lhs, Vector3 rhs) { return rhs -= lhs; }
Vector3 operator*(const float lhs, Vector3 rhs) { return rhs *= lhs; }
Vector3 operator/(const float lhs, Vector3 rhs) { return rhs /= lhs; }
Vector3 operator+(Vector3 lhs, const Vector3 rhs) { return lhs += rhs; }
Vector3 operator-(Vector3 lhs, const Vector3 rhs) { return lhs -= rhs; }

bool operator==(const Vector3 lhs, const Vector3 rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator!=(const Vector3 lhs, const Vector3 rhs)
{
    return !(lhs == rhs);
}

struct Quaternion
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
        float data[4];
    };


    /**
     * Constructors.
     */
    inline Quaternion();
    inline Quaternion(float data[]);
    inline Quaternion(Vector3 vector, float scalar);
    inline Quaternion(float x, float y, float z, float w);


    /**
     * Constants for common quaternions.
     */
    static inline Quaternion Identity();


    /**
     * Returns the angle between two quaternions.
     * The quaternions must be normalized.
     * @param a: The first quaternion.
     * @param b: The second quaternion.
     * @return: A scalar value.
     */
    static inline float Angle(Quaternion a, Quaternion b);

    /**
     * Returns the conjugate of a quaternion.
     * @param rotation: The quaternion in question.
     * @return: A new quaternion.
     */
    static inline Quaternion Conjugate(Quaternion rotation);

    /**
     * Returns the dot product of two quaternions.
     * @param lhs: The left side of the multiplication.
     * @param rhs: The right side of the multiplication.
     * @return: A scalar value.
     */
    static inline float Dot(Quaternion lhs, Quaternion rhs);

    /**
     * Creates a new quaternion from the angle-axis representation of
     * a rotation.
     * @param angle: The rotation angle in radians.
     * @param axis: The vector about which the rotation occurs.
     * @return: A new quaternion.
     */
    static inline Quaternion FromAngleAxis(float angle, Vector3 axis);

    /**
     * Create a new quaternion from the euler angle representation of
     * a rotation. The z, x and y values represent rotations about those
     * axis in that respective order.
     * @param rotation: The x, y and z rotations.
     * @return: A new quaternion.
     */
    static inline Quaternion FromEuler(Vector3 rotation);

    /**
     * Create a new quaternion from the euler angle representation of
     * a rotation. The z, x and y values represent rotations about those
     * axis in that respective order.
     * @param x: The rotation about the x-axis in radians.
     * @param y: The rotation about the y-axis in radians.
     * @param z: The rotation about the z-axis in radians.
     * @return: A new quaternion.
     */
    static inline Quaternion FromEuler(float x, float y, float z);

    /**
     * Create a quaternion rotation which rotates "fromVector" to "toVector".
     * @param fromVector: The vector from which to start the rotation.
     * @param toVector: The vector at which to end the rotation.
     * @return: A new quaternion.
     */
    static inline Quaternion FromToRotation(Vector3 fromVector,
        Vector3 toVector);

    /**
     * Returns the inverse of a rotation.
     * @param rotation: The quaternion in question.
     * @return: A new quaternion.
     */
    static inline Quaternion Inverse(Quaternion rotation);

    /**
     * Interpolates between a and b by t, which is clamped to the range [0-1].
     * The result is normalized before being returned.
     * @param a: The starting rotation.
     * @param b: The ending rotation.
     * @return: A new quaternion.
     */
    static inline Quaternion Lerp(Quaternion a, Quaternion b, float t);

    /**
     * Interpolates between a and b by t. This normalizes the result when
     * complete.
     * @param a: The starting rotation.
     * @param b: The ending rotation.
     * @param t: The interpolation value.
     * @return: A new quaternion.
     */
    static inline Quaternion LerpUnclamped(Quaternion a, Quaternion b,
        float t);

    /**
     * Creates a rotation with the specified forward direction. This is the
     * same as calling LookRotation with (0, 1, 0) as the upwards vector.
     * The output is undefined for parallel vectors.
     * @param forward: The forward direction to look toward.
     * @return: A new quaternion.
     */
    static inline Quaternion LookRotation(Vector3 forward);

    /**
     * Creates a rotation with the specified forward and upwards directions.
     * The output is undefined for parallel vectors.
     * @param forward: The forward direction to look toward.
     * @param upwards: The direction to treat as up.
     * @return: A new quaternion.
     */
    static inline Quaternion LookRotation(Vector3 forward, Vector3 upwards);

    /**
     * Returns the norm of a quaternion.
     * @param rotation: The quaternion in question.
     * @return: A scalar value.
     */
    static inline float Norm(Quaternion rotation);

    /**
     * Returns a quaternion with identical rotation and a norm of one.
     * @param rotation: The quaternion in question.
     * @return: A new quaternion.
     */
    static inline Quaternion Normalized(Quaternion rotation);

    /**
     * Returns a new Quaternion created by rotating "from" towards "to" by
     * "maxRadiansDelta". This will not overshoot, and if a negative delta is
     * applied, it will rotate till completely opposite "to" and then stop.
     * @param from: The rotation at which to start.
     * @param to: The rotation at which to end.
     # @param maxRadiansDelta: The maximum number of radians to rotate.
     * @return: A new Quaternion.
     */
    static inline Quaternion RotateTowards(Quaternion from, Quaternion to,
        float maxRadiansDelta);

    /**
     * Returns a new quaternion interpolated between a and b, using spherical
     * linear interpolation. The variable t is clamped to the range [0-1]. The
     * resulting quaternion will be normalized.
     * @param a: The starting rotation.
     * @param b: The ending rotation.
     * @param t: The interpolation value.
     * @return: A new quaternion.
     */
    static inline Quaternion Slerp(Quaternion a, Quaternion b, float t);

    /**
     * Returns a new quaternion interpolated between a and b, using spherical
     * linear interpolation. The resulting quaternion will be normalized.
     * @param a: The starting rotation.
     * @param b: The ending rotation.
     * @param t: The interpolation value.
     * @return: A new quaternion.
     */
    static inline Quaternion SlerpUnclamped(Quaternion a, Quaternion b,
        float t);

    /**
     * Outputs the angle axis representation of the provided quaternion.
     * @param rotation: The input quaternion.
     * @param angle: The output angle.
     * @param axis: The output axis.
     */
    static inline void ToAngleAxis(Quaternion rotation, float& angle,
        Vector3& axis);

    /**
     * Returns the Euler angle representation of a rotation. The resulting
     * vector contains the rotations about the z, x and y axis, in that order.
     * @param rotation: The quaternion to convert.
     * @return: A new vector.
     */
    static inline Vector3 ToEuler(Quaternion rotation);

    /**
     * Operator overloading.
     */
    inline struct Quaternion& operator+=(const float rhs);
    inline struct Quaternion& operator-=(const float rhs);
    inline struct Quaternion& operator*=(const float rhs);
    inline struct Quaternion& operator/=(const float rhs);
    inline struct Quaternion& operator+=(const Quaternion rhs);
    inline struct Quaternion& operator-=(const Quaternion rhs);
    inline struct Quaternion& operator*=(const Quaternion rhs);
};

inline Quaternion operator-(Quaternion rhs);
inline Quaternion operator+(Quaternion lhs, const float rhs);
inline Quaternion operator-(Quaternion lhs, const float rhs);
inline Quaternion operator*(Quaternion lhs, const float rhs);
inline Quaternion operator/(Quaternion lhs, const float rhs);
inline Quaternion operator+(const float lhs, Quaternion rhs);
inline Quaternion operator-(const float lhs, Quaternion rhs);
inline Quaternion operator*(const float lhs, Quaternion rhs);
inline Quaternion operator/(const float lhs, Quaternion rhs);
inline Quaternion operator+(Quaternion lhs, const Quaternion rhs);
inline Quaternion operator-(Quaternion lhs, const Quaternion rhs);
inline Quaternion operator*(Quaternion lhs, const Quaternion rhs);
inline Vector3 operator*(Quaternion lhs, const Vector3 rhs);
inline bool operator==(const Quaternion lhs, const Quaternion rhs);
inline bool operator!=(const Quaternion lhs, const Quaternion rhs);

Quaternion::Quaternion() : x(0), y(0), z(0), w(1) {}
Quaternion::Quaternion(float data[]) : x(data[0]), y(data[1]), z(data[2]), w(data[3]) {
}
Quaternion::Quaternion(Vector3 vector, float scalar) : x(vector.x), y(vector.y), z(vector.z), w(scalar) {
}
Quaternion::Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {
}

Quaternion Quaternion::Identity() { return Quaternion(0, 0, 0, 1); }

float Quaternion::Angle(Quaternion a, Quaternion b)
{
    float dot = Dot(a, b);
    return acos(fmin(fabs(dot), 1)) * 2;
}

Quaternion Quaternion::Conjugate(Quaternion rotation)
{
    return Quaternion(-rotation.x, -rotation.y, -rotation.z, rotation.w);
}

float Quaternion::Dot(Quaternion lhs, Quaternion rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

Quaternion Quaternion::FromAngleAxis(float angle, Vector3 axis)
{
    Quaternion q;
    float m = sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
    float s = sin(angle / 2) / m;
    q.x = axis.x * s;
    q.y = axis.y * s;
    q.z = axis.z * s;
    q.w = cos(angle / 2);
    return q;
}

Quaternion Quaternion::FromEuler(Vector3 rotation)
{
    return FromEuler(rotation.x, rotation.y, rotation.z);
}

Quaternion Quaternion::FromEuler(float x, float y, float z)
{
    float cx = cos(x * 0.5);
    float cy = cos(y * 0.5);
    float cz = cos(z * 0.5);
    float sx = sin(x * 0.5);
    float sy = sin(y * 0.5);
    float sz = sin(z * 0.5);
    Quaternion q;
    q.x = cx * sy * sz + cy * cz * sx;
    q.y = cx * cz * sy - cy * sx * sz;
    q.z = cx * cy * sz - cz * sx * sy;
    q.w = sx * sy * sz + cx * cy * cz;
    return q;
}

Quaternion Quaternion::FromToRotation(Vector3 fromVector, Vector3 toVector)
{
    float dot = Vector3::Dot(fromVector, toVector);
    float k = sqrt(Vector3::SqrMagnitude(fromVector) *
        Vector3::SqrMagnitude(toVector));
    if (fabs(dot / k + 1) < 0.00001)
    {
        Vector3 ortho = Vector3::Orthogonal(fromVector);
        return Quaternion(Vector3::Normalized(ortho), 0);
    }
    Vector3 cross = Vector3::Cross(fromVector, toVector);
    return Normalized(Quaternion(cross, dot + k));
}

Quaternion Quaternion::Inverse(Quaternion rotation)
{
    float n = Norm(rotation);
    return Conjugate(rotation) / (n * n);
}

Quaternion Quaternion::Lerp(Quaternion a, Quaternion b, float t)
{
    if (t < 0) return Normalized(a);
    else if (t > 1) return Normalized(b);
    return LerpUnclamped(a, b, t);
}

Quaternion Quaternion::LerpUnclamped(Quaternion a, Quaternion b, float t)
{
    Quaternion quaternion;
    if (Dot(a, b) >= 0)
        quaternion = a * (1 - t) + b * t;
    else
        quaternion = a * (1 - t) - b * t;
    return Normalized(quaternion);
}

Quaternion Quaternion::LookRotation(Vector3 forward)
{
    return LookRotation(forward, Vector3(0, 1, 0));
}

Quaternion Quaternion::LookRotation(Vector3 forward, Vector3 upwards)
{
    // Normalize inputs
    forward = Vector3::Normalized(forward);
    upwards = Vector3::Normalized(upwards);
    // Don't allow zero vectors
    if (Vector3::SqrMagnitude(forward) < SMALL_float || Vector3::SqrMagnitude(upwards) < SMALL_float)
        return Quaternion::Identity();
    // Handle alignment with up direction
    if (1 - fabs(Vector3::Dot(forward, upwards)) < SMALL_float)
        return FromToRotation(Vector3::Forward(), forward);
    // Get orthogonal vectors
    Vector3 right = Vector3::Normalized(Vector3::Cross(upwards, forward));
    upwards = Vector3::Cross(forward, right);
    // Calculate rotation
    Quaternion quaternion;
    float radicand = right.x + upwards.y + forward.z;
    if (radicand > 0)
    {
        quaternion.w = sqrt(1.0 + radicand) * 0.5;
        float recip = 1.0 / (4.0 * quaternion.w);
        quaternion.x = (upwards.z - forward.y) * recip;
        quaternion.y = (forward.x - right.x) * recip;
        quaternion.z = (right.y - upwards.x) * recip;
    }
    else if (right.x >= upwards.y && right.x >= forward.z)
    {
        quaternion.x = sqrt(1.0 + right.x - upwards.y - forward.z) * 0.5;
        float recip = 1.0 / (4.0 * quaternion.x);
        quaternion.w = (upwards.z - forward.y) * recip;
        quaternion.z = (forward.x + right.z) * recip;
        quaternion.y = (right.y + upwards.x) * recip;
    }
    else if (upwards.y > forward.z)
    {
        quaternion.y = sqrt(1.0 - right.x + upwards.y - forward.z) * 0.5;
        float recip = 1.0 / (4.0 * quaternion.y);
        quaternion.z = (upwards.z + forward.y) * recip;
        quaternion.w = (forward.x - right.z) * recip;
        quaternion.x = (right.y + upwards.x) * recip;
    }
    else
    {
        quaternion.z = sqrt(1.0 - right.x - upwards.y + forward.z) * 0.5;
        float recip = 1.0 / (4.0 * quaternion.z);
        quaternion.y = (upwards.z + forward.y) * recip;
        quaternion.x = (forward.x + right.z) * recip;
        quaternion.w = (right.y - upwards.x) * recip;
    }
    return quaternion;
}

float Quaternion::Norm(Quaternion rotation)
{
    return sqrt(rotation.x * rotation.x +
        rotation.y * rotation.y +
        rotation.z * rotation.z +
        rotation.w * rotation.w);
}

Quaternion Quaternion::Normalized(Quaternion rotation)
{
    return rotation / Norm(rotation);
}

Quaternion Quaternion::RotateTowards(Quaternion from, Quaternion to, float maxRadiansDelta)
{
    float angle = Quaternion::Angle(from, to);
    if (angle == 0)
        return to;
    maxRadiansDelta = fmax(maxRadiansDelta, angle - M_PI);
    float t = fmin(1, maxRadiansDelta / angle);
    return Quaternion::SlerpUnclamped(from, to, t);
}

Quaternion Quaternion::Slerp(Quaternion a, Quaternion b, float t)
{
    if (t < 0) return Normalized(a);
    else if (t > 1) return Normalized(b);
    return SlerpUnclamped(a, b, t);
}

Quaternion Quaternion::SlerpUnclamped(Quaternion a, Quaternion b, float t)
{
    float n1;
    float n2;
    float n3 = Dot(a, b);
    bool flag = false;
    if (n3 < 0)
    {
        flag = true;
        n3 = -n3;
    }
    if (n3 > 0.999999)
    {
        n2 = 1 - t;
        n1 = flag ? -t : t;
    }
    else
    {
        float n4 = acos(n3);
        float n5 = 1 / sin(n4);
        n2 = sin((1 - t) * n4) * n5;
        n1 = flag ? -sin(t * n4) * n5 : sin(t * n4) * n5;
    }
    Quaternion quaternion;
    quaternion.x = (n2 * a.x) + (n1 * b.x);
    quaternion.y = (n2 * a.y) + (n1 * b.y);
    quaternion.z = (n2 * a.z) + (n1 * b.z);
    quaternion.w = (n2 * a.w) + (n1 * b.w);
    return Normalized(quaternion);
}

void Quaternion::ToAngleAxis(Quaternion rotation, float& angle, Vector3& axis)
{
    if (rotation.w > 1)
        rotation = Normalized(rotation);
    angle = 2 * acos(rotation.w);
    float s = sqrt(1 - rotation.w * rotation.w);
    if (s < 0.00001) {
        axis.x = 1;
        axis.y = 0;
        axis.z = 0;
    }
    else {
        axis.x = rotation.x / s;
        axis.y = rotation.y / s;
        axis.z = rotation.z / s;
    }
}

Vector3 Quaternion::ToEuler(Quaternion rotation)
{
    float sqw = rotation.w * rotation.w;
    float sqx = rotation.x * rotation.x;
    float sqy = rotation.y * rotation.y;
    float sqz = rotation.z * rotation.z;
    // If normalized is one, otherwise is correction factor
    float unit = sqx + sqy + sqz + sqw;
    float test = rotation.x * rotation.w - rotation.y * rotation.z;
    Vector3 v;
    // Singularity at north pole
    if (test > 0.4995f * unit)
    {
        v.y = 2 * atan2(rotation.y, rotation.x);
        v.x = M_PI_2;
        v.z = 0;
        return v;
    }
    // Singularity at south pole
    if (test < -0.4995f * unit)
    {
        v.y = -2 * atan2(rotation.y, rotation.x);
        v.x = -M_PI_2;
        v.z = 0;
        return v;
    }
    // Yaw
    v.y = atan2(2 * rotation.w * rotation.y + 2 * rotation.z * rotation.x,
        1 - 2 * (rotation.x * rotation.x + rotation.y * rotation.y));
    // Pitch
    v.x = asin(2 * (rotation.w * rotation.x - rotation.y * rotation.z));
    // Roll
    v.z = atan2(2 * rotation.w * rotation.z + 2 * rotation.x * rotation.y,
        1 - 2 * (rotation.z * rotation.z + rotation.x * rotation.x));
    return v;
}

struct Quaternion& Quaternion::operator+=(const float rhs)
{
    x += rhs;
    y += rhs;
    z += rhs;
    w += rhs;
    return *this;
}

struct Quaternion& Quaternion::operator-=(const float rhs)
{
    x -= rhs;
    y -= rhs;
    z -= rhs;
    w -= rhs;
    return *this;
}

struct Quaternion& Quaternion::operator*=(const float rhs)
{
    x *= rhs;
    y *= rhs;
    z *= rhs;
    w *= rhs;
    return *this;
}

struct Quaternion& Quaternion::operator/=(const float rhs)
{
    x /= rhs;
    y /= rhs;
    z /= rhs;
    w /= rhs;
    return *this;
}

struct Quaternion& Quaternion::operator+=(const Quaternion rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
    return *this;
}

struct Quaternion& Quaternion::operator-=(const Quaternion rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;
    return *this;
}

struct Quaternion& Quaternion::operator*=(const Quaternion rhs)
{
    Quaternion q;
    q.w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
    q.x = x * rhs.w + w * rhs.x + y * rhs.z - z * rhs.y;
    q.y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
    q.z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;
    *this = q;
    return *this;
}

Quaternion operator-(Quaternion rhs) { return rhs * -1; }
Quaternion operator+(Quaternion lhs, const float rhs) { return lhs += rhs; }
Quaternion operator-(Quaternion lhs, const float rhs) { return lhs -= rhs; }
Quaternion operator*(Quaternion lhs, const float rhs) { return lhs *= rhs; }
Quaternion operator/(Quaternion lhs, const float rhs) { return lhs /= rhs; }
Quaternion operator+(const float lhs, Quaternion rhs) { return rhs += lhs; }
Quaternion operator-(const float lhs, Quaternion rhs) { return rhs -= lhs; }
Quaternion operator*(const float lhs, Quaternion rhs) { return rhs *= lhs; }
Quaternion operator/(const float lhs, Quaternion rhs) { return rhs /= lhs; }
Quaternion operator+(Quaternion lhs, const Quaternion rhs)
{
    return lhs += rhs;
}
Quaternion operator-(Quaternion lhs, const Quaternion rhs)
{
    return lhs -= rhs;
}
Quaternion operator*(Quaternion lhs, const Quaternion rhs)
{
    return lhs *= rhs;
}

Vector3 operator*(Quaternion lhs, const Vector3 rhs)
{
    Vector3 u = Vector3(lhs.x, lhs.y, lhs.z);
    float s = lhs.w;
    return u * (Vector3::Dot(u, rhs) * 2)
        + rhs * (s * s - Vector3::Dot(u, u))
        + Vector3::Cross(u, rhs) * (2.0 * s);
}

bool operator==(const Quaternion lhs, const Quaternion rhs)
{
    return lhs.x == rhs.x &&
        lhs.y == rhs.y &&
        lhs.z == rhs.z &&
        lhs.w == rhs.w;
}

bool operator!=(const Quaternion lhs, const Quaternion rhs)
{
    return !(lhs == rhs);
}

struct MathZ {
    static float Abs(float value) {
        return value < 0.0f ? -value : value;
    }

    static Vector2 Abs(const Vector2& v) {
        return Vector2(Abs(v.x), Abs(v.y));
    }
};

struct Vector4
{
    float x, y, z, w;

    Vector4() : x(0), y(0), z(0), w(0) {}

    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    void print() const {
        std::cout << "Vector4(" << x << ", " << y << ", " << z << ", " << w << ")\n";
    }
};

struct Matrix4x4 {
    float m00, m01, m02, m03;
    float m10, m11, m12, m13;
    float m20, m21, m22, m23;
    float m30, m31, m32, m33;

    Matrix4x4()
        : m00(0), m01(0), m02(0), m03(0),
        m10(0), m11(0), m12(0), m13(0),
        m20(0), m21(0), m22(0), m23(0),
        m30(0), m31(0), m32(0), m33(0)
    {
    }
};

static inline Matrix4x4 QuaternionToMatrix(const Quaternion& q) {
    Matrix4x4 m;
    float xx = q.x * q.x, yy = q.y * q.y, zz = q.z * q.z;
    float xy = q.x * q.y, xz = q.x * q.z, yz = q.y * q.z;
    float wx = q.w * q.x, wy = q.w * q.y, wz = q.w * q.z;

    m.m00 = 1 - 2 * (yy + zz); m.m01 = 2 * (xy - wz);     m.m02 = 2 * (xz + wy);     m.m03 = 0;
    m.m10 = 2 * (xy + wz);     m.m11 = 1 - 2 * (xx + zz); m.m12 = 2 * (yz - wx);     m.m13 = 0;
    m.m20 = 2 * (xz - wy);     m.m21 = 2 * (yz + wx);     m.m22 = 1 - 2 * (xx + yy); m.m23 = 0;
    m.m30 = 0;                 m.m31 = 0;                 m.m32 = 0;                 m.m33 = 1;

    return m;
}

float GetCameraYaw(const Matrix4x4& matrix)
{
    float m20 = matrix.m20; // fila 3, columna 1
    float m22 = matrix.m22; // fila 3, columna 3
    return atan2f(m20, m22); // Devuelve yaw en radianes
}

// Esta funci n convierte las coordenadas del mundo a las coordenadas de pantalla
Vector2 WorldToScreen(const Matrix4x4& viewMatrix, const Vector3& pos, int width, int height) {
    Vector2 result(-1, -1);

    float v9 = (pos.x * viewMatrix.m00) + (pos.y * viewMatrix.m10) + (pos.z * viewMatrix.m20) + viewMatrix.m30;
    float v10 = (pos.x * viewMatrix.m01) + (pos.y * viewMatrix.m11) + (pos.z * viewMatrix.m21) + viewMatrix.m31;
    float v12 = (pos.x * viewMatrix.m03) + (pos.y * viewMatrix.m13) + (pos.z * viewMatrix.m23) + viewMatrix.m33;

    if (v12 >= 0.001f) {
        float v13 = static_cast<float>(width) / 2.0f;
        float v14 = static_cast<float>(height) / 2.0f;

        result.x = v13 + (v13 * v9) / v12;
        result.y = v14 - (v14 * v10) / v12;
    }

    return result;
}

// Esta funci n convierte las coordenadas del mundo a las coordenadas de pantalla
ImVec2 WorldToScreenImVec2(const Matrix4x4& viewMatrix, const Vector3& pos, int width, int height) {
    Vector2 result = WorldToScreen(viewMatrix, pos, width, height);
    return ImVec2(result.x, result.y);  // Convertir Vector2 a ImVec2
}

class AimBZ {
private:
    static constexpr float SMALL_FLOAT = 0.0000000001f;

public:
    static Quaternion GetRotationToLocation(const Vector3& targetLocation,
        float y_bias, const Vector3& myLoc) {
        Vector3 forwards = targetLocation + Vector3(0, y_bias, 0) - myLoc;
        Vector3 upwards(0, 1, 0);

        return LookRotation(forwards, upwards);
    }

private:
    static Quaternion LookRotation(Vector3 forwards, Vector3 upwards) {
        forwards = Normalized(forwards);
        upwards = Normalized(upwards);

        if (SqrMagnitude(forwards) < SMALL_FLOAT || SqrMagnitude(upwards) < SMALL_FLOAT)
            return Quaternion::Identity();

        if (1 - std::abs(Vector3::Dot(forwards, upwards)) < SMALL_FLOAT)
            return FromToRotation(forwards, upwards);

        Vector3 right = Normalized(Vector3::Cross(upwards, forwards));
        upwards = Vector3::Cross(forwards, right);

        Quaternion quaternion;
        float radicand = right.x + upwards.y + forwards.z;

        if (radicand > 0) {
            quaternion.w = std::sqrt(1.0f + radicand) * 0.5f;
            float recip = 1.0f / (4.0f * quaternion.w);
            quaternion.x = (upwards.z - forwards.y) * recip;
            quaternion.y = (forwards.x - right.z) * recip;
            quaternion.z = (right.y - upwards.x) * recip;
        }
        else if (right.x >= upwards.y && right.x >= forwards.z) {
            quaternion.x = std::sqrt(1.0f + right.x - upwards.y - forwards.z) * 0.5f;
            float recip = 1.0f / (4.0f * quaternion.x);
            quaternion.w = (upwards.z - forwards.y) * recip;
            quaternion.z = (forwards.x + right.z) * recip;
            quaternion.y = (right.y + upwards.x) * recip;
        }
        else if (upwards.y > forwards.z) {
            quaternion.y = std::sqrt(1.0f - right.x + upwards.y - forwards.z) * 0.5f;
            float recip = 1.0f / (4.0f * quaternion.y);
            quaternion.z = (upwards.z + forwards.y) * recip;
            quaternion.w = (forwards.x - right.z) * recip;
            quaternion.x = (right.y + upwards.x) * recip;
        }
        else {
            quaternion.z = std::sqrt(1.0f - right.x - upwards.y + forwards.z) * 0.5f;
            float recip = 1.0f / (4.0f * quaternion.z);
            quaternion.y = (upwards.z + forwards.y) * recip;
            quaternion.x = (forwards.x + right.z) * recip;
            quaternion.w = (right.y - upwards.x) * recip;
        }
        return quaternion;
    }

    static Quaternion FromToRotation(const Vector3& forwards, const Vector3& upwards) {
        float dot = Vector3::Dot(forwards, upwards);
        float k = std::sqrt(SqrMagnitude(forwards) * SqrMagnitude(upwards));

        if (std::abs(dot / k + 1) < 0.00001) {
            Vector3 ortho = Orthogonal(forwards);
            return Quaternion(Normalized(ortho).x, Normalized(ortho).y, Normalized(ortho).z, 0);
        }
        Vector3 cross = Vector3::Cross(forwards, upwards);
        return Normalized(Quaternion(cross.x, cross.y, cross.z, dot + k));
    }

    static Quaternion Normalized(const Quaternion& rotation) {
        float norm = Norm(rotation);
        return Quaternion(rotation.x / norm, rotation.y / norm, rotation.z / norm, rotation.w / norm);
    }

    static float Norm(const Quaternion& rotation) {
        return std::sqrt(rotation.x * rotation.x +
            rotation.y * rotation.y +
            rotation.z * rotation.z +
            rotation.w * rotation.w);
    }

    static Vector3 Orthogonal(const Vector3& v) {
        return v.z < v.x ? Vector3(v.y, -v.x, 0) : Vector3(0, -v.z, v.y);
    }

    static Vector3 Normalized(const Vector3& vector) {
        float mag = Magnitude(vector);

        if (mag == 0)
            return Vector3::Zero();
        return vector / mag;
    }

    static float Magnitude(const Vector3& vector) {
        return std::sqrt(SqrMagnitude(vector));
    }

    static float SqrMagnitude(const Vector3& v) {
        return v.x * v.x + v.y * v.y + v.z * v.z;
    }
};

class AimBZv2 {
private:
    static constexpr float SMALL_FLOAT = 0.0000000001f;

public:
    static Quaternion GetRotationToLocation(const Vector3& targetLocation,
        float y_bias, const Vector3& myLoc) {
        Vector3 direction = targetLocation - myLoc;
        direction.y += y_bias;  // Aplica el sesgo en Y si es necesario
        direction = Normalized(direction);

        Vector3 upwards(0, 1, 0);

        return LookRotation(direction, upwards);
    }

    // Mueve la funci n Normalized a la secci n public para que sea accesible
    static Vector3 Normalized(const Vector3& vector) {
        float mag = Magnitude(vector);
        return (mag == 0) ? Vector3::Zero() : vector / mag;
    }

private:
    static Quaternion LookRotation(Vector3 direction, Vector3 upwards) {
        direction = Normalized(direction);
        upwards = Normalized(upwards);

        if (SqrMagnitude(direction) < SMALL_FLOAT || SqrMagnitude(upwards) < SMALL_FLOAT)
            return Quaternion::Identity();

        if (1 - std::abs(Vector3::Dot(direction, upwards)) < SMALL_FLOAT)
            return FromToRotation(direction, upwards);

        Vector3 right = Normalized(Vector3::Cross(upwards, direction));
        upwards = Vector3::Cross(direction, right);

        Quaternion quaternion;
        float radicand = right.x + upwards.y + direction.z;

        if (radicand > 0) {
            quaternion.w = std::sqrt(1.0f + radicand) * 0.5f;
            float recip = 1.0f / (4.0f * quaternion.w);
            quaternion.x = (upwards.z - direction.y) * recip;
            quaternion.y = (direction.x - right.z) * recip;
            quaternion.z = (right.y - upwards.x) * recip;
        }
        else if (right.x >= upwards.y && right.x >= direction.z) {
            quaternion.x = std::sqrt(1.0f + right.x - upwards.y - direction.z) * 0.5f;
            float recip = 1.0f / (4.0f * quaternion.x);
            quaternion.w = (upwards.z - direction.y) * recip;
            quaternion.z = (direction.x + right.z) * recip;
            quaternion.y = (right.y + upwards.x) * recip;
        }
        else if (upwards.y > direction.z) {
            quaternion.y = std::sqrt(1.0f - right.x + upwards.y - direction.z) * 0.5f;
            float recip = 1.0f / (4.0f * quaternion.y);
            quaternion.z = (upwards.z + direction.y) * recip;
            quaternion.w = (direction.x - right.z) * recip;
            quaternion.x = (right.y + upwards.x) * recip;
        }
        else {
            quaternion.z = std::sqrt(1.0f - right.x - upwards.y + direction.z) * 0.5f;
            float recip = 1.0f / (4.0f * quaternion.z);
            quaternion.y = (upwards.z + direction.y) * recip;
            quaternion.x = (direction.x + right.z) * recip;
            quaternion.w = (right.y - upwards.x) * recip;
        }
        return quaternion;
    }

    static Quaternion FromToRotation(const Vector3& from, const Vector3& to) {
        float dot = Vector3::Dot(from, to);
        float k = std::sqrt(SqrMagnitude(from) * SqrMagnitude(to));

        if (std::abs(dot / k + 1) < SMALL_FLOAT) {
            Vector3 ortho = Orthogonal(from);
            return Quaternion(Normalized(ortho).x, Normalized(ortho).y, Normalized(ortho).z, 0);
        }
        Vector3 cross = Vector3::Cross(from, to);
        return Normalized(Quaternion(cross.x, cross.y, cross.z, dot + k));
    }

    static Quaternion Normalized(const Quaternion& rotation) {
        float norm = Norm(rotation);
        return Quaternion(rotation.x / norm, rotation.y / norm, rotation.z / norm, rotation.w / norm);
    }

    static float Norm(const Quaternion& rotation) {
        return std::sqrt(rotation.x * rotation.x +
            rotation.y * rotation.y +
            rotation.z * rotation.z +
            rotation.w * rotation.w);
    }

    static Vector3 Orthogonal(const Vector3& v) {
        return v.z < v.x ? Vector3(v.y, -v.x, 0) : Vector3(0, -v.z, v.y);
    }

    static float Magnitude(const Vector3& vector) {
        return std::sqrt(SqrMagnitude(vector));
    }

    static float SqrMagnitude(const Vector3& v) {
        return v.x * v.x + v.y * v.y + v.z * v.z;
    }
};

// Definición del enum para los estados de la partida
enum MatchStatusEnum {
    MATCH_NOT_STARTED = 0,
    MATCH_RUNNING = 1,
    MATCH_PAUSED = 2,
    MATCH_ENDED = 3
};
