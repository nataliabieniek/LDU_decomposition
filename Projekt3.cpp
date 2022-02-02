#include <stdio.h>
#include <stdlib.h>

static char* komunikat[] = { " ", "Blad odczytu z pliku", "Blad alokacji","Blad otwarcia pliku", };

void error(int i)
{
	printf_s("%s", komunikat[i]);
}
void odczyt(FILE* a, double** macierz_a, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			fscanf_s(a, "%lf", &macierz_a[i][j]);
		}
	}
}
void zwalnianie_pamieci_wektor(double* w)
{
	free(w);
}
void zwalnianie_pamieci_macierz(double** w, int n)
{
	for (int i = 0; i < n; i++)
	{
		free(w[i]);
	}
}
double** dynamiczna_alokacja(int n)
{
	double** macierz_a;
	macierz_a = (double**)malloc(n * sizeof(double));

	for (int i = 0; i < n; i++)
	{
		macierz_a[i] = (double*)malloc(n * sizeof(double));
	}
	if (macierz_a == NULL) error(2);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			macierz_a[i][j] = 0;
	}
	return macierz_a;
}
double* alokacja_wektora(int n)
{
	double* wektor;
	wektor = (double*)malloc(n * sizeof(double));
	for (int i = 0; i < n; i++)
		wektor[i] = 0;
	return wektor;
}
double* LDU(double** a, double* b, int n)
{
	printf_s("\nMacierz a:\n");
	for (int i = 0; i < n; i++) //drukuje macierz a
	{
		for (int j = 0; j < n; j++)
		{
			printf_s("%lf\t", a[i][j]);
		}
		printf_s("\n");
	}
	double** d, ** l, ** u;
	d = dynamiczna_alokacja(n);
	l = dynamiczna_alokacja(n);
	u = dynamiczna_alokacja(n);
	d[0][0] = a[0][0];
	for (int i = 0; i < n; i++) 
	{
		l[i][0] = a[i][0] / d[0][0];
		u[0][i] = a[0][i] / d[0][0];
	}
	double suma = 0;
	for (int i = 1; i < n; i++) 
	{
		for (int j = 1; j < n; j++)
		{
			 if (i > j && j > 0) //uzupelnianie L
			{
				suma = 0;
				int k = 0;
				do
				{
					suma += l[i][k] * u[k][j] * d[k][k];
					k++;
				} while (k < j );
				l[i][j] = (a[i][j] - suma) / d[j][j];
			}
			else if (0 < i && i < j) // uzupelnianie U
			{
				suma = 0;
				int k = 0;
				do
				{
					suma += l[i][k] * u[k][j] * d[k][k];
					k++;
				} while (k < j );
				u[i][j] = (a[i][j] - suma) / d[i][i];
			}
			  else if (j == i)
			 {
				 suma = 0;
				 int k = 0;
				 do
				 {
					 suma += l[i][k] * u[k][i] * d[k][k];
					 k++;
				 } while (k < i );

				 d[i][i] = a[i][i] - suma;
				 u[i][i] = 1;
				 l[i][i] = 1;
			 }
		}
	}
	printf_s("\nMacierz u:\n");
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf_s("%lf\t", u[i][j]);
		}
		printf_s("\n");
	}
	printf_s("\nMacierz l:\n");
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf_s("%lf\t", l[i][j]);
		}
		printf_s("\n");
	}
	printf_s("\nMacierz d:\n");
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf_s("%lf\t", d[i][j]);
		}
		printf_s("\n");
	}
	double* y, * z, * x;
	y = alokacja_wektora(n);
	z = alokacja_wektora(n);
	x = alokacja_wektora(n);

	y[0] = b[0];
	for (int i = 1; i < n; i++)//wektor y
	{
		suma = 0;
		for (int k = 0; k < i; k++)
		{
			suma += l[i][k] * y[k];
		}
		y[i] = b[i] - suma;
	}
	printf_s("\nWekor y:\n");
	for (int i = 0; i<n; i++)
	{
		printf_s("%lf\t", y[i]);
	}
	for (int i = 0; i < n; i++)
	{
		z[i] = y[i] / d[i][i];
	}
	printf_s("\nWekor z:\n");
	for (int i = 0; i < n; i++)
	{
		printf_s("%lf\t", z[i]);
	}
	x[n - 1] = z[n - 1];
	for (int i = n - 2; i >= 0; i--)
	{
			double suma = 0;
			if (i < n)
			{
				int k = n-1 ;
				do {
					suma += u[i][k] * x[k];
					k--;
				} while (k >= 0);

				x[i] = (z[i] - suma);
			}
	}
	zwalnianie_pamieci_wektor(y);
	zwalnianie_pamieci_wektor(z);
	zwalnianie_pamieci_macierz(l, n);
	zwalnianie_pamieci_macierz(u, n);
	zwalnianie_pamieci_macierz(d, n);
	return x;
}
int main()
{
    FILE* od;
    if (!(od= fopen("dane.txt", "r"))) error(3);
    int n;
    double** a, *x;
    fscanf_s(od, "%d", &n);
    a = dynamiczna_alokacja(n);
    x = alokacja_wektora(n);
    odczyt(od,a, n);
    double* b;
    b = alokacja_wektora(n);
	printf_s("Wektor b:\n");
	for (int j=0; j < n; j++)
	{
		fscanf_s(od, "%lf", &b[j]);
		printf_s("%lf\t", b[j]);
	}
    x = LDU(a, b, n);
    zwalnianie_pamieci_macierz(a,n);
    zwalnianie_pamieci_wektor(b);
	printf_s("\n\nWektor x:");
    for (int i = 0; i < n; i++)
    {
		printf_s("\n%lf", x[ i]);
    }
	zwalnianie_pamieci_wektor(x);
	fclose(od);
}
