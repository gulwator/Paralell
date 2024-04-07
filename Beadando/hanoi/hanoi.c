#include <stdio.h>

void hanoi(int n, char kezdo, char kozep, char cel);

int main() {
    int n;
    printf("Add meg a korongok számát: ");
    scanf("%d", &n);
    hanoi(n, 'A', 'B', 'C');
    return 0;
}

void hanoi(int n, char kezdo, char kozep, char cel) {
    if (n == 1) {
        printf("Korong 1 mozgatása %c-ről %c-re\n", kezdo, cel);
        return;
    }
    hanoi(n - 1, kezdo, cel, kozep);
    printf("Korong %d mozgatása %c-ről %c-re\n", n, kezdo, cel);
    hanoi(n - 1, kozep, kezdo, cel);
}