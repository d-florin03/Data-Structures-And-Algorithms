/*DINU Florin-Cristian -313CC*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//structura pentru noduri
typedef struct nod{
    char *data;
    struct nod *prec;
    struct nod *urm;
}Nod;

//structura pentru lista 
typedef struct lista{
    Nod *santinela;
    Nod *curent;
}Lista;

//structura pentru coada
typedef struct queue{
    Nod *front;
    Nod *rear;
}Queue;

//structura pentru nodurile unei stive
typedef struct snod{
  void* adr;
  struct snod* next;
}Snod;

//structura pentru stiva
typedef struct stack{
    Snod *top;
}Stack;

//functie pentru crearea unei stive
Stack *creare_stiva() {
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->top = NULL;
    return stack;
}

//functie pentru adaugarea unui element in stiva
void push(Stack* stack, void* adr){
  Snod* nou = (Snod*)malloc(sizeof(Snod));
  //retin in campul adr al noului nod adresa dorita
  nou->adr = adr;
  //adaug nodul in stiva legandu-l de primul element
  nou->next = NULL;
  nou->next = stack->top;
  stack->top = nou;
}

//functie pentru extragerea unui element din stiva
void* pop(Stack* stack){
  Snod* aux = stack->top;
  void* adr = aux->adr;
  stack->top = aux->next;
  free(aux);
  return adr;
}

//functie pentru eliberarea memoriei ocupate de o stiva
void deleteStack(Stack *stack) {
  Snod *curent = stack->top;
  while (curent != NULL) {
    Snod *next = curent->next;
    free(curent);
    curent = next;
  }
  free(stack);
}

//functie pentru crearea cozii
Queue* creare_coada(){
    Queue *q=(Queue*)malloc(sizeof(Queue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}

//functie pentru adaugarea unui element in coada
void enqueue(Queue* q, char *c){
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->data = (char*)malloc((strlen(c) + 1) * sizeof(char));
    strcpy(nou->data, c);
    nou->urm = NULL;
    nou->prec = NULL;
    //daca coada este goala noul element devine si primul si ultimul
    if (q->rear == NULL) {
      q->front = nou;
      q->rear = nou;
      return;
    }
    //leg noul nod in coada
    nou->prec = q->rear;
    q->rear->urm = nou;
    q->rear = nou;
}

//functie pentru extragerea primului element din coada
void dequeue(Queue *q, char rez[20]){
  if(q->front == NULL){
    printf("ERROR\n");
    return;
  }
  Nod *temp = q->front;
  strcpy(rez, q->front->data);
  q->front = q->front->urm;
  if (q->front == NULL) {
    q->rear = NULL;
  }
  free(temp->data);
  free(temp);
}

//functie pentru eliberarea memoriei ocupate de coada
void deleteQueue(Queue *q) {
  Nod *aux;
  while (q->front != NULL) {
    aux = q->front;
    q->front = q->front->urm;
    free(aux->data);
    free(aux);
  }
  free(q);
}

//functie pentru crearea listei
Lista* creare_lista(){
  //aloc memorie pentru lista
  //leg santinela si ii dau valoarea # primului element din lista
  //adica lst->santinela->urm
   Lista* lst = (Lista*)malloc(sizeof(Lista));
    lst->santinela = (Nod*)malloc(sizeof(Nod));
    lst->santinela->data = (char*)malloc(2 * sizeof(char));
    lst->santinela->prec = NULL;
    lst->santinela->urm = (Nod *)malloc(sizeof(Nod));
    char s[]="#";
    lst->santinela->urm->data = malloc(2 * sizeof(char));
    lst->santinela->urm->prec = lst->santinela;
    lst->santinela->urm->urm = NULL;
    strcpy(lst->santinela->urm->data, s);
    strcpy(lst->santinela->data, s);
    lst->curent = lst->santinela->urm;
    return lst;
}

//functie pentru eliberarea memoriei ocupate de lista
void freeList(Lista *lst) {
    Nod *curent = lst->santinela;
    while (curent != NULL) {
      Nod *nextNode = curent->urm;
      curent->urm = NULL;
      curent->prec = NULL;
      free(curent->data);
      free(curent);
      curent = nextNode;
    }
    free(lst);
}

//functe pentru operatia de MOVE_LEFT
void MOVE_LEFT(Lista *lst, Stack *UNDO) {
    //daca elementul curent nu este primul din lista 
    //putem executa operatia
    if(lst->curent != lst->santinela->urm){
      Nod *temp = lst->curent;
      push(UNDO, temp);
      lst->curent = lst->curent->prec;
    }
}

//functie pentru operatia de MOVE_RIGHT
void MOVE_RIGHT(Lista *lst, Stack *UNDO){
    //daca elementul din dreapta exista mutam elementul curent 
    //cu o pozitie la dreapta
    if(lst->curent->urm != NULL){
        Nod *temp = lst->curent;
        lst->curent = lst->curent->urm;
        push(UNDO, temp);
    } else{
      //daca nu exista element in dreapta celui curent
      //cream un nod nou, caruia ii atribuim valoarea #
      //si il legam de ultimul din lista
      //elementul nou devine astfel elementul curent
        Nod *temp = lst->curent;
        Nod *nou = (Nod *)malloc(sizeof(Nod));
        nou->data = malloc(2 * sizeof(char));
        strcpy(nou->data, "#");
        nou->prec = lst->curent;
        nou->urm = NULL;
        lst->curent->urm = nou;
        lst->curent = nou;
        push(UNDO, temp);
    }
}

//functie pentru atribuirea unei valori a campului data
//al elementului curent
void WRITE(Lista *lst, char *c){
    strncpy(lst->curent->data, c , 1);    
}

//functie pentru mutarea in dreapta pana la valoarea dorita
void MOVE_RIGHT_CHAR(Lista *lst, char *c, Stack* UNDO){
   int ok = 0;
   Nod *aux;
   //parcurgem lista de la elementul curent pana la final
    while (lst->curent != NULL) {
        //daca am gasit valoarea cautata ne putem opri
        if (strcmp(lst->curent->data, c) == 0) {
          ok = 1;
        }
        if (ok) break;
        aux = lst->curent;
        lst->curent = lst->curent->urm;
   }
   //in cazul in care se ajunge pana la sfarsitul listei
   //am retinut in aux adresa ultimului element din lista
   //si executam operatia de MOVE_RIGHT pentru a adauga un nou nod in lista
    if(ok == 0){
        lst->curent = aux;
        MOVE_RIGHT(lst, UNDO);
    }
    
}

//functie pentru mutarea in stanga pana la valoarea dorita
void MOVE_LEFT_CHAR(Lista *lst, char *c, FILE *output) {
    int ok = 0;
    //parcurgem lista
    while (lst->curent != NULL) {
        //daca am gasit valoarea dorita ne putem opri
        if (strcmp(lst->curent->data, c) == 0) {
          ok = 1;
        }
        if (ok) break;
        lst->curent = lst->curent->prec;
    }
    //daca nu am gasit valoarea dorita afisam mesajul ERROR
    if (ok == 0) {
        fprintf(output, "ERROR\n"); 
    }
}

//functie pentru inserarea in dreapta nodului curent
void INSERT_RIGHT(Lista *lst, char *c, Stack* UNDO){
      //daca nu exista elementul urmator
      //ne mutam la dreapta, adica il cream si ii dam valoarea data
      if(lst->curent->urm == NULL){
        MOVE_RIGHT(lst, UNDO);
        WRITE(lst, c);
      } else {
        //daca exista elementul din dreapta
        //legam noul nod de cel curent si cel urmator
          Nod *nou = (Nod *)malloc(sizeof(Nod));
          nou->data = malloc(2 * sizeof(char));
          strcpy(nou->data, c);
          nou->prec = lst->curent;
          nou->urm = lst->curent->urm;
          lst->curent->urm = nou;
          if (nou->urm != NULL) {
            nou->urm->prec = nou;
          }
          lst->curent = nou;
      }
}

//functie pentru inserarea in stanga nodului curent
void INSERT_LEFT(Lista *lst, char *c, FILE *output){
     //daca nodul curent este primul din lista, nu putem adauga in stanga sa
    //si afisam mesajul ERROR     
     if(lst->curent == lst->santinela->urm){
        fprintf(output, "ERROR\n");
     } else {
      //daca nodul curent nu este primul din lista
      //legam noul nod de precedentul curentului si de curent
        Nod *nou = (Nod *)malloc(sizeof(Nod));
        nou->data = malloc(2 * sizeof(char));
        strcpy(nou->data, c);
        nou->prec = lst->curent->prec;
        lst->curent->prec->urm = nou;
        lst->curent->prec = nou;
        nou->urm = lst->curent;
        lst->curent = nou;
     }
}

//functie pentru afisarea elementului curent
void SHOW_CURRENT(Lista *lst, FILE *output){
    fprintf(output, "%s\n", lst->curent->data);
}

//functie pentru afisarea listei
void SHOW(Lista *lst, FILE *output) {
    Nod *curent  = lst->santinela->urm;
    while (curent != NULL) {
        if (lst->curent == curent) {
          fprintf(output, "|%s|", curent->data);
        } else {
           fprintf(output, "%s", curent->data);
       }
       curent = curent->urm;
   }
   fprintf(output, "\n");
}

int main(){
    //creez lista, coada, si cele doua stive
    Lista *lst = creare_lista();
    Queue *q = creare_coada();
    Stack *UNDO = creare_stiva();
    Stack *REDO = creare_stiva();
    Nod *curent = NULL;
    int n, i;
    char cerinta[20];
    FILE *input;
    input = fopen("tema1.in", "rt");
    if (input == NULL) {
      printf("Nu s-a putut deschide fisierul");
    }
    FILE *output;
    output = fopen("tema1.out", "wt");
    //citesc din fisierul de intrare numarul de operatii
    fscanf(input, "%d", &n);
    for(i = 0; i < n; i++){
      //citesc operatiile si in functie de fiecare, le adaug in coada 
      //sau le execut direct
        fscanf(input, " %[^\n]s", cerinta);
        //adaug in coada cerintele ce nu trebuie executate direct
        //si fara cele de tip EXECUTE
        if (strcmp(cerinta, "EXECUTE") != 0 && strcmp(cerinta, "SHOW") != 0 && 
           strcmp(cerinta, "SHOW_CURRENT") != 0 && strcmp(cerinta, "UNDO") != 0
           && strcmp(cerinta, "REDO") != 0) {
           enqueue(q, &cerinta[0]);
        } else {
           char rez[20];
           //pentru operatiile de tip EXECUTE extrag prima operatie din coada 
           //si o execut
           if (strcmp(cerinta, "EXECUTE") == 0){
                  dequeue(q, rez);
              if (strncmp(rez, "WRITE", 5) == 0) {
                WRITE(lst, &rez[6]);
              } else if (strcmp(rez, "MOVE_RIGHT") == 0) {
                MOVE_RIGHT(lst, UNDO);
              } else if (strcmp(rez, "MOVE_LEFT") == 0 ||
                          strcmp(rez, "MOVE_LEFT ") == 0) {
                MOVE_LEFT(lst, UNDO);
              } else if (strncmp(rez, "MOVE_LEFT_CHAR", 14) == 0) {
                curent = lst->curent;
                MOVE_LEFT_CHAR(lst, &rez[15], output);
                if (lst->curent == NULL) {
                  lst->curent = curent;
                }
              } else if (strncmp(rez, "MOVE_RIGHT_CHAR", 15) == 0) {
                MOVE_RIGHT_CHAR(lst, &rez[16], UNDO);
              } else if (strncmp(rez, "INSERT_LEFT", 11) == 0) {
                INSERT_LEFT(lst, &rez[12], output);
              } else if (strncmp(rez, "INSERT_RIGHT", 12) == 0) {
                INSERT_RIGHT(lst, &rez[13], UNDO);
              }
           }
           //operatiile de mai jos se executa direct
            if (strcmp(cerinta, "SHOW_CURRENT") == 0) {
                SHOW_CURRENT(lst, output);
           } else if(strcmp(cerinta, "SHOW") == 0){
                SHOW(lst, output);
          } else if(strcmp(cerinta, "UNDO") == 0){
              push(REDO, lst->curent);
              lst->curent = pop(UNDO);
          } else if (strcmp(cerinta, "REDO") == 0) {
              push(UNDO, lst->curent);
              lst->curent = pop(REDO);
          }
        }
    } 
    //inchid fisierele si eliberez memoria pentru toate structurile folosite   
    fclose(input);
    fclose(output);   
    freeList(lst);
    deleteQueue(q);
    deleteStack(UNDO);
    deleteStack(REDO);
}