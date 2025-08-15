#pragma once

#include "type.h"

#define loop while (1)
#define unless(p) if (!(p))
#define until(p) while (!(p))
#define skip(p) if (p) continue

#define accept(p) if (p) break
#define reject(p) if (p) return
#define check(p) if (!(p)) break
#define guard(p) if (!(p)) return

#define cpy8(dest, src) *(u64*)(dest) = *(u64*)(src)

#define find(ch, it, step) { char c; do { c = *it; it += step; } until (c == ch); }
#define cpy(dest, src, ch) { char c; until ((c = *src++) == ch) *dest++ = c; }
#define cpylen(dest, src, ch, len) { char* start = dest; cpy(dest, src, ch); len = dest - start; }