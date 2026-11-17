#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <iconv.h>
#include <cctype>

using namespace std;
int amount = 0;

/*string codir(const char* src, int maxLen) {
    iconv_t cd = iconv_open("UTF-8", "CP866");

    if (cd == (iconv_t)-1) return string(src);

    char inb[256];
    strncpy(inb, src, maxLen);
    inb[maxLen] = '\0';

    char outb[1024] = {0};
    char* pin = inb;
    char* pout = outb;
    size_t inleft = strlen(inb);
    size_t outleft = sizeof(outb) - 1;

    iconv(cd, &pin, &inleft, &pout, &outleft);
    iconv_close(cd);

    return string(outb);
}*/

struct Book{
    char author[12];
    char title[32];
    char publisher[16];
    short int year;
    short int pages;
};

struct spis{
    spis *next;
    Book *data;
};

struct Tree{
    Book *data;
    struct Tree *left;
    struct spis *mid;
    struct Tree *right;
    int balance;
};

Tree *createNode(Book *data){
    Tree *newTree = (Tree*)malloc(sizeof(Tree));
    newTree->data = data;
    newTree->left = NULL;
    newTree->mid = NULL;
    newTree->right = NULL;
    newTree->balance = 0;
    return newTree;
}


struct tle{
    spis *head;
    spis *tail;
};

char* surname(char* ifo){
    int n = (int)strlen(ifo);
    int end = n -1;
    while(end >= 0 && isspace((unsigned char)ifo[end])) end--;
    if(end < 0) return ifo;

    ifo[end + 1] = '\0';

    int i = end;
    while(i >= 0 && !isspace((unsigned char)ifo[i])) i--;
    return ifo+i+1;
}


void prnt(spis *head){
    spis *p;
    int i = 0, g;
    p = head;
    cout << "\n   Author       | Title                            | Publisher        | Year | Pages" << endl;
    cout << "----------------+----------------------------------+------------------+------+------" << endl;
    while((i++) < 4000){
        //string a = codir(p->author, 12);
        //string t = codir(p->title, 32);
        //string pub = codir(p->publisher, 16);
        //printf("%d) %-12s | %-32s | %-16s | %4hd | %4hd\n", i,
              // a.c_str(), t.c_str(), pub.c_str(), p->year, p->pages);
        printf("%d) %-12s | %-32s | %-16s | %4hd | %4hd\n",i,
               p->data->author, p->data->title, p->data->publisher, p->data->year, p->data->pages);
        p=p->next;
        if(i%20==0){
            cout << "Продолжить? (1/0): ";
            cin >> g;
            if(g==0){
                break;
            }
        }
    }
}

spis *DigitalSort(spis *&S){
    spis *p;
    int KDI[]={7,6,5,4,3,2,1,0};
    tle Q[256];
    for(int j = 0; j < 8; j++){
        for(int i = 0; i < 256; i++){Q[i].tail = (spis*)&Q[i].head;}
            p = S;
            while(p){
                char key[8] = {0};
                strncpy(key, surname(p->data->title), 8);
                int g = (int)(unsigned char)key[KDI[j]];

                Q[g].tail ->next = p;
                Q[g].tail = p;              
                p=p->next;
            }
            p=(spis*)&S;
            for(int i = 0; i < 256; i++){
                if(Q[i].tail != (spis*)&Q[i].head){
                    p->next = Q[i].head;
                    p=Q[i].tail;
                }
            }
            p->next = NULL;
        }
        return S;
    }

spis *makeSpis(Book a[], int n){
    spis *head, *p;
    head = new spis;
    head->next = 0;
    head->data = &a[0];
    for(int i = 1; i < n; i++){
        p = new spis;
        p->data = &a[i];
        p->next = head;
        head = p;
    }
    return head;
}

void binSearchList(spis *head){
    spis *p;
    int i = 1;
    p = head->next;
    for(p = head->next; p; p=p->next, i++){cout << i << ") " << p->data->title << endl;}
}

spis* binSearch(spis *head, Book** a, int R, const char* fam2){
    char fam[4];
    strncpy(fam, fam2, 3);
    fam[3] = '\0';
    int i;
    char* b[4000];
    for(i = 0; i < 4000; i++){ b[i] = surname(a[i]->title);  }
    int L = 0;
    int m;

    while(L < R){
        m = (L + R) / 2;
        //printf("%s %s\n", b[m], fam);
        if(strncmp(b[m], fam,3) < 0){ L = m + 1; }
        else{R = m;}
    }

    if(R < 4000 && strncmp(b[R], fam,3) == 0){
        spis *result = nullptr;
        spis *tail = nullptr;

        for(int k = R; k < 4000 && strncmp(b[k], fam,3) == 0; k++){
            spis *node = new spis;
            node->data = a[k];
            node->next = nullptr;

            if(!result){result = node; tail = node;}
            else{tail->next = node; tail = node;}
        }
        return result;
    }
    else{
        cout << "no" << endl;
        return nullptr;
    }
}

Tree *LLturn(Tree *p){
    Tree *q = p->left;
    q->balance = 0;
    p->balance = 0;
    p->left = q->right;
    q->right = p;
    p = q;
    return p;
}

Tree *LRturn(Tree *p){
    Tree *q = p->left;
    Tree *r = q->right;

    if(r->balance < 0){p->balance = 1;}
    else{p->balance = 0;}

    if(r->balance > 0){q->balance = -1;}
    else{q->balance = 0;}
 
    r->balance = 0;

    p->left = r->right;
    q->right = r->left;
    r->left =  q;
    r->right = p;
    p = r;
    return p;
}

Tree *RRturn(Tree *p){
    Tree *q = p->right;
    q->balance = 0;
    p->balance = 0;
    p->right = q->left;
    q->left = p;
    p = q;
    return p;
}

Tree *RLturn(Tree *p){
    Tree *q = p->right;
    Tree *r = q->left;

    if(r->balance > 0){p->balance = -1;}
    else{p->balance = 0;}

    if(r->balance < 0){q->balance = 1;}
    else{q->balance = 0;}
 
    r->balance = 0;

    p->right = r->left;
    q->left = r->right;
    r->left =  p;
    r->right = q;
    p = r;
    return p;
}

Tree *InsertAVLTree(Tree *p, Book *data, int *grown){
    if(p==NULL){
        Tree *newNode = createNode(data);
        *grown = true;
        return newNode;
    }

    if(p->data->year > data->year){
        p->left = InsertAVLTree(p->left, data, grown);

        if(*grown){
            if(p->balance > 0){
                p->balance = 0;
                *grown = false;}
            else if(p->balance == 0){p->balance = -1;}
            else{
                if(p->left->balance < 0){p = LLturn(p);}
                else{p = LRturn(p);}
                *grown = false;}
        }
    }
    else if(p->data->year < data->year){
        p->right = InsertAVLTree(p->right, data, grown);

        if(*grown){
            if(p->balance < 0){
                p->balance = 0;
                *grown = false;}
            else if(p->balance == 0){
                p->balance = 1;
                *grown = true;
            }
            else{
                if(p->right->balance > 0){p = RRturn(p);}
                else{p = RLturn(p);}
                *grown = false;
            }
        } 
    }
    else{
        spis *m = (spis*)malloc(sizeof(spis));
        m->data = data;
        m->next = NULL;

        if(p->mid == NULL){p->mid = m;}
        else{
            spis *cur = p->mid;
            while (cur->next) cur = cur->next;
            cur->next = m;}
        *grown = false;}

    return p;
}

Tree *InsertAVL(Tree *root, Book *data){
    int grown = false;
    return InsertAVLTree(root, data, &grown);
}

void printLR_2(Tree *root, int &i, int &c){
    if(root == NULL || c==0){return;}

    printLR_2(root->left, i, c);
    if(c==0){return;}

    printf("%-12s | %-32s | %-16s | %4hd | %4hd\n",
           root->data->author, root->data->title,
           root->data->publisher, root->data->year, root->data->pages);
    i++;
    if(i % 20 == 0){
        cout << "Продолжить? (1/0): ";
        cin >> c;
        if(c == 0){return;}
    }
    for(spis *m = root->mid; m; m = m->next){
        printf("%-12s | %-32s | %-16s | %4hd | %4hd\n",
               m->data->author, m->data->title,
               m->data->publisher, m->data->year, m->data->pages);
        i++;
        if(i % 20 == 0){
            cout << "Продолжить? (1/0): ";
            cin >> c;
            if (c == 0) {return;}
        }
    }

    printLR_2(root->right, i, c);
}

void printLR(Tree *root) {
    int i = 0;
    int c = 1;
    printLR_2(root, i, c);
}

int main(){
    FILE *fp;
    fp = fopen("testBase1.dat", "rb");
    Book a[4000]={0};
    fread((Book *)a, sizeof(Book), 4000, fp);
    spis *head = makeSpis(a,4000);
    spis *p = head;
    Book** b = new Book*[4000];    

    head = DigitalSort(p);
    p = head;
    spis *p2 = head;
    prnt(p2);
    for(int i = 0; i < 4000; i++){
        b[i] = p->data;
        p=p->next;}
    cout << "key: ";
    char key[64];
    cin >> key;
    key[3]='\0';
    spis *found = binSearch(p2, b, 4000, key);
    //if(found){prnt(found);}

    Tree *rootAVL = NULL;
    for(spis *q = found; q != NULL; q = q->next){
        rootAVL = InsertAVL(rootAVL, q->data);}
    
    printf("\nAVL:\n");
    printLR(rootAVL);
    
    
    return 0;
}