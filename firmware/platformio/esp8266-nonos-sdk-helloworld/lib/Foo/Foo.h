#ifndef FOO_H
#define FOO_H

  #include <stdio.h>

  /* Forward declare the primary workhorse function */
  void primary(void);

  /* Also define a helper function */
  void helper(void);

  void quick_blink(void);

  void blink(void *arg);

  int Sum(int a, int b);

#endif

