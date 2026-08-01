#!/usr/bin/env bash
# Regression stress tests for the codebook.
#
# Each stress/<section>/<file>.cpp holds ONLY test logic: an `int stress()` that
# returns nonzero on failure and prints "<name> PASS <n>" on success. run.sh
# assembles it with the contest-template shim + the matching src/ listing (its
# demo main() stripped) and runs it. Zero duplication of algorithm code: the
# harness always tests the CURRENT source.
#
# Usage: stress/run.sh [name-substring]   (no arg = run all)
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
INC=/tmp/inc
mkdir -p "$INC/bits"
cat > "$INC/bits/stdc++.h" <<'H'
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <numeric>
#include <queue>
#include <deque>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <cmath>
#include <climits>
#include <cassert>
#include <functional>
#include <tuple>
#include <random>
#include <chrono>
#include <complex>
#include <iomanip>
H
HDR='#include <bits/stdc++.h>
using namespace std;
const int INF = 0x3f3f3f3f;
const long long LINF = 0x3f3f3f3f3f3f3f3fLL;
const long long MOD = 1e9 + 7;
int __lg_(int x){return 31 - __builtin_clz(x);}
#define __lg __lg_
'
filter="${1:-}"
pass=0; fail=0; skip=0
for h in $(find "$ROOT/stress" -name '*.cpp' | sort); do
  rel=${h#"$ROOT"/stress/}
  [ -n "$filter" ] && case "$rel" in *"$filter"*) ;; *) continue;; esac
  src="$ROOT/src/$rel"
  if [ ! -f "$src" ]; then echo "MISSING-SRC $rel"; fail=$((fail+1)); continue; fi
  compiler=clang++
  if [[ "$rel" == *"13-order-statistics-tree.cpp" ]]; then
    if command -v g++-15 >/dev/null; then
      compiler=g++-15
    else
      echo "SKIP  $rel   GNU g++-15 is required for pb_ds"
      skip=$((skip+1))
      continue
    fi
  fi
  tmp=$(mktemp /tmp/stress_XXXXXX).cpp
  printf '%s' "$HDR" > "$tmp"
  awk '/^int main\(/{exit} {print}' "$src" >> "$tmp"
  cat "$h" >> "$tmp"
  printf '\nint main(){ return stress(); }\n' >> "$tmp"
  bin=$(mktemp /tmp/stressb_XXXXXX)
  if "$compiler" -std=c++17 -I"$INC" "$tmp" -o "$bin" 2>/tmp/stress_err; then
    if out=$("$bin" 2>&1); then echo "PASS  $rel   $out"; pass=$((pass+1));
    else echo "FAIL(run)     $rel   $out"; fail=$((fail+1)); fi
  else echo "FAIL(compile) $rel"; head -4 /tmp/stress_err; fail=$((fail+1)); fi
  rm -f "$tmp" "$bin"
done
echo "=== $pass passed, $fail failed, $skip skipped ==="
[ $fail -eq 0 ]
