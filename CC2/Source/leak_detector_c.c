#ifndef NDEBUG
#include	<stdio.h>
#ifdef MACOS
#include <stdlib.h>
#else
#include	<malloc.h>
#endif
#include	<string.h>
#include	"leak_detector_c.h"

//#ifndef NULL
//#define NULL		((void *)(0))
//#endif

#undef		malloc
#undef		calloc
#undef		realloc
#undef 		free


static MEM_LEAK * ptr_start = NULL;
static MEM_LEAK * ptr_next =  NULL;

static int counter = 0;
static int counter_m = 0;
static int counter_t = 0;
static int counter_f = 0;
static int counter_r = 0;

static int index0=0;

/*
 * adds allocated memory info. into the list
 *
 */
static void add(MEM_INFO alloc_info)
{

	MEM_LEAK * mem_leak_info = NULL;
	mem_leak_info = (MEM_LEAK *) malloc (sizeof(MEM_LEAK));
	mem_leak_info->mem_info.address = alloc_info.address;
	mem_leak_info->mem_info.size = alloc_info.size;
	strcpy(mem_leak_info->mem_info.file_name, alloc_info.file_name); 
	mem_leak_info->mem_info.line = alloc_info.line;
	mem_leak_info->next = NULL;
    mem_leak_info->flag=0;

	if (ptr_start == NULL)	
	{
		ptr_start = mem_leak_info;
		ptr_next = ptr_start;
	}
	else {
		ptr_next->next = mem_leak_info;
		ptr_next = ptr_next->next;
	}

	counter++;
    counter_t++;
}

/*
 * erases memory info. from the list
 *
 */
static void erase_pos(unsigned int pos)
{

	unsigned index = 0;
	MEM_LEAK * alloc_info, * temp;
	
	if(pos == 0)
	{
		//MEM_LEAK *
        temp = ptr_start;
		ptr_start = ptr_start->next;
		free(temp);
        counter--;
	}
	else 
	{
		for(index = 0, alloc_info = ptr_start; index < pos; 
			alloc_info = alloc_info->next, ++index)
		{
			if(pos == index + 1)
			{
				temp = alloc_info->next;
				alloc_info->next =  temp->next;
				free(temp);
                counter--;
				break;
			}
		}
	}
}

/*
 * deletes all the elements from the list
 */
static void clear_report()
{
	MEM_LEAK * temp = ptr_start;
	MEM_LEAK * alloc_info = ptr_start;

	while(alloc_info != NULL) 
	{
		alloc_info = alloc_info->next;
		free(temp);
		temp = alloc_info;
	}
}

/*
 * replacement of malloc
 */
void * xmalloc (unsigned int size, const char * file, unsigned int line)
{
	void * ptr = malloc (size);
	if (ptr != NULL) 
	{
		add_mem_info(ptr, size, file, line);
        counter_m++;
	}
	return ptr;
}

/*
 * replacement of calloc
 */
void * xcalloc (unsigned int elements, unsigned int size, const char * file, unsigned int line)
{
	unsigned total_size;
	void * ptr = calloc(elements , size);
	if(ptr != NULL)
	{
		total_size = elements * size;
		add_mem_info (ptr, total_size, file, line);
	}
	return ptr;
}

void * xrealloc (void *adr, unsigned int size, const char * file, unsigned int line)
{
    unsigned short index;
    MEM_LEAK  * leak_info = ptr_start;

    void *ptr = realloc(adr, size);
    if(ptr != NULL)
    {
        /* check if allocate memory is in our list, if yes, change address */
        for (index = 0; leak_info != NULL; ++index, leak_info = leak_info->next) {
            if (leak_info->mem_info.address == adr) {
                leak_info->mem_info.address = ptr;
                break;
            }
        }
        counter_r++;
    }
    return ptr;
}

/*
 * replacement of free
 */
void xfree(void * mem_ref)
{
	remove_mem_info(mem_ref);
    if (mem_ref!=NULL) free(mem_ref);
    else
    {
        int a=0;
    }
    counter_f++;
}

/*
 * gets the allocated memory info and adds it to a list
 *
 */
static void add_mem_info (void * mem_ref, unsigned int size,  const char * file, unsigned int line)
{
	MEM_INFO mem_alloc_info;

	/* fill up the structure with all info */
	memset( &mem_alloc_info, 0, sizeof ( mem_alloc_info ) );
	mem_alloc_info.address 	= mem_ref;
	mem_alloc_info.size = size;
	strncpy(mem_alloc_info.file_name, file, FILE_NAME_LENGTH);
	mem_alloc_info.line = line;
	
	/* add the above info to a list */
	add(mem_alloc_info);
}

/*
 * if the allocated memory info is part of the list, removes it
 *
 */
static void remove_mem_info (void * mem_ref)
{
	unsigned short index;
	MEM_LEAK  * leak_info = ptr_start;

	/* check if allocate memory is in our list */
    int found=0;
	for(index = 0; leak_info != NULL; ++index, leak_info = leak_info->next)
	{
		if (( leak_info->mem_info.address == mem_ref ) && (leak_info->flag==0))
		{
			erase_pos ( index ); //REM it to see all history
            //leak_info->flag=1;
            found=1;
			break;
		}
	}
    if (found==0)
    {
        int a=0;
    }
    counter_t--;
}

/*
 * writes all info of the unallocated memory into a file
 */
void report_mem_leak(void)
{
	unsigned short index;
	MEM_LEAK * leak_info;

	FILE * fp_write = fopen (OUTPUT_FILE, "wt");
	char info[1024];

	if(fp_write != NULL)
	{
		sprintf(info, "%s\n", "Memory Leak Summary");
		fwrite(info, (strlen(info) + 1) , 1, fp_write);
		sprintf(info, "%s\n", "-----------------------------------");	
		fwrite(info, (strlen(info) + 1) , 1, fp_write);
		index0=0;
		for(leak_info = ptr_start; leak_info != NULL; leak_info = leak_info->next)
		{
            if (leak_info->flag==0)
            {
                sprintf(info, "address : %llu\n", leak_info->mem_info.address);
                fwrite(info, (strlen(info) + 1), 1, fp_write);
                sprintf(info, "size    : %d bytes\n", leak_info->mem_info.size);
                fwrite(info, (strlen(info) + 1), 1, fp_write);
                sprintf(info, "file    : %s\n", leak_info->mem_info.file_name);
                fwrite(info, (strlen(info) + 1), 1, fp_write);
                sprintf(info, "line    : %d\n", leak_info->mem_info.line);
                fwrite(info, (strlen(info) + 1), 1, fp_write);
                sprintf(info, "%s\n", "-----------------------------------");
                fwrite(info, (strlen(info) + 1), 1, fp_write);
            }
            index0++;
		}
		sprintf(info, "counter: %d counter_t: %d counter_m: %d counter_f: %d counter_r: %d index0: %d\n", counter, counter_t,counter_m,counter_f,counter_r, index0);
		fwrite(info, (strlen(info) + 1), 1, fp_write);
        fclose(fp_write);
	}	
	clear_report();
}
#endif