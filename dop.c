#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Tree{
    int data;
    int weight;
    struct Tree *left;
    struct Tree *right;
} Tree;

int **AW, **AP, **AR;

typedef struct {
    int key;
    int Wesi;
} Wes;

long long get_weightSum = 0;
int get_weightTotal = 0;

Tree *createNode(int data, int weight){
    Tree *newTree = (Tree*)malloc(sizeof(Tree));
    newTree->data = data;
    newTree->weight = weight;
    newTree->left = NULL;
    newTree->right = NULL;
    return newTree;}

void CreateArray(int N){
    AW = (int**)malloc((N+1)*sizeof(int*));
    AP = (int**)malloc((N+1)*sizeof(int*));
    AR = (int**)malloc((N+1)*sizeof(int*));

    for(int i=0;i<=N;i++){
        AW[i] = (int*)calloc(N+1, sizeof(int));
        AP[i] = (int*)calloc(N+1, sizeof(int));
        AR[i] = (int*)calloc(N+1, sizeof(int));}
}

void prntMatrix(const char* name, int** matrix, int n) {
    printf("\n%s:\n", name);
    printf("     ");
    for(int j = 0; j <= n; j++){printf("| %4d ", j);}
    printf("|\n");
    printf("-----");
    for(int j = 0; j <= n; j++){printf("-------");}
    printf("-\n");

    for(int i = 0; i <= n; i++){
        printf(" %3d ", i);
        for(int j = 0; j <= n; j++){
            if(i < j){printf("| %4d ", matrix[i][j]);}
            else{printf("| %4d ", 0);}
        }
        printf("|\n");
        
        if(i < n){
            printf("-----");
            for(int j = 0; j <= n; j++){
                printf("-------");}
            printf("-\n");
        }
    }
    
    printf("-----");
    for(int j = 0; j <= n; j++){printf("-------");}
    printf("-\n");
}

void PrintMatrix(int n){
    prntMatrix("AW", AW, n);
    prntMatrix("AP", AP, n);
    prntMatrix("AR", AR, n);
    printf("\n");
}

void Freematrix(int N){
    for(int i = 0; i <= N; i++){
        free(AW[i]);
        free(AP[i]);
        free(AR[i]);}
    free(AW);
    free(AP);
    free(AR);}

void MatrixAW(Wes V[], int n){
    for(int i = 0;i <= n; i++){
        for(int j = i + 1; j <= n; j++){
            AW[i][j] = AW[i][j-1] + V[j-1].Wesi;}
    }
}

void MatrixAP_AR(Wes V[], int n){
    for(int i = 0; i < n; i++){
        int j = i+1;
        AP[i][j] = AW[i][j];
        AR[i][j] = j;}

    for(int h = 2; h <= n; h++){
        for(int i = 0; i + h <= n; i++){
            int j = i+h;

            int m = AR[i][j-1];
            int min = AP[i][m-1] + AP[m][j];

            int k_end = AR[i+1][j];

            for(int k = m + 1; k <= k_end; k++){
                int x = AP[i][k-1] + AP[k][j];
                if(x<min){m = k; min = x;}
            }

            AP[i][j] = min + AW[i][j];
            AR[i][j] = m;}
    }
}

Tree *TreeFromAR(Wes V[], int L, int R){
    if(L >= R){return NULL;}
    int k = AR[L][R];
    Tree *Tree = createNode(V[k-1].key, V[k-1].Wesi);
    Tree->left  = TreeFromAR(V, L, k-1);
    Tree->right = TreeFromAR(V, k, R);
    return Tree;}

Tree *Create_DOP(Wes V[], int N){
    CreateArray(N);
    MatrixAW(V, N);
    MatrixAP_AR(V,N);
    PrintMatrix(N);
    Tree *root = TreeFromAR(V, 0, N);
    return root;}

void Random(int A[], int N){
    int used[201] = {0};
    int count = 0;
    while(count < N){
        int num = rand() % 201;  
        if(!used[num]){
            used[num] = 1;
            A[count] = num;
            count++;}
    }
    printf("\n");}

void PrintArray(int A[], int N){
    for(int i = 0; i < N; i++){
        printf("%d ", A[i]);
        if((i + 1) % 20 == 0){printf("\n");}
    }
    printf("\n\n");}

void sortt(int A[], int n){
    for(int i = 0; i < n - 1; i++){
        for(int j = 0; j < n - i - 1; j++){
            if(A[j]>A[j+1]){
                int temp = A[j];
                A[j] = A[j+1];
                A[j+1] = temp;}
        }
    } 
}


int getSize(Tree *root){
    if(root == NULL){return 0;}
    return 1 + getSize(root->left) + getSize(root->right);}
int Sum(Tree *root) {
    if(root == NULL){return 0;}
    return root->data + Sum(root->left) + Sum(root->right);}
int MaxHeight(int a, int b) {return (a > b) ? a : b;}
int Height(Tree *root){
    if(root == NULL){return 0;}
    return 1 + MaxHeight(Height(root->left), Height(root->right));}
void AccumWeighted(Tree *root, int level){
    if(root == NULL){return;}

    AccumWeighted(root->left, level + 1);

    int w = (root->weight == 0) ? 1 : root->weight;
    get_weightSum   += (long long)w * level;
    get_weightTotal += w;

    AccumWeighted(root->right, level + 1);}

float SrVesHeight(Tree *root) {
    get_weightSum   = 0;
    get_weightTotal = 0;
    AccumWeighted(root, 1);
    if (get_weightTotal == 0) return 0.0f;
    return (float)get_weightSum / get_weightTotal;
}

void FreeTree(Tree *root){
    if(root == NULL){return;}
    FreeTree(root->left);
    FreeTree(root->right);
    free(root);}

int main(){
    srand(time(NULL));
    const int N = 5;

    int A[N];
    Wes V[N];

    Random(A, N);
    sortt(A,N);

    for(int i = 0; i < N; i++){
        V[i].key  = A[i];
        V[i].Wesi = rand() % 100 + 1;}
    for(int i = 0; i < N; i++){
        //printf("%d ", V[i].Wesi);
        if((i + 1) % 20 == 0){printf("\n");}
    }
    printf("\n\n");

    Tree *rootDOP = Create_DOP(V, N);

    printf("|---------------------------------------------------------|\n");
    printf("| n = 100 |  Size  |  Summ  |  Height  |     Av. Height   |\n");
    printf("|---------------------------------------------------------|\n");
    printf("|    DOP  |  %3d   |  %4d   |   %2d    |      %2.2f        |\n", getSize(rootDOP), Sum(rootDOP), Height(rootDOP), SrVesHeight(rootDOP));
    printf("|---------------------------------------------------------|\n");

    
    Freematrix(N); 
    FreeTree(rootDOP);
    return 0;
}