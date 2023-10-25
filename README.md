
    In implementare am declarat o structura q_nod, cu campurile x, y
size, ce vor ajuta la crearea arborelui, un camp in care retin 0 sau 1
pentru a vedea care este tipul nodului, frunza sau intern, si pointeri 
catre cei 4 fii ai fiecarui nod, in functie de impartirea imaginii, 
ss reprezentand sus stanga, sd- sus dreapta, jd-jos dreapta si js-jos stanga.
    Am creat si o functie de maxim ce va fi de folos pentru aflarea numarului
de niveluri al arborelui.
    Am realizat o functie pentru crearea arborelui cuaternar, ce pleaca de la 
punctul (0,0) si calculeaza media culorilor din submatricea de pixeli 
corespunzatoare unui anumit bloc. Blocurile se formeaza dupa ce a fost calculat
scorul similaritatii, iar daca acesta era mai mare decat un factor dat, 
imaginea trebuia sparta in 4, si tot asa pana cand nu mai era nevoie de 
divizare. In aceasta functie am calculat si numarul de blocuri din imagine 
pentru care scorul similaritatii pixelilor este mai mic sau egal decât factorul
furnizat si dimensiunea laturii patratului pentru cea mai mare zona din imagine
care a ramas nedivizata. 
    Urmatoarea functie, max_depth, calculeaza numarul de niveluri din arborele
cuaternar.
    Functia creare_coada creeaza un vector pe care il utilizez pe post de 
coada, pentru cerinta 2, adica adaug in acesta nodurile din arbore pe niveluri.
    In functia cerinta2 am implementat rezolvarea pentru cerinta2, in care am 
afisat in fisier dimensiunea imaginii, iar apoi am parcurs nodurile din acel 
vector coada, afisand tipul lor, iar daca acestea erau frunze, afisam si 
valorile pentru culorile acestora. 
    Functia free_quadtree elibereaza memoria ocupata de arborele cuaternar.
    In main, am declarat o variabila factor de tip int, pentru a pune in 
aceasta valoarea factorului dat ca argument in linia de comanda.
    Am citit din fisier tipul acestuia si dimensiunile imaginii, alaturi de
valoarea maxima pe care o pot lua culorile. Am declarat grid de tip Pixel, 
pentru a citi valorile pentru imagine din fisierul binar. Dupa ce am citit
toate valorile pentru imagine, creez arborele cuaternar.
    In functie de cerinta, deschideam fisierul de output in modul text sau 
binar, si afisam in acesta valorile cerute. Pentru a doua cerinta cream si 
vectorul pe care il utilizez pe post de coada.
    La final eliberez si memoria ocupata de imagine, in grid, apoi cea ocupata
de arbore si inchid fisierele.