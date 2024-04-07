#include <stdio.h>


// TornákMozgatása(n, forrás, cél, segédtorony):
//     Ha n = 1:
//         Kiírjuk az első korongot forrásról célra
//     Különben:
//         TornákMozgatása(n - 1, forrás, segédtorony, cél)
//         Kiírjuk a n-edik korongot forrástól célra
//         TornákMozgatása(n - 1, segédtorony, cél, forrás)

void moveDisk(int db, int a, int b, int c){
    if (db==1)
    {   
        a= a-1;
        b= b+1;
        printf("A      B      C\n\n");
        printf("%d      %d      %d\n", a,b,c);
        printf("==================\n");


    }
    else{
        printf("A      B      C\n\n");
        printf("%d      %d      %d\n", a,b,c);
        printf("==================\n");
        
        moveDisk(db-1, a, c, b);
        a= a-1;
        b= b+1;
        moveDisk(db-1, c, b, a);
        
    }
    
}

int main() {
    printf("Hello, World!\n");
    moveDisk(3,3,0,0);
    return 0;
}

