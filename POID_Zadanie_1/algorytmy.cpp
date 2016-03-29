#include <stdio.h>

//Funkcja pomocnicza do mediany
int partition(int c[], int a, int b)
{
    int e,tmp;
    e=c[a];

    while (a<b)
    {
        while ((a<b) && (c[b]>=e))
            b--;
        while ((a<b) && (c[a]<e))
            a++;
        if (a<b)
        {
            tmp=c[a];
            c[a]=c[b];
            c[b]=tmp;
        }
    }
    return a;
}

int median(int array[], int arraySize)
{
    //Zmienne podstawowe
    int myMedian = arraySize / 2;
    int myResult;

    //Zmienne pomocnicze
    int i = 0,
            j = arraySize-1,
            k;

    //Algorytm Hoare'a
    arraySize = myMedian;
    while (i != j)
    {
        k=partition(array,i,j);
        k=k-i+1;
        if (k>=myMedian)
            j=i+k-1;
        if (k<myMedian)
        {
            myMedian-=k;
            i+=k;
        }
    }
    myResult = (int)((array[i]+array[i+1])/2.0);
    return myResult;
}
