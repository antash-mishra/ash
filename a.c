#include <_stdio.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct car {
    char* name;
    float price;
    int speed;
};

void increment(int *p) {
    *p = *p + 1;
}

int my_strlen(char* s) {
    int count = 0;
    while ( s[count] != '\0') {
        count++;
    }
    return count;
}

void print_goat_count(int i) {
    switch (i) {
        case 0:
            printf("You have no goats.\n");
            break;
        case 1:
        printf("You have singular goat.\n");
        break;
        case 2:
        printf("You have brace of goats.\n");
        break;
        default:
        printf("You have a bona fide plethora of goats!\n");
        break;
    }
}

void set_price(struct car *c, float price){
    // (*c).price = price;
    c->price = price;
}

void read_file_by_char(char* file_path) {
    FILE* fp = fopen(file_path, "r");
    int c;

    while((c = fgetc(fp)) != EOF) {
        printf("%c", c);
    }

    fclose(fp);
}

void read_file_by_line(char* file_path) {
    FILE *fp = fopen(file_path, "r");
    char s[1024];
    int linecount = 0;

    while (fgets(s, sizeof(s), fp) != NULL) {
        printf("%s", s);
    }
}

int main(void) {

    int a;
    int *p;
    p = &a;

    struct car nano;
    nano.name = "sumo";
    nano.speed = 12999;

    set_price(&nano, 799.9);

    char* file_path = "text.txt";
    // read file by char using getc
    // read_file_by_char(file_path);
    read_file_by_line(file_path);




    char* s = "Hello, World!";

    printf("sizeof p: %zu\n", sizeof(p));
    printf("siz of *p: %zu\n", sizeof(*p));

    for (int j=0; j<13; j++){
        printf("%c", s[j]);
    }
    printf("Length: %d\n", my_strlen(s));

    for (a=0; a<10; ) {
        printf("\na is %d and its address is %p\n", *p, (void *)&a);
        increment(p);
        printf("a is %d and its address is %p\n", a,p);
    }

    int goat_count = 2;
    print_goat_count(goat_count);
}
