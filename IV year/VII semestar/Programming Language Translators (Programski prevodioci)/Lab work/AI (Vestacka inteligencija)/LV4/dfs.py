from queue import LifoQueue
from state import cilj, nova_stanja, validan_niz

def pretraga(pocetno_stanje):
    stack = LifoQueue()
    visited = set() 

    stack.put((pocetno_stanje, [])) 

    while not stack.empty():
        stanje, putanja = stack.get()
        niz, preostale = stanje

        if not validan_niz(niz): 
            continue

        if cilj(stanje):
            return putanja

        kljuc = (tuple(niz), frozenset(preostale)) 
        if kljuc in visited: 
            continue

        visited.add(kljuc)

        sledeca = list(nova_stanja(stanje)) 
        for novo_stanje, potez in reversed(sledeca):
            stack.put((novo_stanje, putanja + [potez]))

    return None
