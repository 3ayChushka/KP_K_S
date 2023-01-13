#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <math.h>
#include <locale.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#define N 1000


// Структура, содержащая данные о потребителе
struct China_partiya {
	char name[N]; // Название
	char X[N]; // Фамилия председателя
	char I[N]; // Имя председателя
	char P[N]; // Отчество председателя
	float numbers; // Численность
	float perc; // Процент голосов
	int year; // Год выборов

	float q;  //Коэфф. для сортировки
};
typedef struct China_partiya data;

void menu();  /*Меню*/
void add_data(data* ptr_struct, int size);  /*Добавление записи*/
void out(data p);  /*Вывод в консоль*/
void add_to_file(data* p, int size); /* Запись в файл*/
int out_file(data* p);   /*Чтение из файла*/
int searchPecr(data* p, int size, float perc, int ind);  /*Поиск по проценту*/
int searchName(data* p, int size, char* name_search_X, char* name_search_I, char* name_search_P, int ind);  /*Поиск по ФИО*/
void sort(data* p, int size); /*Сортировка*/
int compare_q(const void* av, const void* bv); /*Вспомогательная функция для сортировки*/

void main() {
	setlocale(LC_ALL, "RUS");
	int size=0, end = -1;
	data* p=NULL;
	//p = (data*)malloc(size * sizeof(data));

	while (end != 0) {
		menu(); // Вызываем функцию табуляции меню в консоль;
		scanf("%d", &end);

		switch (end) {
		case 1: // Ввести данные о потребителе
		{
			system("cls");
			FILE* KP = fopen("KP.txt", "r");
			int q=0;
			while (!feof(KP))
				if (fgetc(KP) == '\n')q++;
			q = q / 6;
			p = (data*)malloc((q+1) * sizeof(data));

			size=out_file(p);
			add_data(p, size);
			add_to_file(p, size);
			size++;
		}
			break;
		case 2:
		{
			size = out_file(p);
			int z, ind=-1;

			printf("Выберите поиск\n1-процент голосов выше заданного\n2-по председателю\n");
			scanf("%d", &z);
			getchar();
			
			if (z == 1) { 
				float perc_search;
				
				printf("Введите процент голосов\n");
				scanf("%f", &perc_search);

				searchPecr(p, size, perc_search, ind);
				
			}
			else if (z == 2) {
				char name_search_X[N];
				char name_search_I[N];
				char name_search_P[N];

				printf("Введите ФИО председателя\n");
				scanf("%s %s %s", name_search_X, name_search_I, name_search_P);

				searchName(p, size, name_search_X, name_search_I, name_search_P, ind);
			}
			else printf("Ошибка\n");
			break; 
		}
		case 3:
		{
			sort(p, size);
			break;
		}
		case 4:
		{
			system("cls");

			FILE* KP = fopen("KP.txt", "r");
			int q = 0;
			while (!feof(KP))
				if (fgetc(KP) == '\n')q++;
			q = q / 6;
			p = (data*)malloc(q/sizeof(data));

			size = out_file(p);
			for(int i=0; i<size; i++)
				out(p[i]);
			break;
		}
		case 5:
		{
			int n;
			size = out_file(p);
			printf("Сколько председателей вы хотите добавить?\n");
			scanf("%d", &n);

			p = realloc(p, (size + n) * sizeof(data));
			for (int i = size; i < size + n; i++)
			{
				add_data(p, i);
				add_to_file(p, i);
			}
			size += n;
			break;
		}
		case 0: // Выход из программы
			break;
		}
	}
}

void menu() {
	puts("Выберите команду:");
	puts("1. Ввести данные о партиях");
	puts("2. Найти партию");
	puts("3. Отсортировать");
	puts("4. Вывести данные о представителях");
	puts("5. Добавить произвольное число представителей");
	puts("0. Выход из программы");
}
// Меню

void add_data(data* p, int i) {
	printf("Введите название партии: ");
	scanf("%s", p[i].name);
	
	printf("Введите ФИО председателя: ");
	scanf("%s %s %s", p[i].X, p[i].I, p[i].P);

	printf("Введите численность(в тыс.чел.): ");
	scanf("%f", &p[i].numbers);

	printf("Введите процент голосов на последних выборах: ");
	scanf("%f", &p[i].perc);

	printf("Введите год выборов: ");
	scanf("%d", &p[i].year);

	p[i].q = 0.7f * p[i].perc + 0.3f * p[i].numbers;
}
//Добавление записи

void out(data p) {
	printf("Название партии:     %s\n", p.name);
	printf("ФИО председателя:    %s %s %s\n", p.X, &p.I, &p.P);
	printf("Численность(тыс):    %.2f\n", p.numbers);
	printf("Процент голосов:     %.2f\n", p.perc);
	printf("Год выборов:         %d\n", p.year);
	printf("\n");
}
//Вывод одной записи

void add_to_file(data* p, int size) {
	FILE* KP;

	if (size == 0 && out_file(p)==0) KP = fopen("KP.txt", "w+");
	else KP = fopen("KP.txt", "a");

	for (int i = size; i < size+1; i++)
	{
		fprintf(KP, "\n%s\n", p[i].name);
		fprintf(KP, "%s %s %s\n", p[i].X, p[i].I, p[i].P);
		fprintf(KP, "%f\n", p[i].numbers);
		fprintf(KP, "%f\n", p[i].perc);
		fprintf(KP, "%d\n", p[i].year);
		fprintf(KP, "%f", p[i].q);
	}

	fclose(KP);
}
//Запись в файл

int out_file(data* p) {
	FILE* KP;
	int i = 0;

	KP = fopen("KP.txt", "r");

	while (!feof(KP))
	{
		fscanf(KP, "%s\n", p[i].name);
		fscanf(KP, "%s %s %s\n", p[i].X, p[i].I, p[i].P);
		fscanf(KP, "%f\n", &p[i].numbers);
		fscanf(KP, "%f\n", &p[i].perc);
		fscanf(KP, "%d\n", &p[i].year);
		fscanf(KP, "%f", &p[i].q);
		i++;
	}
	return i;
}
//Чтение из файла

int searchPecr(data* p, int size, float perc, int ind) {
	for (int i = ind; i < size; i++)
	{
		if (p[i].perc > perc) out(p[i]);
	}
	return 0;
}
//Поиск по процентам

int searchName(data* p, int size, char* name_search_X, char* name_search_I, char* name_search_P, int ind) {
	for (int i = ind; i < size; i++)
	{
		if (!(strcmp(p[i].X, name_search_X)&& strcmp(p[i].I, name_search_I)&& strcmp(p[i].P, name_search_P))) out(p[i]);
	}
	return 0;
}
//Поиск по имени

void sort(data* p, int size) {
	data *q;
	q = (data*)malloc(size * sizeof(data));

	for (int i = 0; i < size; i++)
		q[i] = *(p + i);

	qsort(q, size, sizeof(data), compare_q);

	system("cls");
	for (int i = 0; i < size; i++)
	{
		out(q[i]);
		printf("q=%.2f\n\n\n", q[i].q);
	}
}
//Сортировка

int compare_q(const void* av, const void* bv) {
	const data* a = av, * b = bv;
	if (a->q > b->q)return -1;
	if (a->q < b->q)return 1;
	return 0;
}
//Для сортировки
