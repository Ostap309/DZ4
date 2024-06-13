#include <stdio.h>
#include <math.h>
#include <string.h>

extern double __attribute__((cdecl)) f1(double x);//Функция 1
extern double __attribute__((cdecl)) f2(double x);//Функция 2
extern double __attribute__((cdecl)) f3(double x);//Функция 3

int iter = -1;

//Функция нахождения корня
double root(double (__attribute__((cdecl)) *f)(double) , double (__attribute__((cdecl)) *g)(double) , double a, double b, double eps1)
{
	double new_x, df_a, df_b, df_new;
	if (iter != -1)//включен ли флаг -i
	{
		iter = 0;
	}
	do
	{
		df_a = f(a) - g(a);//разность в a
		df_b = f(b) - g(b);//разность в b
		new_x = (a + b) / 2;//новая точка
		df_new = f(new_x) - g(new_x);//новая разность
		if (df_new * df_a > 0)//если одного знака
		{
			a = new_x;//пишем
		}
		else//иначе
		{
			b = new_x;//пишем
		}
		if (iter != -1)//включен флаг
		{
			iter += 1;//считаем итерации
		}
	} while (fabs(df_new) >= eps1);
	return new_x;
}

//Функция нахождения интеграла
double integral(double (__attribute__((cdecl)) *f)(double), double a, double b, double eps2)
{
	int n = 10;//начальное количество
	double delta_I, h;
	double ans = (f(a) + f(b)) / 2;//Пишем сумму крайних(не зависит от n)
	do
	{
		h = (b - a) / n;//считаем шаг
		delta_I = 0;//обнуляем
		for (int i = 0; i <= n; ++i)
		{
			delta_I += f(a + i * h) * h;//прибавляем I_n
		}
		h /= 2;//делим на 2 шаг
		for (int i = 0; i <= 2*n; ++i)
		{
			delta_I -= f(a + i * h) * h;//вычитаем I_2n
		}
		delta_I = fabs(delta_I) / 3;//Умножаем на p
		n *= 2;//Умножаем на 2
	} while (delta_I >= eps2);//Проверяем
	h = (b - a) / n;
	for (int i = 1; i < n; ++i)
	{
		ans += f(a + i * h);//считаем интеграл
	}
	ans *= fabs(h);
	return ans;//возвращаем ответ
}

int main(int argc, char **argv)
{
	double x1, x2, x3, i1, i2, i3, i_ans;
	double x_left = 0.01, x_right = 10, eps1 = 0.00001, eps2 = 0.000001;
	if (argc == 1)
	{
		x1 = root(f2, f3, x_left, x_right, eps1);//считаем x1
		x2 = root(f1, f3, x_left, x_right, eps1);//считаем x2
		x3 = root(f1, f2, x_left, x_right, eps1);//считаем x3
		i1 = integral(f1, x2, x3, eps2);//считаем 1 интеграл
		i2 = integral(f2, x1, x3, eps2);//считаем 2 интеграл
		i3 = integral(f3, x1, x2, eps2);//считаем 3 интеграл
		i_ans = i1 - i2 - i3;
		printf("%lf\n", i_ans);
	}
	else
	{
		int i = 1;
		while (i < argc)//Идем по всем аргументам
		{
			int pr_xs = 0;
			if (!strcmp(argv[i],"-h")||!strcmp(argv[i],"--help"))//справка
			{
				printf("-h, --help\t\t\tPrint help\n");
				printf("-r, --root\t\t\tPrint x1, x2, x3\n");
				printf("-i, --iterations\t\tPrint number of iterations\n");
				printf("-R, --test-root F1:F2:A:B:E:R\tTest find of root of functions F1 and F2\n");
				printf("-i, --test-integral F:A:B:E:R\tTest integrate of functon F from A to B\n");
				++i;
			}
			else if (!strcmp(argv[i],"-r")||!strcmp(argv[i],"--root"))//вывод корней
			{
				pr_xs = 1;
				++i;
			}
			else if (!strcmp(argv[i],"-i")||!strcmp(argv[i],"--iterations"))//вывод итераций
			{
				iter = 0;
				++i;
			}
			else if (!strcmp(argv[i],"-R")||!strcmp(argv[i],"--test-root"))//тест функции нахождения корня
			{
				int test = 0, num = 0;
				++i;
				if (i < argc)
				{
					for (int j = 0; j < strlen(argv[i]); ++j)//проверка строки
					{
						if (argv[i][j] == ':')
						{
							++num;
						}
						else if (((argv[i][j] < '0')||(argv[i][j] > '9'))&&(argv[i][j] != ':')&&(argv[i][j] != '.')&&(argv[i][j] != '+')&&(argv[i][j] != '-'))
						{
							test = 1;
						}
					}
					if ((!test)&&(num == 5))//если проверка прошла
					{
						double (__attribute__((cdecl)) *f)(double);
						double (__attribute__((cdecl)) *g)(double);
						int num_f;
						int num_g;
						double a, b, eps, R;
						sscanf(argv[i],"%d:%d:%lf:%lf:%lf:%lf", &num_f, &num_g, &a, &b, &eps, &R);//чтение данных
						if ((num_f >= 1) && (num_f <= 3) && (num_g >= 1) && (num_g <= 3) && (num_f != num_g))
						{
							
							if (num_f == 1)//пишем указатели на функции
							{
								f = f1;
							}
							else if (num_f == 2)
							{
								f = f2;
							}
							else if (num_f == 3)
							{
								f = f3;
							}
							if (num_g == 1)
							{
								g = f1;
							}
							else if (num_g == 2)
							{
								g = f2;
							}
							else if (num_g == 3)
							{
								g = f3;
							}
							double x = root(f, g, a, b, eps);//считаем корень
							double dx = x - R;//абсолютная погрешность
							double sx = dx / R;//относительная погрешность
							printf("%lf\t%lf\t%lf\n", x, dx, sx);
							++i;
						}
						else
						{
							printf("Error: Unknown numbers of functions F1 and F2");
						}
					}
					else
					{
						printf("Error: Unknown format of parameters for -R, --test-root\n");
					}
				}
				else
				{
					printf("Error: you should add parameters for -R, --test-root\n");
				}
			}
			else if (!strcmp(argv[i],"-I")||!strcmp(argv[i],"--test-integral"))//тест функции нахождения интеграла
			{
				int test = 0, num = 0;
				++i;
				if (i < argc)
				{
					for (int j = 0; j < strlen(argv[i]); ++j)// проверка символов
					{
						if (argv[i][j] == ':')
						{
							++num;
						}
						else if (((argv[i][j] < '0')||(argv[i][j] > '9'))&&(argv[i][j] != ':')&&(argv[i][j] != '.')&&(argv[i][j] != '+')&&(argv[i][j] != '-'))
						{
							test = 1;
						}
					}
					if ((!test)&&(num == 4))//проверка условий
					{
						double (__attribute__((cdecl)) *f)(double);
						int num_f;
						double a,b,eps,R;
						sscanf(argv[i],"%d:%lf:%lf:%lf:%lf", &num_f, &a, &b, &eps, &R);//чтение данных
						if ((num_f >= 1) && (num_f <= 3))
						{
							
							if (num_f == 1)//записываем нужный указатель
							{
								f = f1;
							}
							else if (num_f == 2)
							{
								f = f2;
							}
							else if (num_f == 3)
							{
								f = f3;
							}
							double x = integral(f, a, b, eps);//считаем интеграл
							double dx = x - R;//абсолютная погрешность
							double sx = dx / R;//относительная погрешность
							printf("%lf\t%lf\t%lf\n", x, dx, sx);//печать
							++i;
						}
						else
						{
							printf("Error: Unknown number of function F1");
						}
					}
					else
					{
						printf("Error: Unknown format of parameters for -I, --test-integral\n");
					}
				}
				else
				{
					printf("Error: you should add parameters for -I, --test-integral\n");
				}
			}
			else
			{
				printf("Error: %s not working.\nType prog -h for help (also --help).\n", argv[i]);//Ошибка
				++i;
			}
			if ((iter != -1)||(pr_xs))//Если надо считать x1, x2, x3
			{
				x1 = root(f2, f3, x_left, x_right, eps1);
				if (iter != -1)
				{
					printf("Number of iterations for x1: %d\n", iter);//печатаем количество итераций
				}
				x2 = root(f1, f3, x_left, x_right, eps1);
				if (iter != -1)
				{
					printf("Number of iterations for x2: %d\n", iter);//печатаем количество итераций
				}
				x3 = root(f1, f2, x_left, x_right, eps1);
				if (iter != -1)
				{
					printf("Number of iterations for x3: %d\n", iter);//печатаем количество итераций
				}
				if (pr_xs)
				{
					printf("%lf\t%lf\t%lf\n", x1, x2, x3);//печатаем значения x1, x2, x3
				}
			}
		}
	}
	return 0;
}