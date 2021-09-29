#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef struct{

    unsigned Linie,Coloana;
    unsigned Latime,Inaltime;
    int Cifra;
    double Scor;

} Fereastra;

typedef struct{

    unsigned Lungime;
    Fereastra *Vector;

} ListaFerestre;

Pixel* InitializeazaCulorilePentruContur(){

    Pixel *Culoare=(Pixel*)malloc(10*sizeof(Pixel));

    Culoare[0].Albastru=0; Culoare[0].Verde=0; Culoare[0].Rosu=255; /// ROSU
    Culoare[1].Albastru=0; Culoare[1].Verde=255; Culoare[1].Rosu=255; /// GALBEN
    Culoare[2].Albastru=0; Culoare[2].Verde=255; Culoare[2].Rosu=0; /// VERDE
    Culoare[3].Albastru=255; Culoare[3].Verde=255; Culoare[3].Rosu=0; /// CYAN
    Culoare[4].Albastru=255; Culoare[4].Verde=0; Culoare[4].Rosu=255; /// MAGENTA
    Culoare[5].Albastru=255; Culoare[5].Verde=0; Culoare[5].Rosu=0; /// ALBASTRU
    Culoare[6].Albastru=192; Culoare[6].Verde=192; Culoare[6].Rosu=192; /// ARGINTIU
    Culoare[7].Albastru=0; Culoare[7].Verde=140; Culoare[7].Rosu=255; /// PORTOCALIU
    Culoare[8].Albastru=128; Culoare[8].Verde=0; Culoare[8].Rosu=128; /// ROZINCHIS
    Culoare[9].Albastru=0; Culoare[9].Verde=0; Culoare[9].Rosu=128; /// CASTANIU

    return Culoare;

}

void CitesteImagineSiSabloane(char *FisierBitmap,char **FisierSablon,
                              BitmapPrelucrat *Imagine, BitmapPrelucrat **Sabloane){

    (*Imagine)=FaMatriceDinBitmap(FisierBitmap);

    (*Sabloane)=(BitmapPrelucrat*)malloc(10*sizeof(BitmapPrelucrat));

    for(int i=0;i<10;i++){

        (*Sabloane)[i]=FaMatriceDinBitmap(FisierSablon[i]);

        (*Sabloane)[i].Spatiu=i;

    }

}

void DeseneazaContur(BitmapPrelucrat Imagine,Fereastra Detectie,Pixel Culoare){

    unsigned MargineStanga,MargineDreapta,MargineSus,MargineJos;
    MargineStanga=Detectie.Coloana;
    MargineDreapta=Detectie.Coloana+Detectie.Latime-1;
    MargineSus=Detectie.Linie;
    MargineJos=Detectie.Linie+Detectie.Inaltime-1;

    for(int i=Detectie.Linie;i<=Detectie.Linie+Detectie.Inaltime-1;i++)
        Imagine.Tablou[i][MargineStanga]=Imagine.Tablou[i][MargineDreapta]=Culoare;

    for(int j=Detectie.Coloana;j<=Detectie.Coloana+Detectie.Latime-1;j++)
        Imagine.Tablou[MargineSus][j]=Imagine.Tablou[MargineJos][j]=Culoare;

}

BitmapPrelucrat CreezaImagineGrayScale(BitmapPrelucrat Imagine){

    BitmapPrelucrat ImagineGri;

    ImagineGri.Antet=(unsigned char*) malloc(LUNGIME_ANTET*sizeof(unsigned char));
    for(int i=0;i<LUNGIME_ANTET;i++) ImagineGri.Antet[i]=Imagine.Antet[i];

    if(Imagine.CataUmplutura!=0)

        ImagineGri.Umplutura=(unsigned char*)malloc((Imagine.CataUmplutura)*sizeof(unsigned char));

    else ImagineGri.Umplutura=NULL;

    ImagineGri.CataUmplutura=Imagine.CataUmplutura;
    ImagineGri.Latime=Imagine.Latime;
    ImagineGri.Inaltime=Imagine.Inaltime;
    ImagineGri.Spatiu=Imagine.Spatiu;

    unsigned ValoareGri;

    if(Imagine.Vector==NULL){

        ImagineGri.Vector=NULL;

        ImagineGri.Tablou=(Pixel**)malloc(Imagine.Inaltime*sizeof(Pixel*));
        for(int i=0;i<Imagine.Inaltime;i++)
            ImagineGri.Tablou[i]=(Pixel*)malloc(Imagine.Latime*sizeof(Pixel));

        for(int i=0;i<Imagine.Inaltime;i++)
            for(int j=0;j<Imagine.Latime;j++){

                ValoareGri=0.114*Imagine.Tablou[i][j].Albastru+
                        0.587*Imagine.Tablou[i][j].Verde+
                        0.299*Imagine.Tablou[i][j].Rosu;

                ImagineGri.Tablou[i][j].Albastru=ValoareGri;
                ImagineGri.Tablou[i][j].Verde=ValoareGri;
                ImagineGri.Tablou[i][j].Rosu=ValoareGri;

            }

    }

    else{

        ImagineGri.Tablou=NULL;

        ImagineGri.Vector=(Pixel*)malloc(Imagine.Spatiu*sizeof(Pixel));

        for(int i=0;i<Imagine.Spatiu;i++){

                ValoareGri=0.114*Imagine.Vector[i].Albastru+
                        0.587*Imagine.Vector[i].Verde+
                        0.299*Imagine.Vector[i].Rosu;

                ImagineGri.Vector[i].Albastru=ValoareGri;
                ImagineGri.Vector[i].Verde=ValoareGri;
                ImagineGri.Vector[i].Rosu=ValoareGri;

            }

    }

    return ImagineGri;

}

ListaFerestre DetecteazaPotriviriInImagine(BitmapPrelucrat Imagine,BitmapPrelucrat Sablon,double Prag){

    ListaFerestre Detectie;
    Detectie.Lungime=0;
    Detectie.Vector=NULL;

    double NrPixeli=Sablon.Latime*Sablon.Inaltime;;
    double MediaSablon=0;

    for(int i=0;i<Sablon.Inaltime;i++)
        for(int j=0;j<Sablon.Latime;j++)
            MediaSablon=MediaSablon+Sablon.Tablou[i][j].Albastru;

    MediaSablon=MediaSablon/NrPixeli;

    double DeviatiaSablon=0;

     for(int i=0;i<Sablon.Inaltime;i++)
        for(int j=0;j<Sablon.Latime;j++)
            DeviatiaSablon=DeviatiaSablon+(Sablon.Tablou[i][j].Albastru-MediaSablon)*
                                        (Sablon.Tablou[i][j].Albastru-MediaSablon);

    DeviatiaSablon=DeviatiaSablon/(NrPixeli-1);
    DeviatiaSablon=sqrt(DeviatiaSablon);

    for(int i=0;i<Imagine.Inaltime-Sablon.Inaltime+1;i++)
        for(int j=0;j<Imagine.Latime-Sablon.Latime+1;j++){

            double MediaFereastra=0;

            for(int k=i;k<=i+Sablon.Inaltime-1;k++)
                for(int p=j;p<=j+Sablon.Latime-1;p++)
                    MediaFereastra=MediaFereastra+Imagine.Tablou[k][p].Albastru;

            MediaFereastra=MediaFereastra/NrPixeli;

            double DeviatiaFereastra=0;

            for(int k=i;k<=i+Sablon.Inaltime-1;k++)
                for(int p=j;p<=j+Sablon.Latime-1;p++)
                    DeviatiaFereastra=DeviatiaFereastra+(Imagine.Tablou[k][p].Albastru-MediaFereastra)*
                                                    (Imagine.Tablou[k][p].Albastru-MediaFereastra);

            DeviatiaFereastra=DeviatiaFereastra/(NrPixeli-1);
            DeviatiaFereastra=sqrt(DeviatiaFereastra);

            double Corelatie=0;

            for(int ii=0,k=i;ii<Sablon.Inaltime;ii++,k++)
                for(int jj=0,p=j;jj<Sablon.Latime;jj++,p++)
                    Corelatie=Corelatie+(Imagine.Tablou[k][p].Albastru-MediaFereastra)*
                                        (Sablon.Tablou[ii][jj].Albastru-MediaSablon)/
                                        (DeviatiaFereastra*DeviatiaSablon);

            Corelatie=Corelatie/NrPixeli;

            if(Corelatie>Prag){

                ++Detectie.Lungime;
                Detectie.Vector=(Fereastra*)realloc(Detectie.Vector,Detectie.Lungime*sizeof(Fereastra));
                Detectie.Vector[Detectie.Lungime-1].Scor=Corelatie;
                Detectie.Vector[Detectie.Lungime-1].Linie=i;
                Detectie.Vector[Detectie.Lungime-1].Coloana=j;
                Detectie.Vector[Detectie.Lungime-1].Latime=Sablon.Latime;
                Detectie.Vector[Detectie.Lungime-1].Inaltime=Sablon.Inaltime;
                Detectie.Vector[Detectie.Lungime-1].Cifra=Sablon.Spatiu;

            }


        }

    return Detectie;

}

int cmpScor(const void *a,const void *b){

    Fereastra Fa=*(Fereastra*)a;
    Fereastra Fb=*(Fereastra*)b;

    if(Fa.Scor<Fb.Scor) return 1;
    if(Fa.Scor>Fb.Scor) return -1;
    return 0;

}

unsigned  Minim(unsigned x,unsigned y){

    if(x<y) return x;
    return y;

}

void Interschimba(unsigned *x,unsigned *y){

    unsigned aux;
    aux=*x;
    *x=*y;
    *y=aux;

}

double SuprapunereFerestre(Fereastra Fa,Fereastra Fb){

    unsigned L1Fa,L2Fa,C1Fa,C2Fa;
    unsigned L1Fb,L2Fb,C1Fb,C2Fb;

    L1Fa=Fa.Linie; L2Fa=L1Fa+Fa.Inaltime-1;
    L1Fb=Fb.Linie; L2Fb=L1Fb+Fb.Inaltime-1;

    C1Fa=Fa.Coloana; C2Fa=C1Fa+Fa.Latime-1;
    C1Fb=Fb.Coloana; C2Fb=C1Fb+Fb.Latime-1;

    if(L1Fa>L1Fb){

        Interschimba(&L1Fa,&L1Fb);
        Interschimba(&L2Fa,&L2Fb);

    }

    if(C1Fa>C1Fb){

        Interschimba(&C1Fa,&C1Fb);
        Interschimba(&C2Fa,&C2Fb);

    }

    if(L2Fa<L1Fb || C2Fa<C1Fb) return 0;

    unsigned ArieIntersectie=(Minim(L2Fa,L2Fb)-L1Fb+1)*(Minim(C2Fa,C2Fb)-C1Fb+1);
    unsigned ArieReuniune=2*Fa.Latime*Fa.Inaltime-ArieIntersectie;

    return (double)ArieIntersectie/ArieReuniune;

}

void SorteazaDetectiiDescrescaror(ListaFerestre Detectie){

    qsort(Detectie.Vector,Detectie.Lungime,sizeof(Fereastra),cmpScor);

}

void EliminaNonMaximele(ListaFerestre Detectie){

    SorteazaDetectiiDescrescaror(Detectie);

    for(int i=0;i<Detectie.Lungime-1;i++)
        for(int j=i+1;j<Detectie.Lungime;j++)
            if(Detectie.Vector[i].Cifra>=0 && SuprapunereFerestre(Detectie.Vector[i],Detectie.Vector[j])>0.2)
                Detectie.Vector[j].Cifra=-1;

}

BitmapPrelucrat CreezaImagineCuDetectii(char *FisierBitmap,char **FisierSablon){

    Pixel *Culoare;
    Culoare=InitializeazaCulorilePentruContur();

    BitmapPrelucrat Imagine,*Sabloane;

    CitesteImagineSiSabloane(FisierBitmap,FisierSablon,&Imagine,&Sabloane);

    BitmapPrelucrat ImagineGri,*SabloaneGri;
    SabloaneGri=(BitmapPrelucrat*)malloc(10*sizeof(BitmapPrelucrat));

    ImagineGri=CreezaImagineGrayScale(Imagine);
    for(int i=0;i<10;i++)
        SabloaneGri[i]=CreezaImagineGrayScale(Sabloane[i]);

    ListaFerestre DetectieCifra,DetectieTotal;

    DetectieTotal.Lungime=0;
    DetectieTotal.Vector=NULL;

    for(int i=0;i<10;i++){

        DetectieCifra=DetecteazaPotriviriInImagine(ImagineGri,SabloaneGri[i],0.5);

        DetectieTotal.Lungime+=DetectieCifra.Lungime;
        DetectieTotal.Vector=(Fereastra*)realloc(DetectieTotal.Vector,DetectieTotal.Lungime*sizeof(Fereastra));

        for(int i=0,j=DetectieTotal.Lungime-DetectieCifra.Lungime;j<DetectieTotal.Lungime;i++,j++)
            DetectieTotal.Vector[j]=DetectieCifra.Vector[i];

    }

    EliminaNonMaximele(DetectieTotal);

    for(int i=0;i<DetectieTotal.Lungime;i++)
        if(DetectieTotal.Vector[i].Cifra>=0)
            DeseneazaContur(Imagine,DetectieTotal.Vector[i],Culoare[DetectieTotal.Vector[i].Cifra]);

    GolesteImagine(ImagineGri);
    for(int i=0;i<10;i++)
       GolesteImagine(Sabloane[i]);
    free(Sabloane);
    for(int i=0;i<10;i++)
       GolesteImagine(SabloaneGri[i]);
    free(SabloaneGri);
    free(Culoare);

    return Imagine;

}

