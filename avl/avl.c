#include "avl.h"

int vazia(AVL *t){
    return t == NULL;
};

void imprime(AVL *t){	
	if(vazia(t))
        printf("<>");
    else
        if(vazia(t->esq) && (vazia(t->dir))){
            printf("\nMatricula:%d",t->matricula);
            printf(" Nome:%s",t->nome);
            printf(" CR:%f",t->cr);
			printf(" Semestre:%d",t->nSemestre);
            printf(" CargaHoraria:%f",t->cargaCursada);
        }
        else{
            printf("\nMatricula:%d",t->matricula);
            printf(" Nome:%s",t->nome);
            printf(" CR:%f",t->cr);
			printf(" Semestre:%d",t->nSemestre);
            printf(" CargaHoraria:%f",t->cargaCursada);
			if(t->esq)
				imprime(t->esq);
			if(t->dir)
				imprime(t->dir);

        }
};

	AVL* cria(){
		return NULL;
	};

AVL* RSE(AVL *t){
    AVL *novo = t->dir;
    t->dir = novo->esq;
    novo->esq = t;
    t->alt = 1 + maximo(calc_alt(t->esq),calc_alt(t->dir));
    novo->alt = maximo(calc_alt(novo->dir),t->alt)+1;
    return novo;
};

AVL* RSD(AVL *t){
    AVL *novo =t->esq;
    t->esq = novo->dir;
    novo->dir = t;
    t->alt = 1 + maximo(calc_alt(t->esq),calc_alt(t->dir));
    novo->alt = maximo(calc_alt(novo->esq),t->alt)+1;
    return novo;
};

AVL* RDE(AVL* t){
     t->dir = RSD(t->dir);
     t = RSE(t);
     return t;
};

AVL* RED(AVL* t){
     t->esq = RSE(t->esq);
     t = RSD(t);
     return t;
};

int maximo(int a, int b){
    if(a>b) return a;
    return b;
};

int calc_alt(AVL *t){
    if(!t) return -1;
    return t->alt;
};



AVL* insere(AVL *t, int mat , char* nome , float c , float carga , int sem){
    if(!t){
        t = (AVL*) malloc((sizeof(AVL)));

        t->matricula = mat;
        t->cargaCursada = carga;
        t->cr = c;
        t->nSemestre = sem;
        strcpy(t->nome,nome);        
        t->alt = 0;
        t->esq = NULL;
		t->dir = NULL;
    }
    else
        if(mat < t->matricula){
            t->esq = insere(t->esq,mat,nome,c,carga,sem);
            if((calc_alt(t->esq) - calc_alt((t->dir))) == 2)
                if(mat < t->esq->matricula)
                    t = RSD(t);
                else
                    t = RED(t);
        }
        else
            if(mat > t->matricula){
                t->dir = insere(t->dir,mat,nome,c,carga,sem);
                if((calc_alt(t->dir) - calc_alt((t->esq))) == 2)
                    if(mat > t->dir->matricula)
                        t = RSE(t);
                    else
                        t = RDE(t);
            }
         t ->alt = maximo(calc_alt(t->esq),calc_alt(t->dir)) + 1;
         return t;
};

AVL* busca(AVL *t , int m){
	if(vazia(t))
		return t;
	if(t->matricula == m)
		return t;
	if(m < t->matricula)
		busca(t->esq,m);
	else
		busca(t->dir,m);
};

int fb(AVL* t){
	int left,right;
	if(!t)
		return 0;
    if(!(t->esq))
        left = 0;
    else
        left = 1 + (t->esq->alt);
    if(!(t->dir))
        right = 0;
    else
        right = 1 + (t->dir->alt);
    return (left - right);
}

AVL* retira(AVL *t,int m){
    if(!t)
        return t;
    if(m > t->matricula){
        t->dir = retira(t->dir,m);
        if(fb(t) == 2)
            if(fb(t->esq) >= 0)
                t = RSD(t);
            else
                t = RSE(t);
    }
    else
        if(m < t->matricula){
            t->esq = retira(t->esq,m);
            if(fb(t) == -2)
                if(fb(t->dir) <= 0)
                    t = RSE(t);
                else
                    t = RDE(t);
        }
    else{
        if(t->dir){
            AVL *p = t->dir;
            while(p->esq)
                p = p->esq;
            t->matricula = p->matricula;
			t->cargaCursada = p->cargaCursada;
			strcpy(t->nome,p->nome);
			t->cr = p->cr;
			t->nSemestre = p->nSemestre;			
            p->matricula = m;
            t->dir = retira(t->dir,m);
            if(fb(t) == 2)
                if(fb(t->esq) >= 0)
                    t = RSD(t);
                else
                    t = RED(t);
        }
        else{
            AVL *q = t;
			if(vazia(t->esq) && vazia(t->dir))
				t = NULL;
			else
				t = t->esq;
            free(q);
            return t;
        }
        if(t){
            int lh,rh;
            if(!t->esq)
                lh = 0;
            else
                lh = 1 + t->esq->alt;
            if(!t->dir)
                rh = 0;
            else
                rh = 1 + t->dir->alt;
            if(lh > rh)
                t->alt = lh;
            else
                t->alt = rh;
        }
    }
	return t;
};

AVL* limpeza(AVL *t){
	float carga = CARGA_MAX;
	if(vazia(t))
		return t;
	else {
		if(t->esq)
			t->esq = limpeza(t->esq);
		if(t->dir)
			t->dir = limpeza(t->dir);		
		if((t->cargaCursada == carga) && (t->nSemestre <= 12)){
			printf("\nMatricula:%d",t->matricula);
			printf(" Nome:%s",t->nome);
			printf(" CR:%f",t->cr);
			printf(" Semestre:%d",t->nSemestre);
			printf(" CargaHoraria:%f\n",t->cargaCursada);
			printf(" foi removido pela Regra de formatura");
			return retira(t,t->matricula);
		}
		else if((t->cargaCursada < carga/2) && (t->nSemestre > 8)){
				printf("\nMatricula:%d",t->matricula);
				printf(" Nome:%s",t->nome);
				printf(" CR:%f",t->cr);
				printf(" Semestre:%d",t->nSemestre);
				printf(" CargaHoraria:%f\n",t->cargaCursada);
				printf(" foi removido pela Regra de Jubilamento A");						
				return retira(t,t->matricula);			
			}
			else
				if((t->cargaCursada < carga) && (t->nSemestre > 12)){		
					printf("\nMatricula:%d",t->matricula);
					printf(" Nome:%s",t->nome);
					printf(" CR:%f",t->cr);
					printf(" Semestre:%d",t->nSemestre);
					printf(" CargaHoraria:%f\n",t->cargaCursada);
					printf(" foi removido pela Regra de Jubilamento B");
					return retira(t,t->matricula);
				}				
	}
};


