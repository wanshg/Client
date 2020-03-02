#include "Vectors.h"

const Vector3 Vector3::ZERO(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::ONE = Vector3(1.0f, 1.0f, 1.0f);

const Vector3 Vector3::Back = Vector3(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::Forward = Vector3(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::Right = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Left = Vector3(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Up = Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Down = Vector3(0.0f, -1.0f, 0.0f);

const Vector3 Vector3::AXES[3];

const Vector2 Vector2::ZERO = Vector2(0.0f, 0.0f);
const Vector2 Vector2::ONE = Vector2(1.0f, 1.0f);

const Vector4 Vector4::ZERO = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
