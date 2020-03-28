#include <Platform.h>

#include <Math/Vec2.h>
#include <Math/Vec3.h>

// Vec2
template<>
const Vec2i Vec2i::One(1, 1);

template<>
const Vec2i Vec2i::Zero(0, 0);

template<>
const Vec2i Vec2i::UnitX(1, 0);

template<>
const Vec2i Vec2i::UnitY(0, 1);

template<>
const Vec2f Vec2f::One(1.0f, 1.0f);

template<>
const Vec2f Vec2f::Zero(0.0, 0.0);

template<>
const Vec2f Vec2f::UnitX(1.0f, 0.0f);

template<>
const Vec2f Vec2f::UnitY(0.0f, 1.0f);

template<>
const Vec2d Vec2d::One(1.0, 1.0);

template<>
const Vec2d Vec2d::Zero(0.0, 0.0);

template<>
const Vec2d Vec2d::UnitX(1.0, 0.0);

template<>
const Vec2d Vec2d::UnitY(0.0, 1.0);

// Vec3
template<>
const Vec3i Vec3i::One(1, 1, 1);

template<>
const Vec3i Vec3i::Zero(0, 0, 0);

template<>
const Vec3i Vec3i::UnitX(1, 0, 0);

template<>
const Vec3i Vec3i::UnitY(0, 1, 0);

template<>
const Vec3i Vec3i::UnitZ(0, 0, 1);

template<>
const Vec3f Vec3f::One(1.0f, 1.0f, 1.0f);

template<>
const Vec3f Vec3f::Zero(0.0f, 0.0f, 0.0f);

template<>
const Vec3f Vec3f::UnitX(1.0f, 0.0f, 0.0f);

template<>
const Vec3f Vec3f::UnitY(0.0f, 1.0f, 0.0f);

template<>
const Vec3f Vec3f::UnitZ(0.0f, 0.0f, 1.0f);

template<>
const Vec3d Vec3d::One(1.0, 1.0, 1.0);

template<>
const Vec3d Vec3d::Zero(0.0, 0.0, 0.0);

template<>
const Vec3d Vec3d::UnitX(1.0, 0.0, 0.0);

template<>
const Vec3d Vec3d::UnitY(0.0, 1.0, 0.0);

template<>
const Vec3d Vec3d::UnitZ(0.0, 0.0, 1.0);
