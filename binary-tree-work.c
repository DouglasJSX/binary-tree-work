#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABETLENGTH 26
#define WORDLENGTH 100

struct letterTree
{
  char word[WORDLENGTH];
  int occurrences;
  int height;
  struct letterTree *left;
  struct letterTree *right;
};
typedef struct letterTree LetterTree;

struct alphabet
{
  char letter;
  struct letterTree *initialTree;
};
typedef struct alphabet Alphabet;

Alphabet alpha[ALPHABETLENGTH];

void initializeAlphabet()
{
  int i, letter = 'a';
  for (i = 0; i < ALPHABETLENGTH; i++)
  {
    alpha[i].letter = letter++;
    alpha[i].initialTree = NULL;
  }
}

void toLowerCase(char *str)
{
  int i;
  for (i = 0; str[i]; i++)
  {
    str[i] = tolower(str[i]);
  }
}

int getLetterIndex(char letter)
{
  return letter - 'a';
}

int getHeight(LetterTree *node)
{
  if (node == NULL)
    return 0;
  return node->height;
}

int maxValue(int a, int b)
{
  return (a > b) ? a : b;
}

int getBalance(LetterTree *node)
{
  if (node == NULL)
    return 0;
  return getHeight(node->left) - getHeight(node->right);
}

LetterTree *rightRotate(LetterTree *y)
{
  LetterTree *x = y->left;
  LetterTree *T2 = x->right;

  x->right = y;
  y->left = T2;

  y->height = maxValue(getHeight(y->left), getHeight(y->right)) + 1;
  x->height = maxValue(getHeight(x->left), getHeight(x->right)) + 1;

  return x;
}

LetterTree *leftRotate(LetterTree *x)
{
  LetterTree *y = x->right;
  LetterTree *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->height = maxValue(getHeight(x->left), getHeight(x->right)) + 1;
  y->height = maxValue(getHeight(y->left), getHeight(y->right)) + 1;

  return y;
}

LetterTree *insert(LetterTree *node, char *word)
{
  if (node == NULL)
  {
    node = (LetterTree *)malloc(sizeof(LetterTree));
    strcpy(node->word, word);
    node->occurrences = 1;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
  }

  int cmp = strcmp(word, node->word);
  if (cmp < 0)
  {
    node->left = insert(node->left, word);
  }
  else if (cmp > 0)
  {
    node->right = insert(node->right, word);
  }
  else
  {
    node->occurrences++;
    return node;
  }

  node->height = 1 + maxValue(getHeight(node->left), getHeight(node->right));

  int balance = getBalance(node);

  if (balance > 1 && strcmp(word, node->left->word) < 0)
    return rightRotate(node);

  if (balance < -1 && strcmp(word, node->right->word) > 0)
    return leftRotate(node);

  if (balance > 1 && strcmp(word, node->left->word) > 0)
  {
    node->left = leftRotate(node->left);
    return rightRotate(node);
  }

  if (balance < -1 && strcmp(word, node->right->word) < 0)
  {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  return node;
}

LetterTree *search(LetterTree *node, char *word)
{
  if (node == NULL)
  {
    return NULL;
  }

  int cmp = strcmp(word, node->word);
  if (cmp == 0)
  {
    return node;
  }
  else if (cmp < 0)
  {
    return search(node->left, word);
  }
  else
  {
    return search(node->right, word);
  }
}

LetterTree *findMin(LetterTree *node)
{
  while (node && node->left != NULL)
  {
    node = node->left;
  }
  return node;
}

LetterTree *removeNode(LetterTree *node, char *word)
{
  if (node == NULL)
  {
    return node;
  }

  int cmp = strcmp(word, node->word);
  if (cmp < 0)
  {
    node->left = removeNode(node->left, word);
  }
  else if (cmp > 0)
  {
    node->right = removeNode(node->right, word);
  }
  else
  {
    if (node->left == NULL)
    {
      LetterTree *temp = node->right;
      free(node);
      return temp;
    }
    else if (node->right == NULL)
    {
      LetterTree *temp = node->left;
      free(node);
      return temp;
    }

    LetterTree *temp = findMin(node->right);
    strcpy(node->word, temp->word);
    node->occurrences = temp->occurrences;
    node->right = removeNode(node->right, temp->word);
  }

  if (node == NULL)
    return node;

  node->height = 1 + maxValue(getHeight(node->left), getHeight(node->right));

  int balance = getBalance(node);

  if (balance > 1 && getBalance(node->left) >= 0)
    return rightRotate(node);

  if (balance > 1 && getBalance(node->left) < 0)
  {
    node->left = leftRotate(node->left);
    return rightRotate(node);
  }

  if (balance < -1 && getBalance(node->right) <= 0)
    return leftRotate(node);

  if (balance < -1 && getBalance(node->right) > 0)
  {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  return node;
}

int countWords(LetterTree *node)
{
  if (node == NULL)
  {
    return 0;
  }
  return 1 + countWords(node->left) + countWords(node->right);
}

int countOccurrences(LetterTree *node)
{
  if (node == NULL)
  {
    return 0;
  }
  return node->occurrences + countOccurrences(node->left) + countOccurrences(node->right);
}

void displayInOrder(LetterTree *node, int showCount)
{
  if (node != NULL)
  {
    displayInOrder(node->left, showCount);
    if (showCount)
    {
      printf("%s (%d)\n", node->word, node->occurrences);
    }
    else
    {
      printf("%s\n", node->word);
    }
    displayInOrder(node->right, showCount);
  }
}

void displayReverseOrder(LetterTree *node, int showCount)
{
  if (node != NULL)
  {
    displayReverseOrder(node->right, showCount);
    if (showCount)
    {
      printf("%s (%d)\n", node->word, node->occurrences);
    }
    else
    {
      printf("%s\n", node->word);
    }
    displayReverseOrder(node->left, showCount);
  }
}

void findMaxOccurrences(LetterTree *node, int *maxOcc, char results[][WORDLENGTH], int *count)
{
  if (node == NULL)
  {
    return;
  }

  if (node->occurrences > *maxOcc)
  {
    *maxOcc = node->occurrences;
    *count = 0;
    strcpy(results[(*count)++], node->word);
  }
  else if (node->occurrences == *maxOcc)
  {
    strcpy(results[(*count)++], node->word);
  }

  findMaxOccurrences(node->left, maxOcc, results, count);
  findMaxOccurrences(node->right, maxOcc, results, count);
}

void displaySingleOccurrence(LetterTree *node)
{
  if (node != NULL)
  {
    displaySingleOccurrence(node->left);
    if (node->occurrences == 1)
    {
      printf("%s\n", node->word);
    }
    displaySingleOccurrence(node->right);
  }
}

void insertWord(char *word)
{
  char cleanWord[WORDLENGTH];
  strcpy(cleanWord, word);
  toLowerCase(cleanWord);

  if (strlen(cleanWord) == 0)
    return;

  int index = getLetterIndex(cleanWord[0]);
  if (index >= 0 && index < ALPHABETLENGTH)
  {
    alpha[index].initialTree = insert(alpha[index].initialTree, cleanWord);
    printf("Palavra '%s' inserida com sucesso!\n", cleanWord);
  }
}

void consultWord(char *word)
{
  char cleanWord[WORDLENGTH];
  strcpy(cleanWord, word);
  toLowerCase(cleanWord);

  int index = getLetterIndex(cleanWord[0]);
  if (index >= 0 && index < ALPHABETLENGTH)
  {
    LetterTree *result = search(alpha[index].initialTree, cleanWord);
    if (result != NULL)
    {
      printf("Palavra '%s' encontrada com %d ocorrencia(s).\n", cleanWord, result->occurrences);
    }
    else
    {
      printf("Palavra '%s' nao encontrada.\n", cleanWord);
    }
  }
  else
  {
    printf("Palavra '%s' nao encontrada.\n", cleanWord);
  }
}

void removeWord(char *word)
{
  char cleanWord[WORDLENGTH];
  strcpy(cleanWord, word);
  toLowerCase(cleanWord);

  int index = getLetterIndex(cleanWord[0]);
  if (index >= 0 && index < ALPHABETLENGTH)
  {
    LetterTree *result = search(alpha[index].initialTree, cleanWord);
    if (result != NULL)
    {
      alpha[index].initialTree = removeNode(alpha[index].initialTree, cleanWord);
      printf("Palavra '%s' removida com sucesso!\n", cleanWord);
    }
    else
    {
      printf("Palavra '%s' nao encontrada para remocao.\n", cleanWord);
    }
  }
  else
  {
    printf("Palavra '%s' nao encontrada para remocao.\n", cleanWord);
  }
}

void countTotalWords()
{
  int total = 0;
  int i;
  for (i = 0; i < ALPHABETLENGTH; i++)
  {
    total += countWords(alpha[i].initialTree);
  }
  printf("Numero total de palavras: %d\n", total);
}

void countTotalOccurrences()
{
  int total = 0;
  int i;
  for (i = 0; i < ALPHABETLENGTH; i++)
  {
    total += countOccurrences(alpha[i].initialTree);
  }
  printf("Numero total de ocorrencias: %d\n", total);
}

void displayAllWords(int reverse, int showCount)
{
  int i;
  printf("\n=== TODAS AS PALAVRAS ===\n");
  if (reverse)
  {
    for (i = ALPHABETLENGTH - 1; i >= 0; i--)
    {
      displayReverseOrder(alpha[i].initialTree, showCount);
    }
  }
  else
  {
    for (i = 0; i < ALPHABETLENGTH; i++)
    {
      displayInOrder(alpha[i].initialTree, showCount);
    }
  }
  printf("========================\n");
}

void displayWordsByLetter(char letter, int reverse, int showCount)
{
  letter = tolower(letter);
  int index = getLetterIndex(letter);

  if (index >= 0 && index < ALPHABETLENGTH)
  {
    printf("\n=== PALAVRAS INICIADAS POR '%c' ===\n", letter);
    if (reverse)
    {
      displayReverseOrder(alpha[index].initialTree, showCount);
    }
    else
    {
      displayInOrder(alpha[index].initialTree, showCount);
    }
    printf("================================\n");
  }
  else
  {
    printf("Letra invalida.\n");
  }
}

void displayMaxOccurrences()
{
  int maxOcc = 0;
  char results[1000][WORDLENGTH];
  int count = 0;
  int i;

  for (i = 0; i < ALPHABETLENGTH; i++)
  {
    findMaxOccurrences(alpha[i].initialTree, &maxOcc, results, &count);
  }

  if (count > 0)
  {
    printf("\n=== PALAVRA(S) COM MAIOR NUMERO DE OCORRENCIAS ===\n");
    printf("Numero de ocorrencias: %d\n", maxOcc);
    for (i = 0; i < count; i++)
    {
      printf("%s\n", results[i]);
    }
    printf("================================================\n");
  }
  else
  {
    printf("Nenhuma palavra encontrada.\n");
  }
}

void displaySingleOccurrenceWords()
{
  int i;
  printf("\n=== PALAVRAS COM UMA OCORRENCIA ===\n");
  for (i = 0; i < ALPHABETLENGTH; i++)
  {
    displaySingleOccurrence(alpha[i].initialTree);
  }
  printf("=================================\n");
}

void processText(char *text)
{
  char *word = strtok(text, " \t\n\r\f\v.,!?;:");
  while (word != NULL)
  {
    insertWord(word);
    word = strtok(NULL, " \t\n\r\f\v.,!?;:");
  }
}

int main()
{
  initializeAlphabet();

  printf("=== SISTEMA DE ORGANIZACAO DE PALAVRAS ===\n");
  printf("Implementacao usando Arvores AVL\n\n");

  char texto[] = "joao gosta muito de comer banana. ele gosta de frutas doces como banana e manga. "
                 "joao come banana todos os dias. a banana eh uma fruta muito nutritiva. "
                 "gosta de preparar vitamina de banana. banana banana banana.";

  printf("Processando texto de exemplo...\n");
  printf("Texto: %s\n\n", texto);

  char textoCopia[1000];
  strcpy(textoCopia, texto);
  processText(textoCopia);

  printf("\n=== DEMONSTRACAO DAS FUNCIONALIDADES ===\n\n");

  printf("1. CONTAGEM TOTAL DE PALAVRAS:\n");
  countTotalWords();

  printf("\n2. CONTAGEM TOTAL DE OCORRENCIAS:\n");
  countTotalOccurrences();

  printf("\n3. CONSULTA DE PALAVRAS ESPECIFICAS:\n");
  consultWord("banana");
  consultWord("joao");
  consultWord("inexistente");

  printf("\n4. EXIBICAO DE TODAS AS PALAVRAS (com contadores):\n");
  displayAllWords(0, 1);

  printf("\n5. EXIBICAO DE PALAVRAS INICIADAS POR 'b':\n");
  displayWordsByLetter('b', 0, 1);

  printf("\n6. PALAVRA(S) COM MAIOR NUMERO DE OCORRENCIAS:\n");
  displayMaxOccurrences();

  printf("\n7. PALAVRAS COM APENAS UMA OCORRENCIA:\n");
  displaySingleOccurrenceWords();

  printf("\n8. DEMONSTRACAO DE REMOCAO:\n");
  printf("Removendo palavra 'banana'...\n");
  removeWord("banana");
  printf("Consultando 'banana' apos remocao:\n");
  consultWord("banana");

  printf("\n9. CONTAGEM APOS REMOCAO:\n");
  countTotalWords();
  countTotalOccurrences();

  printf("\n=== DEMONSTRACAO CONCLUIDA ===\n");

  return 0;
}