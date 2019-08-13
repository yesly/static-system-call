/*
##########################################################
## COP4610 – Principles of Operating Systems – Summer C 2019
## Prof. Jose F. Osorio
## Student Name: Yesly Rodriguez – 6152205
##
## Project: Customizing Linux Kernel
## Specs:   Adding Static System Call
## Due Date: 06/10/2019 by 11:55pm
## Module Name: Pinfo
##
## I Certify that this program code has been written by me
## and no part of it has been taken from any sources.
##########################################################
*/

#include <linux/syscalls.h>
#include <linux/linkage.h>
#include <asm/current.h>
#include <linux/list.h>
#include <linux/time.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/cred.h>

long get_nice(void);
int get_children(void);
int get_threads(void);
int get_youngest_child(void);
int get_younger_silbing(void);
int get_older_sibling(void);

struct task_struct *task;

asmlinkage long sys_pinfo(struct pinfo *info)
{
  task = current;

  if(info == NULL)
    return -22;

  info->pid = task->pid;
  info->state = task->state;
  info->nice = get_nice();
  info->parent_pid = task->parent->pid;
  info->nr_children = get_children();
  info->nr_threads = get_threads();
  info->youngest_child_pid = get_youngest_child();
  info->younger_sibling_pid = get_younger_silbing();
  info->older_sibling_pid = get_older_sibling();
  info->start_time = task->start_time.tv_sec;
  info->user_time = task->utime;
  info->sys_time = task->stime;
  info->cutime = task->signal->cutime;
  info->cstime = task->signal->cstime;
  info->uid = current_uid().val;
  strcpy(info->comm, task->comm);

return 0;
}
/*
  Converts the priority value to the nice value.
*/
long get_nice(void)
{
  long width = 19 - (-20) + 1; // maximum nice - minimum nice + 1
  long default_prio = 100 + (width / 2); // maximum rt prio + (nice width / 2)

  return (task->prio) - default_prio;
}

/*
  Gives total number of children.
*/
int get_children(void)
{
  struct list_head *ptr;
  int nr_children = 0;

  list_for_each(ptr, &(task->children))
  nr_children++;

  return nr_children;
}

/*
  Gives total number of threads.
*/
int get_threads(void)
{
  struct task_struct *task_ptr = task;
  int nr_threads = -1;
  do {
    nr_threads++;
  } while_each_thread(task, task_ptr);

  return nr_threads;
}

/*
  Gives pid of youngest child, which is the last element on children.
*/
int get_youngest_child(void)
{
  struct task_struct *youngest_child = list_entry(task->children.prev, struct task_struct, sibling);
  int pid = youngest_child->pid;
  if (pid == task->pid) // No children.
    return -1;

  return pid;
}

/*
  Gives pid of younger sibling, which is the first element on sibling.
*/
int get_younger_silbing(void)
{
  struct task_struct *younger_sibling = list_entry(task->sibling.next, struct task_struct, sibling);
  int pid = younger_sibling->pid;
  if (pid == 0) // No younger sibling.
    return -1;

  return pid;
}

/*
  Gives pid of older sibling, which is the last element on sibling.
*/
int get_older_sibling(void)
{
  struct task_struct *older_sibling = list_entry(task->sibling.prev, struct task_struct, sibling);
  int pid = older_sibling->pid;
  if (pid == 0) // No older sibling.
    return -1;

  return pid;
}