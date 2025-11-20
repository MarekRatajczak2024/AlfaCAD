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
/*0*/{u8"",
/*1*/u8"Datos correctos",
/*2*/u8"Espera...",
/*3*/u8"Interrumpir --> Esc ",
/*4*/u8"Cambiando estructura de datos, espere...",
/*5*/u8"Prueba de datos...",
/*6*/u8"",
/*7*/u8"Espera: estructura de bloques de prueba. Interrupción --> Esc",
/*8*/u8"Espera: eliminando bloques...",
/*9*/u8"Configuración de ventana guardada.",
};

static char* messages[] =
{
	/*0*/u8"%s",
	/*1*/u8"Interrumpir --> Esc (%s%%)",
	/*2*/u8"ESPERAR -> romper líneas: (%s%%)",
	/*3*/u8"Búfer de imagen (%s) Interrupción --> Esc",
};

static char* messages1[] =
{
	/*0*/u8"%s",
	/*1*/u8"Interrumpir --> Esc (%s%%)",
	/*2*/u8"ESPERAR -> romper líneas: (%s%%)",
};

static char* errors_kom[] =
/* 0*/{u8"",
/* 1*/u8"Espaciado de cuadrícula demasiado bajo, la cuadrícula no se mostrará",
/* 2*/u8"Formato incorrecto del número real",
/* 3*/u8"Formato incorrecto del parámetro",
/* 4*/u8"Archivo de macro no encontrado.",
/* 5*/u8"Tamaño del formato demasiado grande",
/* 6*/u8"Archivo macro demasiado grande",
/* 7*/u8"Datos de configuración del plotter incorrectos o faltantes",
/* 8*/u8"Disco lleno o operación de disco fallida",
/* 9*/u8"Formato incorrecto de número entero positivo",
/* 10*/u8"Archivo de configuración de inicio de trazador incorrecto o faltante",
/* 11*/u8"No se pudo cargar el archivo de fuente. Formato de archivo de fuente incorrecto",
/* 12*/u8"No hay suficiente memoria para cargar el archivo de fuentes",
/* 13*/u8"Pantalla más allá del formato de dibujo",
/* 14*/u8"El proceso hijo no comenzó",
/* 15*/u8"Error interno. Informar al desarrollador.",
/* 16*/u8"Formato de número incorrecto",
/* 17*/u8"Falta fuente",
/* 18*/u8"Sin memoria",
/* 19*/u8"No se encontró ningún objeto",
/* 20*/u8"Parámetros incorrectos",
/* 21*/u8"Límite de tamaño de bloque superado (máx. 2 GB)",
/* 22*/u8"Datos de tipos de línea incorrectos o faltantes",
/* 23*/u8"Error interno. Informar al desarrollador.",
/* 24*/u8"Formato incorrecto de número entero positivo",
/* 25*/u8"Formato de archivo incorrecto",
/* 26*/u8"Datos de configuración de impresora incorrectos o faltantes",
/* 27*/u8"Búfer de macros desbordado",
/* 28*/u8"Nombre de archivo incorrecto",
/* 29*/u8"Las lineas no pueden ser paralelas",
/* 30*/u8"Formato incorrecto de número real positivo",
/* 31*/u8"Archivo no encontrado o error al leer archivo",
/* 32*/u8"Radio de arco demasiado grande",
/* 33*/u8"Memoria insuficiente",
/* 34*/u8"Radio de arco demasiado pequeño o formato de número incorrecto",
/* 35*/u8"Formato de número real distinto de cero incorrecto",
/* 36*/u8"No se pudo crear más capas",
/* 37*/u8"La capa actual no puede ser invisible",
/* 38*/u8"Formato de datos incorrecto, radio de arco <= 0 ",
/* 39*/u8"Formato de datos incorrecto, radio del círculo <= 0 ",
/* 40*/u8"Formato de datos incorrecto",
/* 41*/u8"Impresión terminada o error de impresión.",
/* 42*/u8"Error de impresión. Continuar / Terminar :",
/* 43*/u8"Error de dispositivo. Continuar / Terminar :",
/* 44*/u8"El objeto seleccionado no pertenece al bloque.",
/* 45*/u8"Formato de archivo incorrecto.",
/* 46*/u8"Disco o carpeta incorrectos.",
/* 47*/u8"Nuevo dibujo",
/* 48*/u8"Error de escritura de archivo.",
/* 49*/u8"Archivo no encontrado",
/* 50*/u8"Precaución: Los objetos del último bloque están en la capa '-V'",
/* 51*/u8"Formato incorrecto del archivo de configuración ALFACAD.INI ",
/* 52*/u8"No se pudo realizar la operación.",
/* 53*/u8"Ningún objeto eliminado.",
/* 54*/u8"Precaución: Los objetos del último bloque están en la capa '-E'",
/* 55*/u8"Cifras incorrectas",
/* 56*/u8"El bloque contiene arcos, los factores de escala X e Y deben ser iguales.",
/* 57*/u8"Error en la expresión.",
/* 58*/u8"División por 0.",
/* 59*/u8"Falta el paréntesis de cierre.",
/* 60*/u8"Esto no es una función.",
/* 61*/u8"Número incorrecto de argumentos de función.",
/* 62*/u8"La función requiere un argumento no negativo.",
/* 63*/u8"Variable o función no definida.",
/* 64*/u8"Parántesis '(' esperado.",
/* 65*/u8"Argumento de función matemática fuera de rango.",
/* 66*/u8"Ángulo de arco igual a 0.",
/* 67*/u8"Número de página incorrecto.",
/* 68*/u8"Número permitido de aristas: 3 - 1024.",
/* 69*/u8"No se encontró ningún bloque.",
/* 70*/u8"El número de bloques excede el número permitido de 2000.",
/* 71*/u8"No hay segmentos de croquis para eliminar.",
/* 72*/u8"Bloque con parámetros inválidos ha sido explotado.",
/* 73*/u8"El radio del arco no puede ser < 0.",
/* 74*/u8"El radio del círculo no puede ser < 0.",
/* 75*/u8"Formato incorrecto de dos enteros positivos.",
/* 76*/u8"No hay segmentos de seguimiento para eliminar.",
/* 77*/u8"No hay segmentos de polilínea para eliminar.",
/* 78*/u8"Recursión no permitida",
/* 79*/u8"Formato incorrecto de número real distinto de cero.",
/* 80*/u8"El objeto seleccionado está en la capa '-E'.",
/* 81*/u8"El segmento seleccionado de la polilínea no se puede extender.",
/* 82*/u8"Se debe seleccionar el segmento de polilínea.",
/* 83*/u8"Puede empalmar solo dos segmentos de polilínea adyacentes.",
/* 84*/u8"El radio del arco es demasiado pequeño.",
/* 85*/u8"Puede empalmar solo segmentos de polilínea exteriores.",
/* 86*/u8"Puedes achaflanar solo dos segmentos de polilínea adyacentes.",
/* 87*/u8"Los objetos seleccionados no forman un área cerrada.",
/* 88*/u8"El punto indicado no se encuentra dentro del área cerrada.",
/* 89*/u8"La función no es continua en el punto de inicio",
/* 90*/u8"La función tiene demasiados puntos de discontinuidad",
/* 91*/u8"Los objetos del bloque seleccionado están en las capas '-V'.",
/* 92*/u8"Los objetos del bloque seleccionado están en las capas '-E'.",
/* 93*/u8"Ángulo del sistema de coordenadas local definido incorrectamente (dx=0,dy=0)",
/* 94*/u8"No hay bloque en el portapapeles",
/* 95*/u8"Asignación de memoria insuficiente para la inicialización del menú",
/* 96*/u8"Asignación de memoria insuficiente para búfer macro y capas",
/* 97*/u8"Asignación de memoria insuficiente para la inicialización de datos",
/* 98*/u8"Asignación de memoria insuficiente para la inicialización de datos de dibujo",
/* 99*/u8"Formato de archivo incorrecto o asignación de memoria insuficiente para cargar el dibujo adjunto",
/*100*/u8"Error al leer dibujo adjunto",
/*101*/u8"Asignación de memoria insuficiente para leer el archivo de trabajo 'RYSUJ.$2'",
/*102*/u8"Error al leer el archivo de trabajo 'RYSUJ.$2'",
/*103*/u8"Error al abrir el archivo de datos 'ALFPROF.TMP'",
/*104*/u8"Error al leer el archivo de datos 'ALFPROF.TMP'",
/*105*/u8"La distancia no puede ser < 0", ///////////////////////////////////// //////
/*106*/u8"",
/*107*/u8"",
/*108*/u8"",
/*109*/u8"",
/*110*/u8"",
/*111*/u8"",
/*112*/u8"",
/*113*/u8"",
/*114*/u8"",
/*115*/u8"",
/*116*/u8"",
/*117*/u8"",
/*118*/u8"",
/*119*/u8"",
/*120*/u8"",
/*121*/u8"",
/*122*/u8"",
/*123*/u8"",
/*124*/u8"Aviso: el punto de origen local <0,0> está en el punto de origen global 0,0. Presione cualquier tecla:",
/*125*/u8"",
/*126*/u8"",
/*127*/u8"",
/*128*/u8"",
/*129*/u8"",
/*130*/u8"",
/*131*/u8"",
/*132*/u8"",
/*133*/u8"",
/*134*/u8"",
/*135*/u8"",
/*136*/u8"",
/*137*/u8"",
/*138*/u8"",
/*139*/u8"",
/*140*/u8"",
/*141*/u8"",
/*142*/u8"",
/*143*/u8"",
/*144*/u8"",
/*145*/u8"",
/*146*/u8"",
/*147*/u8"",
/*148*/u8"",
/*149*/u8"",
/*150*/u8"",
/*151*/u8"",
/*152*/u8"",
/*153*/u8"",
/*154*/u8"",
/*155*/u8"",
/*156*/u8"",
/*157*/u8"",
/*158*/u8"",
/*159*/u8"",
/*160*/u8"",
/*161*/u8"",
/*162*/u8"",
/*163*/u8"",
/*164*/u8"",
/*165*/u8"",
/*166*/u8"",
/*167*/u8"",
/*168*/u8"",
/*169*/u8"",
/*170*/u8"Error de conversión de archivo DXF.",
/*171*/u8"Símbolo Mo en catálogo",
/*172*/u8"Versión incorrecta del archivo DXF (versiones permitidas: R9, R10, R11, R12, R13, R14, 2000)",
/*173*/u8"Demasiados archivos (máx. 1000)",
/*174*/u8"",
/*175*/u8"",
/*176*/u8"",
/*177*/u8"",
/*178*/u8"",
/*179*/u8"",
/*180*/u8"",
/*181*/u8"",
/*182*/u8"",
/*183*/u8"",
/*184*/u8"",
/*185*/u8"",
/*186*/u8"",
/*187*/u8"",
/*188*/u8"",
/*189*/u8"",
/*190*/u8"",
/*191*/u8"",
/*192*/u8"",
/*193*/u8"",
/*194*/u8"",
/*195*/u8"",
/*196*/u8"¡La capa actual no no se puede eliminar!",
/*197*/u8"",
/*198*/u8"",
/*199*/u8"",
/*200*/u8"",
/*201*/u8"La longitud del vector debe ser mayor que cero",
/*202*/u8"Error al leer archivo de imagen",
/*203*/u8"",
/*204*/u8"",
/*205*/u8"",
/*206*/u8"",
/*207*/u8"",
/*208*/u8"",
/*209*/u8"",
/*210*/u8"Configuración de capa '0' forzada como +V +E +P",
/*211*/u8"Error interno",
/*212*/u8"Código de error: ",
/*213*/u8"Valor fuera de rango 1024 .. 2000000 [KB]",
/*214*/u8"Número de capa fuera del rango de 1...256",
/*215*/u8"Ninguno de los semiejes de la elipse puede tener una longitud menor que 0.",
/*216*/u8"Formato no válido para números reales no negativos",
};

static char* komunikaty_kom[] =
/* 0*/{u8"",
/* 1*/u8"Ingrese la dimensión: ",
/* 2*/u8"Ingrese los multiplicadores de coordenadas X;Y : ",
/* 3*/u8"Dibujo no guardado. ¿Guardar en archivo? (Sí / No / Esc): ",
/* 4*/u8"Ingrese el formato de la hoja de dibujo: ",
/* 5*/u8"Ingrese el nombre del archivo a guardar: ",
/* 6*/u8"Ingrese la escala del dibujo: 1: ",
/* 7*/u8"Ingrese el paso del cursor: ",
/* 8*/u8"Ingrese la densidad de cuadrícula: ",
/* 9*/u8"Ingrese el nombre del archivo para leer: ",
/*10*/u8"Ingrese la altura del carácter: ",
/*11*/u8"Ingrese la descripción del bloque: ",
/*12*/u8"Función inactiva.",
/*13*/u8"Ingrese el texto: ",
/*14*/u8"Ingrese el tipo de bloque: ",
/*15*/u8"Ingrese el ángulo: ",
/*16*/u8"Ingrese el ángulo de rotación del bloque: ",
/*17*/u8"Ingrese el factor de escala del bloque: ",
/*18*/u8"En versión demo función inactiva.",
/*19*/u8"Ingrese la longitud mínima del segmento de croquis: ",
/*20*/u8"Ingrese el ángulo de relleno: ",
/*21*/u8"Ingrese el número de elementos: ",
/*22*/u8"Saltar por dX ; dY : ",
/*23*/u8"Saltar a X ; Y : ",
/*24*/u8"Ingrese el ancho del trazo: ",
/*25*/u8"Ingrese el tamaño del punto: ",
/*26*/u8"Ingrese el nombre del archivo: ",
/*27*/u8"Ingrese el número de segmentos de división: ",
/*28*/u8"Apunta a",
/*29*/u8"Ingrese la longitud del segmento de división: ",
/*30*/u8"Saltar por longitud - ángulo : ",
/*31*/u8"Escala del original: ",
/*32*/u8"Ángulo de rotación del original: ",
/*33*/u8"Factor de ancho de texto: ",
/*34*/u8"Altura del texto de cota: ",
/*35*/u8"Precisión de dimensionamiento: ",
/*36*/u8"Desplazamiento de línea de dimensión: ",
/*37*/u8"Distancia del texto desde la línea de dimensión: ",
/*38*/u8"Desplazamiento del punto inicial de la línea de extensión: ",
/*39*/u8"Ingrese la precisión de dibujo (mm) : ",
/*40*/u8"Ventana",
/*41*/u8"Eliminar extensión",
/*42*/u8"Comienzo",
/*43*/u8"Fin",
/*44*/u8"Programa externo: ",
/*45*/u8"Ingrese intervalo: ",
/*46*/u8"Interrupción -> Esc",
/*47*/u8"Ingrese el numero de filas y columnas r;c : ",
/*48*/u8"Ingrese la longitud del segmento: ",
/*49*/u8"Ingrese el número máximo de puntos de división: ",
/*50*/u8"¿Explorar catálogos? (Sí / No / Esc) : ",
/*51*/u8"Espaciado entre líneas: ",
/*52*/u8"Punto de inicio de sombreado: ",
/*53*/u8"Ingrese el número de vértices de la polilínea que se aproximan a la elipse: ",
/*54*/u8"Ingrese un paso de aproximación dx : ",
/*55*/u8"Ingresar función f(x) : ",
/*56*/u8"Calculadora: ",
/*57*/u8"Cambiar página, pulsar cualquier tecla",
/*58*/u8"¿Eliminar los objetos seleccionados? (Sí / No / Esc) : ",
/*59*/u8"",
/*60*/u8"¿Eliminar dibujo adjunto? (Sí / No / Esc) : ",
/*61*/u8"Sin dibujo adjunto...",
/*62*/u8"Presiona la tecla Esc...",
/*63*/u8"Ingrese el ángulo del sistema de coordenadas local: α =",
/*64*/u8"¿Estás seguro que quieres salir del programa? (Sí / No / Esc)",
/*65*/u8"",
/*66*/u8"Punto -> exactamente el final",
/*67*/u8"Punto -> extremo más cercano",
/*68*/u8"Punto -> medio",
/*69*/u8"Punto -> intersección",
/*70*/u8"Punto -> centro",
/*71*/u8"Punto -> perpendicular",
/*72*/u8"Punto -> tangencial",
/*73*/u8"Punto -> adyacente",
/*74*/u8"Punto -> punto",
/*75*/u8"Punto de línea para determinar el ángulo",
/*76*/u8"El objeto seleccionado no pertenece al bloque.",
/*77*/u8"El objeto seleccionado está en la capa '-E'.",
/*78*/u8"Los objetos del bloque seleccionado están en las capas '-V'.",
/*79*/u8"Los objetos del bloque seleccionado están en las capas '-E'.",
/*80*/u8"Elige un objeto para determinar la capa",
/*81*/u8"Ingrese el número de color (0..255) : ",
/*82*/u8"Elige un objeto para determinar el color",
/*83*/u8"Seleccione un objeto para determinar el tipo y ancho/parámetros de texto",
/*84*/u8"Los bloques congelados no han explotado.",
/*85*/u8"Los bloques no congelados no han sido explotados.",
/*86*/u8"El texto indicado no es un nombre de archivo.",
/*87*/u8"Ya estás en la raíz de la estructura de árbol",
/*88*/u8"Texto no encontrado.",
/*89*/u8"Precaución: Demasiados objetos 'PIN' en el bloque ampliado.",
/*90*/u8"Ingrese TIPO de dispositivo: ",
/*91*/u8"Ingrese el SÍMBOLO del dispositivo: ",
/*92*/u8"Ingrese COMENTARIO: ",
/*93*/u8"Definición incorrecta del dispositivo. Presiona cualquier tecla...",
/*94*/u8"Ingrese el tipo (nombre) o el símbolo: ",
/*95*/u8"Precaución: Dispositivos de reemplazo generados (*). Complete el diagrama",
/*96*/u8"Ingrese la escala de vista: ",
/*97*/u8"Ingrese el nombre del Esquema + : ",
/*98*/u8"Ingrese el nombre del archivo del Esquema +: ",
/*99*/u8"Ingrese las coordenadas del punto seleccionado: ",
/*100*/u8"Ingrese la unidad de dibujo en el archivo DXF: ",
/*101*/u8"Ingrese la escala de transformación del archivo DXF: ",
/*102*/u8"Ingrese el texto de búsqueda: ",
/*103*/u8"Buscar: ",
/*104*/u8"Cambiar a: ",
/*105*/u8"Está bien",
/*106*/u8"Ingrese el desplazamiento del eje relativo al centro de simetría de la traza: ",
/*107*/u8"La función está temporalmente inactiva en esta versión del programa.",
/*108*/u8"Ingrese un nombre para el nuevo subdirectorio: ",
/*109*/u8"¿Borrar prototipos de bloque? (Sí / No / Esc) : ", ////////////////////////////
/*110*/u8"Seleccione el objeto para obtener la lista de parámetros",
/*111*/u8"Seleccione la fila para editar o agregar el parámetro a la lista del portapapeles o presione Esc",
/*112*/u8"Elige el punto de inicio de la escotilla: ",
/*113*/u8"Ingrese el tamaño del punto dx [mm]: ",
/*114*/u8"Ingrese el tamaño del punto dy [mm]: ",
/*115*/u8"Número máximo de capas: 256",
/*116*/u8"Número mínimo de capa: 1",
/*117*/u8"La capa no existe.",
/*118*/u8"Primer símbolo: ",
/*119*/u8"¿Archivo no guardado. ¿Quiere guardarloć? (Sí / No / Esc): ",
/*120*/u8"",
/*121*/u8"",
/*122*/u8"",
/*123*/u8"",
/*124*/u8"",
/*125*/u8"",
/*126*/u8"",
/*127*/u8"",
/*128*/u8"",
/*129*/u8"",
/*130*/u8"",
/*131*/u8"",
/*132*/u8"",
/*133*/u8"",
/*134*/u8"",
/*135*/u8"",
/*136*/u8"Ingrese prefijos de números de zona: ",
/*137*/u8"Ingrese el numero de la primera zona: ",
/*138*/u8"Ingrese altura de zona [mm] : ",
/*139*/u8"Ingrese ancho de zona [mm] : ",
/*140*/u8"Ingrese ancho de margen de zona [mm] : ",
/*141*/u8"Ingrese el desplazamiento del marco de las zonas [mm]: ",
/*142*/u8"",
/*143*/u8"",
/*144*/u8"Ingrese la coordenada Z: ",
/*145*/u8"",
/*146*/u8"",
/*147*/u8"",
/*148*/u8"",
/*149*/u8"",
/*150*/u8"",
/*151*/u8"",
/*152*/u8"",
/*153*/u8"",
/*154*/u8"Ingrese el factor de autopanorama 0.01...1 : ",
/*155*/u8"",
/*156*/u8"",
/*157*/u8"",
/*158*/u8"",
/*159*/u8"",
/*160*/u8"..........¿Para ignorar el fondo mientras se imprime?",
/*161*/u8"Seleccione el color de fondo (o ESC)",
/*162*/u8"¿Ignorar el fondo?",
/*163*/u8"Seleccione el color impuesto de los puntos de la imagen (o ESC)",
/*164*/u8"¿Inserción B/N en la impresión?",
/*165*/u8"",
/*166*/u8"",
/*167*/u8"",
/*168*/u8"",
/*169*/u8"",
/*170*/u8"",
/*171*/u8"",
/*172*/u8"",
/*173*/u8"Los objetos seleccionados no forman un área cerrada.",
/*174*/u8"",
/*175*/u8"",
/*176*/u8"",
/*177*/u8"",
/*178*/u8"",
/*179*/u8"",
/*180*/u8"",
/*181*/u8"",
/*182*/u8"Señala un bloque de hoja (@SHEET)",
/*183*/u8"¿Editar bloque de imagen?",
/*184*/u8"Ingrese el ángulo de rotación del bloque de imagen: ",
/*185*/u8"¿Recargar bloque de imagen editado? (Sí / No / Esc): ",
/*186*/u8"",
/*187*/u8"",
/*188*/u8"",
/*189*/u8"",
/*190*/u8"",
/*191*/u8"Apunta al objeto para especificar la capa oculta",
/*192*/u8"Número de vértices de la polilínea que se aproximan al círculo o arco escalados en escala anisotrópica: ",
/*193*/u8"Ingrese el valor predeterminado del tamaño máximo de memoria para dibujar [KB]: ",
/*194*/u8"Ingrese el valor predeterminado del búfer de impresión [KB]: ",
/*195*/u8"Introduce numero de capa: ",
/*196*/u8"Seleccione un área rellena de patrón, sólido o trazo para determinar los parámetros de relleno",
/*197*/u8"Seleccione un área rellena de patrón, sólido o trazo para cambiar los parámetros de relleno",
/*198*/u8"Ingrese el factor de escala fijo del bloque: ",
/*199*/u8"Ingrese el factor de reescalado de fuerza: ",
/*200*/u8"Ingrese el factor de reescalado de momento: ",
/*201*/u8"ngrese el factor de reescalado de desplazamiento: ",
/*202*/u8"Ingrese el factor de reescalado de rotación: ",
/*203*/u8"Ingrese el factor de reescalado de carga: ",
/*204*/u8"Ingrese el factor de reescalado de carga térmica: ",
/*205*/u8"Ingrese la precisión de la fuerza: ",
/*206*/u8"Ingrese la precisión del momento: ",
/*207*/u8"Ingrese la precisión de desplazamiento: ",
/*208*/u8"Ingrese la precisión de rotación: ",
/*209*/u8"Ingrese la precisión de carga: ",
/*210*/u8"Ingrese la precisión de la carga térmica: ",
/*211*/u8"Ingrese el número de propiedad: ",
/*212*/u8"Ingrese el factor de cambio de escala para las fuerzas axiales resultantes: ",
/*213*/u8"Ingrese un factor de cambio de escala para las fuerzas cortantes resultantes: ",
/*214*/u8"Ingrese el factor de cambio de escala para los momentos flectores resultantes: ",
/*215*/u8"Ingrese el factor de reescalado para los desplazamientos resultantes: ",
/*216*/u8"Ingrese el factor de cambio de escala para las fuerzas de reacción concentradas de apoyo resultantes: ",
/*217*/u8"Ingrese el factor de reescalado para los momentos de reacción de soporte resultantes: ",
/*218*/u8"Ingrese el factor de reescalado de profundidad de la sección: ",
/*219*/u8"Ingrese el factor de reescalado para las tensiones resultantes (acero, madera): ",
/*220*/u8"Ingrese la precisión de la tensión: ",
/*221*/u8"Ingrese el factor exagerado de la deformación de los modos de vibración modal: ",
/*222*/u8"Ingrese el factor de reescalado para el porcentaje de refuerzo resultante: ",
/*223*/u8"Ingrese el factor de reescalado de carga superficial: ",
/*224*/u8"Ingrese el factor de reescalado para las tensiones resultantes (hormigón armado): ",
/*225*/u8"Ingrese el factor de cambio de escala para las fuerzas de reacción distribuidas de apoyo resultantes: ",
/*226*/u8"Ingrese la precisión de las reacciones: ",
"",
"",
};


static char* komunikaty0[] =
/*0*/{ u8"",
/*1*/u8"Texto",
/*2*/u8"Estirar - a punto",
/*3*/u8"Estirar - desde el punto",
/*4*/u8"Mover - desde el punto",
/*5*/u8"Mover - al punto",
/*6*/u8"Dimensionamiento",
/*7*/u8"Línea desde el punto",
/*8*/u8"Línea a punto",
/*9*/u8"Círculo - centro",
/*10*/u8"Círculo - rojo",
/*11*/u8"Filete - seleccionar primera línea",
/*12*/u8"Filete - seleccionar segunda línea",
/*13*/u8"Arco - tres puntos",
/*14*/u8"Arco - inicio-centro-final",
/*15*/u8"Arco - inicio-centro-ángulo",
/*16*/u8"Arco - inicio-centro-acorde",
/*17*/u8"Arco - inicio-final-radio",
/*18*/u8"Arco - inicio-final-ángulo",
/*19*/u8"Arco - inicio-final-dirección ",
/*20*/u8"Arco - continuación",
/*21*/u8"Seleccionar objetos a borrar",
/*22*/u8"Seleccionar objetos para mover",
/*23*/u8"Seleccionar objetos para copiar",
/*24*/u8"Copiar - desde el punto",
/*25*/u8"Copiar - a punto",
/*26*/u8"Seleccionar objetos a escala",
/*27*/u8"Punto base",
/*28*/u8"Escala",
/*29*/u8"Dibujar cuadrado",
/*30*/u8"Ventana desde punto",
/*31*/u8"Ventana a punto",
/*32*/u8"Seleccionar objetos para rotar",
/*33*/u8"Rotación",
/*34*/u8"Seleccionar por marco objetos para estirar",
/*35*/u8"Memoria insuficiente",
/*36*/u8"Seleccionar objetos para guardar en archivo",
/*37*/u8"Marcar punto de inserción (Inicio +90°, Fin -90°)",
/*38*/u8"Sección transversal del perfil",
/*39*/u8"Editar texto",
/*40*/u8"Color, Ángulo, Alturać, Fuente", /*nie uzywane*/
/*41*/u8"Seleccionar bloques para explotar",
/*42*/u8"Disco - centro",
/*43*/u8"Disco - radio",
/*44*/u8"Chaflán - seleccionar primera línea",
/*45*/u8"Chaflán - seleccionar segunda línea",
/*46*/u8"Romper - seleccionar objeto",
/*47*/u8"Break - marca el primer punto",
/*48*/u8"Break - marca el segundo punto",
/*49*/u8"Guardar automáticamente en el archivo BACKUP.ALF",
/*50*/u8"Seleccionar objeto para cambiar propiedades",
/*51*/u8"Seleccionar bordes para extensión",
/*52*/u8"Seleccionar objetos a extender",
/*53*/u8"Marcar vértice del área",
/*54*/u8"Marcar punto",
/*55*/u8"Distancia - primer punto",
/*56*/u8"Distancia - segundo punto",
/*57*/u8"Polígono - primer punto de un lado",
/*58*/u8"Polígono - inscrito en un círculo",
/*59*/u8"Polígono - circunscrito a un círculo",
/*60*/u8"Polígono - segundo punto de un lado",
/*61*/u8"Polígono - radio de un círculo",
/*62*/u8"Bosquejo -Marcar punto de inicio",
/*63*/u8"Bosquejo",
/*64*/u8"Seleccionar objeto para agrupar",
/*65*/u8"Copiar paralelo (desplazamiento) a punto",
/*66*/u8"Copiar paralelo (desplazamiento) por distancia",
/*67*/u8"Seleccionar objeto para copiar en paralelo (desplazamiento)",
/*68*/u8"Seleccionar filos de corte",
/*69*/u8"Seleccione el objeto a cortar",
/*70*/u8"Seleccionar objeto para reflejar",
/*71*/u8"Primer punto de la línea de simetría (eje)",
/*72*/u8"Segundo punto de simetría (eje)",
/*73*/u8"Seleccionar objetos para copia rectangular",
/*74*/u8"Seleccionar objetos para copia polar",
/*75*/u8"Indicar con rectangular las distancias entre columnas y filas",
/*76*/u8"Elegir punto central",
/*77*/u8"Elegir punto base",
/*78*/u8"Trazar desde el punto",
/*79*/u8"Trazar al punto",
/*80*/u8"Polilinea desde punto",
/*81*/u8"Seleccionar línea o arco para continuación",
/*82*/u8"Seleccionar objeto a dividir",
/*83*/u8"Seleccionar objeto a medir",
/*84*/u8"Ángulo - Tercer punto",
/*85*/u8"Ángulo - Primer punto",
/*86*/u8"Ángulo - Segundo punto",
/*87*/u8"Indicar los límites del sombreado",
/*88*/u8"Indicar el interior del área (Home +45°,End -45°,PgUp +3°,PgDn -3°)",
/*89*/u8"Indicar el inicio del primer eje de la elipse",
/*90*/u8"Indicar el final del primer eje de la elipse",
/*91*/u8"Indicar el centro de la elipse",
/*92*/u8"Indicar la longitud del segundo semieje",
/*93*/u8"Indicar el punto de inicio del gráfico",
/*94*/u8"Indicar el punto final del gráfico",
/*95*/u8"Seleccionar objetos para eliminarlos instantáneamente",
/*96*/u8"",
/*97*/u8"Indicar los límites del área",
/*98*/u8"Indicar el interior del área",
/*99*/u8"",
/*100*/u8"2 líneas desde el punto",
/*101*/u8"2 líneas a punto",
/*102*/u8"Eje - Primer punto",
/*103*/u8"Eje - Segundo punto",
/*104*/u8"3 líneas desde el punto",
/*105*/u8"3 líneas a punto",
/*106*/u8"Cambio de formato de datos del dibujo...",
/*107*/u8"Actualizando el dibujo en un nuevo formato",
/*108*/u8"Cambio de formato de datos del bloque...",
/*109*/u8"Actualizando el bloque en un nuevo formato",
/*110*/u8"Polilinea desde punto",
/*111*/u8"Polilinea a punto",
/*112*/u8"Seleccionar bloques congelados (*,$) para explotar",
/*113*/u8"Entrada -> señalar el nombre del archivo del dibujo",
/*114*/u8"Indicar las dimensiones del dispositivo",
/*115*/u8"Indicar dimensiones del Esquema +",
/*116*/u8"Seleccionar objetos para especificación de dispositivos",
/*117*/u8"Indicar las dimensiones del rectángulo",
/*118*/u8"Seleccionar textos a cambiar",
/*119*/u8"Seleccionar textos (símbolos) a cambiar",
/*120*/u8"Biblioteca de símbolos -> seleccionar símbolo :",
/*121*/u8"Texto (PgUp +3°, PgDn -3°, Home +90°, End -90°, 'E' editar)",
/*122*/u8"Indicar el interior del área (Home +45°,End -45°,PgUp +3°,PgDn -3°)",
/*123*/u8"Marca el punto de inicio de la curva Bezier",
/*124*/u8"Marca el punto de control de la curva Bezier",
/*125*/u8"",
/*126*/u8"Eje desde punto:",
/*127*/u8"Eje a punto:",
/*128*/u8"leyendo...",
/*129*/u8"esperando...",
/*130*/u8"",
/*131*/u8"",
/*132*/u8"",
/*133*/u8"Seleccionar objetos para guardar en el portapapeles",
/*134*/u8"Seleccionar imagen para cambiar propiedades",
/*135*/u8"Seleccionar imagen para exportar",
/*136*/u8"Búfer de imágenes",
/*137*/u8"Interrumpir --> Esc ",
/*138*/u8"Seleccione el bloque de sombreado para cambiar el patrón (Home +45°,End -45°,PgUp +3°,PgDn -3°)",
/*139*/u8"Indicar el interior del área (Inicio +90°, Fin -90°)",
/*140*/u8"Indicar el interior del área",
/*141*/u8"Seleccione spline para cambiar tensiones",
/*142*/u8"Indicar magnitud de tensiones",
/*143*/u8"Desplazamiento panorámico desde el punto",
/*144*/u8"Desplazamiento panorámico a punto",
/*145*/u8"Línea inicial desde el punto",
/*146*/u8"Línea guía a punto",
/*147*/u8"Seleccione un objeto para la transformación de cuadrilátero",
/*148*/u8"Convirtiendo archivo a formato BMP",
/*149*/u8"Convertiendo archivo a formato PBM",
/*150*/u8"Convertiendo archivo a formato ALX",
/*151*/u8"Indique el ángulo inicial del arco elíptico",
/*152*/u8"Indique el ángulo final del arco elíptico",
/*153*/u8"Seleccione la línea o lado del polígono o sólido al que la elipse debe ser tangencial",
/*154*/u8"Indique la ubicación de un punto en la elipse para determinar el segundo semieje",
/*155*/u8"Vector desde el punto",
/*156*/u8"Vector a punto",
/*157*/u8"Vector desde el punto  (Home->invertir flechas, End->intercambiar valores)",
/*158*/u8"Vector a punto  (Home->invertir flechas, End->intercambiar valores)",
/*159*/u8"",
/*160*/u8"",
/*161*/u8"",
/*162*/u8"",
/*163*/u8"",
/*164*/u8"",
/*165*/u8"",
/*166*/u8"",  //custom text
/*167*/u8"Indique un diagrama de marco o armadura para análisis estático/dinámico",
/*168*/u8"Dimensionamiento (para comenzar a dimensionar un ángulo, toque cualquier línea)",
/*169*/u8"Indicar el elemento del marco o cercha",
/*170*/u8"Posicione la línea de sección en las coordenadas deseado del elemento. Haga clic para guardar los valores en el portapapeles",
/*171*/u8"Indique un diagrama de losa para análisis estático (FEM)",
};

static char* messages_str[] =
/*0*/{u8"Arco - inicio-punto-final",
/*1*/u8"Arco - inicio-centro-final",
/*2*/u8"Arco - inicio-centro-ángulo",
/*3*/u8"Arco - inicio-centro-acorde",
/*4*/u8"Arco - inicio-final-radio",
/*5*/u8"Arco - inicio-final-ángulo",
/*6*/u8"Arco - inicio-final-dirección ",
/*7*/u8"Arco - continuación",
/*8*/u8"",
/*9*/u8"Archivo %s",
/*10*/u8"%s - CENTRO - radio",
/*11*/u8"%s - centro - RADIO",
/*12*/u8"%s - centro - DIÁMETRO",
/*13*/u8"%s - primer punto de diámetro",
/*14*/u8"%s - segundo punto de diámetro",
/*15*/u8"%s - primer punto",
/*16*/u8"%s - segundo punto",
/*17*/u8"%s - tercer punto",
/*18*/u8"%s - seleccionar primer objeto",
/*19*/u8"%s - seleccionar segundo objeto",
/*20*/u8"%s - CENTRO - diámetro",
/*21*/u8"",
/*22*/u8"",
/*23*/u8"DXF-> %s",
/*24*/u8"%s - primer punto del radio",
/*25*/u8"%s - segundo punto del radio",
};


#define ORTHO u8"   Orto."
#define _LAYER_ u8"Capa"
#define _MODE_ u8"Modo"
#define _V_ u8"V"
#define _E_ u8"E"
#define _P_ u8"P"

static char gg[10][20] = { u8"Punto final", u8"Extremo más cercano", u8"Medio", u8"Intersección", u8"Centro", u8"Perpendicular",
u8"Tangencial", u8"Adyacente", u8"punto","----" };

#endif

#ifdef __O_LUK__
/*static*/ char* komunikaty_arc[] = {
	u8"Arco - inicio-punto-final",
	u8"Arco - inicio-centro-final",
	u8"Arco - inicio-centro-ángulo",
	u8"Arco - inicio-centro-acorde",
	u8"Arco - inicio-final-radio",
	u8"Arco - inicio-final-ángulo",
	u8"Arco - inicio-final-dirección",
	u8"Arco - continuación"};

#define r6 6

#endif

#ifdef __BUF__

#define _NOT_ENOUGH_MEMORY_ u8"MEMORIA INSUFICIENTE O ERROR DE PROGRAMA"

#endif

#ifdef __DXF_O__

#define _PCX_FILE_ERROR_  u8"Error de escritura del archivo PCX"
#define _PNG_FILE_ERROR_  u8"Error de escritura del archivo PNG"
#define _UNKNOWN_IMAGE_FORMAT_ u8"Formato de archivo de imagen desconocido"
#define _FILE_ u8"Archivo "
#define _FILE_EXISTS_ u8" existe. ¿Sobrescribirlo? (Sí / No / Esc):"

#define _YES_NO_ESC_ u8"SNsn\033"
#define _YES_ 'S'
#define _yes_ 's'
#define _NO_ 'N'
#define _no_ 'n'

#endif

#ifdef __O_ZB__

#define _DRAWING_V_1_ u8"Dibujo <ver.1.0> Convertir con ALF v.1.1 - 1.9 primero"
#define _BLOCK_V_1_ u8"Bloque <ver.1.0>. Convertir si usa ALF v.1.1 - 1.9"

#define VECTORIZATION u8"Vectorización"

POLE pmNie_Tak[] = {
	{u8"No",'N',113,NULL},
	{u8"Sí",'S',112,NULL} };

char zb_err_message[] = u8"Error al convertir el archivo. Código de salida:";
char zb_err_message1[] = u8"Error al convertir el archivo";
char zb_unknown[] = u8"Desconocido";
static char zb_confirm[] = u8"Confirmar";

#define _FILE_CONVERSION_ERROR_ u8"Error de conversión de archivo"
#define _INCORRECT_FILE_FORMAT_ u8"Formato de archivo incorrecto"
#define _ERROR_WREITING_AUX_FILE_ u8"Error al escribir el archivo auxiliar"
#define _FILE_NOT_FOUND_ u8"Archivo no encontrado "

static char* vectorization_param[] =
{
    u8"escala PBM",
    u8"interpolación PBM",
    u8"umbral PBM",
    u8"Reversión de PBM",
    u8"filtro PBM",
    u8"sin filtro PBM",
    u8"escala ALX",
    u8"Nivel de negro ALX",
    u8"Tamaño de punto ALX",
    u8"Umbral de esquina ALX",
    u8"Tolerancia de optimización de la curva ALX",
    u8"Cuantificación ALX"
};

static int no_vectorization_param = sizeof(vectorization_param) / sizeof(vectorization_param[0]);

#define _YES_NO_ESC_ u8"SNsn\033"
#define _YES_ 'S'
#define _yes_ 's'
#define _NO_ 'N'
#define _no_ 'n'
#define _YES__ "S"
#define _NO__ "N"

#define _Yes_ u8"Sí"
#define _No_ u8"No"

#define _TOOLS_TO_INSTALL_ u8"No instalado"
#define _INSTALL_TOOLS_ u8"Instalar los paquetes necesarios"

#endif

#ifdef __WINSIZE__

TCHAR greeting1[] = _T("Configurar la ventana de dibujo");
TCHAR greeting2[] = _T("Usa los bordes para estirar la ventana");
TCHAR greeting3[] = _T("Usar encabezado para mover ventana");
TCHAR greeting4[] = _T("Haga clic dentro para completar");

#endif

#ifdef __PCX__

#define _PROCESSED_ u8"procesada "

#endif

#ifdef __O_HATCH1__

char _TO_MANY_OBJECTS_[]= u8"Se seleccionan demasiados objetos para continuar (el máximo es 6000)";

#endif

#ifdef __O_HATCH3__

char* vocabulary[] = { (char*)u8"  presione cualquier tecla:", (char*)u8"A",(char*)u8"centro de masa",(char*)u8"Primer momento de área",(char*)u8"Área momento de inercia" };
#endif

#ifdef __O_LTTYPE__

char* add_new_font = u8"añadir nueva fuente";
#define LINE u8"Líneas"

char lin_config_param[31][MaxLenNameLine] =
{
	u8"discontinua",
	u8"punteada",
	u8"2-punteada",
	u8"multipunteada",
	u8"borde",
	u8"mediana",
	u8"escondida",
	u8"fantasma",
	u8"discontinua_2",
	u8"punteada_2",
	u8"2-punteada_2",
	u8"multipunteada_2",
	u8"borde_2",
	u8"mediana_2",
	u8"escondida_2",
	u8"fantasma_2",
	u8"discontinua_x2",
	u8"punteada_x2",
	u8"2-punteada_x2",
	u8"multipunteada_x2",
	u8"borde_x2",
	u8"mediana_x2",
	u8"escondida_x2",
	u8"fantasma_x2",
	u8"3-punteada",
	u8"2-mediana",
	u8"3-discontinua",
	u8"fantasma_3",
	u8"2-mediana_x2",
	u8"3-discontinua_x2",
	u8"multidiscontinua",
};

#define CZCIONKI "Fuentes"
#define CZCIONKI_DXF "Fuentes-DXF"

char str[40] = u8"Ningún archivo : ";

#endif

#ifdef __O_MEASURE__

char measure_comm[9][54] = { u8"área", u8"Punto", u8"Nuevo ángulo del sistema de coordenadas local", u8"  ->presione cualquier tecla:", u8"Ángulo", u8"Ángulo actual del sistema de coordenadas local:", u8"Marca en imagen de dos puntos base", u8"Marca en el dibujo de dos puntos cartométricos", u8"Ángulo" };
#define _SUBSTRACT_ u8"restar"
#define _ADD_ u8"sumar"

#endif

#ifdef __O_CONFIG__

static char konf_global[8] = "Global";
static char konf_lokal[8] = "Local";
static char local_global[8] = "Local";

#define config_labels0 u8"Cuadro"
#define config_labels1 u8"Fondo"
#define config_labels2 u8"Texto"
#define config_labels3 u8"Cursor"
#define config_labels4 u8"Bloque"
#define config_labels5 u8"Cuadro"
#define config_labels6 u8"Fondo"
#define config_labels7 u8"Texto"
#define config_labels8 u8"Editar fondo"
#define config_labels9 u8"Editar texto"
#define config_labels10 u8"configuración:"
#define config_labels11 u8"otro" 

#define config_groups0 u8"COLORES DEL MENÚ"
#define config_groups1 u8"COLORES DE ESCRITORIO"

#define config_buttons0 u8"OK"
#define config_buttons1 u8"Esc"
#define config_buttons2 u8"" //"Global"
#define config_buttons3 u8"" //"Local"
#define config_buttons4 u8"Guardar"

#define config_title  u8"C O N F I G U R A C I Ó N"

#define image_conf_tips0  u8"@Global"
#define image_conf_tips1  u8"Local"

#endif

#ifdef __O_PRNFUN__

static char err_message[] = u8"Error al convertir el archivo. Código de salida:";
static char err_message_cups[] = u8"Error al ejecutar el comando 'lp'. Código de salida:";
static char confirm[] = u8"Confirmar";

#define _CANNOT_CREATE_PDF_ u8"error: no se puede crear el objeto PdfDoc\n"
#define _FILE_WRITING_ERROR_ u8"Error de escritura del archivo de imagen"

#endif


#ifdef __O_SECTION_DATA__
static char confirm[] = u8"Confirmar";
#define _CANNOT_OPEN_ u8"No se pudo abrir "
#define _CANNOT_READ_ u8"No se pudo leer "
#define _DEFAULT_TAKEN_ u8"Se adoptaron parámetros estándar de madera"
#endif

#ifdef __O_STATIC__

#define _SELECT_STATE_ "Estado límite (combinación)"
#define _SELECT_STATE_C_ L'E'

/*
POLE pmSelect_State[] = {
	{u8"estado límite Último (ELU)",L'U',0,NULL},
    {u8"estado límite de Servicio (ELS)",L'S',0,NULL},
    {u8"estado límite de servicio (Cuasipermanente)",L'C',0,NULL},
};

static TMENU mSelect_State = { 3,0,0,32,20,7, 0,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0, (POLE(*)[]) &pmSelect_State,NULL,NULL };
*/

#define _PROCEED_STATIC_ u8"¿Proceder a un análisis estático del marco o cercha indicada?"

#define _incorrectly_defined_ u8"definido incorrectamente"
#define _property_not_defined_ u8"propiedad no definida"
#define _reaction_not_associated_ u8"reacción no asociada con ningún nodo conocido"
#define _reaction_not_enough_in_Y_ u8"Estructura insuficientemente soportada en el eje Y. Agregar soporte faltante."
#define _reaction_not_enough_in_X_ u8"Estructura insuficientemente soportada en el eje X. Agregar soporte faltante."
#define _reaction_not_enough_in_X_Y_ u8"Estructura insuficientemente soportada en los ejes X e Y. Añadir soportes faltantes."
#define _node_size_not_associated_ u8"tamaño de nodo no asociado con ningún nodo conocido"
#define _force_not_associated_ u8"no asociado con ningún nodo conocido"
#define _load_not_associated_ u8"no asociado con ningún elemento conocido"
#define _thermal_load_inside_element_ u8"no en todo el elemento con coordenadas de nodos:"
#define _unknown_standard_ u8"Estándar desconocido"
#define _element_graph_data_failed_ u8"No se pudo crear el bloque de datos de las fuerzas resultantes para el elemento"
#define _cannot_create_folder_ u8"No se pudo crear el directorio de archivos"

#define _FRAME3DD_ u8"%FRAME:"
#define _FRAME3DD_PL u8"%RAMA:"
#define _FRAME3DD_UA u8"%КАРКАС:"
#define _FRAME3DD_ES u8"%MARCO:"

#define _Yes_ "Sí"
#define _No_ "No"
#define _YES_NO_ESC_ u8"SNsn\033"
#define _YES_ 'S'
#define _yes_ 's'
#define _NO_ 'N'
#define _no_ 'n'

#define __FRAME3DD__ "Frame3dd"

static char confirm[] = u8"Confirmar";
#define _CANNOT_CREATE_DEFORMATION_BLOCK_ u8"No se pudo crear un bloque de forma deformada"
#define _CANNOT_CREATE_FORCE_BLOCK_ u8"No se pudo crear un bloque de gráfico de fuerza"
#define _CANNOT_CREATE_MOMENT_BLOCK_ u8"No se pudo crear un bloque de gráfico de momento"
#define _CANNOT_CREATE_STRESS_BLOCK_ u8"No se pudo crear un bloque de gráfico de estrés"
#define _CANNOT_CREATE_SHEAR_STRESS_BLOCK_ u8"No se pudo crear un bloque de gráfico de estrés cortante"
#define _CANNOT_CREATE_RESULTS_FILE_ u8"No se pudo abrir el archivo de resultados"
#define _CANNOT_CREATE_RESULTS_PDF_FILE_ u8"No se pudo crear el archivo de resultados PDF"
#define _CANNOT_OPEN_RESULTS_PDF_FILE_ u8"No se pudo abrir el archivo PDF de resultados"
#define _INSTALL_PDF_VIEWER_ u8"Instalar el visor de PDF \"Okular\" o \"Evince\", por favor"

#define _CANNOT_OPEN_DEFORMATION_DATA_FILE_ u8"No se pudo abrir el archivo de datos de deformación"
#define _CANNOT_OPEN_RESULTS_DATA_FILE_ u8"No se pudo abrir el archivo de datos de resultados"
#define _CANNOT_OPEN_DYNAMIC_RESULTS_DATA_FILE_ u8"No se pudo abrir el archivo de datos de resultados dinámicos"
#define _CANNOT_CREATE_NODES_AND_ELEMENTS_BLOCK_ u8"No se pudo crear nodos y bloques de elementos"
#define _CANNOT_CREATE_REACTIONS_BLOCK_ u8"No se pudo crear el bloque de reacciones"
#define _CANNOT_CREATE_NEW_LAYER_ u8"No se pudo crear una nueva capa. Ya se han creado muchas capas"

char *frame3dd[]={
 /*0*/ u8"finalización sin errores",
 /*1*/ u8"error desconocido",
 /*2*/ u8"error con las opciones de la línea de comando (ver Sección 11, arriba)",
 /*3*/ u8"error con la opción de línea de comando para la deformación por corte -s",
 /*4*/ u8"error con la opción de línea de comando para rigidez geométrica -g",
 /*5*/ u8"error con la opción de línea de comando para masa agrupada -l",
 /*6*/ u8"error con la opción de línea de comando para el método de análisis modal -m",
 /*7*/ u8"error con la opción de línea de comando para la tolerancia del análisis modal -t",
 /*8*/ u8"error con la opción de línea de comando para análisis modal shift -f",
 /*9*/ u8"error con la opción de línea de comando para velocidad panorámica -p",
 /*10*/ u8"error con la opción de línea de comando para condensación de matriz -r",
 /*11*/ u8"error al abrir el archivo de datos de entrada",
 /*12*/ u8"error al abrir el archivo de datos de entrada limpio temporal para escribir",
 /*13*/ u8"error al abrir el archivo de datos de entrada limpio temporal para lectura",
 /*14*/ u8"error al abrir el archivo de datos de salida",
 /*15*/ u8" error al crear la ruta para los archivos de datos de salida temporales",
 /*16*/ u8" error al crear el nombre de la ruta del archivo de datos de salida temporal",
 /*17*/ u8"error al abrir el archivo de datos de salida .CSV (hoja de cálculo)",
 /*18*/ u8"error al abrir el archivo de datos de salida .M (matlab)",
 /*19*/ u8"error al abrir el archivo de datos de salida de fuerza interior para escritura",
 /*20*/ u8"error al abrir el archivo de datos de salida de fuerza interior para lectura",
 /*21*/ u8"error al abrir el archivo de datos de salida de malla no deformada",
 /*22*/ u8"error al abrir el archivo de datos de salida de malla deformada",
 /*23*/ u8"error al abrir el archivo de script de trazado para escribir los primeros diagramas de casos de carga estática",
 /*24*/ u8"error al abrir el archivo de script de trazado para agregar el segundo y superior resultado del caso de carga estática",
 /*25*/ u8"error al abrir el archivo de script de trazado para agregar gráficos modales",
 /*26*/ u8"error al abrir el archivo de script de trazado para agregar animaciones modales",
 /*27*/ u8"error al abrir el archivo de datos de malla modal",
 /*28*/ u8"error al abrir el archivo de datos de animación de malla modal",
 /*29*/ u8"error al abrir el archivo de depuración masiva de datos, MassData.txt",
 /*30*/ u8"la matriz del sistema de ajuste de curva cúbica para la deformación del elemento no es positiva definida",
 /*31*/ u8"matriz de rigidez estática estructural definida no positiva",
 /*32*/ u8"error en el análisis de valores propios. Inténtelo de nuevo con un número reducido o nulo de modos dinámicos",
 /*33*/ u8"finalización sin errores",
 /*34*/ u8"finalización sin errores",
 /*35*/ u8"finalización sin errores",
 /*36*/ u8"finalización sin errores",
 /*37*/ u8"finalización sin errores",
 /*38*/ u8"finalización sin errores",
 /*39*/ u8"finalización sin errores",
 /*40*/ u8"error en el archivo de datos de entrada",
 /*41*/ u8"error de formato de datos de entrada en los datos del nodo, número de nodo fuera de rango",
 /*42*/ u8"error de formato de datos de entrada en datos de nodo o elemento, nodo no conectado",
 /*43*/ u8"finalización sin errores",
 /*44*/ u8"finalización sin errores",
 /*45*/ u8"finalización sin errores",
 /*46*/ u8"finalización sin errores",
 /*47*/ u8"finalización sin errores",
 /*48*/ u8"finalización sin errores",
 /*49*/ u8"finalización sin errores",
 /*50*/ u8"finalización sin errores",
 /*51*/ u8"error de formato de datos de entrada en los datos del elemento del marco, número de elemento del marco fuera de rango",
 /*52*/ u8"error de formato de datos de entrada en los datos del elemento del marco, número de nodo fuera de rango",
 /*53*/ u8"error de formato de datos de entrada en los datos del elemento del marco, valor de sección negativo",
 /*54*/ u8"error de formato de datos de entrada en los datos del elemento del marco, el área de la sección transversal es 0 (cero)",
 /*55*/ u8"El error de formato de los datos de entrada en los datos del elemento del marco, el área de corte y el módulo de corte son 0 (cero)",
 /*56*/ u8"error de formato de datos de entrada en los datos del elemento del marco, el momento de inercia de torsión es 0 (cero)",
 /*57*/ u8"error de formato de datos de entrada en los datos del elemento del marco, el momento de inercia de flexión es 0 (cero)",
 /*58*/ u8"error de formato de datos de entrada en los datos del elemento del marco, el valor del módulo no es positivo",
 /*59*/ u8"error de formato de datos de entrada en los datos del elemento del marco, el valor de densidad de masa no es positivo",
 /*60*/ u8"Error de formato de datos de entrada en los datos del elemento del marco, el elemento del marco comienza y se detiene en el mismo nodo",
 /*61*/ u8"error de formato de datos de entrada en los datos del elemento de marco, el elemento de marco tiene una longitud de cero",
 /*62*/ u8"finalización sin errores",
 /*63*/ u8"finalización sin errores",
 /*64*/ u8"finalización sin errores",
 /*65*/ u8"finalización sin errores",
 /*66*/ u8"finalización sin errores",
 /*67*/ u8"finalización sin errores",
 /*68*/ u8"finalización sin errores",
 /*69*/ u8"finalización sin errores",
 /*70*/ u8"finalización sin errores",
 /*71*/ u8"error de formato de datos de entrada con la variable 'shear' que especifica la deformación por corte",
 /*72*/ u8"error de formato de datos de entrada con la variable 'geom' que especifica la rigidez geométrica",
 /*73*/ u8"error de formato de datos de entrada con la variable 'exagg_static' que especifica una exageración de malla estática",
 /*74*/ u8"error de formato de datos de entrada con la variable 'dx' que especifica la longitud del incremento del eje x de la fuerza interna",
 /*75*/ u8"finalización sin errores",
 /*76*/ u8"finalización sin errores",
 /*77*/ u8"finalización sin errores",
 /*78*/ u8"finalización sin errores",
 /*79*/ u8"finalización sin errores",
 /*80*/ u8"error de formato de datos de entrada en los datos de reacción, número de nodos con reacciones fuera de rango",
 /*81*/ u8"error de formato de datos de entrada en los datos de reacción, número de nodo fuera de rango",
 /*82*/ u8"error de formato de datos de entrada en los datos de reacción, los datos de reacción no son 1 (uno) o 0 (cero)",
 /*83*/ u8"error de formato de datos de entrada en los datos de reacción, el nodo especificado no tiene reacciones",
 /*84*/ u8"error de formato de datos de entrada en datos de reacción, estructura poco restringida",
 /*85*/ u8"error de formato de datos de entrada en datos de reacción, estructura totalmente restringida",
 /*86*/ u8"error de formato de datos de entrada en datos de inercia de nodo adicionales, número de nodo fuera de rango",
 /*87*/ u8"error de formato de datos de entrada en datos de masa de haz adicional, número de elemento de marco fuera de rango",
 /*88*/ u8"error de formato de datos de entrada en datos masivos, elemento de marco con masa no positiva",
 /*89*/ u8"finalización sin errores",
 /*90*/ u8"error de formato de datos de entrada en datos de condensación de matriz, el número de nodos con grados de libertad condensados ​​es menor que el número total de nodos",
 /*91*/ u8"error de formato de datos de entrada en datos de condensación de matriz, número de nodo fuera de rango",
 /*92*/ u8"error de formato de datos de entrada en datos de condensación de matriz, número de modo fuera de rango",
 /*93*/ u8"finalización sin errores",
 /*94*/ u8"error de formato de datos de entrada en datos de condensación de matriz, número de grados de libertad condensados ​​mayor que número de modos",
 /*95*/ u8"finalización sin errores",
 /*96*/ u8"finalización sin errores",
 /*97*/ u8"finalización sin errores",
 /*98*/ u8"finalización sin errores",
 /*99*/ u8"finalización sin errores",
 /*100*/ u8"error de formato de datos de entrada en los datos de carga",
 /*101*/ u8"el número de casos de carga estática debe ser mayor que cero",
 /*102*/ u8"el número de casos de carga estática debe ser inferior a 112",  //64 30
 /*103*/ u8"finalización sin errores",
 /*104*/ u8"finalización sin errores",
 /*105*/ u8"finalización sin errores",
 /*106*/ u8"finalización sin errores",
 /*107*/ u8"finalización sin errores",
 /*108*/ u8"finalización sin errores",
 /*109*/ u8"finalización sin errores",
 /*110*/ u8"finalización sin errores",
 /*111*/ u8"finalización sin errores",
 /*112*/ u8"finalización sin errores",
 /*113*/ u8"finalización sin errores",
 /*114*/ u8"finalización sin errores",
 /*115*/ u8"finalización sin errores",
 /*116*/ u8"finalización sin errores",
 /*117*/ u8"finalización sin errores",
 /*118*/ u8"finalización sin errores",
 /*119*/ u8"finalización sin errores",
 /*120*/ u8"finalización sin errores",
 /*121*/ u8"error de formato de datos de entrada en datos de carga nodal, número de nodo fuera de rango",
 /*122*/ u8"finalización sin errores",
 /*123*/ u8"finalización sin errores",
 /*124*/ u8"finalización sin errores",
 /*125*/ u8"finalización sin errores",
 /*126*/ u8"finalización sin errores",
 /*127*/ u8"finalización sin errores",
 /*128*/ u8"finalización sin errores",
 /*129*/ u8"finalización sin errores",
 /*130*/ u8"finalización sin errores",
 /*131*/ u8"error de formato de datos de entrada en datos de carga distribuidos uniformemente, el número de cargas uniformes es mayor que el número de elementos del marco",
 /*132*/ u8"error de formato de datos de entrada en datos de carga distribuidos uniformemente, número de elemento de marco fuera de rango",
 /*133*/ u8"finalización sin errores",
 /*134*/ u8"finalización sin errores",
 /*135*/ u8"finalización sin errores",
 /*136*/ u8"finalización sin errores",
 /*137*/ u8"finalización sin errores",
 /*138*/ u8"finalización sin errores",
 /*139*/ u8"finalización sin errores",
 /*140*/ u8"error de formato de datos de entrada en datos de carga distribuidos trapezoidalmente, demasiadas cargas distribuidas trapezoidalmente",
 /*141*/ u8"error de formato de datos de entrada en datos de carga distribuidos trapezoidalmente, número de elemento de marco fuera de rango",
 /*142*/ u8"error de formato de datos de entrada en datos de carga distribuidos trapezoidalmente, x1 < 0",
 /*143*/ u8"error de formato de datos de entrada en datos de carga distribuidos trapezoidalmente, x1 > x2",
 /*144*/ u8"error de formato de datos de entrada en datos de carga distribuidos trapezoidalmente, x2 > L",
 /*145*/ u8"finalización sin errores",
 /*146*/ u8"finalización sin errores",
 /*147*/ u8"finalización sin errores",
 /*148*/ u8"finalización sin errores",
 /*149*/ u8"finalización sin errores",
 /*150*/ u8"error de formato de datos de entrada en datos de carga interna concentrada, el número de cargas concentradas es mayor que el número de elementos del marco",
 /*151*/ u8"error de formato de datos de entrada en datos de carga concentrada interna, número de elemento de marco fuera de rango",
 /*152*/ u8"error de formato de datos de entrada en datos de carga concentrada interna, ubicación x menor que 0 o mayor que L",
 /*153*/ u8"finalización sin errores",
 /*154*/ u8"finalización sin errores",
 /*155*/ u8"finalización sin errores",
 /*156*/ u8"finalización sin errores",
 /*157*/ u8"finalización sin errores",
 /*158*/ u8"finalización sin errores",
 /*159*/ u8"finalización sin errores",
 /*160*/ u8"error de formato de datos de entrada en los datos de carga térmica, número de cargas térmicas mayor que el número de elementos del marco",
 /*161*/ u8"error de formato de datos de entrada en datos de carga térmica, número de elemento de marco fuera de rango",
 /*162*/ u8"error de formato de datos de entrada en datos de carga térmica, número de elemento de marco fuera de rango",
 /*163*/ u8"finalización sin errores",
 /*164*/ u8"finalización sin errores",
 /*165*/ u8"finalización sin errores",
 /*166*/ u8"finalización sin errores",
 /*167*/ u8"finalización sin errores",
 /*168*/ u8"finalización sin errores",
 /*169*/ u8"finalización sin errores",
 /*170*/ u8"finalización sin errores",
 /*171*/ u8"error de formato de datos de entrada en datos de desplazamiento prescritos, los desplazamientos prescritos pueden aplicarse sólo en coordenadas con reacciones",
 /*172*/ u8"finalización sin errores",
 /*173*/ u8"finalización sin errores",
 /*174*/ u8"finalización sin errores",
 /*175*/ u8"finalización sin errores",
 /*176*/ u8"finalización sin errores",
 /*177*/ u8"finalización sin errores",
 /*178*/ u8"finalización sin errores",
 /*179*/ u8"finalización sin errores",
 /*180*/ u8"finalización sin errores",
 /*181*/ u8"Advertencia: inestabilidad elástica (matriz de rigidez elástica + geométrica no definida positiva)",
 /*182*/ u8"Advertencia: deformación grande (la deformación axial promedio en uno o más elementos es mayor que 0.001)",
 /*183*/ u8"Advertencia: gran deformación e inestabilidad elástica",
 /*184*/ u8"finalización sin errores",
 /*185*/ u8"finalización sin errores",
 /*186*/ u8"finalización sin errores",
 /*187*/ u8"finalización sin errores",
 /*188*/ u8"finalización sin errores",
 /*189*/ u8"finalización sin errores",
 /*190*/ u8"finalización sin errores",
 /*191*/ u8"finalización sin errores",
 /*192*/ u8"finalización sin errores",
 /*193*/ u8"finalización sin errores",
 /*194*/ u8"finalización sin errores",
 /*195*/ u8"finalización sin errores",
 /*196*/ u8"finalización sin errores",
 /*197*/ u8"finalización sin errores",
 /*198*/ u8"finalización sin errores",
 /*199*/ u8"finalización sin errores",
 /*200*/ u8"error de asignación de memoria",
 /*201*/ u8"error al abrir un archivo de datos de salida al guardar un vector de 'flotantes'",
 /*202*/ u8"error al abrir un archivo de datos de salida al guardar un vector de 'ints'",
 /*203*/ u8"error al abrir un archivo de datos de salida guardando una matriz de 'flotantes'",
 /*204*/ u8"error al abrir un archivo de datos de salida guardando una matriz de 'dobles'",
 /*205*/ u8"error al abrir un archivo de datos de salida al guardar una matriz simétrica de 'flotantes'",
 /*206*/ u8"error al abrir un archivo de datos de salida guardando una matriz simétrica de 'dobles'",
 /*207*/ u8"error de prueba de equilibrio",
 /*208*/ u8"error al abrir el archivo de registro de errores .err",
 };


#define _ERROR_FREE_COMPLETION_ u8"finalización sin errores"

#endif

#ifdef __O_PLATE__

#define _PLATE_ u8"%PLATE:"
#define _PLATE_PL u8"%PŁYTA:"
#define _PLATE_UA u8"%ПЛИТА:"
#define _PLATE_ES u8"%LOSA:"

#define _cannot_create_folder_ u8"No se pudo crear el directorio de archivos"
#define _CANNOT_CREATE_RESULTS_FILE_ u8"No se pudo abrir el archivo de resultados"

static char confirm[] = u8"Confirmar";

#define _gmsh_error_ u8"Error de gmsh"
#define _ElmerGrid_error_ u8"Error de ElmerGrid"
#define _ElmerSolver_error_ u8"Error de ElmerSolver"

#define _CANNOT_OPEN_RESULTS_FILE_ u8"No se pudo abrir el archivo de resultados"
#define _CANNOT_READ_RESULTS_FILE_ u8"No se pudo leer el archivo de resultados"

#define _CANNOT_CREATE_NEW_LAYER_ u8"No se pudo crear una nueva capa. Ya se han creado muchas capas"
#define _CANNOT_CREATE_NODES_AND_ELEMENTS_BLOCK_ u8"No se pudo crear nodos y bloques de elementos"
#define _CANNOT_CREATE_DEFLECTION_BLOCK_ u8"No se pudo crear un bloque de deflexión"
#define _CANNOT_CREATE_STRESS_BLOCK_ u8"No se pudo crear un bloque de tensión"
#define _CANNOT_CREATE_STRAIN_BLOCK_ u8"No se pudo crear un bloque de deformación cortante"
#define _CANNOT_CREATE_REACTIONS_BLOCK_ u8"No se pudo crear un bloque de reacciones"

#define _PROCEED_PLATE_FEM_ u8"¿Proceder a un análisis estático de losa?"

#define _POLYLINE_IS_NOT_CLOSED_ u8"la polilínea no está cerrada"
#define _THE_PLATE_ u8"La losa"
#define _THE_HOLE_ u8"El agujero"
#define _THE_WALL_ u8"La pared"
#define _THE_ZONE_ u8"La zona"
#define _THE_PLATE_POLYLINE_NUMBER_IS_EQUAL_ZERO_ u8"El número de polilíneas de losas es igual a 0"
#define _THE_PLATE_POLYLINE_NUMBER_IS_GREATER_THAN_ONE_ u8"El número de polilíneas de losas es mayor que 1\n. Esta solución aún no se ha implementado."
#define _NO_LOAD_ASSIGNED_ u8"No se ha asignado ninguna carga y no se ha declarado ningún '%g' para el peso propio"

#define _THE_HOLE_NOT_ASSIGNED_TO_ZONE_OR_PLATE_ u8"Agujero no asignado a ninguna zona ni losa"
#define _THE_WALL_NOT_ASSIGNED_TO_ZONE_OR_PLATE_ u8"Muro no asignado a ninguna zona ni losa"
#define _THE_ZONE_NOT_ASSIGNED_TO_PLATE_ u8"Zona no asignada a ninguna losa"

#define _THE_ZONE_IS_ASSIGNED_TO_ANOTHER_ZONE_ u8"La zona está contenida dentro de otra zona, lo cual aún no está permitido"

#define _Yes_ "Sí"
#define _No_ "No"
#define _YES_NO_ESC_ u8"SNsn\033"
#define _YES_ 'S'
#define _yes_ 's'
#define _NO_ 'N'
#define _no_ 'n'

#define _CANNOT_PROCEED_IN_32BIT_ u8"La losa parece estar bien, pero el módulo de cálculo Elmer FEM no funciona en un sistema de 32 bits."
#define _BUY_NEW_COMPUTER_  u8"Compra una computadora nueva"
#define _unknown_standard_ u8"Estándar desconocido"

#endif

#ifdef __O_TEXT3PDF__
#define _CANNOT_CREATE_PDF_ u8"error: no se puede crear el objeto PdfDoc\n"
#endif

#ifdef __O_ANIMATE_DYN__

#define _ANIMATE_ u8"ESC: exit, BackSpace: previous, any other: next"

#endif

#ifdef __O_SEL__

#define _FIRST_  u8"PRIMERO"
#define _SECOND_ u8"SEGUNDO"

#endif