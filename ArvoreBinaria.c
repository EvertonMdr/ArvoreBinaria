
#include<stdio.h>
#include<stdlib.h>

struct tree
{
    int info;
    struct tree* esq, * dir;
};
typedef struct tree Tree;

struct fila
{
    Tree *info;
    struct fila*prox;
};
typedef struct fila Fila;

// ---------------- FILA ----------------

void init(Fila **F)
{
    *F = NULL;
}

char isEmpty(Fila *F)
{
    return F == NULL;
}

void enqueue(Fila**F, Tree *info)
{
    Fila *nova = (Fila*) malloc(sizeof (Fila));
    nova->info = info;
    nova->prox = NULL;
    if (!isEmpty(*F))
    {
        Fila*aux = *F;
        while (aux->prox != NULL)
            aux = aux->prox;
        aux->prox = nova;
    }
    else
        *F = nova;
}

void dequeue(Fila**F, Tree **info)
{
    if (!isEmpty(*F))
    {
        Fila*aux = *F;
        *info = (*F)->info;
        *F = (*F)->prox;
        free(aux);
    }
    else
        *info = NULL;
}

// ---------------- Arvore binária ----------------

Tree * criaNo(int info)
{
    Tree * no = (Tree*) malloc(sizeof (Tree));
    no->info = info;
    no->esq = no->dir = NULL;

    return no;
}

void insereI(Tree **raiz, int info) // inserir Interativo 
{
    Tree * p, *a;

    if (*raiz == NULL)
        *raiz = criaNo(info);
    else
    {
        p = *raiz;
        while (p != NULL)
        {
            a = p;
            if (info < p->info)
                p = p->esq;
            else
                p = p->dir;
        }

        if (info < a->info)
            a->esq = criaNo(info);
        else
            a->dir = criaNo(info);
    }
}

void insereR(Tree **raiz, int info) // inserir recurisvo
{
    if (*raiz == NULL)
        *raiz = criaNo(info);
    else
        if (raiz != NULL)
    {
        if (info < (*raiz)->info)
            insereR(&(*raiz)->esq, info);
        else
            insereR(&(*raiz)->dir, info);
    }
}

void exclusao(Tree **raiz, Tree *pai, Tree *e, char lado)
{
    if (e->esq == NULL && e->dir == NULL)
    {
        if (e == pai)
            *raiz = NULL;
        else
        {
            if (e->info < pai->info)
                pai->esq = NULL;
            else
                pai->dir = NULL;
        }
        free(e);
    }
    else
    {
        if (e->esq == NULL || e->dir == NULL)
        {
            if (e == pai)
            {
                if ((*raiz)->esq != NULL)
                    * raiz = (*raiz)->esq;
                else
                    *raiz = (*raiz)->dir;
            }
            else
            {

                if (e->info != pai->info)
                {
                    if (e->info < pai->info)
                    {
                        if (e->esq != NULL)
                            pai->esq = e->esq;
                        else
                            pai->esq = e->dir;
                    }
                    else
                    {
                        if (e->esq != NULL)
                            pai->dir = e->esq;
                        else
                            pai->dir = e->dir;
                    }
                }
                else
                {
                    if (e->info == pai->esq->info)
                    {
                        if (e->esq != NULL)
                            pai->esq = e->esq;
                        else
                            pai->esq = e->dir;
                    }
                    else
                    {
                        if (e->esq != NULL)
                            pai->dir = e->esq;
                        else
                            pai->dir = e->dir;

                    }

                }

            }
            free(e);
        }
        else
        {
            Tree *paisub, *sub;
            paisub = e;
            if (lado == 'e')
            {
                sub = e->esq;
                while (sub->dir != NULL)
                {
                    paisub = sub;
                    sub = sub->dir;
                }

            }
            else
            {
                sub = e->dir;
                while (sub->esq != NULL)
                {
                    paisub = sub;
                    sub = sub->esq;
                }
            }

            e->info = sub->info;
            exclusao(&*raiz, paisub, sub, lado);
        }
    }
}

void quantNo(Tree * tree, int *cont)
{
    if (tree != NULL)
    {
        (*cont)++;
        quantNo(tree->esq, &*cont);
        quantNo(tree->dir, &*cont);
    }
}

void busca(Tree *tree, Tree **pai, Tree**e, int info)
{
    *pai = *e = tree;

    while (tree != NULL && tree->info != info)
    {
        *pai = tree;
        if (info < tree->info)
            tree = tree->esq;
        else
            tree = tree->dir;
    }

    if (tree == NULL)
        *pai = *e = NULL;
    else
        *e = tree;
}

void balanceamento(Tree** tree)
{
    Fila *F;
    init(&F);
    int qe, qd, FB, info;
    enqueue(&F, *tree);
    Tree * r, *pai, *e;

    while (!isEmpty(F))
    {
        dequeue(&F, &r);
        do
        {
            qd = qe = 0;
            quantNo(r->esq, &qe);
            quantNo(r->dir, &qd);

            FB = qd - qe;

            if (abs(FB) > 1) // não esta balanceada
            {
                info = r->info;
                busca(*tree, &pai, &e, info);

                if (r->esq == NULL) 
                    r = r->dir;
                else
                    if (r->dir == NULL)
                    r = r->esq;

                if (FB > 0)
                    exclusao(&*tree, pai, e, 'd');
                else
                    exclusao(&*tree, pai, e, 'e');

                insereI(&*tree, info);
            }

        }
        while (abs(FB) > 1);

        if (r->esq != NULL)
            enqueue(&F, r->esq);
        if (r->dir != NULL)
            enqueue(&F, r->dir);
    }
}

void excluir(Tree **raiz, int info)
{
    Tree * e, * pai;

    busca(*raiz, &pai, &e, info);
    if (e != NULL)
        exclusao(&*raiz, pai, e, 'd');
    else
        printf("Esse no não existe");
}

void preOrdem(Tree *raiz)
{
    if (raiz != NULL)
    {
        printf("%d ", raiz->info);
        preOrdem(raiz->esq);
        preOrdem(raiz->dir);
    }
}

void inOrdem(Tree *raiz)
{
    if (raiz != NULL)
    {
        inOrdem(raiz->esq);
        printf("%d ", raiz->info);
        inOrdem(raiz->dir);

    }
}

void posOrdem(Tree *raiz)
{
    if (raiz != NULL)
    {

        posOrdem(raiz->esq);
        posOrdem(raiz->dir);
        printf("%d ", raiz->info);
    }
}
int main()
{
    Tree * raiz=NULL;

    insereI(&raiz,10);
    insereI(&raiz,14);
    insereI(&raiz,12);
    insereI(&raiz,20);
    insereI(&raiz,5);
    insereI(&raiz,13);
    
    balanceamento(&raiz);
    
    inOrdem(raiz);
    printf("\n");
    excluir(&raiz,12);
    
    inOrdem(raiz);
    
}

