#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTENT_LEN 250
#define MAX_TODO_SIZE 100
#define MAX_LINE_LEN 275
#define PATH "todos/todos.txt"

/* error codes */
#define EMPTY_CONTENT -1
#define LIST_FULL -2
#define LIST_EMPTY -3

struct todo {
	unsigned int id;
	char *content;
	unsigned char completed;
};
typedef struct todo Todo;

struct todo_list {
	Todo t[MAX_TODO_SIZE];
	unsigned int size;
};
typedef struct todo_list TodoList;

int create_todo(char *content);
int delete_todo(int id);
int mark_todo_complete(int id);

Todo *extract_todo(char *line);
TodoList *get_todo_list();


void print_todos(void);
void print_menu(void);

int main(int, char**);
