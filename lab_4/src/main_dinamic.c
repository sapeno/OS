#include "libs.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>

typedef int (*pgcd_f)(int, int);
typedef float (*parea_f)(float, float);

static pgcd_f gcd_f = NULL;
static parea_f area_f = NULL;

static void *lib_handle = NULL;
static int cur_lib = 1;

int load_lib(int lib_num) {
    if (lib_handle != NULL) {
        dlclose(lib_handle);
        lib_handle = NULL;
    }

    const char *lib_path = (lib_num == 1) ? "./liba1.so" : "./liba2.so";

    lib_handle = dlopen(lib_path, RTLD_NOW);
    if (!lib_handle) {
        const char msg[] = "Ошибка dlopen\n";
        write(STDERR_FILENO, msg, sizeof(msg) - 1);
        return 0;
    }

    gcd_f = (pgcd_f)dlsym(lib_handle, "gcd");
    area_f = (parea_f)dlsym(lib_handle, "area");

    if (!gcd_f || !area_f) {
        const char msg[] = "Ошибка dlsym\n";
        write(STDERR_FILENO, msg, sizeof(msg) - 1);
        dlclose(lib_handle);
        lib_handle = NULL;
        return 0;
    }

    cur_lib = lib_num;
    char buf[1024];
    int len = snprintf(buf, sizeof(buf)-1, "Библиотека %d (%s)\n", cur_lib, lib_path);
    buf[len] = '\0';
    write(STDOUT_FILENO, buf, len);

    return 1;

}

int main(void) {
    if (!load_lib(1)) return EXIT_FAILURE;

    printf("Выберите функцию, 1 - gcd, 2 - area или введите 0, чтобы переключить библиотеку\n");

    int choice;
    while (scanf("%d", &choice) == 1) {
        if (choice == 0) {
            int next_lib = (cur_lib == 1) ? 2 : 1;
            if (!load_lib(next_lib)) return EXIT_FAILURE;
        } else if (choice == 1) {
            int a, b;
            printf("Введите два целых числа для поиска НОД\n");
            if (scanf("%d %d", &a, &b) != 2){
                const char msg[] = "Ошибка чтения\n";
                write(STDERR_FILENO, msg, sizeof(msg) - 1);
                return EXIT_FAILURE;
            }
            int result = gcd_f(a, b);
            char buf[1024];
            int len = snprintf(buf, sizeof(buf)-1, "НОД для %d и %d: %d\n", a, b, result);
            buf[len] = '\0';
            write(STDOUT_FILENO, buf, len);
        } else if (choice == 2) {
            float a, b;
            const char *name = (cur_lib == 2) ? "треугольника" : "прямоугольника";
            printf("Введите два вещественных числа для поиска площади %s\n", name);
            if (scanf("%f %f", &a, &b) != 2 || a < 0 || b < 0){
                const char msg[] = "Ошибка невалидные аргументы\n";
                write(STDERR_FILENO, msg, sizeof(msg) - 1);
                return EXIT_FAILURE;
            }
            float result = area_f(a, b);
            char buf[1024];
            int len = snprintf(buf, sizeof(buf)-1, "Площадь для %s со сторонами %g и %g: %g\n", name, a, b, result);
            buf[len] = '\0';
            write(STDOUT_FILENO, buf, len);
        } else {
            const char msg[] = "Неизвестная команда\n";
            write(STDOUT_FILENO, msg, sizeof(msg) - 1);
        }
        printf("\nВыберите функцию, 1 - gcd, 2 - area или введите 0, чтобы переключить библиотеку\n");


    }
    if (lib_handle != NULL) dlclose(lib_handle);
    

    return 0;
}