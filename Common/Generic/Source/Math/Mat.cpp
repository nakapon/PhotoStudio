#include <Platform.h>

#include <Math/Mat22.h>
#include <Math/Mat33.h>

template<>
const Mat22f Mat22f::Identity(Mat22f::IDENTITY);

template<>
const Mat22d Mat22d::Identity(Mat22d::IDENTITY);

template<>
const Mat33f Mat33f::Identity(Mat33f::IDENTITY);

template<>
const Mat33d Mat33d::Identity(Mat33d::IDENTITY);
