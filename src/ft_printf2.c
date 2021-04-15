#include "../includes/ft_printf.h"

void print_params(struct x_list *params)
{
	printf("\nPRINT PARAMS\n");
	printf("width:			%d\n", params->width);
	printf("precision:		%d\n", params->precision);
	printf("BOOL print precision:		%d\n", params->print_precision);
	printf("BOOL minus		%d\n", params->minus);
	printf("BOOL dot:		%d\n", params->dot);
	printf("BOOL zero_pad:	%d\n", params->zero_padding);
	printf("BOOL d_negative:	%d\n", params->d_negative);
	printf("params->prec_and_len: %d\n", params->prec_and_len);
	printf("params->wid_and_len: %d\n", params->wid_and_len);
	printf("format_len:			%d\n", params->format_len);
	printf("return_size:		%d\n", params->return_size);
	printf("format:			%c\n", params->format);
	printf("\n");
}

void	init_struct(struct x_list *params)
{
	params->print_precision = 0;//BOOL
	params->minus = 0;			//BOOL
	params->dot = 0;			//BOOL
	params->zero_padding = 0;	//BOOL
	params->d_negative = 0;		//BOOL
	params->prec_and_len = 0;		//BOOL
	params->wid_and_len = 0;		//BOOL
	params->width = 0;
	params->precision = 0;
	params->format_len = 0;
	params->to_print = 0;
	params->format = 0;
}

#include "../includes/ft_printf.h"

void	u_print_flag(char c, char flag, struct x_list *params)
{
	int i;
	int to_print;

	to_print = 0;
	if (flag == '1')
	{
		if (params->print_precision)
			flag = 'B';
		else
			flag = 'A';
	}
	if (flag == 'A')
		to_print = params->width - params->format_len;
	else if (flag == 'B')
		to_print = params->width - params->precision;
	else if (flag == 'C')
		to_print = params->precision - params->format_len;
	i = 0;
	while (i++ < to_print)
		ft_putchar_count(c, params);
}

int		u_specific_cases(unsigned int d, struct x_list *params)
{
	if (d == 0 && params->dot)
	{
		if (!params->precision)
		{
			if (!params->width)
				write(1, "", 0);
			else
				ft_putchar_count(' ', params);
		}
		else
		{
			ft_putchar_count('0', params);
		}
		return (1);
	}
	return (0);
}

void	u_print(unsigned int d, struct x_list *params)
{
	if (params->zero_padding && params->wid_and_len
		&& (params->precision < 0 || !params->dot))
		u_print_flag('0', 'A' , params);
	else if (!params->minus && params->wid_and_len && params->wid_and_prec)
		u_print_flag(' ', '1', params);
	if (params->print_precision && params->prec_and_len)
	{
		u_print_flag('0', 'C', params);
	}
	if (!u_specific_cases(d, params))
		ft_putnbr_count_ui(d, params);
	if (params->minus && params->wid_and_len && params->wid_and_prec)
		u_print_flag(' ', '1', params);
}

int	u_config_wp(struct x_list *params, va_list arg)
{
	unsigned int d;
	char *temp;

	d = va_arg(arg, unsigned int);
	temp = ft_itoa_ui(d);
	params->format_len = ft_strlen(temp);
	//free(temp);
	if (params->precision > params->format_len)
		params->prec_and_len = 1;
	if (params->width > params->format_len)
		params->wid_and_len = 1;
	if (params->width > params->precision)
		params->wid_and_prec = 1;
	return (d);
}

void	u_config(struct x_list *params, va_list arg)
{
	unsigned int u;

	u = u_config_wp(params, arg);
	if (params->dot && params->precision >= 0)
	{
		params->zero_padding = 0;
		if (params->prec_and_len)
			params->print_precision = 1;
	}
	if (!params->wid_and_len)
		params->zero_padding = 0;
	u_print(u, params);
}

void		find_format(struct x_list *params, va_list arg)
{
//	printf("FIND FORMAT\n");
	if (params->format == 'd' || params->format == 'i')
		d_config(params, arg);
	else if (params->format == 'c')
		c_print(params, arg);
	else if (params->format == 's')
		s_config(params, arg);
	else if (params->format == 'p')
		p_print(params, arg);
	else if (params->format == 'u')
		u_config(params, arg);
	else if (params->format == 'x' || params->format == 'X')
		xX_print(params, arg);
//	else if (params->format == '%')
//	{
//		ft_putchar_fd('%', 1);
//		params->return_size++;
//	}
	else
		printf("ERROR\n");
}

void	is_valid_format(char *str, struct x_list *params)
{
//	printf("ISVALID?:|%s|\n", str);
	if (*str == 'c'
		|| *str == 'd'
		|| *str == 'i'
		|| *str == 's'
		|| *str == 'u'
		|| *str == 'p'
		|| *str == 'x'
		|| *str == 'X')
	{
//		printf("VALID FORMAT:|%s|\n", str);
		params->format = *str;
	}
	else
		printf("NOT VALID FORMAT\n");
}

void	parse_precision(char *str, struct x_list *params)
{
//	printf("PRECISION:|%s|\n", str);
	while (ft_isdigit(*++str));
	is_valid_format(str, params);
}

void	parse_dot(char *str, struct x_list *params, va_list arg)
{
	//printf("DOT:|%s|\n", str);
	params->dot = 1;
	str++;
	if (*str == '*')
	{
		params->precision = va_arg(arg, int);
		parse_precision(str, params);
	}
	else if (ft_isdigit(*str))
	{
		params->precision = ft_atoi(str);
		while (ft_isdigit(*(++str)));
		is_valid_format(str, params);
	}
	else
		is_valid_format(str, params);
}

void	parse_width(char *str, struct x_list *params, va_list arg)
{
//	printf("WIDTH:|%s|\n", str);
	while (*str == '*')
	{
		str++;
		params->width = va_arg(arg, int);
	}
	if (ft_isdigit(*str))
	{
		params->width = ft_atoi(str);
		while (ft_isdigit(*(++str)));
	}
	if (params->width < 0)
	{
		params->zero_padding = 0;
		params->minus = 1;
		params->width *= -1;
	}
	if (*str == '.')
		parse_dot(str, params, arg);
	else 
		is_valid_format(str, params);
}

void	parse_minus(char *str, struct x_list *params, va_list arg)
{
//	printf("MINUS:|%s|\n", str);
	while (*str == '-' || *str == '0')
		str++;
	if (!params->minus)
	{
		params->minus = 1;
		params->zero_padding = 0;
	}
	if (ft_isdigit(*str) || *str == '*')
		parse_width(str, params, arg);
	else if (*str == '.')
	{
		params->dot = 1;
		parse_dot(str, params, arg);
	}
	else 
		is_valid_format(str, params);
}

void	parse_zero_padding(char *str, struct x_list *params, va_list arg)
{
//	printf("ZERO_PAD:|%s|\n", str);
	while (*str == '0')
		str++;
	if (!params->minus)
		params->zero_padding = 1;
	if (*str == '*' || ft_isdigit(*str))
		parse_width(str, params, arg);
	else if (*str == '-')
		parse_minus(str, params, arg);
	else if (*str == '.')
		parse_dot(str, params, arg);
	else 
		is_valid_format(str, params);
}

void	parsing(char *str, struct x_list *params, va_list arg)
{
	if (*str == '%')
		params->format = '%';
	else if (*str == '0')
		parse_zero_padding(str, params, arg);
	else if (*str == '-')
		parse_minus(str, params, arg);
	else if (ft_isdigit(*str))
		parse_width(str, params, arg);
	else if (*str == '*')
		parse_width(str, params, arg);
	else if (*str == '.')
		parse_dot(str, params, arg);
	else 
		is_valid_format(str, params);
}

void		print_parsing(char *parse, struct x_list *params, va_list arg)
{
	while (*parse)
	{
		while (*parse && *parse != '%')
		{
			ft_putchar_count(*parse, params);
			parse++;
		}
		if (*parse == '%')
		{
			parse++;
			parsing(parse, params, arg);
			find_format(params, arg);
			parse = ft_strchr(parse, params->format) + 1;
			init_struct(params);
		}
	}
}

int		ft_printf(char *format, ...)
{
	char *parse;
	struct x_list *params;
	va_list	arg;

	va_start(arg, format);
	params = malloc(sizeof(p_list));
	init_struct(params);
	params->return_size = 0;
	parse = format;
	print_parsing(parse, params, arg);
	va_end(arg);
	return (params->return_size);
}
