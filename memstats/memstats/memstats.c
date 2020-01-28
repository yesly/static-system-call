/*
##########################################################
## COP4610 – Principles of Operating Systems – Summer C 2019
## Prof. Jose F. Osorio
## Student Name: Yesly Rodriguez – 6152205
## Project: Virtual Memory Management
## Specs: Static system call to report on memory management statistics.
## Due Date: 07/20/2019 by 11:55pm
## Module Name: memstats.c
##
## I Certify that this program code has been written by me
## and no part of it has been taken from any sources.
##########################################################
*/

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/mmzone.h>
#include <linux/vmstat.h>
#include <linux/list.h>
#include <linux/memstats.h>
#include <linux/linkage.h>
#include <linux/uaccess.h>

asmlinkage long sys_memstats(struct memstats *stats) {

	struct zone *zone;
	struct page *page;
	struct list_head *active, *inactive;

	unsigned long active_ref = 0,
	inactive_ref = 0,
   	active_to_inactive = 0,
	evicted_from_inactive = 0;

	if(stats == NULL) // Check argument.
		return -22;

  	for_each_zone(zone) { // Get stats over all memory zones.
		list_for_each(active, &(zone->lruvec.lists[LRU_ACTIVE_FILE])) { // Traverse list.
			page = list_entry(active, struct page, lru); // Get each page.
			active_ref += test_bit(PG_referenced, &page->flags); // Check the referenced bit.	
    		}

		list_for_each(inactive, &(zone->lruvec.lists[LRU_INACTIVE_FILE])) {
			page = list_entry(inactive, struct page, lru);
			inactive_ref += test_bit(PG_referenced, &page->flags);		
    		}
  	}

	stats->free_pages = global_page_state(NR_FREE_PAGES);
  	stats->slab_allocator_pages = global_page_state(NR_SLAB_RECLAIMABLE) + global_page_state(NR_SLAB_UNRECLAIMABLE);
	stats->active_pages = global_page_state(NR_ACTIVE_ANON);
	stats->inactive_pages = global_page_state(NR_INACTIVE_ANON);
	stats->active_reference_set_pages = active_ref;
	stats->inactive_reference_set_pages = inactive_ref;
	stats->active_to_inactive_pages = active_to_inactive;
	stats->evicted_from_inactive_pages = evicted_from_inactive;

	return 0;
}
