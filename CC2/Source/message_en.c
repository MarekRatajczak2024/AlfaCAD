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
/*1*/u8"Data OK",
/*2*/u8"Wait ...",
/*3*/u8"Interrupt --> Esc ",
/*4*/u8"Changing data structure, wait ...",
/*5*/u8"Data test ...",
/*6*/u8"",
/*7*/u8"Wait:  testing blocks structure. Interrupt --> Esc",
/*8*/u8"Wait:  deleting blocks ...",
/*9*/u8"Window setup saved.",
};

static char* messages[] =
{
	/*0*/u8"%s",
	/*1*/u8"Interrrupt  --> Esc   (%s%%)",
	/*2*/u8"WAIT -> breaking lines:  (%s%%)",
	/*3*/u8"Image buffering  (%s)   Interrrupt --> Esc",
};

static char* messages1[] =
{
	/*0*/u8"%s",
	/*1*/u8"Interrupt --> Esc   (%s%%)",
	/*2*/u8"WAIT -> breaking lines:  (%s%%)",
};

static char* errors_kom[] =
/*  0*/{ u8"",
/*  1*/u8"Grid spacing too low, grid won't be displayed",
/*  2*/u8"Incorrect format of real number",
/*  3*/u8"Incorrect format of parameter",
/*  4*/u8"Macro file not found.",
/*  5*/u8"Format size too big",
/*  6*/u8"Macro file too big",
/*  7*/u8"Incorrect or missing plotter configuration data",
/*  8*/u8"Disc full or failed disc operation",
/*  9*/u8"Incorrect format of positive integer number",
/* 10*/u8"Incorrect or missing plotter initiation configuration file",
/* 11*/u8"Font file cannot be loaded. Incorrect font file format",
/* 12*/u8"Not enough memory to load font file",
/* 13*/u8"Screen beyond drawing format",
/* 14*/u8"Child process didn't start",
/* 15*/u8"Internal error. Report to the developer.",
/* 16*/u8"Incorrect number format",
/* 17*/u8"Missing font",
/* 18*/u8"Out of memory",
/* 19*/u8"No object found",
/* 20*/u8"Incorrect parameters",
/* 21*/u8"Block size limit exceeded (max 2 GB)",
/* 22*/u8"Incorrect or missing line types data",
/* 23*/u8"Internal error. Report to the developer.",
/* 24*/u8"Incorrect format of positive integer number",
/* 25*/u8"Incorrect file format",
/* 26*/u8"Incorrect or missing printer configuration data",
/* 27*/u8"Macro buffer overrun",
/* 28*/u8"Incorrect file name",
/* 29*/u8"Lines cannot be parallel",
/* 30*/u8"Incorrect format of positive real number",
/* 31*/u8"File not found or file reading error",
/* 32*/u8"Arc radius too big",
/* 33*/u8"Out of memory",
/* 34*/u8"Arc radius too small or incorrect number format",
/* 35*/u8"Incorrect non-zero real number format",
/* 36*/u8"No more layers can be created",
/* 37*/u8"Current layer cannot be invisible",
/* 38*/u8"Incorrect data format, arc radius <= 0 ",
/* 39*/u8"Incorrect data format, circle radius <= 0 ",
/* 40*/u8"Incorrect data format ",
/* 41*/u8"Printing terminated or printing error.",
/* 42*/u8"Printing error. Continue / Terminate :",
/* 43*/u8"Device error. Continue / Terminate :",
/* 44*/u8"Selected object doesn't belong to the block.",
/* 45*/u8"Incorrect file format.",
/* 46*/u8"Incorrect disk or folder.",
/* 47*/u8"New drawing",
/* 48*/u8"File write error.",
/* 49*/u8"File not found",
/* 50*/u8"Caution : Objects of the last block are on the layer '-V'",
/* 51*/u8"Incorrect format of configuration file ALFACAD.INI ",
/* 52*/u8"The operation cannot be done.",
/* 53*/u8"No objects removed.",
/* 54*/u8"Caution : Objects of the last block are on the layer '-E'",
/* 55*/u8"Incorrect figures",
/* 56*/u8"Block contains arcs, X and Y scale factors must be equal.",
/* 57*/u8"Error in expression.",
/* 58*/u8"Division by 0.",
/* 59*/u8"Missing closing parenthesis.",
/* 60*/u8"This is not a function.",
/* 61*/u8"Incorrect number of function arguments.",
/* 62*/u8"The function requires a non-negative argument.",
/* 63*/u8"Undefined variable or function.",
/* 64*/u8"Paranthesis '(' expected.",
/* 65*/u8"Mathematical function argument out of range.",
/* 66*/u8"Arc angle equal 0.",
/* 67*/u8"Incorrect page number.",
/* 68*/u8"Allowed number of edges : 3 - 1024.",
/* 69*/u8"No block found.",
/* 70*/u8"The number of blocks exceeds the allowable number of 2000.",
/* 71*/u8"No sketch segments to remove.",
/* 72*/u8"Block with invalid parameters has been exploded.",
/* 73*/u8"Arc radius cannot be < 0.",
/* 74*/u8"Circle radius cannot be < 0.",
/* 75*/u8"Incorrect format of two positive integers.",
/* 76*/u8"No trace segments to remove.",
/* 77*/u8"No polyline segments to remove.",
/* 78*/u8"Impermissible recursion",
/* 79*/u8"Incorrect format of non-zero real number.",
/* 80*/u8"Selected object is on layer '-E'.",
/* 81*/u8"Selected segment of polyline cannot be extended.",
/* 82*/u8"Polyline segment should be selected.",
/* 83*/u8"You can fillet only two adjacent polyline segments.",
/* 84*/u8"Arc radius is too small.",
/* 85*/u8"You can fillet only outer polyline segments.",
/* 86*/u8"You can chamfer only two adjacent polyline segments .",
/* 87*/u8"Selected objects do not form enclosed area.",
/* 88*/u8"Indicated point doesn't lie within enclosed area.",
/* 89*/u8"The function is not continuous at the start point",
/* 90*/u8"The function has too many discontinuity points",
/* 91*/u8"Objects of selected block are on layers '-V'.",
/* 92*/u8"Objects of selected block are on layers '-E'.",
/* 93*/u8"Incorrectly defined local coordinate system angle (dx=0,dy=0)",
/* 94*/u8"No block in the clipboard",
/* 95*/u8"Insufficient memory allocation for menu initialization",
/* 96*/u8"Insufficient memory allocation for macro buffer and layers",
/* 97*/u8"Insufficient memory allocation for data initialization",
/* 98*/u8"Insufficient memory allocation for drawing data initialization",
/* 99*/u8"Incorrect file format or Insufficient memory allocation for loading attached drawing",
/*100*/u8"Error reading attached drawing",
/*101*/u8"Insufficient memory allocation for reading working file 'RYSUJ.$2'",
/*102*/u8"Error reading working file 'RYSUJ.$2'",
/*103*/u8"Error opening data file 'ALFPROF.TMP'",
/*104*/u8"Error reading data file 'ALFPROF.TMP'",
/*105*/u8"Distance cannot be < 0",  //////////////////////////////////////////
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
/*124*/u8"Notice : Local origin point <0,0> is at the global origin point 0,0. Press any key:",
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
/*170*/u8"DXF file conversion error.",
/*171*/u8"Mo symbol in catalog",
/*172*/u8"Incorrect DXF file version (allowed versions: R9, R10, R11, R12, R13, R14, 2000)",
/*173*/u8"Too many files (max. 1000)",
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
/*196*/u8"Current layer cannot be erased !",
/*197*/u8"",
/*198*/u8"",
/*199*/u8"",
/*200*/u8"",
/*201*/u8"The vector length must be greated than zero",
/*202*/u8"Error reading image file",
/*203*/u8"",
/*204*/u8"",
/*205*/u8"",
/*206*/u8"",
/*207*/u8"",
/*208*/u8"",
/*209*/u8"",
/*210*/u8"Enforced layer '0' setting as +V +E +P",
/*211*/u8"Internal error",
/*212*/u8"Error code: ",
/*213*/u8"Value out of range 1024 .. 2000000 [KB]",
/*214*/u8"Layer number out of the range of 1...256",
/*215*/u8"None of the semi-axes of the ellipse can have a length less than 0.",
/*216*/u8"Incorrect format of non-negative real number",
};

static char* komunikaty_kom[] =
/* 0*/{ u8"",
/* 1*/u8"Enter the dimension : ",
/* 2*/u8"Enter the coordinates multipliers X;Y : ",
/* 3*/u8"Drawing not saved. Save to file ? (Yes / No / Esc): ",
/* 4*/u8"Enter the format of the drawing sheet : ",
/* 5*/u8"Enter file name to be saved : ",
/* 6*/u8"Enter the scale of drawing:  1: ",
/* 7*/u8"Enter the cursor step : ",
/* 8*/u8"Enter grid density : ",
/* 9*/u8"Enter file name to read : ",
/*10*/u8"Enter the height of the character : ",
/*11*/u8"Enter the block description : ",
/*12*/u8"Function inactive.",
/*13*/u8"Enter the text : ",
/*14*/u8"Enter the block type : ",
/*15*/u8"Enter the angle : ",
/*16*/u8"Enter the block rotation angle : ",
/*17*/u8"Enter the block scale factor : ",
/*18*/u8"In demo version function inactive.",
/*19*/u8"Enter the sketch segment minimum length : ",
/*20*/u8"Enter the fill angle : ",
/*21*/u8"Enter the number of elements : ",
/*22*/u8"Jump by dX ; dY : ",
/*23*/u8"Jump to X ; Y : ",
/*24*/u8"Enter the trace width : ",
/*25*/u8"Enter point size : ",
/*26*/u8"Enter file name : ",
/*27*/u8"Enter the number of division segments : ",
/*28*/u8"Point to",
/*29*/u8"Enter the length of the division segment : ",
/*30*/u8"Jump by length - angle : ",
/*31*/u8"Scale of the original : ",
/*32*/u8"Rotation angle of the original : ",
/*33*/u8"Text width factor : ",
/*34*/u8"Height of dimension text : ",
/*35*/u8"Dimensioning accuracy : ",
/*36*/u8"Dimension line offset : ",
/*37*/u8"Text distance from the dimension line : ",
/*38*/u8"Offset of the starting point of the extension line : ",
/*39*/u8"Enter drawing accuracy (mm) : ",
/*40*/u8"Window",
/*41*/u8"Remove Stretch",
/*42*/u8"Beginning",
/*43*/u8"End",
/*44*/u8"External program : ",
/*45*/u8"Enter interval : ",
/*46*/u8"Interruption -> Esc",
/*47*/u8"Enter the number of rows and columns  r;c : ",
/*48*/u8"Enter the length of the segment : ",
/*49*/u8"Enter the maximum number of division points : ",
/*50*/u8"Browse catalogs ? (Yes / No / Esc) : ",
/*51*/u8"Line spacing : ",
/*52*/u8"Hatch start point : ",
/*53*/u8"Enter the number of vertices of the polyline approximating the ellipse: ",
/*54*/u8"Enter an aproximation step dx : ",
/*55*/u8"Enter function f(x) : ",
/*56*/u8"Calculator : ",
/*57*/u8"Change page, press any key",
/*58*/u8"Delete selected objects ? (Yes / No / Esc) : ",
/*59*/u8"",
/*60*/u8"Delete attached drawing ? (Yes / No / Esc) : ",
/*61*/u8"No attached drawing ...",
/*62*/u8"Press Esc key...",
/*63*/u8"Enter the angle of the local coordinate system: α =",
/*64*/u8"Are you sure you want the exit program ? (Yes / No / Esc)",
/*65*/u8"",
/*66*/u8"Point -> exactly the end",
/*67*/u8"Point -> nearest end",
/*68*/u8"Point -> middle",
/*69*/u8"Point -> intersection",
/*70*/u8"Point -> center",
/*71*/u8"Point -> perpendicular",
/*72*/u8"Point -> tangential",
/*73*/u8"Point -> adjacent",
/*74*/u8"Point -> point",
/*75*/u8"Point line to determine the angle",
/*76*/u8"Selected object doesn't belong to the block.",
/*77*/u8"Selected object is on layer '-E'.",
/*78*/u8"Objects of selected block are on layers '-V'.",
/*79*/u8"Objects of selected block are on layers '-E'.",
/*80*/u8"Pick an object to determine the layer",
/*81*/u8"Enter color number (0..255) : ",
/*82*/u8"Pick an object to determine color",
/*83*/u8"Pick an object to determine type and width / text parameters",
/*84*/u8"Frozen blocks have not been exploded.",
/*85*/u8"Non-frozen blocks have not been exploded.",
/*86*/u8"Indicated text is not a file name.",
/*87*/u8"You are already in the root of the tree structure",
/*88*/u8"Text not found.",
/*89*/u8"Caution : Too many 'PIN' objects in stretched block.",
/*90*/u8"Enter device TYPE : ",
/*91*/u8"Enter device SYMBOL : ",
/*92*/u8"Enter COMMENT : ",
/*93*/u8"Improper definition of the divice. Press any key ...",
/*94*/u8"Enter type (name) ot he symbol : ",
/*95*/u8"Caution: Replacement devices generated (*). Complete the diagram",
/*96*/u8"Enter the view scale : ",
/*97*/u8"Enter the name of Scheme + : ",
/*98*/u8"Enter the file name of Scheme + : ",
/*99*/u8"Enter the coordinates of the selected point : ",
/*100*/u8"Enter the drawing unit in the DXF file : ",
/*101*/u8"Enter the transformation scale of the DXF file : ",
/*102*/u8"Enter search text : ",
/*103*/u8"Search : ",
/*104*/u8"Change to : ",
/*105*/u8"O.K.",
/*106*/u8"Enter the axis offset relative to the center of symmetry of the trace : ",
/*107*/u8"The function is temporary inactive in this version of the program.",
/*108*/u8"Enter a name of the new subdirectory : ",
/*109*/u8"Delete block prototypes ? (Yes / No / Esc) : ",  ///////////////////////////
/*110*/u8"Select object to get parameters list",
/*111*/u8"Select row to edit parameter or append to the clipboard list, or press Esc",
/*112*/u8"Pick the hatch start point : ",
/*113*/u8"Enter the point dx size [mm]: ",
/*114*/u8"Enter the point dy size [mm]: ",
/*115*/u8"The maximum layer number: 256",
/*116*/u8"The minimum layer number: 1",
/*117*/u8"Layer doesn't exist.",
/*118*/u8"First symbol : ",
/*119*/u8"File not saved. Do you want to save itć ? (Yes / No / Esc): ",
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
/*136*/u8"Enter prefiks of zone numbers : ",
/*137*/u8"Enter the numer of first zone : ",
/*138*/u8"Enter zone hight [mm] : ",
/*139*/u8"Enter zone width [mm] : ",
/*140*/u8"Enter zone margin width [mm] : ",
/*141*/u8"Enter zones frame offset [mm] : ",
/*142*/u8"",
/*143*/u8"",
/*144*/u8"Enter the Z coordinate : ",
/*145*/u8"",
/*146*/u8"",
/*147*/u8"",
/*148*/u8"",
/*149*/u8"",
/*150*/u8"",
/*151*/u8"",
/*152*/u8"",
/*153*/u8"",
/*154*/u8"Enter the autopanorama factor 0.01...1 : ",
/*155*/u8"",
/*156*/u8"",
/*157*/u8"",
/*158*/u8"",
/*159*/u8"",
/*160*/u8"..........To ignore background while printing ?",
/*161*/u8"Select the background color (or ESC)",
/*162*/u8"Ignore the background ?",
/*163*/u8"Select the imposed color of the image points (or ESC)",
/*164*/u8"Inwersion B/W on the printout ?",
/*165*/u8"",
/*166*/u8"",
/*167*/u8"",
/*168*/u8"",
/*169*/u8"",
/*170*/u8"",
/*171*/u8"",
/*172*/u8"",
/*173*/u8"Selected objects do not form enclosed area.",
/*174*/u8"",
/*175*/u8"",
/*176*/u8"",
/*177*/u8"",
/*178*/u8"",
/*179*/u8"",
/*180*/u8"",
/*181*/u8"",
/*182*/u8"Point a sheet block (@SHEET)",
/*183*/u8"Edit image block ?",
/*184*/u8"Enter the rotation angle of the image block : ",
/*185*/u8"Reload edited image block ? (Yes / No / Esc): ",
/*186*/u8"",
/*187*/u8"",
/*188*/u8"",
/*189*/u8"",
/*190*/u8"",
/*191*/u8"Point to the object to specify the hidden layer",
/*192*/u8"Number of vertices of the polyline approximating the circle or arc scaled on an anisotropic scale: ",
/*193*/u8"Enter the default value of the maximum memory size for drawing [KB]: ",
/*194*/u8"Enter the default value of the printing buffer [KB]: ",
/*195*/u8"Indicate layer number: ",
/*196*/u8"Pick a pattern filled area, solid or trace to determine fill parameters",
/*197*/u8"Pick a pattern filled area, solid or trace to change fill parameters",
/*198*/u8"Enter the block fixed scale factor: ",
/*199*/u8"Enter force rescaling factor: ",
/*200*/u8"Enter moment rescaling factor: ",
/*201*/u8"Enter dicplacement rescaling factor: ",
/*202*/u8"Enter rotation rescaling factor: ",
/*203*/u8"Enter load rescaling factor: ",
/*204*/u8"Enter thermal load rescaling factor: ",
/*205*/u8"Enter force precision: ",
/*206*/u8"Enter moment precision: ",
/*207*/u8"Enter dicplacement precision: ",
/*208*/u8"Enter rotation precision: ",
/*209*/u8"Enter load precision: ",
/*210*/u8"Enter thermal load precision: ",
/*211*/u8"Enter property number: ",
/*212*/u8"Enter the rescaling factor for the resulting axial forces: ",
/*213*/u8"Enter the rescaling factor for the resulting shear forces: ",
/*214*/u8"Enter the rescaling factor for the resulting bending moments: ",
/*215*/u8"Enter the rescaling factor for the resulting displacements: ",
/*216*/u8"Enter the rescaling factor for the resulting support concentrated reaction forces: ",
/*217*/u8"Enter the rescaling factor for the resulting support reaction moments: ",
/*218*/u8"Enter section depth rescaling factor: ",
/*219*/u8"Enter the rescaling factor for the resulting stresses (steel, timber): ",
/*220*/u8"Enter axial & shear stress precision: ",
/*221*/u8"Enter the exaggerating factor of modal vibration modes deformation: ",
/*222*/u8"Enter the rescaling factor for the resulting reinforcement percentage: ",
/*223*/u8"Enter surface load rescaling factor: ",
/*224*/u8"Enter the rescaling factor for the resulting stresses (RC): ",
/*225*/u8"Enter the rescaling factor for the resulting support distributed reaction forces: ",
/*226*/u8"Enter reactions precision: ",
"",
"",
};


static char* komunikaty0[] =
/*0*/{ u8"",
/*1*/u8"Text",
/*2*/u8"Stretch - to point",
/*3*/u8"Stretch - from point",
/*4*/u8"Move - from point",
/*5*/u8"Move - to point",
/*6*/u8"Dimensioning",
/*7*/u8"Line from point",
/*8*/u8"Line to point",
/*9*/u8"Circle - center",
/*10*/u8"Circle - redius",
/*11*/u8"Fillet - select first line",
/*12*/u8"Fillet - select second line",
/*13*/u8"Arc - three point",
/*14*/u8"Arc - beginning-center-end",
/*15*/u8"Arc - beginning-center-angle",
/*16*/u8"Arc - beginning-center-chord",
/*17*/u8"Arc - beginning-end-radius",
/*18*/u8"Arc - beginning-end-angle",
/*19*/u8"Arc - beginning-end-direction",
/*20*/u8"Arc - continuation",
/*21*/u8"Select objects to delete",
/*22*/u8"Select objects to move",
/*23*/u8"Select objects to copy",
/*24*/u8"Copy - from point",
/*25*/u8"Copy - to point",
/*26*/u8"Select objects to scale",
/*27*/u8"Base point",
/*28*/u8"Scale",
/*29*/u8"Draw square",
/*30*/u8"Window from point",
/*31*/u8"Window to point",
/*32*/u8"Select objects to rotate",
/*33*/u8"Rotation",
/*34*/u8"Select by frame objects to stretch",
/*35*/u8"Not enough memory",
/*36*/u8"Select objects to save in file",
/*37*/u8"Mark insert point (Home +90°, End -90°)",
/*38*/u8"Profile cross-section",
/*39*/u8"Edit text",
/*40*/u8"Color, Angle, Heightć, Font",    /*nie uzywane*/
/*41*/u8"Select blocks to explode",
/*42*/u8"Disc - center",
/*43*/u8"Disc - radius",
/*44*/u8"Chamfer - select first line",
/*45*/u8"Chamfer - select second line",
/*46*/u8"Break - select object",
/*47*/u8"Break - mark first point",
/*48*/u8"Break - mark second point",
/*49*/u8"Autosave to BACKUP.ALF file",
/*50*/u8"Select object to change properties",
/*51*/u8"Select edges for extension",
/*52*/u8"Select objects to extend",
/*53*/u8"Mark vertex of the area",
/*54*/u8"Mark point",
/*55*/u8"Distance - first point",
/*56*/u8"Distance - second point",
/*57*/u8"Polygon - first point of a side",
/*58*/u8"Polygon - inscribed in a circle",
/*59*/u8"Polygon - circumscribed on a circle",
/*60*/u8"Polygon - second point of a side",
/*61*/u8"Polygon - radius of a circle",
/*62*/u8"Sketch  -Mark start point",
/*63*/u8"Sketch",
/*64*/u8"Select object for grouping",
/*65*/u8"Copy parallel (offset) to point",
/*66*/u8"Copy parallel (offset) by distance",
/*67*/u8"Select object to parallel copy (offset)",
/*68*/u8"Select cutting edges",
/*69*/u8"Select object to be cut",
/*70*/u8"Select object to mirror",
/*71*/u8"First point of symmetry line (axis)",
/*72*/u8"Second point of symmetry line (axis)",
/*73*/u8"Select objects for rectangular copying",
/*74*/u8"Select objects for polar copying",
/*75*/u8"Indicate with rectangular the distances between columns and rows",
/*76*/u8"Pick center point",
/*77*/u8"Pick base point",
/*78*/u8"Trace from point",
/*79*/u8"Trace to point",
/*80*/u8"Polyline from point",
/*81*/u8"Select line or arc for continuation",
/*82*/u8"Select object to divide",
/*83*/u8"Select object to measure",
/*84*/u8"Angle - Third point",
/*85*/u8"Angle - First point",
/*86*/u8"Angle - Second point",
/*87*/u8"Indicate hatch boundaries",
/*88*/u8"Indicate the inside of the area (Home +45°,End -45°,PgUp +3°,PgDn -3°)",
/*89*/u8"Indicate the beginning of the first axis of the ellipse",
/*90*/u8"Indicate the end of the first axis of the ellipse",
/*91*/u8"Indicate the center of the ellipse",
/*92*/u8"Indicate the length of the second semi-axis",
/*93*/u8"Indicate the starting point of the chart",
/*94*/u8"Indicate the ending point of the chart",
/*95*/u8"Select objects for instant remove",
/*96*/u8"",
/*97*/u8"Indicate area boundaries",
/*98*/u8"Indicate the inside of the area",
/*99*/u8"",
/*100*/u8"2-line from point",
/*101*/u8"2-line to point",
/*102*/u8"Axis - First point",
/*103*/u8"Axis - Second point",
/*104*/u8"3-line from point",
/*105*/u8"3-line to point",
/*106*/u8"Changing of data format of drawing...",
/*107*/u8"Updating the drawing in a new format",
/*108*/u8"Changing of data format of block...",
/*109*/u8"Updating the block in a new format",
/*110*/u8"Polyline from point",
/*111*/u8"Polyline to point",
/*112*/u8"Select frozen blocks (*,$) to explode",
/*113*/u8"Entrance ->  point the file name of drawing",
/*114*/u8"Indicate device dimansions",
/*115*/u8"Indicate dimensions of Scheme +",
/*116*/u8"Select objects for devices specification",
/*117*/u8"Indicate rectangle dimensions",
/*118*/u8"Select texts to change",
/*119*/u8"Select texts (symbols) to change",
/*120*/u8"Symbols library  ->  select symbol :",
/*121*/u8"Text (PgUp +3°, PgDn -3°, Home +90°, End -90°, 'E' edit)",
/*122*/u8"Indicate the inside of the area (Home +45°,End -45°,PgUp +3°,PgDn -3°)",
/*123*/u8"Mark the start point of Bezier curve",
/*124*/u8"Mark the control point of Bezier curve",
/*125*/u8"",
/*126*/u8"Axis from point:",
/*127*/u8"Axis to point:",
/*128*/u8"reading...",
/*129*/u8"waiting...",
/*130*/u8"",
/*131*/u8"",
/*132*/u8"",
/*133*/u8"Select objects to save in clipboard",
/*134*/u8"Select image to change properties",
/*135*/u8"Select image to export",
/*136*/u8"Image buffering",
/*137*/u8"Interrupt --> Esc ",
/*138*/u8"Select hatching block to change pattern (Home +45°,End -45°,PgUp +3°,PgDn -3°)",
/*139*/u8"Indicate the inside of the area (Home +90°,End -90°)",
/*140*/u8"Indicate the inside of the area",
/*141*/u8"Select spline to change tensions",
/*142*/u8"Indicate magnitude of tensions",
/*143*/u8"Pan-Shift from point",
/*144*/u8"Pan-Shift to point",
/*145*/u8"Leading line from point",
/*146*/u8"Leading line to point",
/*147*/u8"Select an object for quadrilateral transformation",
/*148*/u8"Converting file to BMP format",
/*149*/u8"Converting file to PBM format",
/*150*/u8"Converting file to ALX format",
/*151*/u8"Indicate the starting angle of the elliptical arc",
/*152*/u8"Indicate the end angle of the elliptical arc",
/*153*/u8"Select the line or side of the polygon or solid to which the ellipse should be tangential",
/*154*/u8"Indicate the location of a point on the ellipse to determine the second semi-axis",
/*155*/u8"Vector from point",
/*156*/u8"Vector to point",
/*157*/u8"Vector from point  (Home->invert arrows, End->swap values)",
/*158*/u8"Vector to point  (Home->invert arrows, End->swap values)",
/*159*/u8"",
/*160*/u8"",
/*161*/u8"",
/*162*/u8"",
/*163*/u8"",
/*164*/u8"",
/*165*/u8"",
/*166*/u8"",  //custom text
/*167*/u8"Indicate the frame or truss diagram for static/dynamic analysis",
/*168*/u8"Dimensioning (to start dimensioning an angle, touch any line or element vector)",
/*169*/u8"Indicate frame or truss element",
/*170*/u8"Place the section line at the desired element coordinates. Click to save the values to the clipboard",
/*171*/u8"Indicate the plate diagram for static analysis (FEM)",
/*172*/u8"Indicate the shield diagram for static analysis (FEM)",
};

static char* messages_str[] =
/*0*/{ u8"Arc - beginning-point-end",
/*1*/u8"Arc - beginning-center-end",
/*2*/u8"Arc - beginning-center-angle",
/*3*/u8"Arc - beginning-center-chord",
/*4*/u8"Arc - beginning-end-radius",
/*5*/u8"Arc - beginning-end-angle",
/*6*/u8"Arc - beginning-end-direction",
/*7*/u8"Arc - continuation",
/*8*/u8"",
/*9*/u8"File  %s",
/*10*/u8"%s - CENTER - radius",
/*11*/u8"%s - center - RADIUS",
/*12*/u8"%s - center - DIAMETER",
/*13*/u8"%s - first point of diameter",
/*14*/u8"%s - second point of diameter",
/*15*/u8"%s - first point",
/*16*/u8"%s - second point",
/*17*/u8"%s - third point",
/*18*/u8"%s - select first object",
/*19*/u8"%s - select second object",
/*20*/u8"%s - CENTER - diameter",
/*21*/u8"",
/*22*/u8"",
/*23*/u8"DXF-> %s",
/*24*/u8"%s - first point of radius",
/*25*/u8"%s - second point of radius",
};

#define ORTHO u8"   Ortho."
#define _LAYER_ u8"Layer"
#define _MODE_ u8"Mode"
#define _V_ u8"V"
#define _E_ u8"E"
#define _P_ u8"P"

static char gg[10][16] = { u8"Endpoint", u8"Nearest end", u8"Middle", u8"Intersection", u8"Center", u8"Perpendicular",
					u8"Tangential", u8"Adjacent", u8"point","----" };


#endif

#ifdef __O_LUK__
/*static*/ char* komunikaty_arc[] = {
   u8"Arc - beginning-point-end",
   u8"Arc - beginning-center-end",
   u8"Arc - beginning-center-angle",
   u8"Arc - beginning-center-chord",
   u8"Arc - beginning-end-radius",
   u8"Arc - beginning-end-angle",
   u8"Arc - beginning-end-direction",
   u8"Arc - continuation", };

#define r6 6

#endif

#ifdef __BUF__

#define _NOT_ENOUGH_MEMORY_ u8"NOENOUGH MEMORY OR PROGRAM ERROR"

#endif

#ifdef __DXF_O__

#define _PCX_FILE_ERROR_  u8"PCX file write error"
#define _PNG_FILE_ERROR_  u8"PNG file write error"
#define _UNKNOWN_IMAGE_FORMAT_ u8"Unknown image file format"
#define _FILE_ u8"File "
#define _FILE_EXISTS_ u8" exists. Overwrite it ? (Yes/No/Esc):"

#define _YES_NO_ESC_ u8"YNyn\033"
#define _YES_ 'Y'
#define _yes_ 'y'
#define _NO_ 'N'
#define _no_ 'n'

#endif

#ifdef __O_ZB__

#define _DRAWING_V_1_ u8"Drawing <ver.1.0> Convert with ALF v.1.1 - 1.9 first"
#define _BLOCK_V_1_ u8"Block <ver.1.0>. Convert if using ALF v.1.1 - 1.9"

#define VECTORIZATION "Vectorization"

POLE pmNie_Tak[] = {
	{u8"No",'N',113,NULL},
	{u8"Yes",'Y',112,NULL} };

char zb_err_message[] = u8"Error converting file.Exit code :";
char zb_err_message1[] = u8"Error converting file";
char zb_unknown[] = u8"Unknown";
static char zb_confirm[] = u8"Confirm";

#define _FILE_CONVERSION_ERROR_ u8"File conversion error"
#define _INCORRECT_FILE_FORMAT_ u8"Incorrect file format"
#define _ERROR_WREITING_AUX_FILE_ u8"Error writing auxiliary file"
#define _FILE_NOT_FOUND_ u8"File not found "

static char* vectorization_param[] =
{
	"PBM scale",
	"PBM interpolation",
	"PBM threshold",
	"PBM invertion",
	"PBM filter",
	"PBM nofilter",
    "ALX scale",
    "ALX blacklevel",
    "ALX turdsize",
    "ALX alphamax",
    "ALX curve optimization tolerance",
    "ALX quantization"
};

static int no_vectorization_param = sizeof(vectorization_param) / sizeof(vectorization_param[0]);

#define _YES_NO_ESC_ u8"YNyn\033"
#define _YES_ 'Y'
#define _yes_ 'y'
#define _NO_ 'N'
#define _no_ 'n'
#define _YES__ "Y"
#define _NO__ "N"

#define _Yes_ u8"Yes"
#define _No_ u8"No"

#define _TOOLS_TO_INSTALL_ u8"not installed"
#define _INSTALL_TOOLS_ u8"Install necessary packages"

#endif

#ifdef __WINSIZE__

TCHAR greeting1[] = _T("Set up the drawing window");
TCHAR greeting2[] = _T("Use edges to stretch window");
TCHAR greeting3[] = _T("Use header to move window");
TCHAR greeting4[] = _T("Click inside to finish");

#endif

#ifdef __PCX__

#define _PROCESSED_ u8"processed "

#endif

#ifdef __O_HATCH1__

char _TO_MANY_OBJECTS_[]= {u8"To many objects are selected to proceed (max is 6000)"};

#endif

#ifdef __O_HATCH3__

char* vocabulary[] = { (char*)u8"   press any key:", (char*)u8"A", (char*)u8"center of mass",(char*)u8"First moment of area",(char*)u8"Area moment of inertia" };

#endif

#ifdef __O_LTTYPE__

char* add_new_font = u8"add new font";
#define LINE u8"Lines"

char lin_config_param[31][MaxLenNameLine] =
{
	"Dashed",
	"Dashdotted",
	"2-dotted",
	"Dotted",
	"Border",
	"Center",
	"Hidden",
	"Phantom",
	"Dashed_2",
	"Dashdotted_2",
	"2-dotted_2",
	"Dotted_2",
	"Border_2",
	"Center_2",
	"Hidden_2",
	"Phantom_2",
	"Dashed_x2",
	"Dashdotted_x2",
	"2-dotted_x2",
	"Dotted_x2",
	"Border_x2",
	"Center_x2",
	"Hidden_x2",
	"Phantom_x2",
	"3-dotted",
	"2-center",
	"3-dashed",
	"Phantom_3",
	"2-center_x2",
	"3-dashed_x2",
	"Multidashed",
};

#define CZCIONKI "Fonts"
#define CZCIONKI_DXF "Fonts-DXF"

char str[40] = u8"No file : ";

#endif

#ifdef __O_MEASURE__

char measure_comm[9][54] = { u8"area", u8"Point", u8"New angle of local coordinate system", u8"   ->presss any key:", u8"Angle", u8"Current angle of local coordinate system:", u8"Mark on image two base points", u8"Mark on drawing two cartometric points", u8"Angle" };
#define _SUBSTRACT_ u8"substract"
#define _ADD_ u8"add"

#endif

#ifdef __O_CONFIG__

static char konf_global[8] = "Global";
static char konf_lokal[8] = "Local";
static char local_global[8] = "Local";

#define config_labels0 u8"Frame"
#define config_labels1 u8"Background"
#define config_labels2 u8"Text"
#define config_labels3 u8"Cursor"
#define config_labels4 u8"Block"
#define config_labels5 u8"Frame"
#define config_labels6 u8"Background"
#define config_labels7 u8"Text"
#define config_labels8 u8"Edit background"
#define config_labels9 u8"Edit text"
#define config_labels10 u8"config:"
#define config_labels11 u8"other" 

#define config_groups0 u8"MENU COLORS"
#define config_groups1 u8"DESKTOP COLORS"

#define config_buttons0 u8"OK"
#define config_buttons1 u8"Esc"
#define config_buttons2 u8"" //"Global"
#define config_buttons3 u8"" //"Local"
#define config_buttons4 u8"Save"

#define config_title  u8"C O N F I G U R A T I O N"

#define image_conf_tips0  u8"@Global"
#define image_conf_tips1  u8"Local"

#endif

#ifdef __O_PRNFUN__

static char err_message[] = u8"Error converting file.Exit code :";
static char err_message_cups[] = u8"Error executing 'lp' command.Exit code :";
static char confirm[] = u8"Confirm";

#define _CANNOT_CREATE_PDF_ u8"error: cannot create PdfDoc object\n"
#define _FILE_WRITING_ERROR_ u8"Image file writing error"

#endif

#ifdef __O_SECTION_DATA__
static char confirm[] = u8"Confirm";
#define _CANNOT_OPEN_ u8"Unable to open "
#define _CANNOT_READ_ u8"Unable to read "
#define _DEFAULT_TAKEN_ u8"Default wood parameters were adopted"
#endif

#ifdef __O_STATIC__

#define _SELECT_STATE_ "Limit state (combination)"
#define _SELECT_STATE_C_ L'L'

/*
POLE pmSelect_State[] = {
	{u8"Ultimate limit state (ULS)",'U',0,NULL},
	{u8"Serviceability limit state (SLS)",'S',0,NULL},
    {u8"serviceability limit state (Quasi-permanent)",'Q',0,NULL},
};

static TMENU mSelect_State = { 3,0,0,32,20,7, 0,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0, (POLE(*)[]) &pmSelect_State,NULL,NULL };
*/

#define _PROCEED_STATIC_ u8"Proceed static analysis of the specified frame or truss?"

#define _incorrectly_defined_ u8"incorrectly defined"
#define _property_not_defined_ u8"property not defined or defined incorrectly"
#define _reaction_not_associated_ u8"reaction not associated with any known node"
#define _reaction_not_enough_in_Y_ u8"Structure insufficiently supported in Y axis. Add missing support."
#define _reaction_not_enough_in_X_ u8"Structure insufficiently supported in X axis. Add missing support."
#define _reaction_not_enough_in_X_Y_ u8"Structure insufficiently supported in X and Y axes. Add missing supports."
#define _node_size_not_associated_ u8"node size not associated with any known node"
#define _force_not_associated_ u8"not associated with any known node"
#define _load_not_associated_ u8"not associated with any known element"
#define _thermal_load_inside_element_ u8"not on the entire element with nodes coordinates:"
#define _unknown_standard_ u8"Unknown standard"
#define _element_graph_data_failed_ u8"It was unabled to create resulting forces data block for element"
#define _cannot_create_folder_ u8"Cannot create file directory"

#define _FRAME3DD_ u8"%FRAME:"
#define _FRAME3DD_PL u8"%RAMA:"
#define _FRAME3DD_UA u8"%КАРКАС:"
#define _FRAME3DD_ES u8"%MARCO:"

#define _FRAME_ID_NOT_FOUND_ u8"'%FRAME: ID' not found"

#define _Yes_ "Yes"
#define _No_ "No"
#define _YES_NO_ESC_ u8"YNyn\033"
#define _YES_ 'Y'
#define _yes_ 'y'
#define _NO_ 'N'
#define _no_ 'n'

#define __FRAME3DD__ "Frame3dd"

static char confirm[] = u8"Confirm";
#define _CANNOT_CREATE_DEFORMATION_BLOCK_ u8"Unable to create deform shape block"
#define _CANNOT_CREATE_FORCE_BLOCK_ u8"Unable to create force graph block"
#define _CANNOT_CREATE_MOMENT_BLOCK_ u8"Unable to create moment graph block"
#define _CANNOT_CREATE_STRESS_BLOCK_ u8"Unable to create stress graph block"
#define _CANNOT_CREATE_SHEAR_STRESS_BLOCK_ u8"Unable to create shear stress graph block"
#define _CANNOT_CREATE_RESULTS_FILE_ u8"Unable to open results file"
#define _CANNOT_CREATE_RESULTS_PDF_FILE_ u8"Unable to create results PDF file"
#define _CANNOT_OPEN_RESULTS_PDF_FILE_ u8"Unable to open results PDF file"
#define _INSTALL_PDF_VIEWER_ u8"Install \"Okular\" or \"Evince\" PDF viewer, please"

#define _CANNOT_OPEN_DEFORMATION_DATA_FILE_ u8"Unable to open deformation data file"
#define _CANNOT_OPEN_RESULTS_DATA_FILE_ u8"Unable to open results data file"
#define _CANNOT_OPEN_DYNAMIC_RESULTS_DATA_FILE_ u8"Unable to open dynamic results data file"
#define _CANNOT_CREATE_NODES_AND_ELEMENTS_BLOCK_ u8"Unable to create nodes and elements block"
#define _CANNOT_CREATE_REACTIONS_BLOCK_ u8"Unable to create reactions block"
#define _CANNOT_CREATE_NEW_LAYER_ u8"Unable to create new layer. To many layers already created"

char *frame3dd[]={
    /*0*/ u8"error-free completion",
    /*1*/ u8"unknown error",
    /*2*/ u8"error with the command line options (see Section 11, above)",
    /*3*/ u8"error with the command line option for shear deformation -s",
    /*4*/ u8"error with the command line option for geometric stiffness -g",
    /*5*/ u8"error with the command line option for lumped mass -l",
    /*6*/ u8"error with the command line option for modal analysis method -m",
    /*7*/ u8"error with the command line option for modal analysis tolerance -t",
    /*8*/ u8"error with the command line option for modal analysis shift -f",
    /*9*/ u8"error with the command line option for pan rate -p",
    /*10*/ u8"error with the command line option for matrix condensation -r",
    /*11*/ u8"error in opening the Input Data file",
    /*12*/ u8"error in opening the temporary cleaned input data file for writing",
    /*13*/ u8"error in opening the temporary cleaned input data file for reading",
    /*14*/ u8"error in opening the Output Data file",
    /*15*/ u8" error in creating the path for temporary output data files",
    /*16*/ u8" error in creating the temporary output data file path name",
    /*17*/ u8"error in opening the .CSV (spread-sheet) output data file",
    /*18*/ u8"error in opening the .M (matlab) output data file",
    /*19*/ u8"error in opening the interior force output data file for writing",
    /*20*/ u8"error in opening the interior force output data file for reading",
    /*21*/ u8"error in opening the undeformed mesh ouput data file",
    /*22*/ u8"error in opening the deformed mesh ouput data file",
    /*23*/ u8"error in opening the plotting script file for writing first static load case plots",
    /*24*/ u8"error in opening the plotting script file for appending second and higher static load case results",
    /*25*/ u8"error in opening the plotting script file for appending modal plots",
    /*26*/ u8"error in opening the plotting script file for appending modal animations",
    /*27*/ u8"error in opening the modal mesh data file",
    /*28*/ u8"error in opening the modal mesh animation data file",
    /*29*/ u8"error in opening the mass data debugging file , MassData.txt",
    /*30*/ u8"cubic curvefit system matrix for element deformation is not positive definite",
    /*31*/ u8"non-positive definite structural static stiffness matrix",
    /*32*/ u8"error in eigen-problem analysis. Try again with reduced or zero number of dynamic modes",
    /*33*/ u8"error-free completion",
    /*34*/ u8"error-free completion",
    /*35*/ u8"error-free completion",
    /*36*/ u8"error-free completion",
    /*37*/ u8"error-free completion",
    /*38*/ u8"error-free completion",
    /*39*/ u8"error-free completion",
    /*40*/ u8"error in input data file",
    /*41*/ u8"input data formatting error in the node data, node number out of range",
    /*42*/ u8"input data formatting error in node or element data, unconnected node",
    /*43*/ u8"error-free completion",
    /*44*/ u8"error-free completion",
    /*45*/ u8"error-free completion",
    /*46*/ u8"error-free completion",
    /*47*/ u8"error-free completion",
    /*48*/ u8"error-free completion",
    /*49*/ u8"error-free completion",
    /*50*/ u8"error-free completion",
    /*51*/ u8"input data formatting error in the frame element data, frame element number out of range",
    /*52*/ u8"input data formatting error in the frame element data, node number out of range",
    /*53*/ u8"input data formatting error in the frame element data, negative section value",
    /*54*/ u8"input data formatting error in the frame element data, cross section area is 0 (zero)",
    /*55*/ u8"input data formatting error in the frame element data, shear area and shear modulus are 0 (zero)",
    /*56*/ u8"input data formatting error in the frame element data, torsional moment of inertia is 0 (zero)",
    /*57*/ u8"input data formatting error in the frame element data, bending moment of inertia is 0 (zero)",
    /*58*/ u8"input data formatting error in the frame element data, modulus value is non-positive",
    /*59*/ u8"input data formatting error in the frame element data, mass density value is non-positive",
    /*60*/ u8"nput data formatting error in the frame element data, frame element starts and stops at the same node",
    /*61*/ u8"input data formatting error in the frame element data, frame element has length of zero",
    /*62*/ u8"error-free completion",
    /*63*/ u8"error-free completion",
    /*64*/ u8"error-free completion",
    /*65*/ u8"error-free completion",
    /*66*/ u8"error-free completion",
    /*67*/ u8"error-free completion",
    /*68*/ u8"error-free completion",
    /*69*/ u8"error-free completion",
    /*70*/ u8"error-free completion",
    /*71*/ u8"input data formatting error with the 'shear' variable specifying shear deformation",
    /*72*/ u8"input data formatting error with the 'geom' variable specifying geometric stiffness",
    /*73*/ u8"input data formatting error with the 'exagg_static' variable specifying static mesh exageration",
    /*74*/ u8"input data formatting error with the 'dx' variable specifying the length of the internal force x-axis increment",
    /*75*/ u8"error-free completion",
    /*76*/ u8"error-free completion",
    /*77*/ u8"error-free completion",
    /*78*/ u8"error-free completion",
    /*79*/ u8"error-free completion",
    /*80*/ u8"input data formatting error in reaction data, number of nodes with reactions out of range",
    /*81*/ u8"input data formatting error in reaction data, node number out of range",
    /*82*/ u8"input data formatting error in reaction data, reaction data is not 1 (one) or 0 (zero)",
    /*83*/ u8"input data formatting error in reaction data, specified node has no reactions",
    /*84*/ u8"input data formatting error in reaction data, under-restrained structure",
    /*85*/ u8"input data formatting error in reaction data, fully restrained structure",
    /*86*/ u8"input data formatting error in extra node inertia data, node number out of range",
    /*87*/ u8"input data formatting error in extra beam mass data, frame element number out of range",
    /*88*/ u8"input data formatting error in mass data, frame element with non-positive mass",
    /*89*/ u8"error-free completion",
    /*90*/ u8"input data formatting error in matrix condensation data, number of nodes with condensed degrees of freedom are less than the total number of nodes",
    /*91*/ u8"input data formatting error in matrix condensation data, node number out of range",
    /*92*/ u8"input data formatting error in matrix condensation data, mode number out of range",
    /*93*/ u8"error-free completion",
    /*94*/ u8"input data formatting error in matrix condensation data, number of condensed degrees of freedom greater than number of modes",
    /*95*/ u8"error-free completion",
    /*96*/ u8"error-free completion",
    /*97*/ u8"error-free completion",
    /*98*/ u8"error-free completion",
    /*99*/ u8"error-free completion",
    /*100*/ u8"input data formatting error in load data",
    /*101*/ u8"number of static load cases must be greater than zero",
    /*102*/ u8"number of static load cases must be less than 112",   //64 30
    /*103*/ u8"error-free completion",
    /*104*/ u8"error-free completion",
    /*105*/ u8"error-free completion",
    /*106*/ u8"error-free completion",
    /*107*/ u8"error-free completion",
    /*108*/ u8"error-free completion",
    /*109*/ u8"error-free completion",
    /*110*/ u8"error-free completion",
    /*111*/ u8"error-free completion",
    /*112*/ u8"error-free completion",
    /*113*/ u8"error-free completion",
    /*114*/ u8"error-free completion",
    /*115*/ u8"error-free completion",
    /*116*/ u8"error-free completion",
    /*117*/ u8"error-free completion",
    /*118*/ u8"error-free completion",
    /*119*/ u8"error-free completion",
    /*120*/ u8"error-free completion",
    /*121*/ u8"input data formatting error in nodal load data, node number out of range",
    /*122*/ u8"error-free completion",
    /*123*/ u8"error-free completion",
    /*124*/ u8"error-free completion",
    /*125*/ u8"error-free completion",
    /*126*/ u8"error-free completion",
    /*127*/ u8"error-free completion",
    /*128*/ u8"error-free completion",
    /*129*/ u8"error-free completion",
    /*130*/ u8"error-free completion",
    /*131*/ u8"input data formatting error in uniformly-distributed load data, number of uniform loads is greater than the number of frame elements",
    /*132*/ u8"input data formatting error in uniformly-distributed load data, frame element number out of range",
    /*133*/ u8"error-free completion",
    /*134*/ u8"error-free completion",
    /*135*/ u8"error-free completion",
    /*136*/ u8"error-free completion",
    /*137*/ u8"error-free completion",
    /*138*/ u8"error-free completion",
    /*139*/ u8"error-free completion",
    /*140*/ u8"input data formatting error in trapezoidally-distributed load data, too many trapezoidally distributed loads",
    /*141*/ u8"input data formatting error in trapezoidally-distributed load data, frame element number out of range",
    /*142*/ u8"input data formatting error in trapezoidally-distributed load data, x1 < 0",
    /*143*/ u8"input data formatting error in trapezoidally-distributed load data, x1 > x2",
    /*144*/ u8"input data formatting error in trapezoidally-distributed load data, x2 > L",
    /*145*/ u8"error-free completion",
    /*146*/ u8"error-free completion",
    /*147*/ u8"error-free completion",
    /*148*/ u8"error-free completion",
    /*149*/ u8"error-free completion",
    /*150*/ u8"input data formatting error in concentrated internal load data, number concentrated loads greater than number of frame elements",
    /*151*/ u8"input data formatting error in internal concentrated load data, frame element number out of range",
    /*152*/ u8"input data formatting error in internal concentrated load data, x-location less than 0 or grater than L",
    /*153*/ u8"error-free completion",
    /*154*/ u8"error-free completion",
    /*155*/ u8"error-free completion",
    /*156*/ u8"error-free completion",
    /*157*/ u8"error-free completion",
    /*158*/ u8"error-free completion",
    /*159*/ u8"error-free completion",
    /*160*/ u8"input data formatting error in thermal load data, number thermal loads greater than number of frame elements",
    /*161*/ u8"input data formatting error in thermal load data, frame element number out of range",
    /*162*/ u8"input data formatting error in thermal load data, frame element number out of range",
    /*163*/ u8"error-free completion",
    /*164*/ u8"error-free completion",
    /*165*/ u8"error-free completion",
    /*166*/ u8"error-free completion",
    /*167*/ u8"error-free completion",
    /*168*/ u8"error-free completion",
    /*169*/ u8"error-free completion",
    /*170*/ u8"error-free completion",
    /*171*/ u8"input data formatting error in prescribed displacement data, prescribed displacements may be applied only at coordinates with reactions",
    /*172*/ u8"error-free completion",
    /*173*/ u8"error-free completion",
    /*174*/ u8"error-free completion",
    /*175*/ u8"error-free completion",
    /*176*/ u8"error-free completion",
    /*177*/ u8"error-free completion",
    /*178*/ u8"error-free completion",
    /*179*/ u8"error-free completion",
    /*180*/ u8"error-free completion",
    /*181*/ u8"Warning: elastic instability (elastic + geometric stiffness matrix not positive definite)",
    /*182*/ u8"Warning: large strain (the average axial strain in one or more elements is greater than 0.001)",
    /*183*/ u8"Warning: large strain and elastic instability",
    /*184*/ u8"error-free completion",
    /*185*/ u8"error-free completion",
    /*186*/ u8"error-free completion",
    /*187*/ u8"error-free completion",
    /*188*/ u8"error-free completion",
    /*189*/ u8"error-free completion",
    /*190*/ u8"error-free completion",
    /*191*/ u8"error-free completion",
    /*192*/ u8"error-free completion",
    /*193*/ u8"error-free completion",
    /*194*/ u8"error-free completion",
    /*195*/ u8"error-free completion",
    /*196*/ u8"error-free completion",
    /*197*/ u8"error-free completion",
    /*198*/ u8"error-free completion",
    /*199*/ u8"error-free completion",
    /*200*/ u8"memory allocation error",
    /*201*/ u8"error in opening an output data file saving a vector of 'floats'",
    /*202*/ u8"error in opening an output data file saving a vector of 'ints'",
    /*203*/ u8"error in opening an output data file saving a matrix of 'floats'",
    /*204*/ u8"error in opening an output data file saving a matrix of 'doubles'",
    /*205*/ u8"error in opening an output data file saving a symmetric matrix of 'floats'",
    /*206*/ u8"error in opening an output data file saving a symmetric matrix of 'doubles'",
    /*207*/ u8"equilibrium test error",
    /*208*/ u8"error opening error log file .err",
    };

#define _ERROR_FREE_COMPLETION_ u8"error-free completion"

#endif

#ifdef __O_PLATE__

#define _PLATE_ u8"%PLATE:"
#define _PLATE_PL u8"%PŁYTA:"
#define _PLATE_UA u8"%ПЛИТА:"
#define _PLATE_ES u8"%LOSA:"

#define _property_not_defined_ u8"property not defined or defined incorrectly"

#define _cannot_create_folder_ u8"Cannot create file directory"
#define _CANNOT_CREATE_RESULTS_FILE_ u8"Unable to open results file"
static char confirm[] = u8"Confirm";

#define _gmsh_error_ u8"gmsh error"
#define _ElmerGrid_error_ u8"ElmerGrid error"
#define _ElmerSolver_error_ u8"ElmerSolver error"

#define _CANNOT_OPEN_RESULTS_FILE_ u8"Cannot open results file"
#define _CANNOT_READ_RESULTS_FILE_ u8"Unable to read results file"

#define _CANNOT_CREATE_NEW_LAYER_ u8"Unable to create new layer. To many layers already created"
#define _CANNOT_CREATE_NODES_AND_ELEMENTS_BLOCK_ u8"Unable to create nodes and elements block"
#define _CANNOT_CREATE_DEFLECTION_BLOCK_ u8"Unable to create deflection block"
#define _CANNOT_CREATE_STRESS_BLOCK_ u8"Unable to create stress block"
#define _CANNOT_CREATE_STRAIN_BLOCK_ u8"Unable to create strain block"
#define _CANNOT_CREATE_REACTIONS_BLOCK_ u8"Unable to create reactions block"

#define _PROCEED_PLATE_FEM_ u8"Proceed static analysis of the plate?"

#define _POLYLINE_IS_NOT_CLOSED_ u8"polyline is not closed"
#define _THE_PLATE_ u8"The plate"
#define _THE_HOLE_ u8"The hole"
#define _THE_WALL_ u8"The wall"
#define _THE_ZONE_ u8"The zone"
#define _THE_PLATE_POLYLINE_NUMBER_IS_EQUAL_ZERO_ u8"The number of plate polylines is equal 0"
#define _THE_PLATE_POLYLINE_NUMBER_IS_GREATER_THAN_ u8"The number of plate polylines is greater than"
#define _NO_LOAD_ASSIGNED_ u8"No load assigned and no '%g' declared for self weight load"

#define _THE_HOLE_NOT_ASSIGNED_TO_ZONE_OR_PLATE_ u8"Hole not assigned to any zone or plate"
#define _THE_WALL_NOT_ASSIGNED_TO_ZONE_OR_PLATE_ u8"Wall not assigned to any zone or plate"
#define _THE_ZONE_NOT_ASSIGNED_TO_PLATE_ u8"Zone not assigned to any plate"

#define _THE_ZONE_IS_ASSIGNED_TO_ANOTHER_ZONE_ u8"The zone is contained within another zone, what is not yet allowed"

#define _PLATE_ID_NOT_FOUND_ u8"'%PLATE: ID' not found"
#define _PREPARING_DATA_ u8"Structure review and data preparation"
#define _BUILDING_MESH_ u8"Building mesh"
#define _BUILDING_GRID_ u8"Preparing grid for solver"
#define _SOLVING_ u8"Solving"
#define _GEOMETRY_ u8"Geometry"
#define _BUILDING_BLOCKS_ u8"Building blocks for "

#define _Yes_ "Yes"
#define _No_ "No"
#define _YES_NO_ESC_ u8"YNyn\033"
#define _YES_ 'Y'
#define _yes_ 'y'
#define _NO_ 'N'
#define _no_ 'n'

#define _CANNOT_PROCEED_IN_32BIT_ u8"The plate looks OK, but the Elmer FEM compute module doesn't work in a 32-bit system"
#define _BUY_NEW_COMPUTER_  u8"Buy a new computer"
#define _unknown_standard_ u8"Unknown standard"

#endif

#ifdef __O_SHIELD__

#define _SHIELD_ u8"%SHIELD:"
#define _SHIELD_PL u8"%TARCZA:"
#define _SHIELD_UA u8"%ЩИТ:"
#define _SHIELD_ES u8"%ESCUDO:"

#define _property_not_defined_ u8"property not defined or defined incorrectly"

#define _cannot_create_folder_ u8"Cannot create file directory"
#define _CANNOT_CREATE_RESULTS_FILE_ u8"Unable to open results file"
static char confirm[] = u8"Confirm";

#define _gmsh_error_ u8"gmsh error"
#define _ElmerGrid_error_ u8"ElmerGrid error"
#define _ElmerSolver_error_ u8"ElmerSolver error"

#define _CANNOT_OPEN_RESULTS_FILE_ u8"Cannot open results file"
#define _CANNOT_READ_RESULTS_FILE_ u8"Unable to read results file"

#define _CANNOT_CREATE_NEW_LAYER_ u8"Unable to create new layer. To many layers already created"
#define _CANNOT_CREATE_NODES_AND_ELEMENTS_BLOCK_ u8"Unable to create nodes and elements block"
#define _CANNOT_CREATE_DEFLECTION_BLOCK_ u8"Unable to create deflection block"
#define _CANNOT_CREATE_STRESS_BLOCK_ u8"Unable to create stress block"
#define _CANNOT_CREATE_STRAIN_BLOCK_ u8"Unable to create strain block"
#define _CANNOT_CREATE_REACTIONS_BLOCK_ u8"Unable to create reactions block"

#define _PROCEED_SHIELD_FEM_ u8"Proceed static analysis of the shield?"

#define _POLYLINE_IS_NOT_CLOSED_ u8"polyline is not closed"
#define _THE_SHIELD_ u8"The shield"
#define _THE_HOLE_ u8"The hole"
#define _THE_WALL_ u8"The wall"
#define _THE_ZONE_ u8"The zone"
#define _THE_SHIELD_POLYLINE_NUMBER_IS_EQUAL_ZERO_ u8"The number of shield polylines is equal 0"
#define _THE_SHIELD_POLYLINE_NUMBER_IS_GREATER_THAN_ u8"The number of shield polylines is greater than"
#define _NO_LOAD_ASSIGNED_ u8"No load assigned and no '%g' declared for self weight load"

#define _THE_HOLE_NOT_ASSIGNED_TO_ZONE_OR_SHIELD_ u8"Hole not assigned to any zone or shield"
#define _THE_WALL_NOT_ASSIGNED_TO_ZONE_OR_SHIELD_ u8"Wall not assigned to any zone or shield"
#define _THE_ZONE_NOT_ASSIGNED_TO_SHIELD_ u8"Zone not assigned to any shield"

#define _THE_ZONE_IS_ASSIGNED_TO_ANOTHER_ZONE_ u8"The zone is contained within another zone, what is not yet allowed"

#define _SHIELD_ID_NOT_FOUND_ u8"'%SHIELD: ID' not found"
#define _PREPARING_DATA_ u8"Structure review and data preparation"
#define _BUILDING_MESH_ u8"Building mesh"
#define _BUILDING_GRID_ u8"Preparing grid for solver"
#define _SOLVING_ u8"Solving"
#define _GEOMETRY_ u8"Geometry"
#define _BUILDING_BLOCKS_ u8"Building blocks for "

#define _Yes_ "Yes"
#define _No_ "No"
#define _YES_NO_ESC_ u8"YNyn\033"
#define _YES_ 'Y'
#define _yes_ 'y'
#define _NO_ 'N'
#define _no_ 'n'

#define _CANNOT_PROCEED_IN_32BIT_ u8"The shield looks OK, but the Elmer FEM compute module doesn't work in a 32-bit system"
#define _BUY_NEW_COMPUTER_  u8"Buy a new computer"
#define _unknown_standard_ u8"Unknown standard"

#define _load_not_associated_ u8"not associated with any edge"

#endif

#ifdef __O_TEXT3PDF__
#define _CANNOT_CREATE_PDF_ u8"error: cannot create PdfDoc object\n"
#endif


#ifdef __O_ANIMATE_DYN__

#define _ANIMATE_ u8"ESC: exit, BackSpace: previous, any other: next"

#endif

#ifdef __O_SEL__

#define _FIRST_  "FIRST"
#define _SECOND_ "SECOND"

#endif