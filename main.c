#include "includes/ft_printf.h"

int main()
{
	int		a = -4;
	int		b = 0;
	char	c = 'a';
	int		d = 2147483647;
	int		e = -2147483648;
	int		f = 42;
	int		g = 25;
	int		h = 4200;
	int		i = 8;
	int		j = -12;
	int		k = 123456789;
	int		l = 0;
	int		m = -12345678;
	char	*n = "abcdefghijklmnop";
	char	*o = "-a";
	char	*p = "-12";
	char	*q = "0";
	char	*r = "%%";
	char	*s = "-2147483648";
	char	*t = "0x12345678";
	char	*u = "-0";


	char *str ="%i, %d, %d, %d, %d, %s, %c, %d, %u, %x, %X\n";

	int ret;
	int ret2;

	ret = ft_printf(str, i, j, k, l, m, n, c, c, j, j, j);
//	ret2 = printf(str, i, j, k, l, m, n, c, c, j, j, j);

	if (ret == ret2)
	{
		printf("ret = %d\n", ret);
		printf("CORRECT\n");
	}
	else
		printf("WRONG\n");
	return (0);
}
