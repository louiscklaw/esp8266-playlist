#include "osapi.h"
#include "user_interface.h"

#include "Foo.h"
#include <stdio.h>

void primary()
{
    os_printf("I'm the primary function, I'm doin' work.\n");
    helper();
}

void helper()
{
    os_printf("I'm a helper function and I helped!\n");
}

int Sum(int a, int b)
{
    return a+b;
}

void blink(void *arg)
{
  static uint8_t state = 0;

  if (state) {
    GPIO_OUTPUT_SET(2, 1);
  } else {
    GPIO_OUTPUT_SET(2, 0);
  }
  state ^= 1;
}
