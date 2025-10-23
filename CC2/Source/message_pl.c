/*   ______   ___       ___         ____     ______  ____
*   /\  _  \ /\_ \    /'___\       /\  _`\  /\  _  \/\  _`\
*   \ \ \L\ \\//\ \  /\ \__/   __  \ \ \/\_\\ \ \L\ \ \ \/\ \
*    \ \  __ \ \ \ \ \ \ ,__\/'__`\ \ \ \/_/_\ \  __ \ \ \ \ \
*     \ \ \/\ \ \_\ \_\ \ \_/\ \L\.\_\ \ \L\ \\ \ \/\ \ \ \_\ \
*      \ \_\ \_\/\____\\ \_\\ \__/.\_\\ \____/ \ \_\ \_\ \____/
*       \/_/\/_/\/____/ \/_/ \/__/\/_/ \/___/   \/_/\/_/\/___/
*
*   A CAD
*
*   By Marek Ratajczak 2024
*
*   See readme_alfacad.txt for copyright information.
*
*/

#ifdef __MESSAGE__

static char* komunikaty[] =
/*0*/{ u8"",
/*1*/u8"Dane OK",
/*2*/u8"Czekaj ...",
/*3*/u8"Przerwanie --> Esc ",
/*4*/u8"Zmiana formatu rysunku, czekaj ...",
/*5*/u8"Test danych ...",
/*6*/u8"",
/*7*/u8"Czekaj:  Test struktury bloków...",
/*8*/u8"Czekaj:  Usuwanie struktury bloków. Przerwanie --> Esc",
/*9*/u8"Zapisano ustawienie okna.",
};

static char* messages[] =
{
	/*0*/u8"%s",
	/*1*/u8"Przerwanie  --> Esc   (%s%%)",
	/*2*/u8"CZEKAJ -> rozcinanie linii:  (%s%%)",
	/*3*/u8"Buforowanie obrazu  (%s)   Przerwanie --> Esc",
};

static char* messages1[] =
{
	/*0*/u8"%s",
	/*1*/u8"Przerwanie  --> Esc   (%s%%)",
	/*2*/u8"CZEKAJ -> rozcinanie linii:  (%s%%)",
};

static char* errors_kom[] =
/*  0*/{ u8"",
/*  1*/u8"Zbyt mały krok, siatka nie będzie zaznaczona",
/*  2*/u8"Niewłaściwy format liczby rzeczywistej",
/*  3*/u8"Niewłaściwy format parametru",
/*  4*/u8"Nie znaleziono zbioru z makrem.",
/*  5*/u8"Zbyt duży format",
/*  6*/u8"Za duży zbiór z makrem",
/*  7*/u8"Brak lub niewłaściwe dane konfiguracyjne plotera",
/*  8*/u8"Dysk pełny lub błędna operacja dyskowa",
/*  9*/u8"Zły format dodatniej liczby całkowitej",
/* 10*/u8"Brak lub niewłaściwy zbiór danych inicjacyjnych plotera",
/* 11*/u8"Czcionka nie może być wczytana, brak pamięci lub zły format zbioru",
/* 12*/u8"Brak pamięci na wczytanie czcionki",
/* 13*/u8"Ekran poza formatem rysunku",
/* 14*/u8"Proces potomny nie rozpoczął się",
/* 15*/u8"Błąd wewnętrzny programy. Zgłoś to do producenta.",
/* 16*/u8"Zły format liczby",
/* 17*/u8"Brak czcionki",
/* 18*/u8"Brak pamięci",
/* 19*/u8"Nie znaleziono żadnego obiektu",
/* 20*/u8"Przekazano niewłaściwe parametry",
/* 21*/u8"Przekroczono rozmiar bloku (max 2 GB)",
/* 22*/u8"Brak lub niewłaściwy zbiór typów linii",
/* 23*/u8"Błąd wewnętrzny programu, zgłoś to do producenta",
/* 24*/u8"Zły format dodatniej liczby całkowitej",
/* 25*/u8"Zły format zbioru",
/* 26*/u8"Brak lub niewłaściwe dane konfiguracyjne drukarki",
/* 27*/u8"Przepełnienie bufora makra",
/* 28*/u8"Niewłaściwa nazwa zbioru",
/* 29*/u8"Linie nie mogą być równoległe",
/* 30*/u8"Zły format dodatniej liczby rzeczywistej",
/* 31*/u8"Brak zbioru lub błąd odczytu z dysku",
/* 32*/u8"Promień łuku jest zbyt duży",
/* 33*/u8"Brak pamięci",
/* 34*/u8"Promień łuku jest zbyt mały lub zły format liczby",
/* 35*/u8"Zły format niezerowej liczby rzeczywistej",
/* 36*/u8"Więcej warstw nie można już utworzyć",
/* 37*/u8"Warstwa bieżąca nie może być niewidoczna!",
/* 38*/u8"Błędny format danych, promień łuku <= 0.",
/* 39*/u8"Błędny format danych, promień okręgu <= 0.",
/* 40*/u8"Niewłaściwy format danych.",
/* 41*/u8"Przerwano drukowanie lub błąd zapisu na drukarkę.",
/* 42*/u8"Błąd drukarki. Kontynuować / Przerwać :",
/* 43*/u8"Błąd urządzenia. Kontynuować / Przerwać :",
/* 44*/u8"Wskazany element nie należy do bloku.",
/* 45*/u8"Zły format zbioru.",
/* 46*/u8"Niewłaściwy dysk lub katalog.",
/* 47*/u8"Nowy rysunek",
/* 48*/u8"Błąd zapisu zbioru.",
/* 49*/u8"Nie znaleziono zbioru",
/* 50*/u8"Uwaga : Elementy ostatniego bloku są na warstwach '-W'",
/* 51*/u8"Niewłaściwy format zbioru konfiuracyjnego ALFACADPL.INI.",
/* 52*/u8"Operacja nie może być wykonana.",
/* 53*/u8"Brak usuniętych elementów.",
/* 54*/u8"Uwaga : Elementy ostatniego bloku są na warstwach '-E'",
/* 55*/u8"Błędne dane liczbowe",
/* 56*/u8"Blok zawiera łuki, skale w kierunku X i Y musz być równe.",
/* 57*/u8"Błąd w wyrażeniu.",
/* 58*/u8"Dzielenie przez 0.",
/* 59*/u8"Brak nawiasu zamykającego.",
/* 60*/u8"To nie jest funkcja.",
/* 61*/u8"Zła liczba argumentów funkcji.",
/* 62*/u8"Funkcja wymaga nieujemnego argumentu.",
/* 63*/u8"Niezdefiniowana zmienna lub funkcja.",
/* 64*/u8"Spodziewano się nawiasu '('.",
/* 65*/u8"Argument funkcji matematycznej poza dopuczczlnym zakresem.",
/* 66*/u8"Kąt łuku równy 0.",
/* 67*/u8"Niewłaściwy numer strony.",
/* 68*/u8"Dopuszczalna liczb krawędzi : 3 - 1024.",
/* 69*/u8"Nie znaleziono żadnego bloku.",
/* 70*/u8"Liczba bloków przekracza dopuszczalną liczbę 2000.",
/* 71*/u8"Brak segmentów szkicu do usunięcia.",
/* 72*/u8"Rozbito blok o niewłaściwych parametrach.",
/* 73*/u8"Promień łuku nie może być < 0.",
/* 74*/u8"Promień okręgu nie może być < 0.",
/* 75*/u8"Zły format dwu dodatnich liczb całkowitych.",
/* 76*/u8"Brak segmentów śladu do usunięcia.",
/* 77*/u8"Brak segmentów polilinii do usunięcia.",
/* 78*/u8"Niedopuszczalna rekurencja",
/* 79*/u8"Zły format niezerowej liczby rzeczywistej.",
/* 80*/u8"Wskazany elementy jest na warstwie '-E'.",
/* 81*/u8"Wskazanego segmentu polilinii nie można przedłużyć.",
/* 82*/u8"Należy wskazać segment polilinii.",
/* 83*/u8"Profilować można tylko dwa sąsiednie segmenty polilinii.",
/* 84*/u8"Promień łuku jest zbyt mały.",
/* 85*/u8"Profilować można tylko zewnętrzne segmenty polilinii.",
/* 86*/u8"Fazować można tylko dwa sąsiednie segmenty polilinii.",
/* 87*/u8"Wskazane obiekty nie tworzą zamknietego obszaru.",
/* 88*/u8"Wskazany punkt nie leży wewnątrz zamkniętego obszaru.",
/* 89*/u8"Funkcja nie jest ciągła w punkcie początkowym",
/* 90*/u8"Funkcja ma za dużo punktów nieciągłości",
/* 91*/u8"Elementy wskazanego bloku są na warstwach '-W'.",
/* 92*/u8"Elementy wskazanego bloku są na warstwach '-E'.",
/* 93*/u8"Nieprawidłowo zdefiniowany kąt lokalnego układu współrzędnych (dx=0,dy=0)",
/* 94*/u8"Brak bloku w schowku",
/* 95*/u8"Brak pamięci dla inicjacji menu",
/* 96*/u8"Brak pamięci dla bufora makra i warstw",
/* 97*/u8"Brak pamięci dla zainicjowania danych",
/* 98*/u8"Brak pamięci dla zainicjowania danych rysunku",
/* 99*/u8"Zły format pliku lub brak pamięci na wczytanie rysunku dołączonego",
/*100*/u8"Błąd odczytu zbioru dołaczonego",
/*101*/u8"Brak pamięci na wczytanie zbioru roboczego 'RYSUJ.$2'",
/*102*/u8"Błąd odczytu zbioru roboczego 'RYSUJ.$2'",
/*103*/u8"Błąd otwarcia zbioru danych 'ALFPROF.TMP'",
/*104*/u8"Błąd odczytu zbioru danych 'ALFPROF.TMP'",
/*105*/u8"Odległość  nie może być < 0",
/*106*/u8"Wartość współczynnika (skala_x / skala_y) profilu musi być > 0",
/*107*/u8"Odległość jest mniejsza od zadanej w poprzednim przekroju.",
/*108*/u8"Odległość 'pierwszego przekroju' nie może być < 0",
/*109*/u8"Wartość początkowa hektometrów nie może być < 0",
/*110*/u8"Wartość końcowa hektometrów musi być > lub = początkowej.",
/*111*/u8"Błąd : SkalaX/SkalaY =0 . Należy zadeklarować stosunek skali <> 0",
/*112*/u8"Suma odległości dla średnic kanału istn. przekracza długość odcinka",
/*113*/u8"Suma odległości dla średnic kanału proj. przekracza długość odcinka",
/*114*/u8"Zadane długości dla kanału istn. i projekt. nie są równe przy odlełości>0.",
/*115*/u8"Długość zadana dla kanału istniejącego < 0",
/*116*/u8"Długość zadana dla kanału projektowanego < 0",
/*117*/u8"Odległość (w tym wariancie) nie może być <= 0",
/*118*/u8"Długość dla spadku kanału istniejącego przekracza długość odcinka",
/*119*/u8"Długość dla spadku kanału projektowanego przekracza długość odcinka",
/*120*/u8"Spadek kanału istniejącego przekracza różnice rzędnych odcinka",
/*121*/u8"Spadek kanału projektowanego przekracza różnice rzędnych odcinka",
/*122*/u8"Nieprawidłowa wartość skali profilu. Wybierz funkcję /Skala profilu/",
/*123*/u8"Brak danych pierwszego pomiaru. Wybierz funkcję /Pierwszy pomiar/",
/*124*/u8"Uwaga! lokalny punktu <0,0> w punkcie globalnym 0,0. Naciśnij klawisz",
/*125*/u8"Zaznaczono kabel energetyczny bez podania rzędnej terenu",
/*126*/u8"Zaznaczono kanalizację telefoniczną bez podania rzędnej terenu",
/*127*/u8"Zaznaczono kabel telefoniczny bez podania rzędnej terenu",
/*128*/u8"Odległość mniejsza od wartości początkowej na osi hektometrów",
/*129*/u8"Odległość dla przekroju instalacji nie może być <= 0",
/*130*/u8"Odległość=pierwszemu hektometrowi może być zadana tylko dla 'pierwszego przekroju'",
/*131*/u8"Odległość dla przekroju instalacji nie może być <= pierwszemu hektometrowi",
/*132*/u8"Odległość 'pierwszego przekroju' nie może być < od pierwszego hektometra",
/*133*/u8"UWAGA: obliczona długość odcinka kanału istniejącego =0",
/*134*/u8"Obliczona długość odcinka kanału istniejącego niezgodna z długością zadaną",
/*135*/u8"Obliczony spadek odcinka kanału istniejącego niezgodny ze spadkiem zadanym",
/*136*/u8"UWAGA: zadany spadek odcinka kanału istniejącego = 0",
/*137*/u8"UWAGA: obliczona długość odcinka kanału projektowanego =0",
/*138*/u8"Obliczona długość odcinka kanału projektowanego niezgodna z długością zadaną",
/*139*/u8"Obliczony spadek odcinka kanału projektowanego niezgodny ze spadkiem zadanym",
/*140*/u8"UWAGA: zadany spadek odcinka kanału projektowanego = 0",
/*141*/u8"Rzędna kanału istniejącego >0 ale długość odcinka oraz odległość =0",
/*142*/u8"Spadek kanału istniejącego <>0 ale długość odcinka oraz odległość =0",
/*143*/u8"Rzędna kanału projektowanego >0 ale długość odcinka oraz odległość =0",
/*144*/u8"Spadek kanału projektowanego <>0 ale długość odcinka oraz odległość =0",
/*145*/u8"Odległość oraz długości kanału istniejącego i projektowanego = 0",
/*146*/u8"Wyznaczone odległości dla kanału istniejącego i projektowanego są różne.",
/*147*/u8"Brak zadanej odległości dla rzędnej ulicy",
/*148*/u8"Brak zadanej odległości dla rzędnej terenu",
/*149*/u8"O.K. Przywrócono dane poprzedniego pomiaru. Usuń ostatni blok pomiaru.",
/*150*/u8"<BETA>  W aktualnej wersji 'beta'  funkcja nie jest aktywna ...",
/*151*/u8"Odległość dla odwiertu geologicznego nie może być < 0",
/*152*/u8"Odległość dla odwiertu geologicznego nie może być <= pierwszemu hektometrowi",
/*153*/u8"Podana odległość ale brak (lub zła wartość) rzędnej terenu dla odwiertu",
/*154*/u8"Ujemna wartość jednej z rzędnej względem powierzchni terenu.",
/*155*/u8"Rzedne p.p.t. warstw odwiertu nie sa narastajace",
/*156*/u8"Rzedna p.p.t. pierwszej warstwy = 0",
/*157*/u8"Brak rzędnej zwierciadła napiętego przy zadanej rzędnej zw. ustabilizowanego",
/*158*/u8"Brak rzędnej zwierciadła ustabilizowanego przy zadanej rzędnej zw. napiętego",
/*159*/u8"Rzędna zwierciadła ustabilizowanego poniżej dna odwiertu",
/*160*/u8"Rzędna zwierciadła napiętego poniżej dna odwiertu",
/*161*/u8"Rzędna zwierciadła swobodnego poniżej dna odwiertu",
/*162*/u8"Brak zadanej odległości dla rzędnej terenu projektowanego",
/*163*/u8"Brak zadanej odległości dla rzędnej terenu istniejącego",
/*164*/u8"Zaznaczono inne instalacje bez podania rzędnej terenu",
/*165*/u8"Rzędna sączenia poniżej dna odwiertu",
/*166*/u8"",
/*167*/u8"",
/*168*/u8"",
/*169*/u8"",
/*170*/u8"Błąd konwersji zbioru DXF.",
/*171*/u8"Brak symbolu w katalogu",
/*172*/u8"Nieprawidłowa wersja DXF (dopuszczalne wersje: R9, R10, R11, R12, R13, R14, 2000)",
/*173*/u8"Za duża liczba plików (max. 1000)",
/*174*/u8"",
/*175*/u8"",
/*176*/u8"",
/*177*/u8"",
/*178*/u8"",
/*179*/u8"",
/*180*/u8"Zła wartość znacznika 'parzystości'. Dozwolona wartość 0, 1, lub -1.",
/*181*/u8"Zła wartość znacznika kąta narożnika. Dozwolona wartość 90 (°) lub 135 (°).",
/*182*/u8"Zła wartość znacznika węgarka. Dozwolona wartość 0 lub 1.",
/*183*/u8"Długość nadproża musi być większa od szerokości okna.",
/*184*/u8"Nieprawidłowy numer ściany.",
/*185*/u8"Brak numeru zamówienia w tabliczce systemowej bieżącego rysunku.",
/*186*/u8"Brak numeru zamówienia w opisie nowej ściany.",
/*187*/u8"Błąd tworzenia filara ściany zewnętrznej",
/*188*/u8"Błąd tworzenia filara ściany wewnętrznej / fundamentowej",
/*189*/u8"Zła wartość znacznika okna zespolonego. Dozwolona wartość 0, lub 1.",
/*190*/u8"Wysokość parapetu musi być równa lub mniejsza od wysokości ściany.",
/*191*/u8"",
/*192*/u8"",
/*193*/u8"",
/*194*/u8"",
/*195*/u8"",
/*196*/u8"Warstwa bieżąca nie może być usunięta !",
/*197*/u8"",
/*198*/u8"",
/*199*/u8"",
/*200*/u8"",
/*201*/u8"Długość wektora musi być większa od zera",
/*202*/u8"Błąd odczytu pliku obrazu",
/*203*/u8"Brak lub nieprawidłowy blok parametryczny na warstwie profilu.Podaj dane:",
/*204*/u8"Nieprawidłowa wartość skali X profilu",
/*205*/u8"Nieprawidłowa wartość typu profilu",
/*206*/u8"Nieprawidłowa wartość pierszego hektometra",
/*207*/u8"UWAGA! Brak lub nieprawidłowy blok parametryczny na warstwie profilu.",
/*208*/u8"Brak studni w zadanej sieci",
/*209*/u8"Pojedyncza studnia jest opisana w sieci głównej",
/*210*/u8"Wymuszone ustawienie warstwy '0' jako widocznej i aktywnej",
/*211*/u8"Błąd wewnętrzny",
/*212*/u8"Kod błędu: ",
/*213*/u8"Wartość poza zakresem 1024 .. 2000000 [KB]",
/*214*/u8"Numer warstwy poza zakresem 1...256",
/*215*/u8"Żadna z półosi elipsy nie może mieć długość mniejszą od 0.",
/*216*/u8"Zły format nieujemnej liczby rzeczywistej",
};

static char* komunikaty_kom[] =
/* 0*/{ u8"",
/* 1*/u8"Podaj wymiar : ",
/* 2*/u8"Podaj dzielnik współędnych X;Y : ",
/* 3*/u8"Rysunek nie uaktualniony. Uaktualnić ? (Tak / Nie / Esc): ",
/* 4*/u8"Podaj format rysunku : ",
/* 5*/u8"Podaj nazwę zbioru do zapisu : ",
/* 6*/u8"Podaj skalę:  1: ",
/* 7*/u8"Podaj krok kursora : ",
/* 8*/u8"Podaj gestość siatki : ",
/* 9*/u8"Podaj nazwę zbioru do odczytu : ",
/*10*/u8"Podaj wysokość znaku : ",
/*11*/u8"Podaj opis bloku : ",
/*12*/u8"Funkcja nieaktywna.",
/*13*/u8"Podaj tekst : ",
/*14*/u8"Podaj typ bloku : ",
/*15*/u8"Podaj kąt : ",
/*16*/u8"Podaj kąt obrotu bloku : ",
/*17*/u8"Podaj współczynnik skali bloku : ",
/*18*/u8"W wersji demostracyjnej funkcja nieaktywna.",
/*19*/u8"Podaj minimalną długość odcinka szkicu : ",
/*20*/u8"Podaj kąt wypełnienia : ",
/*21*/u8"Podaj liczbę elementów : ",
/*22*/u8"Skok o dX ; dY : ",
/*23*/u8"Skok do X ; Y : ",
/*24*/u8"Podaj szerokość śladu : ",
/*25*/u8"Podaj wielkość punktu : ",
/*26*/u8"Podaj nazwę zbioru : ",
/*27*/u8"Podaj ilość segmentów podziału : ",
/*28*/u8"Wskaż",
/*29*/u8"Podaj długość segmentu podziału : ",
/*30*/u8"Skok o długość-kąt : ",
/*31*/u8"Skala wzorca : ",
/*32*/u8"Kąt obrotu wzorca : ",
/*33*/u8"Współczynnik szerokości tekstu : ",
/*34*/u8"Wysokość tekstu wymiarowego : ",
/*35*/u8"Dokładność wymiarowania : ",
/*36*/u8"Odsunięcie linii wymiarowej : ",
/*37*/u8"Odległość tekstu od linii wymiarowej : ",
/*38*/u8"Odsunięcie punktu początkowego linii rozszerzenia : ",
/*39*/u8"Podaj dokładność rysowania (mm) : ",
/*40*/u8"Okno",
/*41*/u8"Usuń  Ciągnij",
/*42*/u8"Początek",
/*43*/u8"Koniec",
/*44*/u8"Program zewnętrzny : ",
/*45*/u8"Podaj interwał : ",
/*46*/u8"Przerwanie -> Esc",
/*47*/u8"Podaj liczbę wierszy i kolumn  w;k : ",
/*48*/u8"Podaj długość odcinka : ",
/*49*/u8"Podaj max. ilość punktów podziału : ",
/*50*/u8"Przeglądać podkatalogi ? (Tak / Nie / Esc) : ",
/*51*/u8"Odstęp między liniami : ",
/*52*/u8"Punkt początkowy kreskowania : ",
/*53*/u8"Podaj liczbę wierzchołków polilinii aproksymującej elipsę: ",
/*54*/u8"Podaj krok aproksymacji dx : ",
/*55*/u8"Podaj funkcje f(x) : ",
/*56*/u8"Kalkulator : ",
/*57*/u8"Zmień stronę. Wciśnij dowolny klawisz.",
/*58*/u8"Usunąć wskazane elementy ? (Tak / Nie / Esc) : ",
/*59*/u8"",
/*60*/u8"Usunąć dołączony rysunek ? (Tak / Nie / Esc) : ",
/*61*/u8"Brak dołączonego rysunku ...",
/*62*/u8"Naciśnij klawisz  Esc...",
/*63*/u8"Podaj kąt lokalnego układu współrzędnych: α =",
/*64*/u8"Czy napewno zakończyć program ? (Tak / Nie / Esc)",
/*65*/u8"",
/*66*/u8"Wskaż -> dokładnie koniec",
/*67*/u8"Wskaż -> najbliższy koniec",
/*68*/u8"Wskaż -> środek",
/*69*/u8"Wskaż -> przecięcie",
/*70*/u8"Wskaż -> centrum",
/*71*/u8"Wskaż -> prostopadle",
/*72*/u8"Wskaż -> stycznie",
/*73*/u8"Wskaż -> najbliższy",
/*74*/u8"Wskaż -> punkt",
/*75*/u8"Wskaż linię dla wyznaczenia kąta",
/*76*/u8"Wskazany element nie należy do bloku.",
/*77*/u8"Wskazany elementy jest na warstwie '-E'.",
/*78*/u8"Elementy wskazanego bloku są na warstwach '-W'.",
/*79*/u8"Elementy wskazanego bloku są na warstwach '-E'.",
/*80*/u8"Wskaż obiekt dla określenia warstwy",
/*81*/u8"Podaj numer koloru (0..255) : ",
/*82*/u8"Wskaż obiekt dla określenia koloru",
/*83*/u8"Wskaż obiekt dla określenia typu i grubości/ parametrów tekstu",
/*84*/u8"Nie rozbito bloków 'zamrożonych'",
/*85*/u8"Nie rozbito bloków 'niezamrożonych'",
/*86*/u8"Wskazany tekst nie jest nazwą pliku",
/*87*/u8"Jesteś w <PNIU> struktury drzewiastej",
/*88*/u8"Nie znaleziono tekstu.",
/*89*/u8"UWAGA! Zbyt dużo obiektów 'PIN' w ciągniętym bloku.",
/*90*/u8"Podaj TYP aparatu : ",
/*91*/u8"Podaj SYMBOL aparatu : ",
/*92*/u8"Podaj KOMENTARZ : ",
/*93*/u8"Nieprawidłowa definicja aparatu .  Naciśnij dowolny klawisz...",
/*94*/u8"Podaj typ (nazwę) symbolu : ",
/*95*/u8"Uwaga: wygenerowano aparaty zastępcze (*). Uzupełnij schemat...",
/*96*/u8"Podaj skalę ekranu : ",
/*97*/u8"Podaj nazwę schematu + : ",
/*98*/u8"Podaj nazwę PLIKU schematu + : ",
/*99*/u8"Podaj współrzędne wskazanego punktu : ",
/*100*/u8"Podaj jednostkę rysunkową w zbiorze DXF",
/*101*/u8"Podaj skalę transformacji zbioru DXF",
/*102*/u8"Podaj szukany tekst : ",
/*103*/u8"Szukaj : ",
/*104*/u8"Zamień na : ",
/*105*/u8"O.K.",
/*106*/u8"Podaj przesunięcie osi względem środka symetrii śladu : ",
/*107*/u8"Funkcja czasowo nieaktywna w tej wersji programu.",
/*108*/u8"Podaj nazwę nowego podkatalogu : ",
/*109*/u8"Usunąć prototypy bloków ? (Tak / Nie / Esc) : ",
/*110*/u8"Wskaż obiekt dla uzyskania listy parametrów",
/*111*/u8"Wskaż wiersz parametru w celu edycji lub  dopisania do listy, lub Esc",
/*112*/u8"Wskaż punkt początkowy kreskowania : ",
/*113*/u8"Podaj wielkość dx punktu [mm]: ",
/*114*/u8"Podaj wielkość dy punktu [mm]: ",
/*115*/u8"Maksymalny numer warstwy: 256",
/*116*/u8"Minimalny numer warstwy: 1",
/*117*/u8"Warstwa nie istnieje.",
/*118*/u8"Pierwszy symbol : ",
/*119*/u8"Zbiór nie uaktualniony. Uaktualnić ? (Tak / Nie / Esc): ",
/*120*/u8"Usunąć wypełnienie pustakami ? (Tak / Nie / Esc) : ",
/*121*/u8"Przesunięcie profilu od odległości [m]: ",
/*122*/u8"-> o odległość [m]: ",
/*123*/u8"Wskaż obiekt bloku pomiaru dla uzyskania parametrów",
/*124*/u8"Odległość początkowa [m]: ",
/*125*/u8"Odległość końcowa [m]: ",
/*126*/u8"Podaj materiał: ",
/*127*/u8"Podaj rodzaj nawierzchni: ",
/*128*/u8"Podaj średnicę kanału projektowanego: ",
/*129*/u8"Podaj średnicę kanału istniejącego: ",
/*130*/u8"Wskaż obiekt bloku pomiaru początkowego",
/*131*/u8"Wskaż obiekt bloku pomiaru końcowego",
/*132*/u8"Podaj spadek [w promilach]: ",
/*133*/u8"Brak rzędnej terenu projektowanego",
/*134*/u8"Brak rzędnej terenu istniejącego",
/*135*/u8"Podaj stałe zagłębienie [m]: ",
/*136*/u8"Podaj prefiks numerów sektorów : ",
/*137*/u8"Podaj pierwszy numer sektora : ",
/*138*/u8"Podaj wysokość sektora [mm] : ",
/*139*/u8"Podaj długość sektora [mm] : ",
/*140*/u8"Podaj grubość ramki sektorów [mm] : ",
/*141*/u8"Podaj dystans ramki sektorów [mm] : ",
/*142*/u8"Brak numerów ścian w liście zbiorczej.",
/*143*/u8"Brak numeru zamówienia w opisie nowej ściany",
/*144*/u8"Podaj współrzędną Z : ",
/*145*/u8"Podaj SYMBOL otworu, lub {Esc}: ",
/*146*/u8"Podaj TYP otworu, lub {Esc} : ",
/*147*/u8"Podaj długość osi [mm] : ",
/*148*/u8"Podaj SYMBOL siatki, lub {Esc} : ",
/*149*/u8"Podaj OPIS siatki, lub {Esc} : ",
/*150*/u8"Błąd podziału płyt na prefabrykaty",
/*151*/u8"Brak numeru zamówienia w opisie nowego stropu",
/*152*/u8"Brak płyt",
/*153*/u8"Podaj opis pierwszego punktu sieci : ",
/*154*/u8"Podaj wartość autopanoramy 0.01...1 : ",
/*155*/u8"Wskaż obiekt bloku sieci do wstawienia przyłącza : ",
/*156*/u8"Błąd otwarcia pliku raportu tras sieci",
/*157*/u8"Brak definicji tras sieci w pliku",
/*158*/u8"Błąd czytania pliku raportu tras sieci",
/*159*/u8"Wskaż sieć do generowania profilu",
/*160*/u8"..........Ignorować tło przy wydruku ?",
/*161*/u8"Podaj kolor tła (lub ESC)",
/*162*/u8"Ignorować tło ?",
/*163*/u8"Podaj narzucony kolor linii (lub ESC)",
/*164*/u8"Inwersja koloru B/W przy wydruku ?",
/*165*/u8"Podaj długość zakotwienia siatki stykowej [*d]: ",
/*166*/u8"Podaj długość zakotwienia prętów głównych [cm]: ",
/*167*/u8"Podaj dystans prętów rozdzielczych od krawędzi [cm]: ",
/*168*/u8"Podaj dystans dzwigarów kratowych od krawędzi [cm]: ",
/*169*/u8"Podaj minimalną długość pręta w specyfikacji [cm]: ",
/*170*/u8"Podaj minimalną długość siatki stykowej w specyf.[cm]: ",
/*171*/u8"Podaj tolerancję długości pręta w specyfikacji [cm]: ",
/*172*/u8"Podaj długość zakładek siatek górnych [*d]: ",
/*173*/u8"Zaznaczone obiekty nie tworzą obszaru zamkniętego.",
/*174*/u8"Napewno utworzyć płytę ? (Tak / Nie / Esc): ",
/*175*/u8"Podaj numer prefabrykatu: ",
/*176*/u8"Podaj numer siatki: ",
/*177*/u8"Podaj numer specyfikacji: ",
/*178*/u8"Podaj długość oparcia belek stropowych [cm]: ",
/*179*/u8"Generować profil ?",
/*180*/u8"Błąd zapisu pliku raportu tras sieci",
/*181*/u8"Podaj OPIS płyty, lub {Esc} : ",
/*182*/u8"Wskaż blok arkusza (@SHEET)",
/*183*/u8"Edytować blok obrazu ?",
/*184*/ u8"Podaj kąt obrotu blokuo brazu (°): ",
/*185*/u8"Wczytać ponownie zmieniony blok obrazu ? (Tak / Nie / Esc): ",
/*186*/u8"<Blok parametryczny> Podaj skalę X profilu: ",
/*187*/u8"<Blok parametryczny> Podaj poziom porównawczy PP: ",
/*188*/u8"<Blok parametryczny> Podaj typ profilu (0..2) : ",
/*189*/u8"<Blok parametryczny> Podaj wartość pierwszego hektometra: ",
/*190*/u8"Wskaż sieć do generowania zegarów",
/*191*/u8"Wskaż obiekt dla określenia warstwy ukrywanej",
/*192*/u8"Podaj liczbę wierzchołków polilinii aproksymującej okrąg lub łuk w skali anizotropowej: ",
/*193*/u8"Podaj domyślną wartość maksymalnego rozmiaru pamięci dla rysunku [KB]: ",
/*194*/u8"Podaj domyślną wartość rozmiaru bufora wydruku [KB]: ",
/*195*/u8"Podaj numer warstwy: ",
/*196*/u8"Wskaż obszar lub ślad wypełniony wzorem, aby określić parametry wypełnienia",
/*197*/u8"Wskaż obszar lub ślad wypełniony wzorem, aby zmienić parametry wypełnienia",
/*198*/u8"Podaj stały współczynnik skali bloku: ",
/*199*/u8"Wprowadź współczynnik przeskalowania siły: ",
/*200*/u8"Wprowadź współczynnik przeskalowania momentu: ",
/*201*/u8"Wprowadź współczynnik przeskalowania przemieszczenia: ",
/*202*/u8"Wprowadź współczynnik przeskalowania obrotu: ",
/*203*/u8"Wprowadź współczynnik przeskalowania obciążenia: ",
/*204*/u8"Wprowadź współczynnik przeskalowania obciążenia termicznego: ",
/*205*/u8"Wprowadź precyzję siły: ",
/*206*/u8"Wprowadź precyzję momentu: ",
/*207*/u8"Wprowadź precyzję przemieszczenia: ",
/*208*/u8"Wprowadź precyzję obrotu: ",
/*209*/u8"Wprowadź precyzję obciążenia: ",
/*210*/u8"Wprowadź precyzję obciążenia termicznego: ",
/*211*/u8"Wprowadź numer własności: ",

/*212*/u8"Wprowadź współczynnik przeskalowania wynikowych sił osiowych: ",
/*213*/u8"Wprowadź współczynnik przeskalowania wynikowych sił ścinających: ",
/*214*/u8"Wprowadź współczynnik przeskalowania wynikowych momentów gnących: ",
/*215*/u8"Wprowadź współczynnik przeskalowania wynikowych przemieszczeń: ",
/*216*/u8"Wprowadź współczynnik przeskalowania wynikowych sił reakcji podpór: ",
/*217*/u8"Wprowadź współczynnik przeskalowania wynikowych momentów reakcji podpór: ",

/*218*/u8"Wprowadź współczynnik przeskalowania głębokości przekroju: ",
/*219*/u8"Wprowadź współczynnik przeskalowania wynikowych naprężeń (stal, drewno): ",
/*220*/u8"Wprowadź precyzję naprężeń: ",
/*221*/u8"Wprowadź współczynnik przeskalowania odkształcenia modalnych postaci drgań: ",
/*222*/u8"Wprowadź współczynnik przeskalowania dla wynikowego procentu zbrojenia: ",
/*223*/u8"Wprowadź współczynnik przeskalowania obciążenia płaszczyznowego: ",
/*224*/u8"Wprowadź współczynnik przeskalowania wynikowych naprężeń (żelbet):",
"",
"",
};

static char* komunikaty0[] =
/*0*/{ u8"",
/*1*/u8"Tekst",
/*2*/u8"Ciągnij - do punktu",
/*3*/u8"Ciągnij - od punktu",
/*4*/u8"Przesuń - od punktu",
/*5*/u8"Przesuń - do punktu",
/*6*/u8"Wymiarowanie",
/*7*/u8"Linia od punktu",
/*8*/u8"Linia do punktu",
/*9*/u8"Okrag - środek",
/*10*/u8"Okrag - promień",
/*11*/u8"Zaokrąglenie - Wskaż pierwszy obiekt",
/*12*/u8"Zaokrąglenie - Wskaż drugi obiekt",
/*13*/u8"Łuk - trzy punkty",
/*14*/u8"Łuk - początek-środek-koniec",
/*15*/u8"Łuk - początek-środek-kąt",
/*16*/u8"Łuk - początek-środek-cięciwa",
/*17*/u8"Łuk - początek-koniec-promień",
/*18*/u8"Łuk - początek-koniec-kąt",
/*19*/u8"Łuk - początek-koniec-kierunek",
/*20*/u8"Łuk - kontynuacja",
/*21*/u8"Wskaż obiekty do usunięcia",
/*22*/u8"Wskaż obiekty do przesunięcia",
/*23*/u8"Wskaż obiekty do kopiowania",
/*24*/u8"Kopiuj - od punktu",
/*25*/u8"Kopiuj - do punktu",
/*26*/u8"Wskaż obiekty do zmiany skali",
/*27*/u8"Punkt bazowy",
/*28*/u8"Skala",
/*29*/u8"Narysuj kwadrat",
/*30*/u8"Okno od punktu",
/*31*/u8"Okno do punktu",
/*32*/u8"Wskaż obiekty do obrotu",
/*33*/u8"Obrót",
/*34*/u8"Wskaż okienkiem obiekty do ciągnięcia",
/*35*/u8"Brak pamięci",
/*36*/u8"Wskaż obiekty do zapisu do zbioru",
/*37*/u8"Wskaż punkt wstawienia (Home +90°, End -90°)",
/*38*/u8"Przekrój profilu",
/*39*/u8"Edycja tekstu",
/*40*/u8"Kolor, Kąt, Wysokość, Czcionka",    /*nie uzywane*/
/*41*/u8"Wskaż bloki do rozbicia",
/*42*/u8"Koło - środek",
/*43*/u8"Koło - promień",
/*44*/u8"Fazowanie - Wskaż pierwszą linię",
/*45*/u8"Fazowanie - Wskaż drugą linię",
/*46*/u8"Wycinanie - Wskaż obiekt",
/*47*/u8"Wycinanie - Wskaż pierwszy punkt",
/*48*/u8"Wycinanie - Wskaż drugi punkt",
/*49*/u8"Autozapis do zbioru BACKUP.ALF",
/*50*/u8"Wskaż obiekty do zmiany cech",
/*51*/u8"Wskaż krawędzie wydłużenia",
/*52*/u8"Wskaż obiekty do wydłużenia",
/*53*/u8"Wskaż wierzchołek obszaru",
/*54*/u8"Wskaż punkt",
/*55*/u8"Odległość - Pierwszy punkt",
/*56*/u8"Odległość - Drugi punkt",
/*57*/u8"Wielokąt - pierwszy punkt krawędzi",
/*58*/u8"Wielokąt - wpisany w okrąg",
/*59*/u8"Wielokąt - opisany na okręgu",
/*60*/u8"Wielokąt - drugi punkt krawędzi",
/*61*/u8"Wielokąt - promień okregu",
/*62*/u8"Szkic - Wskaż punkt początkowy",
/*63*/u8"Szkic",
/*64*/u8"Wskaż obiekty do grupowania",
/*65*/u8"Kopiuj równolegle do punktu",
/*66*/u8"Kopiuj równolegle o dystans",
/*67*/u8"Wskaż obiekt do kopiowania równoległego",
/*68*/u8"Wskaż krawędzie obciniania",
/*69*/u8"Wskaż obiekt do obcięcia",
/*70*/u8"Wskaż obiekty do lustrzanego odbicia",
/*71*/u8"Pierwszy punkt linii symetrii (osi)",
/*72*/u8"Drugi punkt linii symetrii (osi)",
/*73*/u8"Wskaż obiekty do kopiowania",  	/*prostokątego*/
/*74*/u8"Wskaż obiekty do kopiowania",		/*biegunowego*/
/*75*/u8"Wskaż oknem odlegości kolumn i wierszy",
/*76*/u8"Wskaż punkt środkowy",
/*77*/u8"Wskaż punkt bazowy",
/*78*/u8"Ślad od punktu",
/*79*/u8"Ślad do punktu",
/*80*/u8"Polilinia od punktu",
/*81*/u8"Wskaż linię lub łuk do kontynuacji",
/*82*/u8"Wskaż obiekt do podziału",
/*83*/u8"Wskaż obiekt do zmierzenia",
/*84*/u8"Kąt - Trzeci punkt",
/*85*/u8"Kąt - Pierwszy punkt",
/*86*/u8"Kąt - Drugi punkt",
/*87*/u8"Wskaż granice kreskowania",
/*88*/u8"Wskaż wnętrze obszaru (Home +45°,End -45°,PgUp +3°,PgDn -3°)",
/*89*/u8"Wskaż początek pierwszej osi elipsy",
/*90*/u8"Wskaż koniec pierwszej osi elipsy",
/*91*/u8"Wskaż centrum elipsy",
/*92*/u8"Wskaż długość drugiej półosi",
/*93*/u8"Wskaż punkt początkowy wykresu",
/*94*/u8"Wskaż punkt końcowy wykresu", /* (zakres)*/
/*95*/u8"Wskaż obiekty do szybkiego usunięcia",
/*96*/u8"",
/*97*/u8"Wskaż granice obszaru",
/*98*/u8"Wskaż wnętrze obszaru",
/*99*/u8"",
/*100*/u8"2-linia od punktu",
/*101*/u8"2-linia do punktu",
/*102*/u8"Oś - Pierwszy punkt",
/*103*/u8"Oś - Drugi punkt",
/*104*/u8"3-linia od punktu",
/*105*/u8"3-linia do punktu",
/*106*/u8"Zmiana formatu danych rysunku...",
/*107*/u8"Uaktualnienie rysunku w nowym formacie",
/*108*/u8"Zmiana formatu danych bloku...",
/*109*/u8"Uaktualnienie bloku w nowym formacie",
/*110*/u8"Polilinia od punktu",
/*111*/u8"Polilinia do punktu",
/*112*/u8"Wskaż bloki zamrożone (*,$) do rozbicia",
/*113*/u8"Wejście ->  wskaż nazwę pliku rysunku",
/*114*/u8"Wskaż oknem wymiary aparatu",
/*115*/u8"Wskaż oknem rozmiar symbolu Schematu +",
/*116*/u8"Wskaż obiekty do specyfikacji aparatów",
/*117*/u8"Wskaż oknem wymiary prostokąta",
/*118*/u8"Wskaż teksty do zmiany",
/*119*/u8"Wskaż teksty (symbole) do zmiany",
/*120*/u8"Biblioteka symboli  ->  wybierz symbol :",
/*121*/u8"Tekst (PgUp +3°, PgDn -3°, Home +90°, End -90°, 'E' edycja)",
/*122*/u8"Wskaż wnętrze obszaru (Home +45°,End -45°,PgUp +3°,PgDn -3°)",
/*123*/u8"Wskaż punkt początkowy krzywej Bezier'a",
/*124*/u8"Wskaż punkt kontrolny krzywej Bezier'a",
/*125*/u8"",
/*126*/u8"Oś od punktu:",
/*127*/u8"Oś w punkcie:",
/*128*/u8"czytanie...",
/*129*/u8"czekanie...",
/*130*/u8"",
/*131*/u8"",
/*132*/u8"",
/*133*/u8"Wskaż obiekt do zapamiętania w schowku",
/*134*/u8"Wskaż blok obrazu do zmiany cech",
/*135*/u8"Wskaż blok obrazu do exportu",
/*136*/u8"Buforowanie obrazu",
/*137*/u8"Przerwanie --> Esc ",
/*138*/u8"Wskaż blok kreskowania do zmiany wzoru (Home +45°,End -45°,PgUp +3°,PgDn -3°)",
/*139*/u8"Wskaż wnętrze obszaru (Home +90°,End -90°)",
/*140*/u8"Wskaż wnętrze obszaru",
/*141*/u8"Wskaż splajn do zmiany napięcia w węzłach",
/*142*/u8"Wskaż wielkość napięcia w węzłach",
/*143*/u8"Panoramuj od punktu",
/*144*/u8"Panoramuj do punktu",
/*145*/u8"Linia odniesienia od punktu",
/*146*/u8"Linia odniesienia do punktu",
/*147*/u8"Wybierz obiekt do transformacji czworobocznej",
/*148*/u8"Konwertowanie pliku do formatu BMP",
/*149*/u8"Konwertowanie pliku do formatu PBM",
/*150*/u8"Konwertowanie pliku do formatu ALX",
/*151*/u8"Wskaż kąt początkowy łuku eliptycznego",
/*152*/u8"Wskaż kąt końcowy łuku eliptycznego"
/*153*/u8"Wybierz linię lub bok wielokąta albo bryły, do której elipsa powinna być styczna",
/*154*/u8"Wskaż położenie punktu na elipsie dla wyznaczenia drugiej półosi",
/*155*/u8"Wektor od punktu",
/*156*/u8"Wektor do punktu",
/*157*/u8"Wektor od punktu  (Home->odwróć strzałki, End->zamień wartości)",
/*158*/u8"Wektor do punktu  (Home->odwróć strzałki, End->zamień wartości)",
/*159*/u8"",
/*160*/u8"",
/*161*/u8"",
/*162*/u8"",
/*163*/u8"",
/*164*/u8"",
/*165*/u8"",
/*166*/u8"",  //custom text
/*167*/u8"Wskaż schemat ramy lyb kratownicy do analizy statycznej/dynamicznej",
/*168*/u8"Wymiarowanie (aby rozpocząć wymiarowanie kąta, dotknij dowolnej linii)",
/*169*/u8"Wskaż element ramy lub kratownicy",
/*170*/u8"Umieść linię przekrojową w żądanych współrzędnych elementu. Kliknij aby zapisać wartości w schowku",
/*171*/u8"Wskaż schemat płyty do analizy statycznej (MES)",
};

static char* messages_str[] =
/*0*/{u8"Łuk - początek-punkt-koniec",
/*1*/u8"Łuk - początek-środek-koniec",
/*2*/u8"Łuk - początek-środek-kąt",
/*3*/u8"Łuk - początek-środek-cięciwa",
/*4*/u8"Łuk - początek-koniec-promień",
/*5*/u8"Łuk - początek-koniec-kąt",
/*6*/u8"Łuk - początek-koniec-kierunek",
/*7*/u8"Łuk - kontynuacja",
/*8*/u8"",
/*9*/ u8"Dysk %s",
/*10*/u8"%s - ŚRODEK - promień",
/*11*/u8"%s - środek - PROMIEŃ",
/*12*/u8"%s - środek - ŚREDNICA",
/*13*/u8"%s - pierwszy punkt średnicy",
/*14*/u8"%s - drugi punkt średnicy",
/*15*/u8"%s - pierwszy punkt",
/*16*/u8"%s - drugi punkt",
/*17*/u8"%s - trzeci punkt",
/*18*/u8"%s - wskaż pierwszy obiekt",
/*19*/u8"%s - wskaż drugi obiekt",
/*20*/u8"%s - ŚRODEK - średnica",
/*21*/u8"",
/*22*/u8"",
/*23*/u8"DXF-> %s",
/*24*/u8"%s - pierwszy punkt promienia",
/*25*/u8"%s - drugi punkt promienia",
};

#define ORTHO u8"   Prost."
#define _LAYER_ u8"Warstwa"
#define _MODE_ u8"Tryb"
#define _V_ u8"W"
#define _E_ u8"E"
#define _P_ u8"P"

static char gg[10][16] = { u8"Koniec", u8"Bliższy koniec", u8"Środek", u8"Przecięcie", u8"Centrum", u8"pRostopadle",
						  u8"Stycznie", u8"Najbliższy", u8"punkt", u8"----" };

#endif

#ifdef __O_LUK__
/*static*/ char* komunikaty_arc[] = {
   u8"Łuk - początek-punkt-koniec",
   u8"Łuk - początek-środek-koniec",
   u8"Łuk - początek-środek-kąt",
   u8"Łuk - początek-środek-cięciwa",
   u8"Łuk - początek-koniec-promień",
   u8"Łuk - początek-koniec-kąt",
   u8"Łuk - początek-koniec-kierunek",
   u8"Łuk - kontynuacja", };

#define r6 7

#endif

#ifdef __BUF__

#define _NOT_ENOUGH_MEMORY_ u8"BRAK PAMIĘCI LUB BŁĄD PROGRAMOWY"

#endif

#ifdef __DXF_O__

#define _PCX_FILE_ERROR_  u8"Błąd zapisu pliku PCX"
#define _PNG_FILE_ERROR_  u8"Błąd zapisu pliku PNG"
#define _UNKNOWN_IMAGE_FORMAT_ u8"Nieznany format pliku obrazu"
#define _FILE_ u8"Zbiór "
#define _FILE_EXISTS_ u8" istnieje. Zapisć ? (Tak/Nie/Esc):"

#define _YES_NO_ESC_ u8"TNtn\033"
#define _YES_ 'T'
#define _yes_ 't'
#define _NO_ 'N'
#define _no_ 'n'

#endif

#ifdef __O_ZB__

#define _DRAWING_V_1_ u8"Rysunek <wer.1.0> Przeprowadź konwersję programem ALF w.1.1 - 1.9"
#define _BLOCK_V_1_ u8"Blok <wer.1.0> Przeprowadz konwersji programem ALF w.1.1 - 1.9"

#define VECTORIZATION u8"Wektoryzacja"

POLE pmNie_Tak[] = {
	{u8"Nie",'N',113,NULL},
	{u8"Tak",'T',112,NULL} };

char zb_err_message[] = u8"Błąd konwersji pliku.Kod błędu :";
char zb_err_message1[] = u8"Błąd konwersji pliku";
char zb_unknown[] = u8"Nieznany";
static char zb_confirm[] = u8"Potwierdź";

#define _FILE_CONVERSION_ERROR_ u8"Błąd konwersji pliku "
#define _INCORRECT_FILE_FORMAT_ u8"Incorrect file format"
#define _ERROR_WREITING_AUX_FILE_ u8"Błąd zapisu pliku roboczego"
#define _FILE_NOT_FOUND_ u8"Nie znaleziono zbioru "

static char* vectorization_param[] =
{
	u8"skala PBM",
	u8"interpolacja PBM",
	u8"prog PBM",
	u8"odwrocenie PBM",
	u8"filtr PBM",
	u8"bez filtru PBM",
    u8"skala ALX",
    u8"poziom czerni ALX",
    u8"rozmiar plamy ALX",
    u8"prog naroznika ALX",
    u8"tolerancja optymalizacji krzywej ALX",
    u8"kwantyzacja ALX"
};

static int no_vectorization_param = sizeof(vectorization_param) / sizeof(vectorization_param[0]);

#define _YES_NO_ESC_ u8"TNtn\033"
#define _YES_ 'T'
#define _yes_ 't'
#define _NO_ 'N'
#define _no_ 'n'
#define _YES__ "T"
#define _NO__ "N"

#define _Yes_ u8"Tak"
#define _No_ u8"Nie"

#define _TOOLS_TO_INSTALL_ u8"nie jest zainstalowane"
#define _INSTALL_TOOLS_ u8"Zainstaluj niezbędne pakiety"

#endif

#ifdef __WINSIZE__

TCHAR greeting1[] = _T("Ustaw okno rysunku");
TCHAR greeting2[] = _T("Użyj krawędzi, aby rozciągnąć okno");
TCHAR greeting3[] = _T("Użyj nagłówka, aby przesunąć okno");
TCHAR greeting4[] = _T("Kliknij wewnątrz, aby zakończyć");

#endif

#ifdef __PCX__

#define _PROCESSED_ u8"przetworzono "

#endif

#ifdef __O_HATCH1__

char _TO_MANY_OBJECTS_[]= u8"Wybrano zbyt wiele obiektów, aby kontynuować (maksymalnie 6000)";

#endif

#ifdef __O_HATCH3__

char* vocabulary[] = { (char*)u8"   naciśnij klawisz:",(char*)u8"A", (char*)u8"środek ciężkości",(char*)u8"Moment statyczny pola",(char*)u8"Moment bezwładności" };

#endif

#ifdef __O_LTTYPE__

char* add_new_font = u8"dodaj nową czcionkę";
#define LINE u8"Linie"

char lin_config_param[31][MaxLenNameLine] =
{
	"Kreskowa",
	"Punktowa",
	"Dwupunktowa",
	"Wielopunktowa",
	"Granica",
	"Srodkowa",
	"Ukryta",
	"Widmo",
	"Kreskowa_2",
	"Punktowa_2",
	"Dwupunktowa_2",
	"Wielopunktowa_2",
	"Granica_2",
	"Srodkowa_2",
	"Ukryta_2",
	"Widmo_2",
	"Kreskowa_x2",
	"Punktowa_x2",
	"Dwupunktowa_x2",
	"Wielopunktowa_x2",
	"Granica_x2",
	"Srodkowa_x2",
	"Ukryta_x2",
	"Widmo_x2",
	"Trzypunktowa",
	"Dwusrodkowa",
	"Trzykreskowa",
	"Widmo_3",
	"Dwusrodkowa_x2",
	"Trzykreskowa_x2",
	"Wielokreskowa",
};

#define CZCIONKI "Czcionki"
#define CZCIONKI_DXF "Czcionki-DXF"

char str[40] = u8"Brak zbioru : ";

#endif

#ifdef __O_MEASURE__

char measure_comm[9][54] = { u8"pole", u8"Punkt", u8"Nowy kąt lokalnego układu współrzędnych", u8"   ->naciśnij klawisz:", u8"Kąt", u8"Aktualny kąt lokalnego układu współrzędnych:", u8"Zaznacz na obrazie 2 punkty bazowe", u8"Zaznacz na rysunku 2 punkty kartometryczne", u8"Kąt" };
#define _SUBSTRACT_ u8"odejmij"
#define _ADD_ u8"dodaj"

#endif

#ifdef __O_CONFIG__

static char konf_global[8] = "Global.";
static char konf_lokal[8] = "Lokal. ";
static char local_global[8] = "Lokal. ";

#define config_labels0 u8"Ramka"
#define config_labels1 u8"Tło"
#define config_labels2 u8"Tekst"
#define config_labels3 u8"Kursor"
#define config_labels4 u8"Blok"
#define config_labels5 u8"Ramka"
#define config_labels6 u8"Tło"
#define config_labels7 u8"Tekst"
#define config_labels8 u8"Tło edycji"
#define config_labels9 u8"Tekst edycji"
#define config_labels10 u8"konfig.:"
#define config_labels11 u8"  inny"

#define config_groups0 u8"KOLORY MENU"
#define config_groups1 u8"KOLORY PULPITU"

#define config_buttons0 u8"OK"
#define config_buttons1 u8"Esc"
#define config_buttons2 u8"" //"Globalne"
#define config_buttons3 u8"" //"Lokalne"
#define config_buttons4 u8"Zapisz"
#define config_title u8"K O N F I G U R A C J A"

#define image_conf_tips0  u8"@Globalne"
#define image_conf_tips1  u8"Lokalne"

#endif

#ifdef __O_PRNFUN__

static char err_message[] = u8"Błąd konwersji pliku.Kod błędu :";
static char err_message_cups[] = u8"Błąd wykonania komendy 'lp'.Kod błędu :";
static char confirm[] = u8"Potwierdź";

#define _CANNOT_CREATE_PDF_ u8"błąd: nie można utworzyć dokumentu PDF\n"
#define _FILE_WRITING_ERROR_ u8"Błąd zapisu pliku obrazu"

#endif

#ifdef __O_SECTION_DATA__
static char confirm[] = u8"Potwierdź";
#define _CANNOT_OPEN_ u8"Nie można otworzyć "
#define _CANNOT_READ_ u8"Nie można odczytać "
#define _DEFAULT_TAKEN_ u8"Przyjęto domyślne parametry drewna"
#endif

#ifdef __O_STATIC__

#define _SELECT_STATE_ "Stan graniczny (kombinacja)"
#define _SELECT_STATE_C_ L'S'

/*
POLE pmSelect_State[] = {
	{u8"Nośności (SGN)",L'N',0,NULL},
	{u8"Użytkowalności (SGU)",L'U',0,NULL},
    {u8"użytkowalności (Quasi-stałe)",L'Q',0,NULL},
};

static TMENU mSelect_State = { 3,0,0,32,20,7, 0,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0, (POLE(*)[]) &pmSelect_State,NULL,NULL };
*/

#define _PROCEED_STATIC_ u8"Przeprowadzić analizę statyczną wskazanej ramy lub kratownicy?"

#define _incorrectly_defined_ u8"błędnie zdefiniowana"
#define _property_not_defined_ u8"właściwość nie zdefiniowana"
#define _reaction_not_associated_ u8"reakcja niezwiązana z żadnym znanym węzłem"
#define _reaction_not_enough_in_Y_ u8"Konstrukcja nie jest wystarczająco podparta w osi Y. Dodaj brakujące podparcie."
#define _reaction_not_enough_in_X_ u8"Konstrukcja nie jest wystarczająco podparta w osi X. Dodaj brakujące podparcie."
#define _reaction_not_enough_in_X_Y_ u8"Konstrukcja nie jest wystarczająco podparta w osiach X i Y. Dodaj brakujące podparcia."
#define _node_size_not_associated_ u8"rozmiar węzła niepowiązany z żadnym znanym węzłem"
#define _force_not_associated_ u8"niepowiązana(y) z żadnym znanym węzłem"
#define _load_not_associated_ u8"niepowiązane z żadnym znanym elementem"
#define _thermal_load_inside_element_ u8"nie na całym elemencie o współrzędnych węzłów:"
#define _unknown_standard_ u8"Nieznany standard"
#define _element_graph_data_failed_ u8"Nie udało się utworzyć bloku danych sił wynikowych dla elementu"
#define _cannot_create_folder_ u8"Nie można utworzyć katalogu plików"

#define _FRAME3DD_ u8"%FRAME:"
#define _FRAME3DD_PL u8"%RAMA:"
#define _FRAME3DD_UA u8"%КАРКАС:"
#define _FRAME3DD_ES u8"%MARCO:"

#define _Yes_ "Tak"
#define _No_ "Nie"
#define _YES_NO_ESC_ u8"TNtn\033"
#define _YES_ 'T'
#define _yes_ 't'
#define _NO_ 'N'
#define _no_ 'n'

#define __FRAME3DD__ "Frame3dd"

static char confirm[] = u8"Potwierdź";
#define _CANNOT_CREATE_DEFORMATION_BLOCK_ u8"Nie można utworzyć bloku kształtu deformacji"
#define _CANNOT_CREATE_FORCE_BLOCK_ u8"Nie można utworzyć bloku wykresu sił"
#define _CANNOT_CREATE_MOMENT_BLOCK_ u8"Nie można utworzyć bloku wykresu momentów"
#define _CANNOT_CREATE_STRESS_BLOCK_ u8"Nie można utworzyć bloku wykresu naprężeń"
#define _CANNOT_CREATE_SHEAR_STRESS_BLOCK_ u8"Nie można utworzyć bloku wykresu naprężeń ścinających"
#define _CANNOT_CREATE_RESULTS_FILE_ u8"Nie można otworzyć pliku wyników"
#define _CANNOT_CREATE_RESULTS_PDF_FILE_ u8"Nie można utworzyć pliku wyników w formacie PDF"
#define _CANNOT_OPEN_RESULTS_PDF_FILE_ u8"Nie można otworzyć pliku PDF z wynikami"
#define _INSTALL_PDF_VIEWER_ u8"Proszę zainstalować przeglądarkę PDF \"Okular\" lub \"Evince\""

#define _CANNOT_OPEN_DEFORMATION_DATA_FILE_ u8"Nie można otworzyć pliku z danymi deformacji"
#define _CANNOT_OPEN_RESULTS_DATA_FILE_ u8"Nie można otworzyć pliku danych wyników"
#define _CANNOT_OPEN_DYNAMIC_RESULTS_DATA_FILE_ u8"Nie można otworzyć pliku danych wyników dynamicznych"
#define _CANNOT_CREATE_NODES_AND_ELEMENTS_BLOCK_ u8"Nie można utworzyć bloku węzłów i elementów"
#define _CANNOT_CREATE_REACTIONS_BLOCK_ u8"Nie można utworzyć bloku reakcji"
#define _CANNOT_CREATE_NEW_LAYER_ u8"Nie można utworzyć nowej warstwy. Utworzono już zbyt wiele warstw"

char *frame3dd[]={
 /*0*/ u8"zakończenie bez błędów",
 /*1*/ u8"nieznany błąd",
 /*2*/ u8"błąd z opcjami wiersza poleceń (patrz sekcja 11 powyżej)",
 /*3*/ u8"błąd z opcją wiersza poleceń dla odkształcenia przy ścinaniu -s",
 /*4*/ u8"błąd z opcją wiersza poleceń dotyczącą sztywności geometrycznej -g",
 /*5*/ u8"błąd z opcją wiersza poleceń dla masy skupionej -l",
 /*6*/ u8"błąd z opcją wiersza poleceń dla metody analizy modalnej -m",
 /*7*/ u8"błąd z opcją wiersza poleceń dla tolerancji analizy modalnej -t",
 /*8*/ u8"błąd z opcją wiersza poleceń dotyczącą przesunięcia analizy modalnej -f",
 /*9*/ u8"błąd z opcją wiersza poleceń dla szybkości panoramowania -p",
 /*10*/ u8"błąd z opcją wiersza poleceń dla kondensacji macierzy -r",
 /*11*/ u8"błąd podczas otwierania pliku danych wejściowych",
 /*12*/ u8"błąd podczas otwierania tymczasowo oczyszczonego pliku danych wejściowych do zapisu",
 /*13*/ u8"błąd podczas otwierania tymczasowo oczyszczonego pliku danych wejściowych do odczytu",
 /*14*/ u8"błąd podczas otwierania pliku danych wyjściowych",
 /*15*/ u8" błąd podczas tworzenia ścieżki do tymczasowych plików danych wyjściowych",
 /*16*/ u8" błąd podczas tworzenia nazwy ścieżki tymczasowego pliku danych wyjściowych",
 /*17*/ u8"błąd podczas otwierania pliku danych wyjściowych .CSV (arkusz kalkulacyjny)",
 /*18*/ u8"błąd podczas otwierania pliku danych wyjściowych .M (matlab)",
 /*19*/ u8"błąd podczas otwierania pliku danych wyjściowych siły wewnętrznej do zapisu",
 /*20*/ u8"błąd podczas otwierania pliku danych wyjściowych sił wewnętrznych do odczytu",
 /*21*/ u8"błąd podczas otwierania pliku danych wyjściowych niezdeformowanej siatki",
 /*22*/ u8"błąd podczas otwierania pliku danych wyjściowych zdeformowanej siatki",
 /*23*/ u8"błąd podczas otwierania pliku skryptu kreślącego w celu zapisania wykresów pierwszego przypadku obciążenia statycznego",
 /*24*/ u8"błąd podczas otwierania pliku skryptu kreślącego w celu dołączenia wyników drugiego i wyższych przypadków obciążenia statycznego",
 /*25*/ u8"błąd podczas otwierania pliku skryptu kreślącego w celu dołączenia wykresów modalnych",
 /*26*/ u8"błąd podczas otwierania pliku skryptu kreślącego w celu dołączenia animacji modalnych",
 /*27*/ u8"błąd podczas otwierania pliku danych siatki modalnej",
 /*28*/ u8"błąd podczas otwierania pliku danych animacji siatki modalnej",
 /*29*/ u8"błąd podczas otwierania pliku debugowania danych masowych, MassData.txt",
 /*30*/ u8"Macierz systemu dopasowania krzywej sześciennej dla odkształcenia elementu nie jest określona dodatnio",
 /*31*/ u8"niedodatnia określona macierz sztywności statycznej strukturalnej",
 /*32*/ u8"błąd w analizie wartości własnych. Spróbuj ponownie ze zmniejszoną lub zerową liczbą postaci drgań",
 /*33*/ u8"zakończenie bez błędów",
 /*34*/ u8"zakończenie bez błędów",
 /*35*/ u8"zakończenie bez błędów",
 /*36*/ u8"bezbłędne zakończenie",
 /*37*/ u8"zakończenie bez błędów",
 /*38*/ u8"zakończenie bez błędów",
 /*39*/ u8"zakończenie bez błędów",
 /*40*/ u8"błąd w pliku danych wejściowych",
 /*41*/ u8"Błąd formatowania danych wejściowych w danych węzła, numer węzła poza zakresem",
 /*42*/ u8"Błąd formatowania danych wejściowych w danych węzła lub elementu, węzeł niepołączony",
 /*43*/ u8"zakończenie bez błędów",
 /*44*/ u8"zakończenie bez błędów",
 /*45*/ u8"zakończenie bez błędów",
 /*46*/ u8"zakończenie bez błędów",
 /*47*/ u8"zakończenie bez błędów",
 /*48*/ u8"zakończenie bez błędów",
 /*49*/ u8"zakończenie bez błędów",
 /*50*/ u8"zakończenie bez błędów",
 /*51*/ u8"błąd formatowania danych wejściowych w danych elementu ramki, numer elementu ramki poza zakresem",
 /*52*/ u8"błąd formatowania danych wejściowych w danych elementu ramki, numer węzła poza zakresem",
 /*53*/ u8"błąd formatowania danych wejściowych w danych elementu ramki, wartość przekroju ujemnego",
 /*54*/ u8"błąd formatowania danych wejściowych w danych elementu ramki, pole przekroju poprzecznego wynosi 0 (zero)",
 /*55*/ u8"błąd formatowania danych wejściowych w danych elementu ramki, powierzchnia ścinania i moduł ścinania wynoszą 0 (zero)",
 /*56*/ u8"błąd formatowania danych wejściowych w danych elementu ramy, skrętny moment bezwładności wynosi 0 (zero)",
 /*57*/ u8"błąd formatowania danych wejściowych w danych elementu ramy, moment bezwładności zginający wynosi 0 (zero)",
 /*58*/ u8"błąd formatowania danych wejściowych w danych elementu ramki, wartość modułu jest niedodatnia",
 /*59*/ u8"błąd formatowania danych wejściowych w danych elementu ramki, wartość gęstości masy jest niedodatnia",
 /*60*/ u8"Błąd formatowania danych wejściowych w danych elementu ramki, element ramki zaczyna się i kończy w tym samym węźle",
 /*61*/ u8"błąd formatowania danych wejściowych w danych elementu ramki, element ramki ma długość zero",
 /*62*/ u8"zakończenie bez błędów",
 /*63*/ u8"zakończenie bez błędów",
 /*64*/ u8"zakończenie bez błędów",
 /*65*/ u8"bezbłędne zakończenie",
 /*66*/ u8"zakończenie bez błędów",
 /*67*/ u8"zakończenie bez błędów",
 /*68*/ u8"bezbłędne zakończenie",
 /*69*/ u8"zakończenie bez błędów",
 /*70*/ u8"zakończenie bez błędów",
 /*71*/ u8"błąd formatowania danych wejściowych ze zmienną 'shear' określającą odkształcenie przy ścinaniu",
 /*72*/ u8"błąd formatowania danych wejściowych ze zmienną 'geom' określającą sztywność geometryczną",
 /*73*/ u8"Błąd formatowania danych wejściowych ze zmienną 'exagg_static' określającą przeskalowanie siatki statycznej",
 /*74*/ u8"błąd formatowania danych wejściowych ze zmienną 'dx' określającą długość przyrostu siły wewnętrznej w osi x",
 /*75*/ u8"zakończenie bez błędów",
 /*76*/ u8"zakończenie bez błędów",
 /*77*/ u8"zakończenie bez błędów",
 /*78*/ u8"zakończenie bez błędów",
 /*79*/ u8"zakończenie bez błędów",
 /*80*/ u8"błąd formatowania danych wejściowych w danych reakcji, liczba węzłów z reakcjami poza zakresem",
 /*81*/ u8"Błąd formatowania danych wejściowych w danych reakcji, numer węzła poza zakresem",
 /*82*/ u8"Błąd formatowania danych wejściowych w danych reakcji, dane reakcji nie są równe 1 (jeden) ani 0 (zero)",
 /*83*/ u8"Błąd formatowania danych wejściowych w danych reakcji, określony węzeł nie ma żadnych reakcji",
 /*84*/ u8"błąd formatowania danych wejściowych w danych reakcji, konstrukcja niedostatecznie utwierdzona",
 /*85*/ u8"błąd formatowania danych wejściowych w danych reakcji, struktura całkowicie utwierdzona",
 /*86*/ u8"Błąd formatowania danych wejściowych w danych dotyczących bezwładności dodatkowego węzła, numer węzła poza zakresem",
 /*87*/ u8"Błąd formatowania danych wejściowych w danych masy dodatkowej wiązki, numer elementu ramki poza zakresem",
 /*88*/ u8"błąd formatowania danych wejściowych w danych masowych, element ramki o masie niedodatniej",
 /*89*/ u8"zakończenie bez błędów",
 /*90*/ u8"błąd formatowania danych wejściowych w danych kondensacji macierzy, liczba węzłów ze skondensowanymi stopniami swobody jest mniejsza niż całkowita liczba węzłów",
 /*91*/ u8"Błąd formatowania danych wejściowych w danych kondensacji matrycy, numer węzła poza zakresem",
 /*92*/ u8"Błąd formatowania danych wejściowych w danych kondensacji matrycy, numer trybu poza zakresem",
 /*93*/ u8"zakończenie bez błędów",
 /*94*/ u8"błąd formatowania danych wejściowych w danych kondensacji matrycy, liczba skondensowanych stopni swobody większa niż liczba modów",
 /*95*/ u8"zakończenie bez błędów",
 /*96*/ u8"zakończenie bez błędów",
 /*97*/ u8"zakończenie bez błędów",
 /*98*/ u8"zakończenie bez błędów",
 /*99*/ u8"zakończenie bez błędów",
 /*100*/ u8"Błąd formatowania danych wejściowych podczas ładowania danych",
 /*101*/ u8"liczba przypadków obciążeń statycznych musi być większa od zera",
 /*102*/ u8"liczba przypadków obciążeń statycznych musi być mniejsza niż 112", //64 30
 /*103*/ u8"zakończenie bez błędów",
 /*104*/ u8"zakończenie bez błędów",
 /*105*/ u8"zakończenie bez błędów",
 /*106*/ u8"zakończenie bez błędów",
 /*107*/ u8"zakończenie bez błędów",
 /*108*/ u8"zakończenie bez błędów",
 /*109*/ u8"zakończenie bez błędów",
 /*110*/ u8"zakończenie bez błędów",
 /*111*/ u8"zakończenie bez błędów",
 /*112*/ u8"zakończenie bez błędów",
 /*113*/ u8"zakończenie bez błędów",
 /*114*/ u8"zakończenie bez błędów",
 /*115*/ u8"zakończenie bez błędów",
 /*116*/ u8"zakończenie bez błędów",
 /*117*/ u8"zakończenie bez błędów",
 /*118*/ u8"zakończenie bez błędów",
 /*119*/ u8"zakończenie bez błędów",
 /*120*/ u8"zakończenie bez błędów",
 /*121*/ u8"Błąd formatowania danych wejściowych w danych obciążenia węzła, numer węzła poza zakresem",
 /*122*/ u8"zakończenie bez błędów",
 /*123*/ u8"zakończenie bez błędów",
 /*124*/ u8"zakończenie bez błędów",
 /*125*/ u8"zakończenie bez błędów",
 /*126*/ u8"zakończenie bez błędów",
 /*127*/ u8"zakończenie bez błędów",
 /*128*/ u8"zakończenie bez błędów",
 /*129*/ u8"zakończenie bez błędów",
 /*130*/ u8"zakończenie bez błędów",
 /*131*/ u8"błąd formatowania danych wejściowych w równomiernie rozłożonych danych obciążenia, liczba równomiernych obciążeń jest większa niż liczba elementów ramy",
 /*132*/ u8"Błąd formatowania danych wejściowych w równomiernie rozłożonych danych obciążenia, numer elementu ramki poza zakresem",
 /*133*/ u8"zakończenie bez błędów",
 /*134*/ u8"zakończenie bez błędów",
 /*135*/ u8"zakończenie bez błędów",
 /*136*/ u8"zakończenie bez błędów",
 /*137*/ u8"zakończenie bez błędów",
 /*138*/ u8"zakończenie bez błędów",
 /*139*/ u8"zakończenie bez błędów",
 /*140*/ u8"Błąd formatowania danych wejściowych w danych obciążenia o rozkładzie trapezowym, za dużo obciążeń o rozkładzie trapezowym",
 /*141*/ u8"Błąd formatowania danych wejściowych w danych obciążenia o rozkładzie trapezowym, numer elementu ramki poza zakresem",
 /*142*/ u8"Błąd formatowania danych wejściowych w danych obciążenia o rozkładzie trapezowym, x1 < 0",
 /*143*/ u8"Błąd formatowania danych wejściowych w danych obciążenia o rozkładzie trapezowym, x1 > x2",
 /*144*/ u8"Błąd formatowania danych wejściowych w danych obciążenia o rozkładzie trapezowym, x2 > L",
 /*145*/ u8"zakończenie bez błędów",
 /*146*/ u8"zakończenie bez błędów",
 /*147*/ u8"zakończenie bez błędów",
 /*148*/ u8"zakończenie bez błędów",
 /*149*/ u8"zakończenie bez błędów",
 /*150*/ u8"błąd formatowania danych wejściowych w skupionych danych obciążenia wewnętrznego, liczba skupionych obciążeń jest większa niż liczba elementów ramy",
 /*151*/ u8"Błąd formatowania danych wejściowych w wewnętrznych danych obciążenia skupionego, numer elementu ramki poza zakresem",
 /*152*/ u8"błąd formatowania danych wejściowych w wewnętrznych danych obciążenia skupionego, lokalizacja x mniejsza niż 0 lub większa niż L",
 /*153*/ u8"zakończenie bez błędów",
 /*154*/ u8"zakończenie bez błędów",
 /*155*/ u8"zakończenie bez błędów",
 /*156*/ u8"zakończenie bez błędów",
 /*157*/ u8"zakończenie bez błędów",
 /*158*/ u8"zakończenie bez błędów",
 /*159*/ u8"zakończenie bez błędów",
 /*160*/ u8"błąd formatowania danych wejściowych w danych obciążenia termicznego, liczba obciążeń termicznych większa niż liczba elementów ramy",
 /*161*/ u8"Błąd formatowania danych wejściowych w danych obciążenia termicznego, numer elementu ramki poza zakresem",
 /*162*/ u8"Błąd formatowania danych wejściowych w danych obciążenia termicznego, numer elementu ramki poza zakresem",
 /*163*/ u8"zakończenie bez błędów",
 /*164*/ u8"zakończenie bez błędów",
 /*165*/ u8"zakończenie bez błędów",
 /*166*/ u8"zakończenie bez błędów",
 /*167*/ u8"zakończenie bez błędów",
 /*168*/ u8"zakończenie bez błędów",
 /*169*/ u8"zakończenie bez błędów",
 /*170*/ u8"zakończenie bez błędów",
 /*171*/ u8"błąd formatowania danych wejściowych w zadanych przemieszczeniach, zadane przemieszczenia można zastosować tylko we współrzędnych z reakcjami",
 /*172*/ u8"zakończenie bez błędów",
 /*173*/ u8"zakończenie bez błędów",
 /*174*/ u8"zakończenie bez błędów",
 /*175*/ u8"zakończenie bez błędów",
 /*176*/ u8"zakończenie bez błędów",
 /*177*/ u8"zakończenie bez błędów",
 /*178*/ u8"zakończenie bez błędów",
 /*179*/ u8"zakończenie bez błędów",
 /*180*/ u8"zakończenie bez błędów",
 /*181*/ u8"Uwaga: niestabilność sprężysta (macierz sztywności sprężystej + geometrycznej nieokreślona dodatnio)",
 /*182*/ u8"Uwaga: duże odkształcenie (średnie odkształcenie osiowe w jednym lub większej liczbie elementów jest większe niż 0,001)",
 /*183*/ u8"Uwaga: duże odkształcenie i niestabilność sprężysta",
 /*184*/ u8"zakończenie bez błędów",
 /*185*/ u8"zakończenie bez błędów",
 /*186*/ u8"zakończenie bez błędów",
 /*187*/ u8"zakończenie bez błędów",
 /*188*/ u8"zakończenie bez błędów",
 /*189*/ u8"zakończenie bez błędów",
 /*190*/ u8"zakończenie bez błędów",
 /*191*/ u8"zakończenie bez błędów",
 /*192*/ u8"zakończenie bez błędów",
 /*193*/ u8"zakończenie bez błędów",
 /*194*/ u8"zakończenie bez błędów",
 /*195*/ u8"zakończenie bez błędów",
 /*196*/ u8"zakończenie bez błędów",
 /*197*/ u8"zakończenie bez błędów",
 /*198*/ u8"zakończenie bez błędów",
 /*199*/ u8"zakończenie bez błędów",
 /*200*/ u8"błąd alokacji pamięci",
 /*201*/ u8"błąd podczas otwierania pliku danych wyjściowych zapisując wektor 'zmiennoprzecinków'",
 /*202*/ u8"błąd podczas otwierania pliku danych wyjściowych podczas zapisywania wektora 'int'",
 /*203*/ u8"błąd podczas otwierania pliku danych wyjściowych zapisującego macierz 'zmiennoprzecinkowych'",
 /*204*/ u8"błąd podczas otwierania pliku danych wyjściowych zapisującego macierz 'podwójnych'",
 /*205*/ u8"błąd podczas otwierania pliku danych wyjściowych zapisującego symetryczną macierz 'zmiennoprzecinkową'",
 /*206*/ u8"błąd podczas otwierania pliku danych wyjściowych zapisując symetryczną macierz 'podwójną'",
 /*207*/ u8"błąd testu równowagi",
 /*208*/ u8"błąd podczas otwierania pliku zapisu błędów .err",
};

#define _ERROR_FREE_COMPLETION_ u8"zakończenie bez błędów"

#endif

#ifdef __O_PLATE__

#define _PLATE_ u8"%PLATE:"
#define _PLATE_PL u8"%PŁYTA:"
#define _PLATE_UA u8"%ПЛИТА:"
#define _PLATE_ES u8"%LOSA:"

#define _cannot_create_folder_ u8"Nie można utworzyć katalogu plików"
#define _CANNOT_CREATE_RESULTS_FILE_ u8"Nie można otworzyć pliku wyników"
static char confirm[] = u8"Potwierdź";

#define _gmsh_error_ u8"Błąd gmsh"
#define _ElmerGrid_error_ u8"Błąd ElmerGrid"
#define _ElmerSolver_error_ u8"Błąd ElmerSolver"

#define _CANNOT_OPEN_RESULTS_FILE_ u8"Nie można otworzyć pliku z wynikami"
#define _CANNOT_READ_RESULTS_FILE_ u8"Nie można odczytać pliku wyników"

#define _CANNOT_CREATE_NEW_LAYER_ u8"Nie można utworzyć nowej warstwy. Utworzono już zbyt wiele warstw"
#define _CANNOT_CREATE_NODES_AND_ELEMENTS_BLOCK_ u8"Nie można utworzyć bloku węzłów i elementów"
#define _CANNOT_CREATE_DEFLECTION_BLOCK_ u8"Nie można utworzyć bloku odkształceń"
#define _CANNOT_CREATE_STRESS_BLOCK_ u8"Nie można utworzyć bloku naprężeń"
#define _CANNOT_CREATE_STRAIN_BLOCK_ u8"Nie można utworzyć bloku odkształceń postaciowych"

#define _PROCEED_PLATE_FEM_ u8"Przeprowadzić analizę statyczną wskazanej płyty?"

#define _POLYLINE_IS_NOT_CLOSED_ u8"polilinia nie jest zamknięta"
#define _THE_PLATE_ u8"Płyta"
#define _THE_HOLE_ u8"Otwór"
#define _THE_WALL_ u8"Sciana"
#define _THE_ZONE_ u8"Strefa"
#define _THE_PLATE_POLYLINE_NUMBER_IS_EQUAL_ZERO_ u8"Liczba polilinii płyt jest równa 0"
#define _THE_PLATE_POLYLINE_NUMBER_IS_GREATER_THAN_ONE_ u8"Liczba polilinii płyt jest większa od 1\n Takiego rozwiązania jeszcze nie wdrożono."
#define _NO_LOAD_ASSIGNED_ u8"Nie przypisano obciążenia i nie zadeklarowano „%g” dla obciażenia ciężarem własnym"

#define _THE_HOLE_NOT_ASSIGNED_TO_ZONE_OR_PLATE_ u8"Otwór nie jest przypisany do żadnej strefy ani płyty"
#define _THE_WALL_NOT_ASSIGNED_TO_ZONE_OR_PLATE_ u8"Ściana nie jest przypisana do żadnej strefy ani płyty"
#define _THE_ZONE_NOT_ASSIGNED_TO_PLATE_ u8"Strefa nie jest przypisana do żadnej płyty"

#define _Yes_ "Tak"
#define _No_ "Nie"
#define _YES_NO_ESC_ u8"TNtn\033"
#define _YES_ 'T'
#define _yes_ 't'
#define _NO_ 'N'
#define _no_ 'n'

#define _CANNOT_PROCEED_IN_32BIT_ u8"Płyta wygląda OK ale moduł obliczeniowy Elmer FEM nie pracuje w systemie 32-bitowym"
#define _BUY_NEW_COMPUTER_  u8"Kup nowy komputer"
#define _unknown_standard_ u8"Nieznany standard"

#endif

#ifdef __O_TEXT3PDF__
#define _CANNOT_CREATE_PDF_ u8"błąd: nie można utworzyć dokumentu PDF\n"
#endif

#ifdef __O_ANIMATE_DYN__

#define _ANIMATE_ u8"ESC: exit, BackSpace: previous, any other: next"

#endif

#ifdef __O_SEL__

#define _FIRST_  "PIERWSZY"
#define _SECOND_ "DRUGI"

#endif