"""Two fixed points of the tutorial's example federation, computed by Kleene iteration.

0. The tag addition used for path delays, checked on cases with microsteps.
1. The min-delay matrix of S -> A <-> B -> C (after 10 ms on B -> C), as the least fixed point
   of F(D) = min(E, D (x) E), iterated from the all-FOREVER matrix.
2. The status of the ports of the feedback pair A <-> B at one tag, for two constructive
   runs and for a causality loop.
Plain Python 3, no Lingua Franca needed. This is a teaching model, not the runtime:
delays are finite, non-negative (milliseconds, microstep) pairs, INF stands for FOREVER,
and the C runtime's overflow handling, NEVER and other sentinels are not modelled.
"""
INF = None  # FOREVER: no path


def tag_add(a, b):
    """a (+) b for delays, following lf_tag_add in reactor-c (core/tag.c): if b adds time,
    the microstep of a is dropped and the microstep of b is kept; otherwise microsteps add."""
    if a is INF or b is INF:
        return INF
    return (a[0] + b[0], b[1]) if b[0] > 0 else (a[0], a[1] + b[1])


def tag_min(a, b):
    if a is INF:
        return b
    if b is INF:
        return a
    return min(a, b)  # tuples compare by time, then microstep


def show(name, m, nodes):
    print(name)
    print("        " + "".join(f"-> {n:<9}" for n in nodes))
    for i in nodes:
        cells = ["FOREVER" if m[i][j] is INF else "(0,0)" if m[i][j] == (0, 0) else f"({m[i][j][0]}ms,{m[i][j][1]})"
                 for j in nodes]
        print(f"from {i}  " + "".join(f"{c:<12}" for c in cells))


# ---------------------------------------------------------------- 0. tag addition
print("tag addition, in path order (earlier part on the left):")
cases = [((2, 4), (5, 1), (7, 1)),   # b adds time: a's microstep dropped, b's kept
         ((5, 0), (0, 1), (5, 1)),   # after 5 ms, then after 0
         ((0, 1), (5, 0), (5, 0))]   # after 0, then after 5 ms: the microstep does not survive
for a, b, want in cases:
    got = tag_add(a, b)
    print(f"  {a} (+) {b} = {got}" + ("" if got == want else f"   WRONG, expected {want}"))
a, b, c = (0, 0), (5, 0), (0, 1)
left, right = tag_add(tag_add(a, b), c), tag_add(a, tag_add(b, c))
print(f"  grouping: ({a} (+) {b}) (+) {c} = {left}, {a} (+) ({b} (+) {c}) = {right}:",
      "same" if left == right else "DIFFERENT")
small = [(t, m) for t in range(3) for m in range(3)]
assoc = all(tag_add(tag_add(x, y), z) == tag_add(x, tag_add(y, z)) for x in small for y in small for z in small)
print("  associative on all delays with time and microstep in 0..2:", assoc, "\n")

# ---------------------------------------------------------------- 1. the min-delay closure
nodes = ["S", "A", "B", "C"]
edges = {("S", "A"): (0, 0), ("A", "B"): (0, 0), ("B", "A"): (0, 0), ("B", "C"): (10, 0)}
E = {i: {j: edges.get((i, j), INF) for j in nodes} for i in nodes}   # direct connections
# F(D) = min(E, D (x) E): a path to j is one connection, or a path to some u followed by u -> j.
# E must be part of F: without it, the all-FOREVER matrix would already be a fixed point.
D = {i: {j: INF for j in nodes} for i in nodes}                       # bottom: no path known
k = 0
while True:
    new = {i: {j: E[i][j] for j in nodes} for i in nodes}
    for i in nodes:
        for (u, j), w in edges.items():
            new[i][j] = tag_min(new[i][j], tag_add(D[i][u], w))
    if new == D:
        print(f"iteration {k + 1}: nothing changes, so the matrix is the least fixed point of F\n")
        break
    D, k = new, k + 1
    show(f"iteration {k}: " + ("direct connections" if k == 1 else f"paths of up to {k} connections"), D, nodes)

# ---------------------------------------------------------------- 2. ports at one tag
# A port's status is unknown, present or absent; it only moves from unknown to one of the other two.
# A reaction is decided once its triggers are resolved (and, inside A, a1 is decided before a2):
# if some trigger is present it runs; if all are absent it is not triggered and does not run.
# A port is resolved once no reaction that may write it is still undecided.
# A: a1 (timer) may write p; a2 is triggered by q (and, in the loop variant, may also write p).
# B: b1 is triggered by p and writes q.  p = A.out -> B.in, q = B.back -> A.back, no after on either.


def ports_at_one_tag(a1_writes_p=True, a2_may_write_p=False):
    status = {"timer": "present", "p": "unknown", "q": "unknown"}
    may_write = {"p": ["a1"] + (["a2"] if a2_may_write_p else []), "q": ["b1"]}
    triggers = {"a1": ["timer"], "b1": ["p"], "a2": ["q"]}
    writes = {"a1": ["p"] if a1_writes_p else [], "b1": ["q"], "a2": []}
    before = {"a2": "a1"}  # declared order inside reactor A
    decided, written, rnd = {}, set(), 0
    while True:
        rnd += 1
        ran, skipped = [], []
        for r in ("a1", "b1", "a2"):
            if r in decided or any(status[t] == "unknown" for t in triggers[r]):
                continue
            if before.get(r) and before[r] not in decided:
                continue
            if any(status[t] == "present" for t in triggers[r]):
                decided[r] = "ran"; ran.append(r); written.update(writes[r])
            else:
                decided[r] = "not triggered"; skipped.append(r)
        changed = False
        for port, ws in may_write.items():
            if status[port] == "unknown" and all(w in decided for w in ws):
                status[port] = "present" if port in written else "absent"
                changed = True
        what = (f"ran {ran}" if ran else "") + ("; " if ran and skipped else "") + (f"not triggered {skipped}" if skipped else "")
        print(f"  round {rnd}: {what or 'nothing decided'}; p is {status['p']}, q is {status['q']}")
        if not ran and not skipped and not changed:
            return status


def verdict(s):
    return "every port resolved" if "unknown" not in (s["p"], s["q"]) else "ports still unknown: the compiler rejects this"


print("constructive, a1 writes p:")
print("  fixed point:", verdict(ports_at_one_tag()), "\n")
print("constructive, a1 leaves p absent (question FB1):")
print("  fixed point:", verdict(ports_at_one_tag(a1_writes_p=False)), "\n")
print("causality loop, a2 may also write p:")
print("  fixed point:", verdict(ports_at_one_tag(a2_may_write_p=True)))
