from state import validan_niz

def ispisi_resenje(koraci):
    print("\nPostavljanje domine:\n")
    for i, (domina, strana) in enumerate(koraci, 1):
        print(f"{i}. domina {domina} na stranu {strana}")

def proveri_resenje(koraci, pocetni_niz=None):
    niz = []
    if pocetni_niz:
        niz = list(pocetni_niz)

    for i, (domina, strana) in enumerate(koraci, 1):
        a, b = domina

        if not niz:
            niz.append((a, b))
            continue

        if strana == "L":
            if b == niz[0][0]:
                niz.insert(0, (a, b))
            elif a == niz[0][0]:
                niz.insert(0, (b, a))
            else:
                print(f"Greska na koraku {i}: domina {domina} ne moze na L")
                return False

        if strana == "R":
            if niz[-1][1] == a:
                niz.append((a, b))
            elif niz[-1][1] == b:
                niz.append((b, a))
            else:
                print(f"Greska na koraku {i}: domina {domina} ne moze na R")
                return False

    if not validan_niz(niz):
        return False

    return niz[0][0] == niz[-1][1]
