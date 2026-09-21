#!/usr/bin/env python3

import sys, re
n, tb, tc, td, tr, req, cd = map(int, sys.argv[1:8])
TOL = 5
def fail(m): print("FAIL:", m); sys.exit(1)
ev = []
for l in sys.stdin:
    m = re.fullmatch(r"(\d+) (\d+) (.+)\n?", l)
    if not m: fail("garbled: " + l)
    ev.append((int(m[1]), int(m[2]), m[3]))
for a, b in zip(ev, ev[1:]):
    if b[0] < a[0]: fail("time went backwards")
takes = {i: 0 for i in range(1, n + 1)}
starts = {i: [] for i in takes}; ends = {i: {} for i in takes}
last_state = {}
for k, (t, i, msg) in enumerate(ev):
    if msg == "burned out":
        if k != len(ev) - 1: fail("output after burnout")
        ref = starts[i][-1] if starts[i] else 0
        if not (ref + tb - TOL <= t <= ref + tb + 10): fail(f"burnout timing {t} vs {ref + tb}")
    elif msg == "has taken a dongle": takes[i] += 1
    elif msg == "is compiling":
        if takes[i] < 2: fail(f"coder {i} compiled with {takes[i]} dongles")
        takes[i] = 0
        if starts[i] and t - starts[i][-1] > tb + TOL: fail(f"coder {i} gap too long")
        starts[i].append(t)
    elif msg == "is debugging":
        if abs(t - starts[i][-1] - tc) > TOL: fail(f"coder {i} compile duration")
        ends[i][starts[i][-1]] = t
        last_state[i] = t
    elif msg == "is refactoring":
        if abs(t - last_state[i] - td) > TOL: fail(f"coder {i} debug duration")
    else: fail("unknown message: " + msg)
def windows(i): return [(s, ends[i].get(s, s + tc)) for s in starts[i]]
if n > 1:
    for i in range(1, n + 1):
        j = i % n + 1
        w = sorted(windows(i) + windows(j))
        for (s1, e1), (s2, e2) in zip(w, w[1:]):
            if s2 < e1 + cd - TOL: fail(f"neighbours {i},{j} overlap or ignore cooldown at {s2}")
if ev and ev[-1][2] != "burned out":
    for i in starts:
        if len(starts[i]) < req: fail(f"coder {i} only compiled {len(starts[i])} times")
print("OK")
