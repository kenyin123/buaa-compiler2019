void main()
{
    int i,j,a[100],temp,min;
    i = 0;
    while(i<100){
        a[i] = 230 - i;
        i = i + 1;
    }
    i = 0;
    while(i<100){
        min = i;
        j = 0;
        while(j<100){
            if(a[min]>a[j]) min=j;
            j = j + 1;
        }
        if(min!=i)
        {
            temp=a[min];
            a[min]=a[i];
            a[i]=temp;
        }
        i = i + 1;
    }
    i = 0;
    while(i<100){
        printf("i:",i);
        printf("a[i]:",a[i]);
        i = i + 1;
    }
}