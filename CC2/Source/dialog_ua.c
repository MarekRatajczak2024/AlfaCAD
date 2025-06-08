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

#ifdef __PRNDLG__

#define PRINTER_WINDOWS "Принтер-WINDOWS"
#define PRINTER_PCX     "Принтер-Імідж"
#define PRINTER_PDF     "Принтер-PDF"
#define PRINTER9        "Printer-9"
#define PRINTER24       "Printer-24"
#define PRINTER48       "Printer-48"
#define PRINTER_ESC_P2  "Printer-ESC/P2"
#define PRINTER_LASER   "Printer-HP PCL"
#define PRINTER_DATA    "Друкування"

static char* prn_config_param[] =
{
	//"Przesow Pionowy",
	//"Przesuw Pionowy HQ",
	//"Gestosc Pionowa EPSON",
	//"Gestosc Pionowa IBM",
	//"Gestosc Pionowa HQ EPSON",
	//"Gestosc Pionowa HQ IBM",
	//"Druk Draft",
	u8"Основна Щільність",
	//"Gestosc Draft IBM",
	//"Druk Normalny",
	u8"Покращена Щільність",
	//"Gestosc Normalna IBM",
	//"Druk Dokladny",
	u8"Хороша Щільність",
	//"Gestosc Dokladna IBM",
	//"Druk Specjalny",
	//"Tryb Specjalny IBM",
	u8"Дуже Хороша Щільність",
	//"Gestosc Specjalna IBM",
	u8"Найкраща Щільність",
	u8"Відрізок Дуги",
	//"Inicjacja Drukarki",
	//"Reset Drukarki",
  //  "Srednica Punktu",
};

static int no_prn_config_param = sizeof(prn_config_param) / sizeof(prn_config_param[0]);

static char* prn_ini_date_param[] =
{
  u8"Тип Принтера",
  u8"Режим Принтера",
  u8"Друк У Масштабі",
  u8"Режим Друку",
  u8"Обертання Друку",
  u8"Інверсія Друку",
  u8"Відображення Друку",
  u8"Друк На Принтері",
  u8"Поля",
  u8"Ширина Дуже Тонкої Лінії",
  u8"Ширина Тонкої Лінії",
  u8"Ширина Товстої Лінії",
  u8"Ширина Дуже Товстої Лінії",
  u8"Ширина Супер Товстої Лінії",
  u8"Верхнє Поле",
  u8"Нижнє Поле",
  u8"Ліве Поле",
  u8"Праве Поле",
  u8"Ширина Сторінки",
  u8"Висота Сторінки",
  u8"Ширина Сторінки Принтера",
  u8"Висота Сторінки Принтера",
  u8"Масштаб Друку",
  u8"Реальний Розмір",
  u8"Пауза",
  u8"Зміна Сторінки",
  u8"Друк У Кольорі",
  u8"Імідж Сірого Кольору",
  u8"Друк У Сірому Кольорі",
  u8"Кольоровий Режим",
  u8"Встановити Довжину Сторінки",
  u8"Вікно Друку",
  u8"Друк Аркуша",
  u8"LPT",
  u8"Відображення Тексту",
  u8"Буферизація",
  u8"Стиснення Даних",
  u8"Плоттер",
  u8"Формат RCF",
  u8"Фіксована Назва",
  u8"Файл",
  u8"Копії",
  u8"Насиченість Іміджу",
  u8"H Коефіцієнт Товщини",
  u8"V Коефіцієнт Товщини",
  u8"Номер Рулону",
  u8"WINPRINT Def",
  u8"Формат Зображення",
  u8"Інтенсивність Сірого",
  u8"Фон"
};

static int no_prn_ini_date_param = sizeof(prn_ini_date_param) / sizeof(prn_ini_date_param[0]);

static char* prn_ini_date_param_comment[] =
{
  u8";ІМІДЖ=0, WINDOWS=1, PDF=2",
  u8";невикористаний",
  u8";1-фактична шкала, 0-заповнена сторінка",
  u8";1-Основна,2-Покращена,3-Хороша,4-Дуже Хороша,5-Найкраща",
  u8";1-обертання, 0-без обертання",
  u8";1-інверсія, 0-без інверсії",
  u8";1-1-дзеркало, 0-нормальне",
  u8";1-принтер, 0-файл - знецінений",
  u8";[мм]",
  u8";[мм]",
  u8";[мм]",
  u8";[мм]",
  u8";[мм]",
  u8";[мм]",
  u8";[мм]",
  u8";[мм]",
  u8";[мм]",
  u8";[мм]",
  u8";[мм]",
  u8";[мм]",
  u8";[мм]",
  u8";[мм]",
  u8";масштабу",
  u8";фактичний розмір для збереження зображення у файлі",
  u8";пауза для зміни сторінки - знецінений",
  u8";",
  u8";",
  u8";",
  u8";1-колір, 0-Ч/Б",
  u8";0-KCMY,1-YMCK,2-MCYK,3-GRAY - знецінений",
  u8";",
  u8";",
  u8";",
  u8";порт принтера",
  u8";текстовий режим",
  u8";буферизація у файлі - знецінений",
  u8";стиснення даних",
  u8";плоттер - знецінений",
  u8";Плоттери формату RCF (OCE) - знецінений",
  u8";фіксована назва для збереження - знецінений",
  u8";ім'я файлу зображення",
  u8";кількість копій - знецінений",
  u8";рівень насиченості зображення - знецінений",
  u8";поправочний коефіцієнт товщини горизонтальної лінії",
  u8";поправочний коефіцієнт товщини вертикальної лінії",
  u8";0-за замовчуванням, 1-ролон 1, 2-ролон 2 - знецінений",
  u8";0-меню, 1-за замовчуванням",
  u8";BMP, PNG, JPEG, TIFF, PCX, EPS, PDF",
  u8";3 - темний, 2 - середній, 1 - світлий",
  u8";"
};

#define prn_comm0 u8"дуже тонкий"
#define prn_comm1 u8"тонкий"
#define prn_comm2 u8"товстий"
#define prn_comm3 u8"дуже товстий"
#define prn_comm4 u8"надто товстий"
#define prn_comm5 u8"масштаб"
#define prn_comm6 u8"сторінка"
#define prn_comm7 u8"Налаштування масштабу/сторінки"
#define prn_comm8 u8"Напрямок друку"
#define prn_comm9 u8"Тип друку"
#define prn_comm10 u8"Коеф.товщ.лінії H/V"        //Коефіцієнт товщини лінії H/V"
#define prn_comm11 u8"Тип друку"
#define prn_comm12 u8"товщ. [мм]"
#define prn_comm13 u8"Так"
#define prn_comm14 u8"Ні"
#define prn_comm15 u8""
#define prn_comm16 u8""
#define prn_comm17 u8"Основна"
#define prn_comm18 u8"Покращена"
#define prn_comm19 u8"Хороша"
#define prn_comm20 u8"Дуже Хороша"
#define prn_comm21 u8"Найкраща"
#define prn_comm22 u8""
#define prn_comm23 u8"Друк"
#define prn_comm24 u8"ESC"
#define prn_comm25 u8""
#define prn_comm26 u8""
#define prn_comm27 u8""
#define prn_comm28 u8""
#define prn_comm29 u8"Зберегти"
#define prn_comm30 u8"Завантажити"
#define prn_comm31 u8""
#define prn_comm32 u8"Перегляд"
#define prn_comm33 u8"Межі"

#define image_pr_tips0 u8"@Обертання"
#define image_pr_tips1 u8"Інверсія"
#define image_pr_tips2 u8"Відображення"
#define image_pr_tips3 u8"Друк у відтінках сірого"
#define image_pr_tips4 u8"Чорно-білий друк"
#define image_pr_tips5 u8"Друк растров у відтінках сірого"
#define image_pr_tips6 u8"Друк на принтері"
#define image_pr_tips7 u8"Зберегти у файлі зображення"
#define image_pr_tips8 u8"Вікно друку"
#define image_pr_tips9 u8"Друкувати вибраний '@SHEET'"
#define image_pr_tips10 u8"Друк у масштабі"
#define image_pr_tips11 u8"З номера сторінки"
#define image_pr_tips12 u8"Ширина сторінки [мм]"
#define image_pr_tips13 u8"Висота сторінки [мм]"
#define image_pr_tips14 u8"Перекриття сторінок [мм]"
#define image_pr_tips15 u8"Ліве поле [мм]"
#define image_pr_tips16 u8"Праве поле [мм]"
#define image_pr_tips17 u8"Верхнє поле [мм]"
#define image_pr_tips18 u8"Нижнє поле [мм]"
#define image_pr_tips19 u8"Фактичний розмір"
#define image_pr_tips20 u8"Дуже тонкий"
#define image_pr_tips21 u8"Тонкий"
#define image_pr_tips22 u8"Товстий"
#define image_pr_tips23 u8"Дуже товстий"
#define image_pr_tips24 u8"Екстра товсті"
#define image_pr_tips25 u8"Зберегти у векторному PDF"
#define image_pr_tips26 u8"Формат паперу"
#define image_pr_tips27 u8"Друк світлого фонового кольору"

#define _FILE_OPENING_ERROR_ u8"Помилка відкриття файлу."
#define _FILE_WRITING_ERROR_ u8"Помилка запису файлу "
#define _FILE_READING_ERROR_ u8"Помилка читання файлу "

#define _PRINTING_ u8"Друк на принтері / Збереження файлу зображення"
#define _PRINTING_SERIAL_ u8"Серійний друк цехових креслень"
#define _PRINTING_SPEC_ u8"Друк специфікації"

#endif

#ifdef  __PLTDLG__


#define PLOTER u8"Плоттер"
#define PLOTER_DATA u8"Плотовання"
//#define PLOTER u8"Плоттер"
//#define PLOTER_DATA u8"Нанесення на плотер"

char* plt_config_param[] =
{
   "Тип Плоттера",
   "Крайні Точки",
   "Резолюція",
   "Центрування",
   "Ініціація",
   "Закінчення",
   "Вибір Пера",
   "Швидкість Пера",
   "Ширина Пера",
   "Перо Вгору",
   "Перо Вниз",
   "Абсолютне Зміщення",
   "Проведіть Лінію",
   "Абсолютний Стрибок",
   "Координат Термінатор",
   "Командний Термінатор",
   "Довжина Сегмента Дуги",
};

int no_plt_config_param = sizeof(plt_config_param) / sizeof(plt_config_param[0]);

static char* plt_ini_data_param[] =
{
	//"Typ Plotera",
	"Зміна",
	"Обертання",
	"Масштаб",
	"Вікно",
	"Аркуш",
	"Чорнильний Плоттер",
	"Порт",
	"Ширина Пера Для Заповнення",
	"Фіксована Назва",
	"Встановити Розміри Сторінки",
	"Кольоровий друк",
	"255 Кольорів",
	"Швидкість Перьев",
	"Ширина Перьев",
	"Кількість Перьев",
	"Колір1","Колір2","Колір3","Колір4","Колір5","Колір6","Колір7","Колір8",
	"Колір9","Колір10","Колір11","Колір12","Колір13","Колір14","Колір15","Колір16",
	/*
	"Колір17","Колір18","Колір19","Колір20",
	"Колір21","Колір22","Колір23","Колір24","Колір25","Колір26","Колір27","Колір28","Колір29","Колір30",
	"Колір31","Колір32","Колір33","Колір34","Колір35","Колір36","Колір37","Колір38","Колір39","Колір40",
	"Колір41","Колір42","Колір43","Колір44","Колір45","Колір46","Колір47","Колір48","Колір49","Колір50",
	"Колір51","Колір52","Колір53","Колір54","Колір55","Колір56","Колір57","Колір58","Колір59","Колір60",
	"Колір61","Колір62","Колір63","Колір64","Колір65","Колір66","Колір67","Колір68","Колір69","Колір70",
	"Колір71","Колір72","Колір73","Колір74","Колір75","Колір76","Колір77","Колір78","Колір79","Колір80",
	"Колір81","Колір82","Колір83","Колір84","Колір85","Колір86","Колір87","Колір88","Колір89","Колір90",
	"Колір91","Колір92","Колір93","Колір94","Колір95","Колір96","Колір97","Колір98","Колір99","Колір100",
	"Колір101","Колір102","Колір103","Колір104","Колір105","Колір106","Колір107","Колір108","Колір109","Колір110",
	"Колір111","Колір112","Колір113","Колір114","Колір115","Колір116","Колір117","Колір118","Колір119","Колір120",
	"Колір121","Колір122","Колір123","Колір124","Колір125","Колір126","Колір127","Колір128","Колір129","Колір130",
	"Колір131","Колір132","Колір133","Колір134","Колір135","Колір136","Колір137","Колір138","Колір139","Колір140",
	"Колір141","Колір142","Колір143","Колір144","Колір145","Колір146","Колір147","Колір148","Колір149","Колір150",
	"Колір151","Колір152","Колір153","Колір154","Колір155","Колір156","Колір157","Колір158","Колір159","Колір160",
	"Колір161","Колір162","Колір163","Колір164","Колір165","Колір166","Колір167","Колір168","Колір169","Колір170",
	"Колір171","Колір172","Колір173","Колір174","Колір175","Колір176","Колір177","Колір178","Колір179","Колір180",
	"Колір181","Колір182","Колір183","Колір184","Колір185","Колір186","Колір187","Колір188","Колір189","Колір190",
	"Колір191","Колір192","Колір193","Колір194","Колір195","Колір196","Колір197","Колір198","Колір199","Колір200",
	"Колір201","Колір202","Колір203","Колір204","Колір205","Колір206","Колір207","Колір208","Колір209","Колір210",
	"Колір211","Колір212","Колір213","Колір214","Колір215","Колір216","Колір217","Колір218","Колір219","Колір220",
	"Колір221","Колір222","Колір223","Колір224","Колір225","Колір226","Колір227","Колір228","Колір229","Колір230",
	"Колір231","Колір232","Колір233","Колір234","Колір235","Колір236","Колір237","Колір238","Колір239","Колір240",
	"Колір241","Колір242","Колір243","Колір244","Колір245","Колір246","Колір247","Колір248","Колір249","Колір250",
	"Колір251","Колір252","Колір253","Колір254","Колір255",
	"PenColor1","PenColor2","PenColor3","PenColor4","PenColor5","PenColor6","PenColor7","PenColor8",
	"PenColor9","PenColor10","PenColor11","PenColor12","PenColor13","PenColor14","PenColor15","PenColor16",
	"PenColor17","PenColor18","PenColor19","PenColor20",
	"PenColor21","PenColor22","PenColor23","PenColor24","PenColor25","PenColor26","PenColor27","PenColor28","PenColor29","PenColor30",
	"PenColor31","PenColor32","PenColor33","PenColor34","PenColor35","PenColor36","PenColor37","PenColor38","PenColor39","PenColor40",
	"PenColor41","PenColor42","PenColor43","PenColor44","PenColor45","PenColor46","PenColor47","PenColor48","PenColor49","PenColor50",
	"PenColor51","PenColor52","PenColor53","PenColor54","PenColor55","PenColor56","PenColor57","PenColor58","PenColor59","PenColor60",
	"PenColor61","PenColor62","PenColor63","PenColor64","PenColor65","PenColor66","PenColor67","PenColor68","PenColor69","PenColor70",
	"PenColor71","PenColor72","PenColor73","PenColor74","PenColor75","PenColor76","PenColor77","PenColor78","PenColor79","PenColor80",
	"PenColor81","PenColor82","PenColor83","PenColor84","PenColor85","PenColor86","PenColor87","PenColor88","PenColor89","PenColor90",
	"PenColor91","PenColor92","PenColor93","PenColor94","PenColor95","PenColor96","PenColor97","PenColor98","PenColor99","PenColor100",
	"PenColor101","PenColor102","PenColor103","PenColor104","PenColor105","PenColor106","PenColor107","PenColor108","PenColor109","PenColor110",
	"PenColor111","PenColor112","PenColor113","PenColor114","PenColor115","PenColor116","PenColor117","PenColor118","PenColor119","PenColor120",
	"PenColor121","PenColor122","PenColor123","PenColor124","PenColor125","PenColor126","PenColor127","PenColor128","PenColor129","PenColor130",
	"PenColor131","PenColor132","PenColor133","PenColor134","PenColor135","PenColor136","PenColor137","PenColor138","PenColor139","PenColor140",
	"PenColor141","PenColor142","PenColor143","PenColor144","PenColor145","PenColor146","PenColor147","PenColor148","PenColor149","PenColor150",
	"PenColor151","PenColor152","PenColor153","PenColor154","PenColor155","PenColor156","PenColor157","PenColor158","PenColor159","PenColor160",
	"PenColor161","PenColor162","PenColor163","PenColor164","PenColor165","PenColor166","PenColor167","PenColor168","PenColor169","PenColor170",
	"PenColor171","PenColor172","PenColor173","PenColor174","PenColor175","PenColor176","PenColor177","PenColor178","PenColor179","PenColor180",
	"PenColor181","PenColor182","PenColor183","PenColor184","PenColor185","PenColor186","PenColor187","PenColor188","PenColor189","PenColor190",
	"PenColor191","PenColor192","PenColor193","PenColor194","PenColor195","PenColor196","PenColor197","PenColor198","PenColor199","PenColor200",
	"PenColor201","PenColor202","PenColor203","PenColor204","PenColor205","PenColor206","PenColor207","PenColor208","PenColor209","PenColor210",
	"PenColor211","PenColor212","PenColor213","PenColor214","PenColor215","PenColor216","PenColor217","PenColor218","PenColor219","PenColor220",
	"PenColor221","PenColor222","PenColor223","PenColor224","PenColor225","PenColor226","PenColor227","PenColor228","PenColor229","PenColor230",
	"PenColor231","PenColor232","PenColor233","PenColor234","PenColor235","PenColor236","PenColor237","PenColor238","PenColor239","PenColor240",
	"PenColor241","PenColor242","PenColor243","PenColor244","PenColor245","PenColor246","PenColor247","PenColor248","PenColor249","PenColor250",
	"PenColor251","PenColor252","PenColor253","PenColor254","PenColor255",
	*/
};

static int no_plt_ini_data_param = sizeof(plt_ini_data_param) / sizeof(plt_ini_data_param[0]);

static char* plt_ini_data_param_comment[] =
{
	//";typ,MERA=2,1 inne",
	";dx, dy",
	"; 0 Ні,1 Так",
	";масштабу",
	";0-весь креслення, 1-вікно",
	";0-креслення/вікно, 1-вибраний аркуш",
	";0-ручний плоттер, 1-чорнильний плотер",
	"",
	"",
	";фіксована назва файлу для друку",
	";для HP=1, для OCE=0",
	";0=Ч/Б, 1=колір",
	";0=відображені кольори, 1=255 кольорів RGB->CMYK",
	";2 до 25",
	";0.0 до 2.0",
	";1 до 255, за замовчуванням 255",
	";Номери ручок для кольорів і ширини ,(0=SP)",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "",
	/*
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	";Wartosci kolorow Cyjan Magenta Yellow, C255 M255 Y255  zakres 0..255 ",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	*/
};

char plt_dlg_labels[][64] = {
	u8"Номери перьев 0-пропустити",/*лінія дуже тонка / тонка / товста / дуже товста / товста",*/
	u8"Швидкість / Товщина",
	u8"Масштаб, зсув",
	u8"",
	u8"Поворот, вікно, колір",
	u8"Друк у файл/порт",
	u8"Наповнення",
};

char plt_dlg_buttons[][64] = {
	u8"Викреслювання",
	u8"ESC",
	u8"фікс.назва",
	u8"встановити розмір сторінки (HP)",
};

#define plt_dlg_labs0 u8"Нанесення на плотер"
#define plt_dlg_labs1 u8"x - за замовчуванням"
#define plt_dlg_labs2 u8""

#define image_pl_tips0 u8"@Дуже тонкий"
#define image_pl_tips1 u8"Тонкий"
#define image_pl_tips2 u8"Товстий"
#define image_pl_tips3 u8"Дуже товстий"
#define image_pl_tips4 u8"Надзвичайно товстий"
#define image_pl_tips5 u8"Чорнильний плоттер"
#define image_pl_tips6 u8"Обертання"
#define image_pl_tips7 u8"Показане вікно графіка"
#define image_pl_tips8 u8"Вибраний графік @SHEET"
#define image_pl_tips9 u8"Побудова кольорового зображення"
#define image_pl_tips10 u8"Малювання в 255 кольорах"
#define image_pl_tips11 u8"Масштаб графіка"
#define image_pl_tips12 u8"Зміщення ліворуч [мм]"
#define image_pl_tips13 u8"Зміщення дна [мм]"

#define _FILE_OPENING_ERROR_ u8"Помилка відкриття файлу."
#define _FILE_WRITING_ERROR_ u8"Помилка запису файлу "
#define _FILE_READING_ERROR_ u8"Помилка читання файлу "

#define _NR_ u8"Но"
#define _CM_S_ u8"[см/с]"
#define _MM_ "[мм]"
#define _FILLINGS_ u8"заповнення"

#endif

#ifdef __PCXDLG__

#define _IMAGE_FILE_IMPORT_ u8"Параметри імпорту графічного файлу";

#define pcx_tips0 u8"@Горизонтальна щільність"
#define pcx_tips1 u8"Вертикальна щільність"
#define pcx_tips2 u8"Горизонтальний розмір у мм"
#define pcx_tips3 u8"Вертикальний розмір у мм"
#define pcx_tips4 u8"Масштабовано по горизонталі"
#define pcx_tips5 u8"Масштабовано по вертикалі"
#define pcx_tips6 u8"Горизонтальний масштаб"
#define pcx_tips7 u8"Вертикальний масштаб"
#define pcx_tips8 u8"Заповнений фон"
#define pcx_tips9 u8"Кут повороту"
#define pcx_tips10 u8"Зберегти співвідношення сторін"
#define pcx_tips11 u8"Покласти зверху"
#define pcx_tips12 u8"Перевернути по горизонталі"
#define pcx_tips13 u8"Перевернути вертикально"

char header_s[] = u8"Заголовок";
char setup_s[] = u8"Налаштування";
char flip_s[] = u8"Перевернути";

#endif

#ifdef __COLORS__

#define KOLORY "КОЛЬОРИ"
#define CHOOSE_COLORS u8"вибрати кольори"

#define image_col_tips0  u8"@Виберіть колір"

#endif

#ifdef __DXFDLG__

#define _DFX_IMPORT_ u8"Параметри імпорту файлів DXF/DWG";

char header_dxf[] = u8"Заголовок";
char setup_dxf[] = u8"Налаштування";
char setup_format[] = u8"Формат";

#define images_dxf_tips0 u8"@Горизонтальні діапазон"
#define images_dxf_tips1 u8"Вертикальні діапазон"
#define images_dxf_tips2 u8"Горизонтальні межі"
#define images_dxf_tips3 u8"Вертикальні межі"
#define images_dxf_tips4 u8"Горизонтальний розмір [мм]"
#define images_dxf_tips5 u8"Вертикальний розмір [мм]"
#define images_dxf_tips6 u8"Ліве поле [мм]"
#define images_dxf_tips7 u8"Нижнє поле [мм]"
#define images_dxf_tips8 u8"Одиниці"
#define images_dxf_tips9 u8"Масштаб"
#define images_dxf_tips10 u8"Формат креслення"
#define images_dxf_tips11 u8"Автоматично коректувати розміри"

#endif

#ifdef __DIALOG__

#define _OTHER_ u8"֍інший>>>"

#endif