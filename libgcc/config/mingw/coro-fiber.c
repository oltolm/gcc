/* Stackful asymmetric coroutines on Windows fibers.
   Copyright (C) 2026 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

#include "tconfig.h"
#include "tsystem.h"
#include "coretypes.h"
#include "tm.h"
#include "unwind.h"

/* For IsThreadAFiber.  */
#if _WIN32_WINNT < 0x0600
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

enum coro_state
{
  CORO_CREATED,
  CORO_RUNNING,
  CORO_SUSPENDED,
  CORO_DESTROYING,
  CORO_DONE
};

/* A record in the per-thread handler chain.  An effect is identified by
   the address of its descriptor.  */

struct __gcc_effect_handler
{
  struct __gcc_effect_handler *next;
  const void *effect;
  void *ops;
};

static __thread struct __gcc_effect_handler *chain;

struct __gcc_coro
{
  void *fiber;
  /* The fiber that last resumed or destroyed this coroutine.  */
  void *resumer;
  void (*fn) (void *);
  void *arg;
  enum coro_state state;
  /* The innermost coroutine of the suspended computation: this one, or a
     coroutine it runs that suspended it through __gcc_effect_suspend.  */
  struct __gcc_coro *top;
  void *message;
  /* The computation's chain while it is not running.  Its records end in
     BASE, the record of the handler the coroutine runs under, which is
     relinked on every switch in to the chain of the fiber switching in;
     a resume from another handler only changes BASE.  */
  struct __gcc_effect_handler *chain;
  struct __gcc_effect_handler base;
  struct _Unwind_Exception exc;
};

struct __gcc_coro *__gcc_coro_create (void (*) (void *), void *, size_t);
int __gcc_coro_resume (struct __gcc_coro *);
void __gcc_coro_yield (void);
void __gcc_coro_destroy (struct __gcc_coro *);
void *__gcc_coro_message (struct __gcc_coro *);
struct __gcc_effect_handler *__gcc_coro_handler (struct __gcc_coro *);
void __gcc_effect_suspend (struct __gcc_effect_handler *, void *);
struct __gcc_effect_handler *__gcc_effect_top (void);
void __gcc_effect_set_top (struct __gcc_effect_handler *);
struct __gcc_effect_handler *__gcc_effect_find (const void *);

static void WINAPI
fiber_main (void *data)
{
  struct __gcc_coro *c = data;

  c->fn (c->arg);
  c->state = CORO_DONE;
  /* Returning from a fiber routine would exit the thread.  */
  SwitchToFiber (c->resumer);
  abort ();
}

static struct __gcc_coro *
current_coro (void)
{
  struct __gcc_coro *c;

  if (!IsThreadAFiber ())
    abort ();
  c = GetFiberData ();
  if (!c || c->fiber != GetCurrentFiber ())
    abort ();
  return c;
}

static void
switch_into (struct __gcc_coro *c)
{
  struct __gcc_effect_handler *outer = chain;

  if (!IsThreadAFiber ()
      && !ConvertThreadToFiberEx (NULL, FIBER_FLAG_FLOAT_SWITCH))
    abort ();
  c->resumer = GetCurrentFiber ();
  c->base.next = outer;
  chain = c->chain;
  SwitchToFiber (c->top->fiber);
  c->chain = chain;
  chain = outer;
}

/* The forced unwind ends at the fiber's outermost frame, once every frame
   of the coroutine has run its cleanups.  The fiber is never resumed;
   __gcc_coro_destroy deletes it.  */

static _Unwind_Reason_Code
destroy_stop (int version __attribute__((unused)), _Unwind_Action actions,
	      _Unwind_Exception_Class exc_class __attribute__((unused)),
	      struct _Unwind_Exception *exc __attribute__((unused)),
	      struct _Unwind_Context *context __attribute__((unused)),
	      void *stop_argument)
{
  struct __gcc_coro *c = stop_argument;

  if (actions & _UA_END_OF_STACK)
    SwitchToFiber (c->resumer);
  return _URC_NO_REASON;
}

static void __attribute__((noreturn))
unwind (struct __gcc_coro *c)
{
  c->state = CORO_DESTROYING;
  _Unwind_ForcedUnwind (&c->exc, destroy_stop, c);
  abort ();
}

struct __gcc_coro *
__gcc_coro_create (void (*fn) (void *), void *arg, size_t stack_size)
{
  struct __gcc_coro *c = malloc (sizeof (*c));

  if (!c)
    return NULL;
  memset (c, 0, sizeof (*c));
  c->fn = fn;
  c->arg = arg;
  c->state = CORO_CREATED;
  c->top = c;
  c->chain = &c->base;
  c->fiber = CreateFiberEx (0, stack_size, FIBER_FLAG_FLOAT_SWITCH,
			    fiber_main, c);
  if (!c->fiber)
    {
      free (c);
      return NULL;
    }
  return c;
}

int
__gcc_coro_resume (struct __gcc_coro *c)
{
  if (c->state != CORO_CREATED && c->state != CORO_SUSPENDED)
    abort ();
  c->state = CORO_RUNNING;
  switch_into (c);
  if (c->state == CORO_DESTROYING)
    {
      /* C was suspended inside a computation that is being destroyed and
	 has been unwound; the computation continues on this fiber.  */
      c->state = CORO_DONE;
      unwind (current_coro ());
    }
  return c->state == CORO_SUSPENDED;
}

void
__gcc_coro_yield (void)
{
  struct __gcc_coro *c = current_coro ();

  c->top = c;
  c->state = CORO_SUSPENDED;
  SwitchToFiber (c->resumer);
  if (c->state == CORO_DESTROYING)
    unwind (c);
}

void
__gcc_coro_destroy (struct __gcc_coro *c)
{
  if (c->state == CORO_SUSPENDED)
    {
      c->state = CORO_DESTROYING;
      switch_into (c);
    }
  else if (c->state != CORO_CREATED && c->state != CORO_DONE)
    abort ();
  DeleteFiber (c->fiber);
  free (c);
}

void *
__gcc_coro_message (struct __gcc_coro *c)
{
  return c->message;
}

struct __gcc_effect_handler *
__gcc_coro_handler (struct __gcc_coro *c)
{
  return &c->base;
}

/* Suspend the coroutine whose handler record is H, together with the
   coroutines it runs, up to the current one.  Control goes directly to
   whoever resumed it; the coroutines in between do not run.  */

void
__gcc_effect_suspend (struct __gcc_effect_handler *h, void *message)
{
  struct __gcc_coro *c
    = (struct __gcc_coro *) ((char *) h - offsetof (struct __gcc_coro, base));
  struct __gcc_coro *self = current_coro ();

  c->top = self;
  c->message = message;
  c->state = CORO_SUSPENDED;
  SwitchToFiber (c->resumer);
  if (c->state == CORO_DESTROYING)
    unwind (self);
}

struct __gcc_effect_handler *
__gcc_effect_top (void)
{
  return chain;
}

void
__gcc_effect_set_top (struct __gcc_effect_handler *h)
{
  chain = h;
}

struct __gcc_effect_handler *
__gcc_effect_find (const void *effect)
{
  struct __gcc_effect_handler *h;

  for (h = chain; h; h = h->next)
    if (h->effect == effect)
      return h;
  abort ();
}
