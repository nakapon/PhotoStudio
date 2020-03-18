#pragma once

// Check basic type size
#ifdef PLATFORM_TYPES

static_assert(sizeof(PlatformTypes::Int8)  == 1, "Int8 size must be 1 byte.");
static_assert(sizeof(PlatformTypes::Int16) == 2, "Int16 size must be 2 bytes.");
static_assert(sizeof(PlatformTypes::Int32) == 4, "Int32 size must be 4 bytes.");
static_assert(sizeof(PlatformTypes::Int64) == 8, "Int64 size must be 8 bytes.");

static_assert(sizeof(PlatformTypes::UInt8)  == 1, "UInt8 size must be 1 byte.");
static_assert(sizeof(PlatformTypes::UInt16) == 2, "UInt16 size must be 2 bytes.");
static_assert(sizeof(PlatformTypes::UInt32) == 4, "UInt32 size must be 4 bytes.");
static_assert(sizeof(PlatformTypes::UInt64) == 8, "UInt64 size must be 8 bytes.");

static_assert(sizeof(PlatformTypes::Float32) == 4, "Float32 size must be 4 bytes.");
static_assert(sizeof(PlatformTypes::Float64) == 8, "Float64 size must be 8 bytes.");

#endif

static_assert(sizeof(BYTE) == 1, "BYTE size must be 1 byte.");
static_assert(sizeof(WORD) == 2, "WORD size must be 2 bytes.");
static_assert(sizeof(DWORD) == 4, "DWORD size must be 4 bytes.");
static_assert(sizeof(QWORD) == 8, "QWORD size must be 8 bytes.");
