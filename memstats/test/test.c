/*
##########################################################
## COP4610 – Principles of Operating Systems – Summer C 2019
## Prof. Jose F. Osorio
## Student Name: Yesly Rodriguez – 6152205
##
## Project: Customizing Linux Kernel
## Specs:   Testing The Added Static System Call
## Due Date: 06/10/2019 by 11:55pm
## Module Name: Test
##
## I Certify that this program code has been written by me
## and no part of it has been taken from any sources.
##########################################################
*/
#include <unistd.h>
#include <stdio.h>
#include "/usr/src/linux/include/linux/memstats.h"
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <stdlib.h>

struct memstats *mem_stats;

/* Call new system call. */
void call_mem_stats() {
	int status;
	mem_stats = malloc(sizeof(*mem_stats));
	status = syscall(333, mem_stats);

	if (status < 0) {
		printf("ERROR Status: %d\n", status);
		exit(-1);
	}
}

/* Output to standard output the stats returned by new system call. */
void print_mem_stats(){
	printf("mem_stats:\n");
	printf(          
		"free_pages = %ld\n"
		"slab_allocator_pages = %ld\n"
		"active_pages = %ld\n"
		"inactive_pages = %ld\n"
		"active_reference_set_pages = %ld\n"
		"inactive_reference_set_pages = %ld\n"
		"active_to_inactive_pages = %ld\n"
		"evicted_from_inactive_pages = %ld\n\n",
		mem_stats->free_pages, mem_stats->slab_allocator_pages,
		mem_stats->active_pages, mem_stats->inactive_pages,
		mem_stats->active_reference_set_pages,
		mem_stats->inactive_reference_set_pages,
		mem_stats->active_to_inactive_pages,
		mem_stats->evicted_from_inactive_pages);

}

/* Allocte memory in order to see changes in statistics. */
void allocate_memory() {
	int i, *a;
	for (i = 0; i < 100000000; i++)
		a = malloc(sizeof(int));
}

int main() {
	call_mem_stats();
	print_mem_stats();

	allocate_memory();
	call_mem_stats();
	print_mem_stats();
	
	return 0;
}
