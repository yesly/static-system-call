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
#include "/usr/src/linux/include/linux/pinfo.h"
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <pthread.h>
void *threadFunction(void *thread_id);

int main()
{
  struct pinfo *p_info;
  int status, pid;
  long child, thread;
  
  /* Create child processes and keep 'em active. */
  for (child = 0; child < 3; child++) {
    pid = fork();

    if (pid < 0) {
      exit(1);
    } else if (pid == 0) {  // Child Process.
      printf("Child %d with pid = %d.\n", child, getpid());
      sleep(child);
      exit(0);
    } else {  // Parent Process.
      /* Create child threads and keep 'em active. */
      pthread_t threads[4];

      for (thread = 0; thread < 4; thread++) {
        pthread_create(&threads[thread], NULL, threadFunction, (void *)thread);
      }

      /* Call new system call */
      p_info = malloc(sizeof(*p_info));
      status = syscall(332, p_info);

      // Join Threads
      for (thread = 0; thread < 4; thread++) {
        pthread_join(threads[thread], NULL);
      }
    }
  }
  /* Wait for children. */
    for (child = 0; child < 3; child++)
      wait(NULL);
  /* Check Status. */
    if (status < 0) {
      printf("ERROR Status: %d\n", status);
      return 0;
    }

  /* Output to standard output the info returned by new system call. */
  printf("p_info:\n");
  printf(          
        "pid = %d\n"
        "state = %ld\n"
        "nice = %ld\n"
        "parent_pid = %d\n"
        "nr_children = %d\n"
        "nr_threads = %d\n"
        "youngest_child_pid = %d\n"
        "younger_sibling_pid = %d\n"
        "older_sibling_pid = %d\n"
        "start_time = %lu\n"
        "user_time = %ld\n"
        "sys_time = %ld\n"
        "cutime = %ld\n"
        "cstime = %ld\n"
        "uid = %ld\n"
        "comm = %s\n",
        p_info->pid, p_info->state, p_info->nice, p_info->parent_pid, p_info->nr_children,
        p_info->nr_threads, p_info->youngest_child_pid, p_info->younger_sibling_pid,
        p_info->older_sibling_pid, p_info->start_time, p_info->user_time,
        p_info->sys_time, p_info->cutime, p_info->cstime, p_info->uid, p_info->comm);

  return 0;
}

void *threadFunction(void *thread_id) {
  sleep(1);
}
