/* PR tree-optimization/71588 */

/* strcpy must not be pure, but make sure we don't ICE even when
   it is declared incorrectly.  */
__extension__ typedef __SIZE_TYPE__ size_t;
char *strcpy (char *, const char *) __attribute__ ((__pure__));
size_t strlen (const char *);
void *malloc (size_t);

char a[20];

char *
foo (void)
{
  size_t b = strlen (a);
  char *c = malloc (b);
  return strcpy (c, a);
}
