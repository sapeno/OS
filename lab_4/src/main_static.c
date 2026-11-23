#include "libs.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    printf("Выберите функцию, 1 - gcd, 2 - area\n");
    int choice;

    while (scanf("%d", &choice) == 1){
        if (choice == 1) {
            int a, b;
            printf("Введите два целых числа для поиска НОД\n");
            if (scanf("%d %d", &a, &b) != 2){
                const char msg[] = "Ошибка чтения\n";
                write(STDERR_FILENO, msg, sizeof(msg));
                return EXIT_FAILURE;
            }
            int result = gcd(a, b);
            char buf[1024];
            int len = snprintf(buf, sizeof(buf)-1, "НОД для %d и %d: %d\n", a, b, result);
            buf[len] = '\0';
            write(STDOUT_FILENO, buf, len);
        } else if (choice == 2) {
            float a, b;
            printf("Введите два вещественных числа для поиска площади прямоугольника\n");
            if (scanf("%f %f", &a, &b) != 2 || a < 0 || b < 0){
                const char msg[] = "Ошибка невалидные аргументы\n";
                write(STDERR_FILENO, msg, sizeof(msg));
                return EXIT_FAILURE;
            }
            float result = area(a, b);
            char buf[1024];
            int len = snprintf(buf, sizeof(buf)-1, "Площадь прямоугольника для %g и %g: %g\n", a, b, result);
            buf[len] = '\0';
            write(STDOUT_FILENO, buf, len);
        }
        printf("\nВыберите функцию, 1 - gcd, 2 - area\n");
    }
    return 0;
}
