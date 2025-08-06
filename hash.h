#pragma once

#include "type.h"

u8 hash1(const u8* str);

extern const u8 hash_map1[226][27];

u16 hash2(const u8* str, u16 len);

extern const u8 hash_map2[4452][3];