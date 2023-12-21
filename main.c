#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "gfx.h"

//Cauê Mendonça Magela do Ó
//rgm : 43558

struct s_no{
    int32_t chave:28;
    int32_t bal:2;
    int32_t reservado:2; /* sem uso */
    struct s_no* esq;
    struct s_no* dir;
};

struct s_arq_no{
    int32_t chave:28;
    int32_t bal:2;
    uint32_t esq:1;
    uint32_t dir:1;
};

void Buscar(struct s_no*, struct s_no**, struct s_no**, int, int*);

void Inserir(struct s_no**, struct s_no*, struct s_no*, int, int*);

void RotDir(struct s_no**, struct s_no*, struct s_no*, int*);

void RotEsq(struct s_no**, struct s_no*, struct s_no*, int*);

void Remover(struct s_no**, struct s_no*, struct s_no*, int, int*);

void ProcSuc(struct s_no**, struct s_no*, struct s_no*, struct s_no*, struct s_no*, int*);

void SalvarArq(struct s_no*, FILE*);

void LerArq(struct s_no**, struct s_no*, FILE*, int);

void Tela(int, int, int, int, int, int, struct s_no*);

void Desalocar(struct s_no*);

int main() {
    FILE *arq;
    int resp, f, num, h;
    char caminho[100];
    struct s_no *arvore = NULL;
    struct s_no *pt = NULL;
    struct s_no *ant = NULL;
    gfx_init(1200, 720, "s_no");
    do{
        printf("1-Busca\n2-Incerção\n3-Remoção\n4-Salvar árvore\n5-Carregar árvore\n6-Sair\n");
        scanf("%d", &resp);
        switch (resp) {
            case 1:
                printf("Informe o número que vai ser buscado: \n");
                scanf("%d", &num);
                f = -1;
                pt = NULL;
                ant = NULL;
                Buscar(arvore, &pt, &ant, num, &f);
                switch (f) {
                    case -1:
                        printf("Ocorreu algum erro ao buscar o número\n");
                        break;
                    case 0:
                        printf("A árvore está vazia\n");
                        break;
                    case 1:
                        printf("O número foi encontrado\n");
                        break;
                    default:
                        printf("O número não foi encontrado na árvore\n");
                        break;
                }
                break;
            case 2:
                printf("Informe o número que vai ser inserido: \n");
                scanf("%d", &num);
                h = 0;
                Inserir(&arvore, arvore, arvore, num, &h);
                break;
            case 3:
                printf("Informe o número que vai ser removido: \n");
                scanf("%d", &num);
                h = 0;
                Remover(&arvore, arvore, arvore, num, &h);
                break;
            case 4:
                printf("Informe o caminho do arquivo que será escrito: \n");
                scanf("%s", caminho);
                arq = fopen(caminho, "wb");
                SalvarArq(arvore, arq);
                fclose(arq);
                break;
            case 5:
                printf("Informe o caminho do arquivo que será lido: \n");
                scanf("%s", caminho);
                arq = fopen(caminho, "rb");
                if(arq != NULL){
                    printf("Sucesso");
                }else{
                    printf("Erro");
                }
                if(arvore != NULL) {
                    Desalocar(arvore);
                }
                LerArq(&arvore, pt, arq, 0);
                fclose(arq);
                break;
            case 6:
                break;
            default:
                printf("Opção inválida\n");
        }
        gfx_clear();
        Tela(0, 1200, 10, 0, 0, 0, arvore);
        gfx_paint();

    } while (resp != 6);
    gfx_quit();
    Desalocar(arvore);
    return 0;
}

/*
Busca uma chave na árvore,
- Se a árvore for vazia então f recebe 0.
- Se a chave buscada for igual a chave de no então f recebe 1 e pt recebe no,
- Se a chave buscada for menor que a chave de no e no possuir um nó esquerdo então ant recebe no Buscar se chama recursivamente passando esse nó esquerdo,
- Se a chave buscada for menor que a chave de no e no não possuir um nó esquerdo então f recebe 2 e pt recebe no,
- Se a chave buscada for maior que a chave de no e no possuir um nó direito então ant recebe no Buscar se chama recursivamente passando esse nó direito,
- Se a chave buscada for maior que a chave de no e no não possuir um nó direito então f recebe 3 e pt recebe no.
*/
void Buscar(struct s_no *no, struct s_no **pt, struct s_no **ant, int chave, int *f){
    if(no == NULL){
        *f = 0;
        return;
    }
    if(no->chave == chave){
        *pt = no;
        *f = 1;
        return;
    } else{
        if(no->chave > chave){
            if(no->esq != NULL){
                *ant = no;
                Buscar(no->esq, pt, ant, chave, f);
                return;
            }else{
                *pt = no;
                *f = 2;
                return;
            }
        }else{
            if(no->chave < chave){
                if(no->dir != NULL){
                    *ant = no;
                    Buscar(no->dir, pt, ant, chave, f);
                    return;
                }else{
                    *pt = no;
                    *f = 3;
                    return;
                }
            }
        }
    }
}

/*
Insere uma chave na árvore,
- Se f = 1 então a chave já está na árvore e não precisa ser inserida,
caso a chave não esteja na árvore ainda então se aloca ptr que vai receber a chave,
- Se f = 0 então ptr será o primeiro elemento da árvore,
- Se f = 2 então pt recebe ptr como seu nó esquerdo,
- Se f = 3 então pt recebe ptr como seu ná direito.
*/

void Inserir(struct s_no **arvore, struct s_no *pt, struct s_no *ant, int chave, int *h){
    if(*arvore == NULL){
        struct s_no *ptr = malloc(sizeof (struct s_no));
        ptr->chave = chave;
        ptr->bal = 0;
        ptr->esq = NULL;
        ptr->dir = NULL;
        *arvore = ptr;
        return;
    }
    if(pt->chave == chave){
        printf("A chave já está na árvore\n");
        return;
    }
    if(pt->chave > chave){
        if(pt->esq != NULL){
            Inserir(arvore, pt->esq, pt, chave, h);
        } else{
            struct s_no *ptr = malloc(sizeof (struct s_no));
            ptr->chave = chave;
            ptr->bal = 0;
            ptr->esq = NULL;
            ptr->dir = NULL;
            pt->esq = ptr;
            *h = 1;
        }
        if(*h == 1){
            switch (pt->bal) {
                case 1:
                    pt->bal = 0;
                    *h = 0;
                    break;
                case 0:
                    pt->bal = -1;
                    break;
                case -1:
                    RotDir(arvore, pt, ant, h);
                    break;
            }
        }
    }else{
        if(pt->dir != NULL){
            Inserir(arvore, pt->dir, pt, chave, h);
        }else{
            struct s_no *ptr = malloc(sizeof (struct s_no));
            ptr->chave = chave;
            ptr->bal = 0;
            ptr->esq = NULL;
            ptr->dir = NULL;
            pt->dir = ptr;
            *h = 1;
        }
        if(*h == 1){
            switch (pt->bal) {
                case -1:
                    pt->bal = 0;
                    *h = 0;
                    break;
                case 0:
                    pt->bal = 1;
                    break;
                case 1:
                    RotEsq(arvore, pt, ant, h);
                    break;
            }
        }
    }
}

void RotDir(struct s_no **arvore, struct s_no *pt, struct s_no *ant, int *h){
    struct s_no *pte = pt->esq;
    if(pte->bal == -1){
        pt->esq = pte->dir;
        pte->dir = pt;
        pt->bal = 0;
        pte->bal = 0;
        if(pt == *arvore){
            *arvore = pte;
        } else {
            if (ant->esq == pt) {
                ant->esq = pte;
            } else {
                ant->dir = pte;
            }
        }
    }else{
        struct s_no *ptd = pte->dir;
        pte->dir = ptd->esq;
        ptd->esq = pte;
        pt->esq = ptd->dir;
        ptd->dir = pt;
        if(ptd->bal == -1) {
            pt->bal = 1;
        }else{
            pt->bal = 0;
        }
        if(ptd->bal == 1){
            pte->bal = -1;
        }else{
            pte->bal = 0;
        }
        ptd->bal = 0;
        if(pt == *arvore){
            *arvore = ptd;
        }else {
            if (ant->esq == pt) {
                ant->esq = ptd;
            } else {
                ant->dir = ptd;
            }
        }
    }
    pt->bal = 0;
    *h = 0;
}

void RotEsq(struct s_no **arvore, struct s_no *pt, struct s_no *ant, int *h){
    struct s_no *ptd = pt->dir;
    if(ptd->bal == 1){
        pt->dir = ptd->esq;
        ptd->esq = pt;
        pt->bal = 0;
        ptd->bal = 0;
        if(pt == *arvore){
            *arvore = ptd;
            ptd->bal = 0;
        }else {
            if (ant->esq == pt) {
                ant->esq = ptd;
            } else {
                ant->dir = ptd;
            }
        }
    }else{
        struct s_no *pte = ptd->esq;
        ptd->esq = pte->dir;
        pte->dir = ptd;
        pt->dir = pte->esq;
        pte->esq = pt;
        if(pte->bal == 1){
            pt->bal = -1;
        } else{
            pt->bal = 0;
        }
        if(pte->bal == -1){
            ptd->bal = 1;
        }else{
            ptd->bal = 0;
        }
        pte->bal = 0;
        if(pt == *arvore){
            *arvore = pte;
        }else {
            if (ant->esq == pt) {
                ant->esq = pte;
            } else {
                ant->dir = pte;
            }
        }
    }
    pt->bal = 0;
    *h = 0;
}
/*
Remove uma chave da árvore
- Se pt não possuir outros nós então ant recebe NULL no lugar de pt e desaloca pt;
- Se pt possuir apenas um nó então ant recebe esse nó no lugar de pt e desaloca pt;
- Se pt possuir dois nós e o nó direito não possuir nó esquerdo então ant recebe o nó direito no lugar de pt, o nó direito recebe o nó esquerdo e desaloca pt,
- Se pt possuir dois nós e o nó direito possuir nó esquerdo então se procura o sucessor de pt, ao encontrar ant recebe o sucessor no lugar de pt,
o nó anterior do sucessor recebe toda subárvore direita do sucessor como sua subárvore esquerda, o sucessor recebe tanto a subárvore esquerda quanto a direita de pt e desaloca pt.
*/
void Remover(struct s_no **arvore, struct s_no *pt, struct s_no *ant, int chave, int *h){
    struct s_no *aux_pt;
    struct s_no *aux_ant;
    if(*arvore == NULL){
        printf("A árvore está vazia\n");
        return;
    }
    if(pt->chave == chave) {
        if (*arvore == pt) {
            if ((pt->esq == NULL) && (pt->dir == NULL)) {
                *arvore = NULL;
                free(pt);
                return;
            }
            if ((pt->esq != NULL) && (pt->dir == NULL)) {
                *arvore = pt->esq;
                return;
            }
            if ((pt->esq == NULL) && (pt->dir != NULL)) {
                *arvore = pt->dir;
            } else {
                if ((pt->esq != NULL) && (pt->dir != NULL)) {
                    aux_pt = pt->dir;
                    aux_ant = pt;
                    if (aux_pt->esq == NULL) {
                        *arvore = aux_pt;
                        aux_pt->esq = pt->esq;
                        switch (pt->bal) {
                            case 1:
                                aux_pt->bal = 0;
                                break;
                            case 0:
                                aux_pt->bal = -1;
                                break;
                            case -1:
                                RotDir(arvore, aux_pt, ant, h);
                        }
                        free(pt);
                        return;
                    } else {
                        return;
                    }
                }
            }
        } else {
            if ((pt->esq == NULL) && (pt->dir == NULL)) {
                if (ant->esq == pt) {
                    ant->esq = NULL;
                    free(pt);
                    return;
                } else {
                    ant->dir = NULL;
                    free(pt);
                    return;
                }
            }
            if ((pt->esq != NULL) && (pt->dir == NULL)) {
                if (ant->esq == pt) {
                    ant->esq = pt->esq;
                    free(pt);
                    return;
                } else {
                    ant->dir = pt->esq;
                    free(pt);
                    return;
                }
            }
            if ((pt->esq == NULL) && (pt->dir != NULL)) {
                if (ant->esq == pt) {
                    ant->esq = pt->dir;
                    free(pt);
                    return;
                } else {
                    ant->dir = pt->dir;
                    free(pt);
                    return;
                }
            }
            if ((pt->esq != NULL) && (pt->dir != NULL)) {
                aux_pt = pt->dir;
                aux_ant = pt;
                if (aux_pt->esq == NULL) {
                    if (ant->esq == pt) {
                        ant->esq = aux_pt;
                        aux_pt->esq = pt->esq;
                        free(pt);
                        return;
                    } else {
                        ant->dir = aux_pt;
                        aux_pt->esq = pt->esq;
                        free(pt);
                        return;
                    }
                } else {
                }
            }
        }
        return;
    }else{
        if(pt->chave > chave){
            if(pt->esq != NULL) {
                Remover(arvore, pt->esq, pt, chave, h);
                if (*h == 1) {
                    switch (pt->bal) {
                        case -1:
                            pt->bal = 0;
                            *h = 0;
                            break;
                        case 0:
                            pt->bal = 1;
                            break;
                        case 1:
                            RotEsq(arvore, pt, ant, h);
                    }
                }
            }else{
                printf("A chave não está na árvore\n");
            }
        }else{
            if(pt->dir != NULL) {
                Remover(arvore, pt->dir, pt, chave, h);
                if (*h == 1) {
                    switch (pt->bal) {
                        case 1:
                            pt->bal = 0;
                            *h = 0;
                            break;
                        case 0:
                            pt->bal = -1;
                            break;
                        case -1:
                            RotDir(arvore, pt, ant, h);
                    }
                }
            }else{
                printf("A chave não está na árvore\n");
            }
        }
    }
}

void ProcSuc(struct s_no **arvore, struct s_no *pt, struct s_no *ant, struct s_no *aux_pt, struct s_no *aux_ant, int *h){
    if(aux_pt->esq != NULL) {
        ProcSuc(arvore, pt, ant, aux_pt->esq, aux_pt, h);
        if(*h == 1){
            switch (aux_pt->bal) {
                case 1:
                    aux_pt->bal = 0;
                    break;
                case 0:
                    aux_pt->bal = -1;
                    break;
                case -1:
                    RotDir(arvore, aux_pt, ant, h);
            }
        }
    }else{
        if(pt == *arvore) {
            *arvore = aux_pt;
            aux_pt->esq = pt->esq;
            aux_ant->esq = aux_pt->dir;
            aux_pt->dir = pt->dir;
            switch (pt->bal) {
                case 1:
                    aux_pt->bal = 0;
                    break;
                case 0:
                    aux_pt->bal = -1;
                    break;
                case -1:
                    RotDir(arvore, aux_pt, ant, h);
            }
            free(pt);
            *h = 1;
            return;
        }
        if (ant->esq == pt) {
            ant->esq = aux_pt;
            aux_pt->esq = pt->esq;
            aux_ant->esq = aux_pt->dir;
            aux_pt->dir = pt->dir;
            switch (pt->bal) {
                case 1:
                    aux_pt->bal = 0;
                    break;
                case 0:
                    aux_pt->bal = -1;
                    break;
                case -1:
                    RotDir(arvore, aux_pt, ant, h);
            }
            free(pt);
            *h = 1;
            return;
        } else {
            ant->dir = aux_pt;
            aux_pt->esq = pt->esq;
            aux_ant->esq = aux_pt->dir;
            aux_pt->dir = pt->dir;
            switch (pt->bal) {
                case 1:
                    aux_pt->bal = 0;
                    break;
                case 0:
                    aux_pt->bal = -1;
                    break;
                case -1:
                    RotDir(arvore, aux_pt, ant, h);
            }
            free(pt);
            *h = 1;
            return;
        }
    }
}

void SalvarArq(struct s_no *arvore, FILE *arq){
    struct s_arq_no *sarqno = malloc(sizeof (struct s_arq_no));
    sarqno->chave = arvore->chave;
    sarqno->bal = arvore->bal;
    if(arvore->esq != NULL){
        sarqno->esq = 1;
    }else{
        sarqno->esq = 0;
    }
    if(arvore->dir != NULL){
        sarqno->dir = 1;
    }else{
        sarqno->dir = 0;
    }
    fwrite(sarqno, sizeof (struct s_arq_no), 1, arq);
    free(sarqno);
    if(arvore->esq != NULL){
        SalvarArq(arvore->esq, arq);
    }
    if(arvore->dir != NULL){
        SalvarArq(arvore->dir, arq);
    }
}

void LerArq(struct s_no **arvore, struct s_no *pt, FILE *arq, int caso){
    struct s_arq_no *arqno = malloc(sizeof (struct s_arq_no));
    struct s_no *ptr = malloc(sizeof (struct s_no));
    fread(arqno, sizeof (struct s_arq_no), 1, arq);
    ptr->chave = arqno->chave;
    ptr->bal = arqno->bal;
    switch (caso) {
        case 0:
            *arvore = ptr;
            printf("Sus");
            break;
        case 1:
            pt->esq = ptr;
            printf("Sus");
            break;
        case 2:
            pt->dir = ptr;
            printf("Sus");
            break;
    }
    if(arqno->esq == 1){
        LerArq(arvore, ptr, arq, 1);
    }
    if(arqno->dir == 1){
        LerArq(arvore, ptr, arq, 2);
    }
    free(arqno);
}


//Desenha a árvore
void Tela(int iwidth, int width, int height, int x, int y, int b, struct s_no *pt){
    char string[50];
    int textwid, texthei, wid;
    sprintf(string, "%d", pt->chave);
    wid = width-iwidth;
    gfx_get_text_size(string, &textwid, &texthei);
    gfx_set_color(255, 255, 255);
    gfx_text(iwidth+((wid - textwid)/2), height, string);
    gfx_set_color(0, 0, 255);
    gfx_ellipse(iwidth+(wid/2), height + (texthei/2), 15, 15);
    gfx_set_color(0, 0, b);
    gfx_line(x, y, iwidth+(wid/2), height + (texthei/2) - 15);
    if(pt->esq != NULL){
        Tela(iwidth, wid/2 + iwidth, height+100, iwidth+(wid/2) - 10, height + (texthei/2) + 10, 255,  pt->esq);
    }
    if(pt->dir != NULL){
        Tela(iwidth + wid/2, width, height+100, iwidth+(wid/2) + 10, height + (texthei/2) + 10, 255, pt->dir);
    }
}

//Desaloca a árvore
void Desalocar(struct s_no *no){
    if(no->esq != NULL){
        Desalocar(no->esq);
    }
    if(no->dir != NULL){
        Desalocar(no->dir);
    }
    if((no->dir == NULL) && (no->esq == NULL)){
        free(no);
        return;
    }
}
