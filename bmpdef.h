#include<stdio.h>
#include<stdlib.h>

#define LUNGIME_ANTET 54
#define LOCATIE_LATIME 18
#define LOCATIE_INALTIME 22

typedef struct {

    unsigned char Albastru,Verde,Rosu;

} Pixel;

typedef struct{

    unsigned char *Antet,*Umplutura;
    unsigned Latime,Inaltime,Spatiu,CataUmplutura;
    Pixel **Tablou,*Vector;

} BitmapPrelucrat;

unsigned ValoareaUnsignedDeLaAntet(unsigned char *Antet,unsigned Locatie){

    unsigned Dimensiune;

    Dimensiune=Antet[Locatie]+
                (1<<8)*Antet[Locatie+1]+
                (1<<16)*Antet[Locatie+2]+
                (1<<24)*Antet[Locatie+3];

    return Dimensiune;

}

BitmapPrelucrat CitesteAntetSiInformatii(BitmapPrelucrat Imagine,FILE *Citire){

    Imagine.Antet=(unsigned char*) malloc(LUNGIME_ANTET*sizeof(unsigned char));
    fread(Imagine.Antet,sizeof(unsigned char),LUNGIME_ANTET,Citire);

    Imagine.Latime = ValoareaUnsignedDeLaAntet(Imagine.Antet,LOCATIE_LATIME);
    Imagine.Inaltime = ValoareaUnsignedDeLaAntet(Imagine.Antet,LOCATIE_INALTIME);

    unsigned OctetiPeLinie=Imagine.Latime*3;
    unsigned MultipluDePatru=OctetiPeLinie/4*4+(OctetiPeLinie%4!=0)*4;

    Imagine.CataUmplutura=MultipluDePatru-OctetiPeLinie;

    Imagine.Spatiu=Imagine.Latime*Imagine.Inaltime;

    Imagine.Umplutura=(unsigned char*)malloc((Imagine.CataUmplutura)*sizeof(unsigned char));
    if(Imagine.CataUmplutura==0) Imagine.Umplutura=NULL;

    return Imagine;

}

BitmapPrelucrat FaVectorDinBitmap(char *FisierBitmap){

    BitmapPrelucrat Imagine;

    FILE *Citire=fopen(FisierBitmap,"rb");
    Imagine.Tablou=NULL;

    Imagine=CitesteAntetSiInformatii(Imagine,Citire);

    Imagine.Vector=(Pixel*)malloc(Imagine.Spatiu*sizeof(Pixel));

    for(int i=Imagine.Inaltime-1;i>=0;i--){
        for(int j=0;j<Imagine.Latime;j++){

            fread(&(Imagine.Vector[i*Imagine.Latime+j].Albastru),sizeof(unsigned char),1,Citire);
            fread(&(Imagine.Vector[i*Imagine.Latime+j].Verde),sizeof(unsigned char),1,Citire);
            fread(&(Imagine.Vector[i*Imagine.Latime+j].Rosu),sizeof(unsigned char),1,Citire);

        }

        fread(Imagine.Umplutura,sizeof(unsigned char),Imagine.CataUmplutura,Citire);

    }

    fclose(Citire);

    return Imagine;

}

BitmapPrelucrat FaMatriceDinBitmap(char *FisierBitmap){

    BitmapPrelucrat Imagine;

    FILE *Citire=fopen(FisierBitmap,"rb");
    Imagine.Vector=NULL;

    Imagine=CitesteAntetSiInformatii(Imagine,Citire);

    Imagine.Tablou=(Pixel**)malloc(Imagine.Inaltime*sizeof(Pixel*));
    for(int i=0;i<Imagine.Inaltime;i++)
        Imagine.Tablou[i]=(Pixel*)malloc(Imagine.Latime*sizeof(Pixel));

    for(int i=Imagine.Inaltime-1;i>=0;i--){
        for(int j=0;j<Imagine.Latime;j++){

            fread(&(Imagine.Tablou[i][j].Albastru),sizeof(unsigned char),1,Citire);
            fread(&(Imagine.Tablou[i][j].Verde),sizeof(unsigned char),1,Citire);
            fread(&(Imagine.Tablou[i][j].Rosu),sizeof(unsigned char),1,Citire);

        }

        fread(Imagine.Umplutura,sizeof(unsigned char),Imagine.CataUmplutura,Citire);

    }

    fclose(Citire);

    return Imagine;

}

void FaBitmapDinVector(char *FisierBitmap,BitmapPrelucrat Imagine){

    FILE *Afisare=fopen(FisierBitmap,"wb");

    fwrite(Imagine.Antet,sizeof(unsigned char),LUNGIME_ANTET,Afisare);

    for(int i=Imagine.Inaltime-1;i>=0;i--){

        for(int j=0;j<Imagine.Latime;j++){

            fwrite(&Imagine.Vector[i*Imagine.Latime+j].Albastru,sizeof(unsigned char),1,Afisare);
            fwrite(&Imagine.Vector[i*Imagine.Latime+j].Verde,sizeof(unsigned char),1,Afisare);
            fwrite(&Imagine.Vector[i*Imagine.Latime+j].Rosu,sizeof(unsigned char),1,Afisare);

        }

        fwrite(Imagine.Umplutura,sizeof(unsigned char),Imagine.CataUmplutura,Afisare);

    }


    fclose(Afisare);

}

void FaBitmapDinMatrice(char *FisierBitmap,BitmapPrelucrat Imagine){

    FILE *Afisare=fopen(FisierBitmap,"wb");

    fwrite(Imagine.Antet,sizeof(unsigned char),LUNGIME_ANTET,Afisare);

    for(int i=Imagine.Inaltime-1;i>=0;i--){

        for(int j=0;j<Imagine.Latime;j++){

            fwrite(&Imagine.Tablou[i][j].Albastru,sizeof(unsigned char),1,Afisare);
            fwrite(&Imagine.Tablou[i][j].Verde,sizeof(unsigned char),1,Afisare);
            fwrite(&Imagine.Tablou[i][j].Rosu,sizeof(unsigned char),1,Afisare);

        }

        fwrite(Imagine.Umplutura,sizeof(unsigned char),Imagine.CataUmplutura,Afisare);

    }


    fclose(Afisare);

}

void GolesteImagine(BitmapPrelucrat Imagine){

    free(Imagine.Antet);

    if(Imagine.Umplutura!=NULL)
        free(Imagine.Umplutura);

    if(Imagine.Vector!=NULL)
        free(Imagine.Vector);

    if(Imagine.Tablou!=NULL){

        for(int i=0;i<Imagine.Inaltime;i++)
            free(Imagine.Tablou[i]);

        free(Imagine.Tablou);

    }

}


