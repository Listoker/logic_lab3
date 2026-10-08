#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

struct node
{
	char inf[256];      // полезная информация
	int prior;
	struct node* next;  // ссылка на следующий элемент 
};

struct node* head = NULL, * last = NULL; // указатели на первый и последний элементы списка
int mode = 1; // 1 - Стек, 2 - Очередь, 3 - Приоритетная очередь

// Функции добавления элемента, просмотра списка
void spstore(void), review(void), del_substr(char* sub);
struct node* get_struct(void);
void insert_node(struct node* p);
void find_substr(char* sub);
void pop(void);
void change_priority(char* name, int new_pr);
void move_to_back(char* name);

struct node* get_struct(void)
{
	struct node* p = NULL;
	char s[256];
	int pr = 0;

	if ((p = (struct node*)malloc(sizeof(struct node))) == NULL)  // выделяем память под новый элемент списка
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

/* Вставка узла в структуру в зависимости от режима */
void insert_node(struct node* p)
{
	if (p == NULL) return;
	p->next = NULL;

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

		if (prev == NULL)
		{
			p->next = head;
			head = p;
			if (last == NULL)
				last = p;
		}
		else
		{
			p->next = prev->next;
			prev->next = p;
			if (p->next == NULL)
				last = p;
		}
	}
}

void spstore(void)
{
	struct node* p = get_struct();
	insert_node(p);
}

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

/* Поиск всех элементов по подстроке */
void find_substr(char* sub)
{
	struct node* struc = head;
	int count = 0;

	while (struc)
	{
		if (strstr(struc->inf, sub) != NULL)
		{
			count++;
			if (mode == 3)
				printf("Найдено: Имя - %s, приоритет - %d\n", struc->inf, struc->prior);
			else
				printf("Найдено: Имя - %s\n", struc->inf);
		}
		struc = struc->next;
	}
	if (count == 0)
		printf("Элементы не найдены\n");
}

/* Удаление всех элементов по подстроке */
void del_substr(char* sub)
{
	struct node* cur = head;
	struct node* prev = NULL;
	int count = 0;

	while (cur != NULL)
	{
		if (strstr(cur->inf, sub) != NULL)
		{
			count++;
			struct node* temp = cur;
			if (prev == NULL) // Удаление из начала
			{
				head = cur->next;
				cur = head;
				if (head == NULL)
					last = NULL;
			}
			else // Удаление из середины или конца
			{
				prev->next = cur->next;
				if (cur == last)
					last = prev;
				cur = prev->next;
			}
			free(temp);
		}
		else
		{
			prev = cur;
			cur = cur->next;
		}
	}

	if (count > 0)
		printf("Удалено элементов: %d\n", count);
	else
		printf("Совпадений для удаления не найдено\n");
}

/* Извлечение элемента (pop) для Стека и Очереди */
void pop(void)
{
	if (head == NULL)
	{
		printf("Структура пуста\n");
		return;
	}

	struct node* temp = head;
	printf("Извлечен элемент: %s\n", temp->inf);
	head = head->next;
	if (head == NULL)
		last = NULL;
	free(temp);
}

/* Изменение приоритета и пересортировка */
void change_priority(char* name, int new_pr)
{
	struct node* cur = head;
	struct node* prev = NULL;

	while (cur != NULL && strcmp(cur->inf, name) != 0)
	{
		prev = cur;
		cur = cur->next;
	}

	if (cur == NULL)
	{
		printf("Элемент с таким именем не найден\n");
		return;
	}

	if (prev == NULL)
	{
		head = cur->next;
		if (head == NULL)
			last = NULL;
	}
	else
	{
		prev->next = cur->next;
		if (cur == last)
			last = prev;
	}

	cur->prior = new_pr;
	insert_node(cur);
	printf("Приоритет обновлен\n");
}

/* Перемещение элемента по имени в конец очереди */
void move_to_back(char* name)
{
	if (head == NULL || head == last)
		return;

	struct node* cur = head;
	struct node* prev = NULL;

	while (cur != NULL && strcmp(cur->inf, name) != 0)
	{
		prev = cur;
		cur = cur->next;
	}

	if (cur == NULL)
	{
		printf("Элемент не найден\n");
		return;
	}

	if (cur == last)
	{
		printf("Элемент уже находится в конце очереди\n");
		return;
	}

	if (prev == NULL)
		head = cur->next;
	else
		prev->next = cur->next;

	last->next = cur;
	cur->next = NULL;
	last = cur;

	printf("Элемент перенесен в конец очереди\n");
}

int main()
{
	setlocale(LC_ALL, "Russian");
	int choice;
	char name[256];
	int new_pr;

	printf("Выберите структуру данных:\n");
	printf("1 - Стек\n");
	printf("2 - Очередь\n");
	printf("3 - Приоритетная очередь\n");
	printf("Ваш выбор: ");
	scanf("%d", &mode);

	while (1)
	{
		printf("\n1-Добавить 2-Просмотр 3-Поиск(подстрока) 4-Удалить(подстрока)\n");
		printf("5-Извлечь 6-Сменить приоритет 7-В конец очереди 0-Выход: ");
		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			spstore();
			break;
		case 2:
			review();
			break;
		case 3:
			printf("Введите подстроку для поиска: ");
			scanf("%s", name);
			find_substr(name);
			break;
		case 4:
			printf("Введите подстроку для удаления: ");
			scanf("%s", name);
			del_substr(name);
			break;
		case 5:
			pop();
			break;
		case 6:
			if (mode != 3)
			{
				printf("Доступно только для приоритетной очереди!\n");
				break;
			}
			printf("Введите имя элемента: ");
			scanf("%s", name);
			printf("Введите новый приоритет: ");
			scanf("%d", &new_pr);
			change_priority(name, new_pr);
			break;
		case 7:
			if (mode != 2)
			{
				printf("Доступно только для обычной очереди!\n");
				break;
			}
			printf("Введите имя элемента: ");
			scanf("%s", name);
			move_to_back(name);
			break;
		case 0:
			return 0;
		default:
			printf("Неверный пункт меню\n");
			break;
		}
	}
}