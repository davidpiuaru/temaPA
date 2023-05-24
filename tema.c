#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Jucator
{
    char prenume[50];
    char nume[50];
    int puncte;
};
typedef struct Jucator Jucator;

//structura pentru lista cu jucatori
struct Echipa
{
    char nume_echipa[50];
    Jucator participanti[20];
    int nr_participanti;
    float puncte_echipa;
};
typedef struct Echipa Echipa;
//lista cu echipele
struct Nod_echipa
{
    Echipa *echipa;
    struct Nod_echipa *next;
};
typedef struct Nod_echipa Nod_echipa;


//structura pentru coada
struct Coada
{
    Nod_echipa *front, *rear;
};
typedef struct Coada Coada;

//structura pentru stiva
struct Stiva
{
    struct Nod_echipa *top;
};
typedef struct Stiva Stiva;


void citire_echipe(Nod_echipa **head, int *nr_echipe, FILE *fisier_echipe)
{
    fscanf(fisier_echipe, "%d", nr_echipe);
    for (int i = 0; i < *nr_echipe; i++) {
        Nod_echipa *newNode = (Nod_echipa *)malloc(sizeof(Nod_echipa));
        newNode->echipa = (Echipa *)malloc(sizeof(Echipa));

        fscanf(fisier_echipe, "%d", &newNode->echipa->nr_participanti);
        fgetc(fisier_echipe);
        fgets(newNode->echipa->nume_echipa, sizeof(newNode->echipa->nume_echipa), fisier_echipe);
        
        //sterge toate caracterele de la final care nu sunt litere sau cifre
        for (int i = strlen(newNode->echipa->nume_echipa) - 1; i >= 0; i--) {
            if (!(newNode->echipa->nume_echipa[i] >= 'a' && newNode->echipa->nume_echipa[i] <= 'z' ||
                newNode->echipa->nume_echipa[i] >= 'A' && newNode->echipa->nume_echipa[i] <= 'Z' ||
                newNode->echipa->nume_echipa[i] >= '0' && newNode->echipa->nume_echipa[i] <= '9')) {
                newNode->echipa->nume_echipa[i] = '\0';
            }
            else {
                break;
            }
        }
        //citeste jucatorii din echipa si punctajul lor
        for (int j = 0; j < newNode->echipa->nr_participanti; j++) {
            fscanf(fisier_echipe, "%s", newNode->echipa->participanti[j].nume);
            fscanf(fisier_echipe, "%s", newNode->echipa->participanti[j].prenume);
            fscanf(fisier_echipe, "%d", &newNode->echipa->participanti[j].puncte);
        }
        newNode->next = *head;
        *head = newNode;


    }
}

void afisare_echipe(Nod_echipa *head, FILE *fisier_iesire)
{
    while (head != NULL) {
        fprintf(fisier_iesire, "%s\n", head->echipa->nume_echipa);
        head = head->next;
    }
}

void calculeaza_puncte(Nod_echipa *head)
{
    while (head != NULL) {
        for (int i = 0; i < head->echipa->nr_participanti; i++) {
            head->echipa->puncte_echipa += head->echipa->participanti[i].puncte;
        }
        head->echipa->puncte_echipa /= head->echipa->nr_participanti;
        head = head->next;
    }
}

void sterge_echipa(Nod_echipa **head, Nod_echipa *nod)
{
    Nod_echipa *aux = *head;
    Nod_echipa *prev = NULL;
    //cauta nodul in lista
    while (aux != NULL) {
        if (aux == nod) {
            if (prev == NULL) {//daca nodul este primul din lista
                *head = aux->next;
                free(aux);
                aux = *head;
            }
            else {//daca nodul este in interiorul listei
                prev->next = aux->next;
                free(aux);
                aux = prev->next;
            }
        }
        else {//
            prev = aux;
            aux = aux->next;
        }
    }
}

int este_putere_2(int nr)
{
    int i = 1;
    while (i < nr)
    {
        i *= 2;
    }
    if(i == nr)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//sterge echipele pana cand ramane o putere a lui 2 nr de echipe, stregand-o pe cea mai mica ca punctaj
void sterge_echipe(Nod_echipa **head, int *nr_echipe)
{
    calculeaza_puncte(*head);
    while (!este_putere_2(*nr_echipe)) 
    {
        Nod_echipa *min = *head;
        Nod_echipa *aux = *head;
        while (aux != NULL) 
        {
            if (aux->echipa->puncte_echipa < min->echipa->puncte_echipa) 
            {
                min = aux;
            }
            aux = aux->next;
        }
        sterge_echipa(head, min);
        (*nr_echipe)--;
    }
}


Coada *creeaza_coada()
{
    Coada *q;
    q = (Coada*)malloc(sizeof(Coada));
    if (q == NULL) 
        return NULL ;
    q->front = q->rear = NULL ;
    return q;
}

void adauga_in_coada(Coada*q, Echipa *v)
{
	Nod_echipa* newNode=(Nod_echipa*)malloc(sizeof(Nod_echipa));
	newNode->echipa = v;
	newNode->next=NULL;
	if (q->rear==NULL) 
        q->rear=newNode; 
	else//daca coada nu e goala
    {
		(q->rear)->next = newNode;//
		(q->rear)=newNode;
	}
	if (q->front==NULL) 
        q->front=q->rear; 
}
//scoate din coada
Echipa *scoate_din_coada(Coada*q)
{
    Nod_echipa* aux;
    Echipa *v;
    if (q->front == NULL) 
        return NULL;

    aux = q->front;
    q->front = (q->front)->next;
    v = aux->echipa;
    
    free(aux);
    return v;
}

int este_coada_goala(Coada *q)
{
    return (q->front == NULL);
}

void adauga_in_stiva(Stiva *stiva_echipe, Echipa *v) {

	Nod_echipa* newNode = (Nod_echipa*)malloc(sizeof(Nod_echipa));
	newNode->echipa = v;
	newNode->next = stiva_echipe->top;
	stiva_echipe->top = newNode;
}


//scoate din stiva
Echipa *scoate_din_stiva(Stiva *stiva_echipe)
{
    Nod_echipa* aux;
    Echipa *v;
    if (stiva_echipe->top == NULL) 
        return NULL;

    aux = stiva_echipe->top;
    stiva_echipe->top = (stiva_echipe->top)->next;
    v = aux->echipa;
    
    free(aux);
    return v;
}

Stiva *creeaza_stiva()
{
    Stiva *s;
    s = (Stiva*)malloc(sizeof(Stiva));
    if (s == NULL) 
        return NULL;
    s->top = NULL;
    return s;
}

int este_stiva_goala(Stiva *s)
{
    return (s->top == NULL);
}

void afisare_stiva(Stiva *s, FILE *fisier_iesire)
{
    Nod_echipa *aux = s->top;
    while (aux != NULL)
    {
        fprintf(fisier_iesire, "%s\n", aux->echipa->nume_echipa);
        aux = aux->next;
    }
}

//afiseaza coada
void afisare_coada(Coada *q, FILE *fisier_iesire)
{
    Nod_echipa *aux = q->front;
    while (aux != NULL)
    {
        fprintf(fisier_iesire, "%s\n", aux->echipa->nume_echipa);
        aux = aux->next;
    }
}

void afisare_formatata2(Echipa *echipa, FILE *fisier_iesire)
{
    int dimensiune_spatiu = 34;
    int dimensiune_nume_echipa = strlen(echipa->nume_echipa);
    fprintf(fisier_iesire, "%s", echipa->nume_echipa);
    for (int i = 0; i < dimensiune_spatiu - dimensiune_nume_echipa; i++)
        fprintf(fisier_iesire, " ");
    fprintf(fisier_iesire, "-  %2.2f\n", echipa->puncte_echipa);   
}

void afisare_formatata1(char nume_echipa1[], char nume_echipa2[], FILE* fisier_iesire)
{
    int dimensiune_spatiu = 33;
    int dimensiune_nume_echipa1 = strlen(nume_echipa1);
    int dimensiune_nume_echipa2 = strlen(nume_echipa2);
    fprintf(fisier_iesire, "%s", nume_echipa1);
    for (int i = 0; i < dimensiune_spatiu - dimensiune_nume_echipa1; i++)
        fprintf(fisier_iesire, " ");

    fprintf(fisier_iesire, "-");

    for (int i = 0; i < dimensiune_spatiu - dimensiune_nume_echipa2; i++)
        fprintf(fisier_iesire, " ");
    
    fprintf(fisier_iesire, "%s\n", nume_echipa2);

} 

void meciuri(Nod_echipa *lista_echipe, int nr_echipe, FILE *fisier_iesire, Nod_echipa *echipe_castigatoare)
{
    Coada *coada_meciuri = creeaza_coada();
    Stiva *stiva_castigatori = creeaza_stiva();
    Stiva *stiva_invinsi = creeaza_stiva();
    Nod_echipa *aux = lista_echipe;
   
    while (aux != NULL)
    {
        adauga_in_coada(coada_meciuri, aux->echipa);
        aux = aux->next;
    }

    int numar_de_runde_total = 1;
    int aux_nr_echipe = nr_echipe;
    while(aux_nr_echipe > 1)
    {
        numar_de_runde_total++;
        aux_nr_echipe = aux_nr_echipe / 2;
    }


    for(int i = 0; i < numar_de_runde_total - 1; i++)
    {
        fprintf(fisier_iesire, "\n");
        fprintf(fisier_iesire, "--- ROUND NO:%d\n", i + 1);
        int nr_meciuri = nr_echipe / 2;
        for (int j = 0; j < nr_meciuri; j++)
        {
            //scoate din coada 2 echipe
            Echipa *echipa1 = scoate_din_coada(coada_meciuri);
            Echipa *echipa2 = scoate_din_coada(coada_meciuri);
            afisare_formatata1(echipa1->nume_echipa, echipa2->nume_echipa, fisier_iesire);
            if (echipa1->puncte_echipa > echipa2->puncte_echipa)
            {//echipa1 castiga
                adauga_in_stiva(stiva_castigatori, echipa1);
                adauga_in_stiva(stiva_invinsi, echipa2);
                echipa1->puncte_echipa++;
            }
            else
            {//echipa2 castiga
                adauga_in_stiva(stiva_castigatori, echipa2);
                adauga_in_stiva(stiva_invinsi, echipa1);
                echipa2->puncte_echipa++;
            }
            nr_echipe--;
        }
        while (!este_stiva_goala(stiva_invinsi))//scoate din stiva invinsi si elibereaza memoria
        {
            Echipa *echipa = scoate_din_stiva(stiva_invinsi);
            free(echipa);
        }
        fprintf(fisier_iesire, "\n");
        fprintf(fisier_iesire, "WINNERS OF ROUND NO:%d\n", i + 1);
        while (!este_stiva_goala(stiva_castigatori))//scoate din stiva castigatori si adauga in coada
        {
            Echipa *echipa = scoate_din_stiva(stiva_castigatori);
            afisare_formatata2(echipa, fisier_iesire);
            adauga_in_coada(coada_meciuri, echipa);
        }
    
    }

    free(coada_meciuri);
    free(stiva_castigatori);
    free(stiva_invinsi); 
}

int main(int argc, char *argv[])
{

    int cerinte[5] = {0, 0, 0, 0, 0};
    int nr_echipe;
    FILE *fisier_cerinte = fopen(argv[1], "rt");
    //citire taskuri
    for (int i = 0; i < 5; i++)
    {
        fscanf(fisier_cerinte, "%d", &cerinte[i]);
    }

    FILE *fisier_echipe = fopen(argv[2], "rt");
    FILE *fisier_iesire = fopen(argv[3], "wt");
    Nod_echipa *lista_echipe = NULL;

    // ex 1
    citire_echipe(&lista_echipe, &nr_echipe, fisier_echipe);

    //afisare_echipe(echipe, fisier_iesire);
    if (cerinte[0] == 1 && cerinte[1] == 0)
    {
        afisare_echipe(lista_echipe, fisier_iesire);
    }

    if (cerinte[1] == 1)
    {
        sterge_echipe(&lista_echipe, &nr_echipe);
        afisare_echipe(lista_echipe, fisier_iesire);
    }

    Nod_echipa *echipe_castigatoare = NULL;
    if(cerinte[2] == 1)
    {
        meciuri(lista_echipe, nr_echipe, fisier_iesire, echipe_castigatoare);
        
    }

}