/*DINU Florin-Cristian - 313CC*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structura pentru a citi imaginea initiala
typedef struct {
  unsigned char red, green, blue;
} Pixel;

// structura pentru arborele cuaternar
typedef struct q_nod {
  // coordonatele si dimensiunea patratului asociat nodului
  int x, y, size;
  // indicator pentru a verifica daca nodul este frunza sau nu
  unsigned char este_frunza;
  // culoarea medie a patratului (folosita doar pentru nodurile frunza)
  unsigned long long red, green, blue;
  // pointeri la cei 4 copii
  struct q_nod *ss, *sd, *jd, *js;
} q_nod;

// functie pentru maximul a 2 numere
int max(int num1, int num2) { return (num1 > num2) ? num1 : num2; }

// functie pentru crearea arborelui cuaternar
q_nod* build_quadtree(Pixel** grid, unsigned int x, unsigned int y,
                      unsigned int size, int factor, int* nrc1_2, int* lat_max,
                      int* numar_noduri) {
  q_nod* root = malloc(sizeof(q_nod));
  (*numar_noduri)++;
  root->x = x;
  root->y = y;
  root->size = size;
  root->este_frunza = 0;
  root->red = 0;
  root->green = 0;
  root->blue = 0;
  root->ss = NULL;
  root->sd = NULL;
  root->jd = NULL;
  root->js = NULL;
  int i, j;
  // calculez mediile culorilor
  int pixel_count = size * size;
  for (i = x; i < x + size; i++) {
    for (j = y; j < y + size; j++) {
      root->red += grid[i][j].red;
      root->green += grid[i][j].green;
      root->blue += grid[i][j].blue;
    }
  }
  root->red /= pixel_count;
  root->green /= pixel_count;
  root->blue /= pixel_count;

  // calculez scorul similaritatii
  unsigned long long mean = 0;
  for (i = x; i < x + size; i++) {
    for (j = y; j < y + size; j++) {
      int red_diff = grid[i][j].red - root->red;
      int green_diff = grid[i][j].green - root->green;
      int blue_diff = grid[i][j].blue - root->blue;
      mean +=
        red_diff * red_diff + green_diff * green_diff + blue_diff * blue_diff;
    }
  }
  mean /= 3 * pixel_count;
  // verificare pentru a vedea daca imaginea mai trebuie impartita
  if (mean <= factor) {
    root->este_frunza = 1;
    if (size > *lat_max) *lat_max = size;
    (*nrc1_2)++;
  } else {
    int half_size = size / 2;
    root->ss = build_quadtree(grid,x, y, half_size, factor, nrc1_2, lat_max,
                              numar_noduri);
    root->sd = build_quadtree(grid,x, y + half_size, half_size, factor, nrc1_2,
                              lat_max, numar_noduri);
    root->js = build_quadtree(grid,x + half_size, y, half_size, factor, nrc1_2,
                              lat_max, numar_noduri);
    root->jd = build_quadtree(grid,x + half_size, y + half_size, half_size,
                              factor, nrc1_2, lat_max, numar_noduri);
  }
  return root;
}

// functie pentru a calcula numarul de niveluri din arbore
int max_depth(q_nod* node) {
  if (node == NULL) {
    return 0;
  } else {
    int sus_stg = max_depth(node->ss);
    int sus_dr = max_depth(node->sd);
    int jos_dr = max_depth(node->jd);
    int jos_stg = max_depth(node->js);
    int nr_niv = max(sus_stg, max(sus_dr, max(jos_dr, jos_stg)));
    return 1 + nr_niv;
  }
}

// functie pentru crearea cozii
q_nod** creare_coada(q_nod* root, int* numar_noduri) {
  q_nod** queue = (q_nod**)malloc((*numar_noduri) * sizeof(q_nod*));
  queue[0] = root;
  int i, j = 0;
  for (i = 0; i < *numar_noduri; i++) {
    // daca nodul nu este frunza
    if (queue[i]->este_frunza == 0) {
      queue[++j] = queue[i]->ss;
      queue[++j] = queue[i]->sd;
      queue[++j] = queue[i]->jd;
      queue[++j] = queue[i]->js;
    }
  }
  return queue;
}

// functie pentru a rezolva cerinta2
void cerinta2(FILE* output,q_nod* root, unsigned int image_size, q_nod** queue,
              int* numar_noduri) {
  fwrite(&image_size, sizeof(unsigned int), 1, output);
  int i;
  for (i = 0; i < *numar_noduri; i++) {
    q_nod* current_node = queue[i];
    if (current_node->este_frunza == 0) {
      unsigned char node_type = 0;
      fwrite(&node_type, sizeof(unsigned char), 1, output);
    } else {
      unsigned char node_type = 1;
      fwrite(&node_type, sizeof(unsigned char), 1, output);
      fwrite(&current_node->red, sizeof(unsigned char), 1, output);
      fwrite(&current_node->green, sizeof(unsigned char), 1, output);
      fwrite(&current_node->blue, sizeof(unsigned char), 1, output);
    }
  }
}

// functie pentru elibarea memoriei ocupate de arbore
void free_quadtree(q_nod* node) {
  if (node == NULL) {
    return;
  } else {
    free_quadtree(node->ss);
    free_quadtree(node->sd);
    free_quadtree(node->jd);
    free_quadtree(node->js);
  }
  free(node);
}

int main(int argc, char* argv[]) {
  // aflu factorul
  int factor = atoi(argv[2]);
  char tip[3];
  unsigned int latime, inaltime, valoare_max;
  FILE* input;
  input = fopen(argv[3], "rb");
  // citesc tipul fisierului, dimensiunile imaginii, alaturi de
  // valoarea maxima pe care o pot lua culorile
  fscanf(input, "%s\n%d %d\n%d", tip, &latime, &inaltime, &valoare_max);
  fseek(input, 1, SEEK_CUR);
  int i, j;
  // citesc valorile pentru imaginea initiala
  Pixel** grid = (Pixel**)malloc(inaltime * sizeof(Pixel*));
  for (i = 0; i < inaltime; i++) {
    grid[i] = malloc(latime * sizeof(Pixel));
  }
  for (i = 0; i < inaltime; i++) {
    for (j = 0; j < latime; j++) {
      fread(&grid[i][j].red, sizeof(unsigned char), 1, input);
      fread(&grid[i][j].green, sizeof(unsigned char), 1, input);
      fread(&grid[i][j].blue, sizeof(unsigned char), 1, input);
    }
  }
  int nrc1_2 = 0, lat_max = 0, numar_noduri = 0;
  // creez vectorul pe post de coada
  q_nod* root = build_quadtree(grid, 0, 0, inaltime, factor, &nrc1_2, &lat_max,
                               &numar_noduri);

  FILE* output;
  // in functie de cerinta afisez ce trebuie
  if (strcmp(argv[1], "-c1") == 0) {
    output = fopen(argv[4], "wt");
    fprintf(output, "%d\n%d\n%d\n", max_depth(root), nrc1_2, lat_max);
  } else if (strcmp(argv[1], "-c2") == 0) {
    output = fopen(argv[4], "wb");
    int nr = 0;
    q_nod** queue = creare_coada(root, &numar_noduri);
    cerinta2(output, root, latime, queue, &numar_noduri);
    free(queue);
  }
  // eliberez memoria
  for (i = 0; i < inaltime; i++) {
    free(grid[i]);
  }
  free(grid);
  free_quadtree(root);
  fclose(input);
  fclose(output);
}