def process(l):
    r = []
    for i in l:
        for j in i:
            if j*j not in r:
                r.append(j*j)
    return sorted(r,reverse=1)
