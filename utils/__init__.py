def vec_diff_len(p1, p2):
    assert(len(p1) == len(p2))
    return sum([(p1[dim] - p2[dim]) ** 2 for dim in range(len(p1))]) ** 0.5