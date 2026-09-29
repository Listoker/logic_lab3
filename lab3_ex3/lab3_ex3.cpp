#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

struct node
{
	char inf[256];      // полезная информация
	int prior;     
	struct node* next;  // ссылка на следующий элемент 
};

struct node* head = NULL, * last = NULL, * f = NULL; // указатели на первый и последний элементы списка
int dlinna = 0;
int mode = 1; // 1 - Стек, 2 - Очередь, 3 - Приоритетная очередь

// Функции добавления элемента, просмотра списка
void spstore(void), review(void), del(char* name);

char find_el[256];
struct node* find(char* name); // функция нахождения элемента
struct node* get_struct(void); // функция создания элемента

struct node* get_struct(void)
{
	struct node* p = NULL;
	char s[256];
	int pr = 0;

	if ((p = (node*)malloc(sizeof(struct node))) == NULL)  // выделяем память под новый элемент списка
	{
		printf("Ошибка при распределении памяти\n");
		exit(1);
	}

	printf("Введите название объекта: \n");   // вводим данные
	scanf("%s", s);
	if (*s == 0)
	{
		printf("Запись не была произведена\n");
		free(p);
		return NULL;
	}
	strcpy(p->inf, s);

	
	if (mode == 3)
	{
		printf("Введите приоритет объекта: \n");
		scanf("%d", &pr);
	}
	p->prior = pr;
	p->next = NULL;

	return p; // возвращаем указатель на созданный элемент
}

/* Добавление элемента в зависимости от режима */
void spstore(void)
{
	struct node* p = get_struct();
	if (p == NULL)
		return;

	// 1. стек
	if (mode == 1)
	{
		p->next = head;
		head = p;
		if (last == NULL)
			last = p;
	}
	// 2. очередь
	else if (mode == 2)
	{
		if (head == NULL)
		{
			head = p;
			last = p;
		}
		else
		{
			last->next = p;
			last = p;
		}
	}
	// 3. приоритетная очередь
	else if (mode == 3)
	{
		struct node* cur = head;
		struct node* prev = NULL;

		while (cur != NULL && cur->prior >= p->prior)
		{
			prev = cur;
			cur = cur->next;
		}

		if (prev == NULL) // вставка в начало
		{
			p->next = head;
			head = p;
			if (last == NULL)
				last = p;
		}
		else // вставка в середину или конец
		{
			p->next = prev->next;
			prev->next = p;
			if (p->next == NULL)
				last = p;
		}
	}
}

/* Просмотр содержимого списка */
void review(void)
{
	struct node* struc = head;
	if (head == NULL)
	{
		printf("Список пуст\n");
		return;
	}
	while (struc)
	{
		if (mode == 3)
			printf("Имя - %s, приоритет - %d\n", struc->inf, struc->prior);
		else
			printf("Имя - %s\n", struc->inf);
		struc = struc->next;
	}
}

/* Поиск элемента по содержимому */
struct node* find(char* name)
{
	struct node* struc = head;
	if (head == NULL)
	{
		printf("Список пуст\n");
		return NULL;
	}
	while (struc)
	{
		if (strcmp(name, struc->inf) == 0)
		{
			if (mode == 3)
				printf("Имя - %s, приоритет - %d, ", struc->inf, struc->prior);
			else
				printf("Имя - %s, ", struc->inf);
			return struc;
		}
		struc = struc->next;
	}
	printf("Элемент не найден\n");
	return NULL;
}

/* Удаление элемента по содержимому */
void del(char* name)
{
	struct node* struc = head;
	struct node* prev = NULL;
	int flag = 0;

	if (head == NULL)
	{
		printf("Список пуст\n");
		return;
	}

	if (strcmp(name, struc->inf) == 0) // если удаляемый элемент - первый
	{
		flag = 1;
		head = struc->next;
		if (head == NULL)
			last = NULL;
		free(struc);
		struc = head;
	}
	else
	{
		prev = struc;
		struc = struc->next;
	}

	while (struc)
	{
		if (strcmp(name, struc->inf) == 0)
		{
			flag = 1;
			if (struc->next)
			{
				prev->next = struc->next;
				free(struc);
				struc = prev->next;
			}
			else
			{
				prev->next = NULL;
				last = prev;
				free(struc);
				return;
			}
		}
		else
		{
			prev = struc;
			struc = struc->next;
		}
	}

	if (flag == 0)
	{
		printf("Элемент не найден\n");
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");
	int choice;
	char name[256];

	printf("Выберите структуру данных:\n");
	printf("1 - Стек\n");
	printf("2 - Очередь\n");
	printf("3 - Приоритетная очередь\n");
	printf("Ваш выбор: ");
	scanf("%d", &mode);

	while (1)
	{
		printf("\n1-Добавить 2-Просмотр 3-Найти 4-Удалить 0-Выход: ");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1: spstore(); break;
		case 2: review(); break;
		case 3:
			printf("Введите имя: "); scanf("%s", name);
			if (find(name)) printf("найден\n");
			break;
		case 4:
			printf("Введите имя: "); scanf("%s", name);
			del(name);
			break;
		case 0: return 0;
		}
	}
}