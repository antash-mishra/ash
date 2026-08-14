#include <stdio.h>
int main(void) {
    FILE *fp;
    int c;
    fp = fopen("text.txt", "r");
    while ((c = fgetc(fp)) != EOF) {
        printf("%c", c);
    }
    fclose(fp);
}
