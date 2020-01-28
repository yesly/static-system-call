/*
##########################################################
## COP4610 – Principles of Operating Systems – Summer C 2019
## Prof. Jose F. Osorio
## Student Name: Yesly Rodriguez – 6152205
## Project: Virtual Memory Management
## Specs: Structure used for system call memstats to hold memory management statistics.
## Due Date: 07/20/2019 by 11:55pm
## Module Name: memstats.h
##
## I Certify that this program code has been written by me
## and no part of it has been taken from any sources.
##########################################################
*/

struct memstats {
	unsigned long free_pages;					/* the current number of free pages (over all memory zones) */
	unsigned long slab_allocator_pages;				/* the current number of pages used by slab allocator (over all memory zones) */
	unsigned long active_pages;					/* the current number of pages in the active list (over all memory zones) */
	unsigned long inactive_pages;					/* the current number of pages in the inactive list (over all memory zones) */
	unsigned long active_reference_set_pages;			/* the current number of pages in the active list whose reference bits are set (over all memory zones) */
	unsigned long inactive_reference_set_pages;			/* the current number of pages in the inactive list whose reference bits are set (over all memory zones) */
	unsigned long active_to_inactive_pages;				/* the cumulative number of pages moved from the active list to the inactive list (since the last machine boot) */
	unsigned long evicted_from_inactive_pages;			/* the cumulative number of pages evicted from the inactive list (since the last machine boot) */
};
