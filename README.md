Elf's Battlegrounds
===================

## Concepte

* Structura `game_t` mentine starea jocului. Aici este memorata harta de joc,
  jucatorii (elfii) si fisierele de intrare si iesire. Am folosit-o pentru a
  grupa in mod convenabil datele inrudite si pentru a mai scadea numarul de
  parametri ce trebuie carati intre apelurile dintre functii.
* Un elf cu HP 0 sau negativ este iesit din joc.
* La inceperea meciului, o celula poate avea MAXIM UN SINGUR ELF. Daca totusi
  sunt mai multi, atunci primul elf ii va elimina pe ceilalti. (este logic
  pentru ca acest elf va fi singurul care va avea echipata o manusa, intrucat
  ceilalti nu vor avea ce sa echipeze). TOTUSI, CERINTA/FORUMUL GARANTEAZA CA
  NU VOR EXISTA ASTFEL DE CAZURI, DAR TESTELE 5-9 NU RESPECTA GARANTIA.
* Fiecare elf poate avea asociata o celula (campul `cell` din structura `elf_t`).
  Daca celula asociata nu e valida (pointer-ul NULL), inseamna ca acel elf a
  iesit din joc.
* Cand se primeste comanda `MELTDOWN`, harta se va realoca, pierzandu-se liniile
  si coloanele de la margini, asadar coordonatele se vor modifica, X si Y scazand
  fiecare cu o unitate.
* In schimb, coordonatele comenzii `SNOWSTORM` vor fi mereu relative la harta
  initiala, deci acestea trebuiesc adaptate noii harti, iar pentru acest fapt
  se vor scadea din X si Y numarul de comenzi SNOWSTORM primite pana la acel
  moment (campul `times_shrinked` al structurii `map_t`).
* Consider axa X - verticala in jos, axa Y - orizontala spre dreapta, cu originea
  in coltul stanga-sus. 

## Modularizare

* `game`: logica generala a jocului. Responsabil pentru citirea si interpretarea
          datelor, tratarea posibilelor cazuri de eroare si eliberarea tuturor
		  resurselor alocate.
* `map`: logica hartii. Responsabil cu operatiile pe memoria hartii (alocare,
         realocare, dealocare); cu citirea si stocarea proprietatilor si cu
		 validarea coordonatelor.
* `elfs`: logica jucatorilor. Responsabil cu operatiile pe memorie (alocare,
          dealocare); cu citirea coordonatelor jucatorilor si plasarea acestora
		  pe harta; cu tratarea cazurilor in care un jucator poate parasi jocul.
	      Contine criteriile dupa care se sorteaza jucatorii pe `SCOREBOARD`
* `cmds`: procesarea comenzilor
* `utils`: diverse functii ajutatoare. (citire unui string de marime nespecificata
           dintr-un stream etc)
* `main`: punctul de intrare in program.


## Probleme intalnite

* Checker-ul nu respecta cerinta la lansarea in executie a programului.
  In cerinta se specifica faptul ca numele pentru fisierele de intrare si iesire
  sunt transmite ca parametru in linia de comanda, in schimb checkerul nu le
  trimite si se asteapta ca numele sa coincida mereu cu numele executabilului.
* Cazul in care 2 elfi ajung in aceeasi celula fara sa fie echipati cu manusi
  este nedefinit. Cerinta nu specifica ce trebuie facut in acest caz.
* Nu se specifica nicaieri ce fel de coordonate sunt primite de la comanda
  `SNOWSTORM` (daca sunt relative la harta actuala sau la harta initiala)
* Nu se specifica nicaieri in cerinta in mod EXPLICIT faptul ca se lucreaza
  doar cu memorie alocata dinamic.
