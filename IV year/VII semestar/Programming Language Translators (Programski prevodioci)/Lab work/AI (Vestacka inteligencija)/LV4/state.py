def sve_domine():
    # (0,0) do (4,4)
    return {(i, j) for i in range(5) for j in range(i, 5)}

def validan_niz(niz):
    for i in range(len(niz) - 1):
        if niz[i][1] != niz[i + 1][0]: 
            return False
    return True

def pocetno_stanje():
    return ([], sve_domine())

def pocetno_stanje_custom(): 
    pocetni_niz = [(1, 1), (1, 2), (2, 2), (2, 0), (0, 0), (0, 1)]

    sve = {(i, j) for i in range(5) for j in range(i, 5)}
    vec_iskoriscene = {tuple(sorted(domina)) for domina in pocetni_niz}
    preostale = sve - vec_iskoriscene

    return (pocetni_niz, preostale)

def cilj(stanje):
    niz, preostale = stanje
    if preostale:
        return False
    return niz[0][0] == niz[-1][1] 

def dodaj_dominu(stanje, domina, strana):
    niz, preostale = stanje

    if domina not in preostale:
        return None

    a, b = domina

    if not niz:
        yield ([(a,b)], preostale - {domina})
        return

    if strana == "L":
        levi_kraj = niz[0][0]
        if b == levi_kraj:
            yield ([(a, b)] + niz, preostale - {domina})
        elif a == levi_kraj:
            yield ([(b, a)] + niz, preostale - {domina})

    if strana == "R":
        desni_kraj = niz[-1][1]
        if a == desni_kraj:
            yield (niz + [(a, b)], preostale - {domina})
        elif b == desni_kraj:
            yield (niz + [(b, a)], preostale - {domina})


def nova_stanja(stanje): 
    niz, preostale = stanje
    for d in preostale:
        for strana in ("L", "R"):
            for novo_stanje in dodaj_dominu(stanje, d, strana):
                yield (novo_stanje, (d, strana)) 