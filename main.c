/*DINU Florin-Cristian-313CC*/

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

// structura pentru a reprezenta nodurile
typedef struct Node {
  char name[50];
  int depth;
  int cost;
  struct Node* next;
} Node;

// structura pentru a reprezenta graful
typedef struct {
  Node** adjacencyList;
  char** nodeNames;
  int* depth;
  int numNodes;
} Graph;

// structura pentru a reprezenta muchiile
typedef struct {
  char source[50];
  char destination[50];
  int cost;
} Edge;

// functie pentru a gasi indexul unui nod
int findNodeIndex(Graph* graph, const char* nodeName) {
  int i;
  for (i = 0; i < graph->numNodes; i++) {
    if (strcmp(graph->nodeNames[i], nodeName) == 0) {
      return i;
    }
  }
  return -1;
}

// functie pentru a crea un nou nod
Node* createNode(const char* name) {
  Node* newNode = (Node*)malloc(sizeof(Node));
  strcpy(newNode->name, name);
  newNode->cost = 0;
  newNode->next = NULL;
  return newNode;
}

// functie pentru a crea un graf
Graph* createGraph(int numNodes) {
  Graph* graph = (Graph*)malloc(sizeof(Graph));
  graph->numNodes = 0;
  graph->adjacencyList = (Node**)calloc(numNodes, sizeof(Node*));
  graph->nodeNames = (char**)malloc(numNodes * sizeof(char*));
  int i;
  for (i = 0; i < numNodes; i++) {
    graph->nodeNames[i] = (char*)malloc(50 * sizeof(char));
  }
  return graph;
}

// functie pentru a adauga o noua muchie in graf, pentru cerinta a doua  
void addEdge(Graph* graph, const char* sourceNode, const char* destinationNode,
             int cost) {
  int sourceIndex = findNodeIndex(graph, sourceNode);
  int destIndex = findNodeIndex(graph, destinationNode);

  Node* newNode = createNode(destinationNode);
  newNode->next = graph->adjacencyList[sourceIndex];
  newNode->cost = cost;
  graph->adjacencyList[sourceIndex] = newNode;
}

// functie pentru a adauga o noua muchie in graf, pentru prima cerinta
void addEdgecer1(Graph* graph, const char* sourceNode,
                 const char* destinationNode, int cost) {

  int sourceIndex = findNodeIndex(graph, sourceNode);
  int destIndex = findNodeIndex(graph, destinationNode);

  Node* newNode = createNode(destinationNode);
  newNode->next = graph->adjacencyList[sourceIndex];
  graph->adjacencyList[sourceIndex] = newNode;

  newNode = createNode(sourceNode);
  newNode->next = graph->adjacencyList[destIndex];
  graph->adjacencyList[destIndex] = newNode;
}

// functie pentru a aplica algoritmul lui Dijkstra pe graf
void dijkstra(Graph* graph,int sourceNodeIndex,int *previous, int *total_cost){
  int numNodes = graph->numNodes;
  float* distances = (float*)malloc(numNodes * sizeof(float));
  bool* visited = (bool*)malloc(numNodes * sizeof(bool));
  int *dist_int = (int*)calloc(numNodes, sizeof(int));
  int i;
  for (i = 0; i < numNodes; i++) {
    distances[i] = FLT_MAX;
    visited[i] = false;
  }

  distances[sourceNodeIndex] = 0.0;
   *total_cost = 0;
  for (i = 0; i < numNodes; i++) {
    int min = -1, j;
    for (j = 0; j < numNodes; j++) {
      if (!visited[j] && (min==-1 || distances[j] < distances[min]) ) {
        min = j;
      }
    }
    if(min==-1) break;
    visited[min] = true;
    Node* adjNode = graph->adjacencyList[min];
    while (adjNode != NULL) {
      int adjIndex = findNodeIndex(graph, adjNode->name);
      // calculam scorul conform formulei
      float scor = (float)adjNode->cost/graph->depth[adjIndex];  
      if (distances[min] + scor < distances[adjIndex]) {
        distances[adjIndex] = distances[min] + scor;
        dist_int[adjIndex] = dist_int[min] + adjNode->cost;
        previous[adjIndex] = min;
      }
      adjNode = adjNode->next;
    }
  }
  int indx = findNodeIndex(graph, "Corabie");
  // aflam costul total al drumului de la insula la corabie
  *total_cost = dist_int[indx];
  free(distances);
  free(visited);
  free(dist_int);
}

// functie pentru a aplica DFS pe graf
void DFS(int currentNode, Graph* graph, bool visited[]) {
  visited[currentNode] = true;
  Node* adjNode = graph->adjacencyList[currentNode];
  while (adjNode != NULL) {
    int adjIndex = findNodeIndex(graph, adjNode->name);
    if (!visited[adjIndex]) {
      DFS(adjIndex, graph, visited);
    }
    adjNode = adjNode->next;
  }
}

// functie pentru a verifica daca avem drum de la insula la corabie 
// si de la corabie la insula, pentru cerinta 2
int verifyPath(int source, int destination, Graph* graph) {
  bool* visited = (bool*)malloc(graph->numNodes * sizeof(bool));
  memset(visited, false, graph->numNodes * sizeof(bool));
  DFS(source, graph, visited);
  bool vrfp = visited[destination];
  free(visited);
  return vrfp ;
}

// functie pentru a calcula numarul de componente conexe ale grafului
// pentru cerinta 1
int countConnectedComponents(Graph* graph, Edge* x, int numEdges, int numNodes,
                             int sume[]) {
  int numComponents = 0;
  bool* visited = (bool*)malloc(graph->numNodes * sizeof(bool));
  memset(visited, false, graph->numNodes * sizeof(bool));
  int i, k, j;
  for (k = 0; k < graph->numNodes; k++) {
    if (!visited[k]) {
      DFS(k, graph, visited);
      
      // aplicam algoritmul lui Kruskal, pentru a determina costul total minim
      // al drumurilor ce ar trebui sa fie renovate pentru fiecare componenta
      // conexa in parte
      int t[100], v[500];
      // sortam muchiile in functie de cost
      for (i = 0; i < numEdges - 1; i++)
        for (j = i + 1; j < numEdges; ++j)
          if (x[i].cost > x[j].cost) {
            Edge aux = x[i];
            x[i] = x[j];
            x[j] = aux;
          }

      for (i = 0; i < numNodes - k; ++i) {
        t[i] = i;
      }

      int S = 0, cnt = 0;
      for (i = 0; i < numEdges && cnt < numNodes; i++) {
        int sourceIndex = findNodeIndex(graph, x[i].source);
        int destIndex = findNodeIndex(graph, x[i].destination);
        if (t[sourceIndex] != t[destIndex]) {
          cnt++;
          v[i] = 1;
          S += x[i].cost;
          int as = t[sourceIndex], ad = t[destIndex];
          for (j = 0; j < numNodes; ++j)
            if (t[j] == ad) t[j] = as;
        }
      }
      numComponents++;
      sume[numComponents] = S;
      
    }
  }
  
  free(visited);
  return numComponents;
}

// functie pentru sortare in ordine crescatoare a unui vector
void bubbleSort(int arr[], int size) {
  int i, j;
  for (i = 1; i <= size; ++i) {
    for (j = 1; j <= size - i; ++j) {
      if (arr[j] > arr[j + 1]) {
        int temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
}

// functie pentru a elibera memoria ocupata de graf
void freeGraph(Graph* graph) {
  int i;
  for (i = 0; i < graph->numNodes; i++) {
    Node* current = graph->adjacencyList[i];
    while (current != NULL) {
      Node* temp = current;
      current = current->next;
      free(temp);
    }
    free(graph->nodeNames[i]);
  }
  free(graph->adjacencyList);
  free(graph->nodeNames);
  free(graph);
}

int main(int argc, char* argv[]) {
  //deschidem fisierele de intrare si iesire
  FILE* inputFile = fopen("tema3.in", "r");
  if (inputFile == NULL) {
    printf("Failed to open the input file.\n");
    return 1;
  }

  FILE* outputFile = fopen("tema3.out", "w");
  if (outputFile == NULL) {
    printf("Failed to open the output file.\n");
    return 1;
  }
  int i,j;
  // aflam cerinta din argumentul liniei de comanda
  char* cer = argv[1];
  // pentru prima cerinta
  if (strcmp(cer, "1") == 0) {
    // citim numarul de noduri si numarul de muchii 
    int numNodes, numEdges, sume[300];
    fscanf(inputFile, "%d %d", &numNodes, &numEdges);
    
    //cream graful si vectorul de muchii
    Graph* graph = createGraph(numNodes);
    Edge* x = malloc(numEdges * sizeof(Edge));
    //citim muchiile si cream lista de adiacenta
    for (i = 0; i < numEdges; i++) {
      fscanf(inputFile, "%s %s %d", x[i].source, x[i].destination, &x[i].cost);

      int sourceIndex = findNodeIndex(graph, x[i].source);
      if (sourceIndex == -1) {
        sourceIndex = graph->numNodes++;
        strcpy(graph->nodeNames[sourceIndex], x[i].source);
        graph->adjacencyList[sourceIndex] = createNode(x[i].source);
      }

      int destIndex = findNodeIndex(graph, x[i].destination);
      if (destIndex == -1) {
        destIndex = graph->numNodes++;
        strcpy(graph->nodeNames[destIndex], x[i].destination);
        graph->adjacencyList[destIndex] = createNode(x[i].destination);
      }
      // adaugam fiecare muchie in graf
      addEdgecer1(graph, x[i].source, x[i].destination, x[i].cost);
    }
    // inchidem fisierul de intrare
    fclose(inputFile);
    // calculam si afisam numarul de componente conexe ale grafului
    int numComponents =
        countConnectedComponents(graph, x, numEdges, numNodes, sume);
    fprintf(outputFile, "%d\n", numComponents);

    // sortam vectorul de cost minim ale componentelor conexe
    // si aflam costul minim al fiecareia 
    bubbleSort(sume, numComponents);
    for (i = numComponents; i > 1; i--) {
      sume[i] = sume[i] - sume[i - 1];
    }
    bubbleSort(sume, numComponents);

    // afisam costul total minim pentru fiecare componenta conexa
    for (i = 1; i <= numComponents; i++) {
      fprintf(outputFile, "%d\n", sume[i]);
    }
    // eliberam memoria utilizata
    free(x);
    freeGraph(graph);
  } // pentru cerinta a doua 
   else if (strcmp(cer, "2") == 0) {
    int numNodes, numEdges, i;
    // citim numarul de noduri si numarul de muchii
    fscanf(inputFile, "%d %d", &numNodes, &numEdges);

    // cream graful si vectorul de muchii
    Graph* graph = createGraph(numNodes);
    Edge* x = malloc(numEdges * sizeof(Edge));
    // citim muchiile si cream lista de adiacenta
    for (i = 0; i < numEdges; i++) {
      fscanf(inputFile, "%s %s %d", x[i].source, x[i].destination, &x[i].cost);

      int sourceIndex = findNodeIndex(graph, x[i].source);
      if (sourceIndex == -1) {
        sourceIndex = graph->numNodes++;
        strcpy(graph->nodeNames[sourceIndex], x[i].source);
        graph->adjacencyList[sourceIndex] = createNode(x[i].source);
      }

      int destIndex = findNodeIndex(graph, x[i].destination);
      if (destIndex == -1) {
        destIndex = graph->numNodes++;
        strcpy(graph->nodeNames[destIndex], x[i].destination);
        graph->adjacencyList[destIndex] = createNode(x[i].destination);
      }
      // adaugam fiecare muchie in graf
      addEdge(graph, x[i].source, x[i].destination, x[i].cost);
    }
    
    // citim si adaugam adancimile fiecarui nod
    graph->depth = (int *)malloc(numNodes * sizeof(int));
    for(i = 0; i < numNodes; i++){
      char name[50]; int depth;
      fscanf(inputFile, "%s %d", name, &depth);
      int nodeindx = findNodeIndex(graph, name);
      graph->depth[nodeindx] = depth;
    }

    // citim greutatea comorii
    int kg;
    fscanf(inputFile, "%d", &kg);
    fclose(inputFile);
    
    // verificam daca echipajul poate ajunge de la corabie la insula si invers
    // folosind DFS si functia pentru verificarea drumului 
    int srcIndex = findNodeIndex(graph, "Insula");
    int dstIndex = findNodeIndex(graph, "Corabie");
    if(!verifyPath(srcIndex, dstIndex, graph)){
      fprintf(outputFile, "Echipajul nu poate transporta comoara inapoi la corabie");
      free(x);
      free(graph->depth);
      freeGraph(graph);
      return 0;
    } else if(!verifyPath(dstIndex, srcIndex, graph)){
      fprintf(outputFile, "Echipajul nu poate ajunge la insula");
      free(x);
      free(graph->depth);
      freeGraph(graph);
      return 0;
    }
    
    // cu ajutorul algoritmului lui Dijkstra vom afla nodurile care constituie
    // cel mai bun drum, costul total al acestuia si greutatea maxima a comorii
    // ce poate fi trasportata pe traseu
    int nr=0, total_cost=0;
    int greut_max= INT_MAX;
    
    // in vectorul previous am retinut nodul precedent 
    // pentru fiecare nod din drumul de cost minim
    int *previous = calloc(numNodes, sizeof(int));
    dijkstra(graph, srcIndex, previous, &total_cost);
    int currentNode = dstIndex;
    
    // am creat un vector pentru a afisa calea pe care a urmat-o echipajul
    int path[numNodes];
    int pathSize = 0;
    while (currentNode != srcIndex) {
      path[pathSize] = currentNode;
      pathSize++;
      currentNode = previous[currentNode];
    }
    path[pathSize] = srcIndex;
    pathSize++;

    // afisez drumul echipajului si calculez greutatea maxima pe care o poate
    // avea comoara
    for ( i = pathSize - 1; i >= 0; i--) {
      fprintf(outputFile, "%s ", graph->nodeNames[path[i]]);
      if (greut_max > graph->depth[path[i]] && graph->depth[path[i]] != 1) {
        greut_max = graph->depth[path[i]];
      }
    }
    
    // afisez costul total al drumului, greutatea maxim a comorii, si numarul 
    // de drumuri pe care echipajul trebuie sa le faca
    fprintf(outputFile, "\n%d\n%d\n%d\n", total_cost, greut_max, kg/greut_max);
    
    // eliberez toata memoria ocupata
    free(x);
    free(previous);
    free(graph->depth);
    freeGraph(graph);
  } 
  // inchid fisierul de iesire
  fclose(outputFile);
  return 0;
}