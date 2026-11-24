#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_chr 256
#define MAX_CODE_LEN 32

int compInd(const void *a, const void *b, double *P){
    int idx1 = *(int*)a;
    int idx2 = *(int*)b;
    if (P[idx1] < P[idx2]){return 1;}
    if (P[idx1] > P[idx2]){return -1;}
    return 0;}

double *global_P; 
int compSwap(const void *a, const void *b){return compInd(a, b, global_P);}



int main(){
    FILE *file;
    unsigned long long counts[MAX_chr] = {0};
    unsigned long long total_chars = 0;

    file = fopen("text.txt", "rb");

    int ch;
    while((ch = fgetc(file)) != EOF){
        counts[(unsigned char)ch]++;
        total_chars++;}
    fclose(file);

    int uniqueNum = 0;
    for(int i = 0; i < MAX_chr; i++){if (counts[i] > 0) uniqueNum++;}

    int *chr = (int*)malloc(uniqueNum * sizeof(int));
    double *P = (double*)malloc(uniqueNum * sizeof(double));
    int *L = (int*)malloc(uniqueNum * sizeof(int));
    char **codes = (char**)malloc(uniqueNum * sizeof(char*));
    
    int idx = 0;
    for(int i = 0; i < MAX_chr; i++){
        if(counts[i] > 0){
            chr[idx] = i;
            P[idx] = (double)counts[i] / total_chars;
            codes[idx] = (char*)malloc(MAX_CODE_LEN * sizeof(char));
            idx++;}
    }
    int *indices = (int*)malloc(uniqueNum * sizeof(int));
    for (int i = 0; i < uniqueNum; i++) indices[i] = i;
    
    global_P = P;
    qsort(indices, uniqueNum, sizeof(int), compSwap);

    int *chrSorted = (int*)malloc(uniqueNum * sizeof(int));
    double *PSorted = (double*)malloc(uniqueNum * sizeof(double));
    for(int i = 0; i < uniqueNum; i++){
        chrSorted[i] = chr[indices[i]];
        PSorted[i] = P[indices[i]];}

    double Q = 0.0;
    for(int i = 0; i < uniqueNum; i++){
        double log_val = -log2(PSorted[i]);
        L[i] = (int)ceil(log_val);
        if(L[i] == 0){L[i] = 1;}

        double current_Q = Q;
        for(int j = 0; j < L[i]; j++){
            current_Q *= 2.0;
            if(current_Q >= 1.0){
                codes[i][j] = '1';
                current_Q -= 1.0;}
            else{codes[i][j] = '0';}
        }
        codes[i][L[i]] = '\0';
        
        Q += PSorted[i];
    }

    printf("----------------------------------------------------------------------\n");
    printf("| %-10s | %-20s | %-15s | %-15s |\n", "Symbol", "Pi", "Kod slovo", "Li");
    printf("----------------------------------------------------------------------\n");
    for (int i = 0; i < uniqueNum; i++) {
        if(chrSorted[i] >= 32 && chrSorted[i] <= 126){
            printf("| %-8c | %-20.6f | %-15s | %-15d |\n", 
                   (char)chrSorted[i], PSorted[i], codes[i], L[i]);}
        else{
            printf("| 0x%02X     | %-20.6f | %-15s | %-15d |\n", 
                   chrSorted[i], PSorted[i], codes[i], L[i]);}
    }
    printf("----------------------------------------------------------------------\n\n");

    double kraft_sum = 0.0;
    double entropy = 0.0;
    double avg_len = 0.0;

    for (int i = 0; i < uniqueNum; i++) {
        kraft_sum += pow(2.0, -L[i]);
        entropy += PSorted[i] * (-log2(PSorted[i]));
        avg_len += PSorted[i] * L[i];
    }
    double redundancy = avg_len - entropy;

    printf("---------------------------------------------------------------------------------\n");
    printf("| %-20s | %-20s | %-14s | %-14s |\n", "Nerav Krafta", "Entropy texta", "Av length", "Izbyt koda");
    printf("---------------------------------------------------------------------------------\n");
    printf("| %-20.6f | %-20.6f | %-14.6f | %-14.6f |\n", kraft_sum, entropy, avg_len, redundancy);
    printf("---------------------------------------------------------------------------------\n\n");
    
    file = fopen("text.txt", "rb");

    
    unsigned long long total_bits = 0;
    int chars_read = 0;

    while((ch = fgetc(file)) != EOF && chars_read < 100){
        unsigned char c = (unsigned char)ch;
        for(int i = 0; i < uniqueNum; i++){
            if(chrSorted[i] == c){
                printf("%s", codes[i]);
                total_bits += L[i];
                break;}
        }
        chars_read++;
    }
    fclose(file);
    printf("\n\n");

    unsigned long long original_bits = chars_read * 8;
    
    printf("Length zakod posled: %llu bit\n", total_bits);
    printf("Length ishod posled(%d byte): %llu bit\n", chars_read, original_bits);
    
    if (total_bits > 0) {
        double compression_ratio = (double)original_bits / total_bits;
        printf("Koef schatiya: %.4f\n", compression_ratio);
    }

    for (int i = 0; i < uniqueNum; i++) free(codes[i]);
    free(codes);
    free(chr);
    free(P);
    free(L);
    free(indices);
    free(chrSorted);
    free(PSorted);

    return 0;
}