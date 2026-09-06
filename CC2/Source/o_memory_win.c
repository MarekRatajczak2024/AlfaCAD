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

#ifndef LINUX
#include <wtypes.h>
#else
#include <unistd.h>
#endif

#ifdef BIT64
#define long_long long long
#else
#define long_long long
#endif


// Handle the width of the field in which to print numbers this way to
// make changes easier. The asterisk in the print format specifier
// "%*ld" takes an int from the argument list, and uses it to pad and
// right-justify the number being formatted.

#ifndef LINUX

typedef struct _MEMORYSTATUS1 {
	DWORD  dwLength;
	DWORD  dwMemoryLoad;
	SIZE_T dwTotalPhys;
	SIZE_T dwAvailPhys;
	SIZE_T dwTotalPageFile;
	SIZE_T dwAvailPageFile;
	SIZE_T dwTotalVirtual;
	SIZE_T dwAvailVirtual;
} MEMORYSTATUS1, *LPMEMORYSTATUS1;

/*
unsigned long_long Get_Free_Physical_Memory(void)
{ MEMORYSTATUS1 stat;
  double dwTotalPhys;
  double dwMemoryLoad;
  unsigned long_long intTotalPhys;

  GlobalMemoryStatus (&stat);

  //intTotalPhys=(unsigned long)stat.dwAvailVirtual;
  intTotalPhys = (unsigned long_long)stat.dwAvailPhys;
  return intTotalPhys;
}
*/

unsigned long long Get_Free_Physical_Memory(void)
{
	MEMORYSTATUSEX stat;
	unsigned long long intTotalPhys = 0;

	// This initialization tells the API exactly which structure layout size to write into
	stat.dwLength = sizeof(MEMORYSTATUSEX);

	// This block is completely safe on 32-bit and 64-bit architectures
	if (GlobalMemoryStatusEx(&stat)) {
		intTotalPhys = (unsigned long long)stat.ullAvailPhys;
	}

	return intTotalPhys;
}
/*
unsigned long_long Get_Free_Virtual_Memory(void)
{ MEMORYSTATUS stat;

  GlobalMemoryStatus (&stat);
  return stat.dwAvailVirtual;
}
 */
#else

unsigned long_long Get_Physical_Memory(void)
{
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    return pages * page_size;
}

#ifdef MACOS

#include <stdio.h>
#include <mach/mach.h>
#include <mach/host_info.h>

unsigned long long Get_Free_Physical_Memory(void) {
    mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;
    vm_statistics64_data_t vm_stats;

    mach_port_t host_port = mach_host_self();
    if (host_statistics64(host_port, HOST_VM_INFO64, (host_info64_t)&vm_stats, &count) != KERN_SUCCESS) {
        return 0;
    }

    long page_size = sysconf(_SC_PAGE_SIZE);

    // "Available" memory in macOS is typically Free + Inactive pages
    unsigned long long free_pages = vm_stats.free_count + vm_stats.inactive_count;

    return free_pages * (unsigned long long)page_size;
}


#else
#include <stdio.h>

unsigned long long Get_Free_Physical_Memory(void) {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) return 0;

    char buf[256];
    unsigned long long kb_available = 0;

    while (fgets(buf, sizeof(buf), fp)) {
        // MemAvailable is the most accurate metric for "free to use" memory
        if (sscanf(buf, "MemAvailable: %llu kB", &kb_available) == 1) {
            break;
        }
    }
    fclose(fp);
    return kb_available * 1024; // Convert KiB to Bytes
}


#endif

/*
unsigned long_long Get_Free_Virtual_Memory(void)
{
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    return pages * page_size;
}
*/

#endif

/*
printf ("The MemoryStatus structure is %ld bytes long.\n",
stat.dwLength);
printf ("It should be %d.\n", sizeof (stat));
printf ("%ld percent of memory is in use.\n",stat.dwMemoryLoad);
printf ("There are %*ld total %sbytes of physical memory.\n",
WIDTH, stat.dwTotalPhys/DIV, divisor);
printf ("There are %*ld free %sbytes of physical memory.\n",
WIDTH, stat.dwAvailPhys/DIV, divisor);
printf ("There are %*ld total %sbytes of paging file.\n",
WIDTH, stat.dwTotalPageFile/DIV, divisor);
printf ("There are %*ld free %sbytes of paging file.\n",
WIDTH, stat.dwAvailPageFile/DIV, divisor);
printf ("There are %*lx total %sbytes of virtual memory.\n",
WIDTH, stat.dwTotalVirtual/DIV, divisor);
printf ("There are %*lx free %sbytes of virtual memory.\n",
WIDTH, stat.dwAvailVirtual/DIV, divisor);
*/

// Sample output:
// The MemoryStatus structure is 32 bytes long.
// It should be 32.
// 78 percent of memory is in use.
// There are 65076 total Kbytes of physical memory.
// There are 13756 free Kbytes of physical memory.
// There are 150960 total Kbytes of paging file.
// There are 87816 free Kbytes of paging file.
// There are 1fff80 total Kbytes of virtual memory.
// There are 1fe770 free Kbytes of virtual memory.
//free Kbytes of virtual memory.

