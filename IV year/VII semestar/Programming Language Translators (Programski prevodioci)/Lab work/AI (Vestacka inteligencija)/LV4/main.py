from state import pocetno_stanje, pocetno_stanje_custom
from dfs import pretraga
from utilis import ispisi_resenje, proveri_resenje

def main():
    stanje = pocetno_stanje()
    koraci = pretraga(stanje)

    if koraci is None:
        print("Nije pronadjeno resenje")
    else:
        ispisi_resenje(koraci)
        if proveri_resenje(koraci):
            print("\nIspravno resenje")
        else:
            print("\nNeispravno resenje")

    print("-------------------------------------------")

    stanje2 = pocetno_stanje_custom()
    koraci2 = pretraga(stanje2)

    if koraci2 is None:
        print("Nije pronadjeno resenje")
    else:
        ispisi_resenje(koraci2)
        if proveri_resenje(koraci2, pocetni_niz=stanje2[0]):
            print("\nIspravno resenje")
        else:
            print("\nNeispravno resenje")

if __name__ == "__main__":
    main()
