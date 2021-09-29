#include<stdio.h>
#include<stdlib.h>

unsigned XORSHIFT32(unsigned *NumereAleatoare){

    unsigned Numar=NumereAleatoare[0];
    Numar^=Numar<<13;
	Numar^=Numar>>17;
	Numar^=Numar<<5;
	NumereAleatoare[0]=Numar;
	return Numar;

}

Pixel PixelXorPixel(Pixel Variabila1,Pixel Variabila2){

    Pixel Rezultat;

    Rezultat.Albastru=Variabila1.Albastru^Variabila2.Albastru;
    Rezultat.Verde=Variabila1.Verde^Variabila2.Verde;
    Rezultat.Rosu=Variabila1.Rosu^Variabila2.Rosu;

    return Rezultat;

}

Pixel PixelXorNumar(Pixel Termen,unsigned Numar){

    Pixel Rezultat;

    unsigned Octet0,Octet1,Octet2;
    Octet0=Numar<<24; Octet0=Octet0>>24;
    Octet1=Numar>>8; Octet1=Octet1<<24; Octet1=Octet1>>24;
    Octet2=Numar>>16; Octet2=Octet2<<24; Octet2=Octet2>>24;

    Pixel NumPixelat={Octet0,Octet1,Octet2};

    Rezultat=PixelXorPixel(Termen,NumPixelat);

    return Rezultat;
}

unsigned* GenereazaNumereAleatoare(unsigned Samanta,unsigned NumarElemente){

    unsigned* NumereAleatoare=(unsigned*)malloc(NumarElemente*sizeof(unsigned));

    NumereAleatoare[0]=Samanta;
    for(int i=1;i<NumarElemente;i++)
        NumereAleatoare[i]=XORSHIFT32(NumereAleatoare);

    return NumereAleatoare;

}

unsigned* GenereazaPermutareAleatoare(unsigned *NumereAleatoare,unsigned NumarElemente){

    unsigned* PermutareAleatoare=(unsigned *)malloc(NumarElemente*sizeof(unsigned));

    for(int i=0;i<NumarElemente;i++)
        PermutareAleatoare[i]=i;

    for(int i=NumarElemente-1,k=1;i>0;i--,k++){

            int j=NumereAleatoare[k]%(i+1);
            int Pahar=PermutareAleatoare[i];
            PermutareAleatoare[i]=PermutareAleatoare[j];
            PermutareAleatoare[j]=Pahar;

    }

    return PermutareAleatoare;

}

unsigned* InverseazaPermutare(unsigned *PermutareAleatoare,unsigned NumarElemente){

    unsigned *InversaPermutareAleatoare=(unsigned *)malloc(NumarElemente*sizeof(unsigned));

    for(int i=0;i<NumarElemente;i++)
        InversaPermutareAleatoare[PermutareAleatoare[i]]=i;

    return InversaPermutareAleatoare;

}

Pixel* GenereazaPixeliPermutati(unsigned *PermutareAleatoare,BitmapPrelucrat Imagine){

    Pixel* PixeliPermutati=(Pixel *)malloc(Imagine.Spatiu*sizeof(Pixel));

    for(int i=0;i<Imagine.Spatiu;i++)
        PixeliPermutati[PermutareAleatoare[i]]=Imagine.Vector[i];

    return PixeliPermutati;

}

void CopiazaPermutareaInImagine(Pixel *PixeliPermutati,BitmapPrelucrat Imagine){

    for(int i=0;i<Imagine.Spatiu;i++)
        Imagine.Vector[i]=PixeliPermutati[i];

}

Pixel* CopiazaImagine(BitmapPrelucrat Imagine){

    Pixel *CopieDupaImagine=(Pixel*)malloc(Imagine.Spatiu*sizeof(Pixel));

    for(int i=0;i<Imagine.Spatiu;i++)
        CopieDupaImagine[i]=Imagine.Vector[i];

    return CopieDupaImagine;

}

void AplicaRegulaDeSubstitutie(unsigned Cheie,BitmapPrelucrat Imagine,unsigned *NumereAleatoare){

    int k=0,p=Imagine.Spatiu;

    Imagine.Vector[0]=PixelXorNumar(Imagine.Vector[0],Cheie);
    Imagine.Vector[0]=PixelXorNumar(Imagine.Vector[0],NumereAleatoare[p]);

    for(k=1,p=Imagine.Spatiu+1;k<Imagine.Spatiu;k++,p++){

        Imagine.Vector[k]=PixelXorNumar(Imagine.Vector[k],NumereAleatoare[p]);
        Imagine.Vector[k]=PixelXorPixel(Imagine.Vector[k],Imagine.Vector[k-1]);

    }

}

void AplicaInversaLaRegulaDeSubstitutie(unsigned Cheie,Pixel *CopieDupaImagine, BitmapPrelucrat Imagine,unsigned *NumereAleatoare){

    int k=0,p=Imagine.Spatiu;

    Imagine.Vector[0]=PixelXorNumar(Imagine.Vector[0],Cheie);
    Imagine.Vector[0]=PixelXorNumar(Imagine.Vector[0],NumereAleatoare[p]);

    for(k=1,p=Imagine.Spatiu+1;k<Imagine.Spatiu;k++,p++){

        Imagine.Vector[k]=PixelXorNumar(Imagine.Vector[k],NumereAleatoare[p]);
        Imagine.Vector[k]=PixelXorPixel(Imagine.Vector[k],CopieDupaImagine[k-1]);

    }

}

void CripteazaImagine(char *FisierInitial,char* FisierCriptat,char *FisierChei){

    BitmapPrelucrat Imagine;
    Imagine=FaVectorDinBitmap(FisierInitial);

    FILE *ExtrageChei=fopen(FisierChei,"r");

    unsigned Cheia1,Cheia2;
    fscanf(ExtrageChei,"%u %u",&Cheia1,&Cheia2);

    fclose(ExtrageChei);

    unsigned *NumereAleatoare,*PermutareAleatoare;
    NumereAleatoare=GenereazaNumereAleatoare(Cheia1,2*Imagine.Spatiu);
    PermutareAleatoare=GenereazaPermutareAleatoare(NumereAleatoare,Imagine.Spatiu);

    Pixel* PixeliPermutati;
    PixeliPermutati=GenereazaPixeliPermutati(PermutareAleatoare,Imagine);

    CopiazaPermutareaInImagine(PixeliPermutati,Imagine);

    AplicaRegulaDeSubstitutie(Cheia2,Imagine,NumereAleatoare);

    FaBitmapDinVector(FisierCriptat,Imagine);

    GolesteImagine(Imagine);
    free(NumereAleatoare);
    free(PermutareAleatoare);
    free(PixeliPermutati);


}

void DecripteazaImagine(char *FisierDecriptat,char* FisierCriptat,char *FisierChei){

    BitmapPrelucrat Imagine;
    Imagine=FaVectorDinBitmap(FisierCriptat);

    FILE *ExtrageChei=fopen(FisierChei,"r");

    unsigned Cheia1,Cheia2;
    fscanf(ExtrageChei,"%u %u",&Cheia1,&Cheia2);

    fclose(ExtrageChei);

    unsigned *NumereAleatoare,*PermutareAleatoare,*InversaPermutareAleatoare;
    NumereAleatoare=GenereazaNumereAleatoare(Cheia1,2*Imagine.Spatiu);
    PermutareAleatoare=GenereazaPermutareAleatoare(NumereAleatoare,Imagine.Spatiu);
    InversaPermutareAleatoare=InverseazaPermutare(PermutareAleatoare,Imagine.Spatiu);

    Pixel *CopieDupaImagine;
    CopieDupaImagine=CopiazaImagine(Imagine);

    AplicaInversaLaRegulaDeSubstitutie(Cheia2,CopieDupaImagine,Imagine,NumereAleatoare);

    Pixel* PixeliPermutati;
    PixeliPermutati=GenereazaPixeliPermutati(InversaPermutareAleatoare,Imagine);

    CopiazaPermutareaInImagine(PixeliPermutati,Imagine);

    FaBitmapDinVector(FisierDecriptat,Imagine);

    GolesteImagine(Imagine);
    free(NumereAleatoare);
    free(PermutareAleatoare);
    free(InversaPermutareAleatoare);
    free(CopieDupaImagine);
    free(PixeliPermutati);

}

void TestulChiPatrat(char *FisierBitmap){

    BitmapPrelucrat Imagine;
    Imagine=FaVectorDinBitmap(FisierBitmap);

    double FrecventaEstimata=Imagine.Spatiu/256.0;
    unsigned *FrecventaAlbastru,*FrecventaVerde,*FrecventaRosu;
    FrecventaAlbastru=(unsigned*)calloc(256,sizeof(unsigned));
    FrecventaVerde=(unsigned*)calloc(256,sizeof(unsigned));
    FrecventaRosu=(unsigned*)calloc(256,sizeof(unsigned));

    for(int i=0;i<Imagine.Spatiu;i++){

        ++FrecventaAlbastru[Imagine.Vector[i].Albastru];
        ++FrecventaVerde[Imagine.Vector[i].Verde];
        ++FrecventaRosu[Imagine.Vector[i].Rosu];

    }

    double ChiAlbastru=0,ChiVerde=0,ChiRosu=0;

    for(int i=0;i<256;i++){

        ChiAlbastru+=(FrecventaAlbastru[i]-FrecventaEstimata)*(FrecventaAlbastru[i]-FrecventaEstimata)/FrecventaEstimata;
        ChiVerde+=(FrecventaVerde[i]-FrecventaEstimata)*(FrecventaVerde[i]-FrecventaEstimata)/FrecventaEstimata;
        ChiRosu+=(FrecventaRosu[i]-FrecventaEstimata)*(FrecventaRosu[i]-FrecventaEstimata)/FrecventaEstimata;

    }

    printf("Testul ChiPatrat pe fiecare canal de culoare pentru %s:\n",FisierBitmap);
    printf("Rosu: %.2lf\nGalben: %.2lf\nAlbastru: %.2lf\n\n",ChiRosu,ChiVerde,ChiAlbastru);

}

