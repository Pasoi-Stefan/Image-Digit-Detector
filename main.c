#include <stdio.h>
#include <stdlib.h>

#include "bmpdef.h"
#include "crypto.h"
#include "pattmatch.h"

int main(){

    FILE *CaiImagine=fopen("cai_imagini_criptare1.txt","r");

    char *FisierInitial,*FisierCriptat,*FisierDecriptat,*FisierChei;
    FisierInitial=(char*)malloc(105);
    FisierCriptat=(char*)malloc(105);
    FisierDecriptat=(char*)malloc(105);
    FisierChei=(char*)malloc(105);

    fscanf(CaiImagine,"%104s",FisierInitial);
    fscanf(CaiImagine,"%104s",FisierCriptat);
    fscanf(CaiImagine,"%104s",FisierDecriptat);
    fscanf(CaiImagine,"%104s",FisierChei);

    fclose(CaiImagine);

    CripteazaImagine(FisierInitial,FisierCriptat,FisierChei);

    DecripteazaImagine(FisierDecriptat,FisierCriptat,FisierChei);

    TestulChiPatrat(FisierInitial);

    TestulChiPatrat(FisierCriptat);

    CaiImagine=fopen("cai_imagini_pattern_match.txt","r");

    char *FisierBitmap,**FisierSablon,*FisierDetectie;
    FisierBitmap=(char*)malloc(105);
    FisierSablon=(char**)malloc(10*sizeof(char*));
    for(int i=0;i<10;i++)
        FisierSablon[i]=(char*)malloc(105);
    FisierDetectie=(char*)malloc(105);

    fscanf(CaiImagine,"%104s",FisierBitmap);
    for(int i=0;i<10;i++)
        fscanf(CaiImagine,"%104s",FisierSablon[i]);
    fscanf(CaiImagine,"%104s",FisierDetectie);

    fclose(CaiImagine);

    BitmapPrelucrat Imagine;

    Imagine=CreezaImagineCuDetectii(FisierBitmap,FisierSablon);

    FaBitmapDinMatrice(FisierDetectie,Imagine);

    CaiImagine=fopen("cai_imagini_criptare2.txt","r");

    fscanf(CaiImagine,"%104s",FisierInitial);
    fscanf(CaiImagine,"%104s",FisierCriptat);
    fscanf(CaiImagine,"%104s",FisierDecriptat);
    fscanf(CaiImagine,"%104s",FisierChei);

    fclose(CaiImagine);

    CripteazaImagine(FisierInitial,FisierCriptat,FisierChei);

    DecripteazaImagine(FisierDecriptat,FisierCriptat,FisierChei);

    TestulChiPatrat(FisierInitial);

    TestulChiPatrat(FisierCriptat);

    GolesteImagine(Imagine);

    free(FisierInitial);
    free(FisierCriptat);
    free(FisierDecriptat);
    free(FisierChei);
    free(FisierBitmap);
    free(FisierDetectie);
    for(int i=0;i<10;i++)
        free(FisierSablon[i]);
    free(FisierSablon);

}
