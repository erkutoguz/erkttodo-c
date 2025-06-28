#include "erkttodo.h"

FILE *fp;
TodoList *tl;

int main(int argc, char *argv[]) {
  int cmd;

  fp = fopen(PATH, "r+");

  if (!fp) {
    perror("file can not be open");
    return -1;
  }

  tl = get_todo_list();
  if (!tl) {
    perror("todo list could not initialized.");
    return -1;
  }

  do {
    print_menu();
    scanf("%d", &cmd);

    switch (cmd) {
    case 1:
      printf("List Todos\n");
      print_todos();
      break;
    case 2:
      printf("Create Todo\n");
      printf("Enter todo:\n");
      int c;
      while ((c = getchar()) != '\n' && c != EOF)
        ;
      char *line;
      size_t size = 0;
      ssize_t len;
      len = getline(&line, &size, stdin);

      if (len != -1 && line[len - 1] == '\n')
        line[len - 1] = '\0';

      create_todo(line);
      free(line);
      break;
    case 3:
      printf("Mark Completed\n");
      printf("Enter ID:\n");
      int id, st;
      scanf("%d", &id);
      printf("\n");
      st = mark_todo_complete(id);
      if (st == OUT_OF_BOUNDS) {
        printf("Invalid Id. Try again\n");
      }
      break;
    case 5:
      printf("Exit\n");
      break;
    default:
      printf("Invalid command. Try again...\n");
      break;
    }
  } while (cmd != 4);

  int p;
  for (p = 0; p < tl->size; ++p) {
    free(tl->t[p].content);
  }
  free(tl);
  fclose(fp);
  fp = NULL;

  return 0;
}

void print_menu(void) {
  printf("Welcome to the erkttodo\n");
  printf("Select option:\n");
  printf("1-List Todos\n");
  printf("2-Create Todo\n");
  printf("3-Mark Completed\n");
  printf("4-Exit\n\n");
}

void print_todos() {
  int i;
  i = 0;
  while (i < tl->size) {
    printf("\nId: %d\nContent: %s\nCompleted: %c\n\n", tl->t[i].id,
           tl->t[i].content, tl->t[i].completed != '0' ? 'Y' : 'X');
    ++i;
  }
}

TodoList *get_todo_list() {
  int size;
  TodoList *tl;
  Todo *t;
  char line[MAX_LINE_LEN];

  tl = (TodoList *)malloc(sizeof(TodoList));
  size = 0;

  while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
    t = extract_todo(line);
    if (!t)
      break;
    memset(line, 0, MAX_LINE_LEN);
    tl->t[size++] = *t;
  }

  tl->size = size;

  return tl;
}

int create_todo(char *content) {
  char line[MAX_LINE_LEN];
  unsigned int id;
  unsigned char completed;
  Todo *t;

  if (strlen(content) < 4) {
    return EMPTY_CONTENT;
  }

  if (tl->size == MAX_TODO_SIZE) {
    return LIST_FULL;
  }

  id = tl->size;

  char buff[10];
  snprintf(buff, sizeof(buff), "%d", id);

  completed = '0';

  memset(line, 0, MAX_LINE_LEN);
  strcat(line, buff);
  strcat(line, ";");
  strcat(line, content);
  strcat(line, ";");
  int len = strlen(line);
  line[len] = completed;
  line[len + 1] = '\n';
  line[len + 2] = '\0';

  fputs(line, fp);

  t = (Todo *)malloc(sizeof(Todo));

  t->content = (char *)malloc(MAX_CONTENT_LEN);
  strcpy(t->content, content);

  t->id = id;
  t->completed = completed;

  tl->t[tl->size++] = *t;

  return 0;
}

int mark_todo_complete(int id) {
  if (id >= tl->size || id < 0) {
    perror("invalid id");
    return OUT_OF_BOUNDS;
  }

  char *buff, *id_buff;
  buff = NULL;
  id_buff = (char *)malloc(10);

  size_t size = 0;
  int i, p, last_pos, pos;

  last_pos = ftell(fp); /* after mark op go back to last pos*/
  /* go back to beginning of file */
  rewind(fp);
  while (getline(&buff, &size, fp)) {
    for (i = 0, p = 0; buff[i] != ';'; ++i) {
      id_buff[p] = buff[i];
    }
    if (atoi(id_buff) == id) {
      pos = ftell(fp);
      break;
    }
  }

  rewind(fp);
  fseek(fp, pos - 2, SEEK_SET); // 2 means move cursor to mark character
  fputc('1', fp);

  p = 0;
  for (; p < tl->size; ++p) {
    if (tl->t[p].id == id) {
      tl->t[p].completed = 'Y';
      break;
    }
  }

  rewind(fp);
  fseek(fp, last_pos, SEEK_SET);

  free(id_buff);
  return 0;
}

Todo *extract_todo(char *line) {
  if (strlen(line) < 3)
    return NULL;

  Todo *t;
  char buff[MAX_CONTENT_LEN];
  memset(buff, 0, sizeof(buff));

  t = (Todo *)malloc(sizeof(Todo));
  t->content = (char *)malloc(MAX_CONTENT_LEN);

  int ch;

  ch = 0;
  while (*line != ';') {
    buff[ch++] = *line;
    line++;
  }
  t->id = atoi(buff);
  line++;
  memset(buff, 0, sizeof(buff));

  ch = 0;
  while (*line != ';') {
    buff[ch++] = *line;
    line++;
  }
  strncpy(t->content, buff, strlen(buff));
  line++;
  memset(buff, 0, sizeof(buff));

  ch = 0;
  t->completed = *line;
  memset(buff, 0, sizeof(buff));

  return t;
}
