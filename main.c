#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

#define MAX_LIBROS 100
#define MAX_PAGINAS 10
#define MAX_TEXTO 300

// Colores ANSI
#define COLOR_ROJO     "\033[1;31m"
#define COLOR_VERDE    "\033[1;32m"
#define COLOR_AMARILLO "\033[1;33m"
#define COLOR_AZUL     "\033[1;34m"
#define COLOR_CYAN     "\033[1;36m"
#define COLOR_RESET    "\033[0m"

typedef struct {
    int id;
    char titulo[50];
    char autor[50];
    int anio;
    char paginas[MAX_PAGINAS][MAX_TEXTO];
    int cantidadPaginas;
} Libro;

// Funciones principales
void altaLibro(Libro libros[], int* cantidad, int* idActual);
void bajaLibro(Libro libros[], int* cantidad);
void modificarLibro(Libro libros[], int cantidad);
void buscarLibro(Libro libros[], int cantidad);
void mostrarLibros(Libro libros[], int cantidad);
void editarContenidoLibro(Libro libros[], int cantidad);

// Utilidades
void limpiarPantalla();
void pausa();
int leerAnioValido(const char* mensaje);

int main() {
    Libro libros[MAX_LIBROS];
    int cantidad = 0, idActual = 1, opcion;

    // === Libro Tutorial por Defecto ===
    Libro tutorial;
    tutorial.id = idActual++;
    strcpy(tutorial.titulo, "Tutorial del sistema");
    strcpy(tutorial.autor, "Biblioteca32");
    tutorial.anio = 2025;
    tutorial.cantidadPaginas = 2;

    strcpy(tutorial.paginas[0],
        "Bienvenido al sistema de Biblioteca32.\n"
        "Usa la opción 1 para agregar un libro nuevo.\n"
        "Usa la opción 6 para escribir o leer páginas del libro.\n"
        "Solo puedes tener hasta 10 páginas por libro.\n"
        "El año solo acepta números (sin letras, símbolos ni espacios).\n"
        "Este tutorial siempre aparece al iniciar.\n"
    );

    strcpy(tutorial.paginas[1],
        "Ejemplo de contenido:\n"
        "Puedes escribir notas personales, cuentos cortos, o incluso\n"
        "usar este sistema como una libreta digital.\n"
        "Cada página puede contener hasta 300 caracteres.\n"
    );

    libros[cantidad++] = tutorial;
    // === Fin del libro tutorial ===

    do {
        limpiarPantalla();
        printf(COLOR_CYAN "\n========= CATALOGO DE LIBROS =========\n" COLOR_RESET);
        printf(COLOR_VERDE "1. Agregar libro\n" COLOR_RESET);
        printf(COLOR_ROJO  "2. Eliminar libro\n" COLOR_RESET);
        printf(COLOR_AMARILLO "3. Modificar libro\n" COLOR_RESET);
        printf(COLOR_AZUL "4. Buscar libro\n" COLOR_RESET);
        printf(COLOR_CYAN "5. Mostrar todos los libros\n" COLOR_RESET);
        printf(COLOR_VERDE "6. Editar, o leer contenido de libro\n" COLOR_RESET);
        printf(COLOR_ROJO "0. Salir del Programa\n" COLOR_RESET);
        printf("--------------------------------------\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        getchar(); // limpiar buffer

        switch(opcion) {
            case 1: altaLibro(libros, &cantidad, &idActual); break;
            case 2: bajaLibro(libros, &cantidad); break;
            case 3: modificarLibro(libros, cantidad); break;
            case 4: buscarLibro(libros, cantidad); break;
            case 5: mostrarLibros(libros, cantidad); break;
            case 6: editarContenidoLibro(libros, cantidad); break;
            case 0: printf(COLOR_AMARILLO "Saliendo del programa...\n" COLOR_RESET); break;
            default: printf(COLOR_ROJO "Opción inválida.\n" COLOR_RESET); pausa(); break;
        }

    } while(opcion != 0);

    return 0;
}

// Funciones generales
void limpiarPantalla() {
    system(CLEAR);
}

void pausa() {
    printf(COLOR_CYAN "\nPresione ENTER para continuar..." COLOR_RESET);
    getchar();
}

// Validación del año
int leerAnioValido(const char* mensaje) {
    char buffer[20];
    int anio = 0, valido = 0;

    do {
        printf("%s", mensaje);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        valido = 1;
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (!isdigit(buffer[i])) {
                valido = 0;
                break;
            }
        }

        if (valido && strlen(buffer) > 0)
            anio = atoi(buffer);
        else
            printf(COLOR_ROJO "Error: Solo se permiten números sin espacios, letras ni símbolos.\n" COLOR_RESET);

    } while (!valido);

    return anio;
}

// Alta
void altaLibro(Libro libros[], int* cantidad, int* idActual) {
    if (*cantidad >= MAX_LIBROS) {
        printf(COLOR_ROJO "No se pueden agregar más libros.\n" COLOR_RESET);
        pausa();
        return;
    }

    Libro nuevo;
    nuevo.id = (*idActual)++;
    nuevo.cantidadPaginas = 0;

    printf(COLOR_VERDE "\n--- Agregar Libro ---\n" COLOR_RESET);
    printf("Titulo: ");
    fgets(nuevo.titulo, sizeof(nuevo.titulo), stdin);
    nuevo.titulo[strcspn(nuevo.titulo, "\n")] = '\0';

    printf("Autor: ");
    fgets(nuevo.autor, sizeof(nuevo.autor), stdin);
    nuevo.autor[strcspn(nuevo.autor, "\n")] = '\0';

    nuevo.anio = leerAnioValido("Año: ");

    libros[*cantidad] = nuevo;
    (*cantidad)++;

    printf(COLOR_VERDE "Libro agregado con ID: %d\n" COLOR_RESET, nuevo.id);
    pausa();
}

// Baja
void bajaLibro(Libro libros[], int* cantidad) {
    int id, encontrado = 0;
    printf("Ingrese ID del libro a eliminar: ");
    scanf("%d", &id);
    getchar();

    for(int i = 0; i < *cantidad; i++) {
        if(libros[i].id == id) {
            for(int j = i; j < (*cantidad)-1; j++)
                libros[j] = libros[j+1];
            (*cantidad)--;
            encontrado = 1;
            printf(COLOR_VERDE "Libro eliminado.\n" COLOR_RESET);
            break;
        }
    }

    if (!encontrado)
        printf(COLOR_ROJO "Libro no encontrado.\n" COLOR_RESET);

    pausa();
}

// Modificar
void modificarLibro(Libro libros[], int cantidad) {
    int id;
    printf("Ingrese ID del libro a modificar: ");
    scanf("%d", &id);
    getchar();

    for(int i = 0; i < cantidad; i++) {
        if(libros[i].id == id) {
            printf("Nuevo titulo: ");
            fgets(libros[i].titulo, sizeof(libros[i].titulo), stdin);
            libros[i].titulo[strcspn(libros[i].titulo, "\n")] = '\0';

            printf("Nuevo autor: ");
            fgets(libros[i].autor, sizeof(libros[i].autor), stdin);
            libros[i].autor[strcspn(libros[i].autor, "\n")] = '\0';

            libros[i].anio = leerAnioValido("Nuevo año: ");

            printf(COLOR_VERDE "Libro actualizado.\n" COLOR_RESET);
            pausa();
            return;
        }
    }

    printf(COLOR_ROJO "Libro no encontrado.\n" COLOR_RESET);
    pausa();
}

// Buscar
void buscarLibro(Libro libros[], int cantidad) {
    char titulo[50];
    printf("Ingrese titulo a buscar: ");
    fgets(titulo, sizeof(titulo), stdin);
    titulo[strcspn(titulo, "\n")] = '\0';

    int encontrados = 0;
    for(int i = 0; i < cantidad; i++) {
        if(strstr(libros[i].titulo, titulo) != NULL) {
            printf(COLOR_CYAN "\nID: %d\nTitulo: %s\nAutor: %s\nAño: %d\n" COLOR_RESET,
                   libros[i].id, libros[i].titulo, libros[i].autor, libros[i].anio);
            encontrados++;
        }
    }

    if(encontrados == 0)
        printf(COLOR_ROJO "No se encontraron libros con ese título.\n" COLOR_RESET);

    pausa();
}

// Mostrar todos
void mostrarLibros(Libro libros[], int cantidad) {
    if(cantidad == 0) {
        printf(COLOR_ROJO "No hay libros cargados.\n" COLOR_RESET);
    } else {
        printf(COLOR_CYAN "\nLista de libros:\n" COLOR_RESET);
        for(int i = 0; i < cantidad; i++) {
            printf(COLOR_VERDE "\nID: %d\n" COLOR_RESET, libros[i].id);
            printf(COLOR_AMARILLO "Titulo: %s\nAutor: %s\nAño: %d\nPáginas: %d\n" COLOR_RESET,
                   libros[i].titulo, libros[i].autor, libros[i].anio, libros[i].cantidadPaginas);
        }
    }
    pausa();
}

// Editar o leer contenido del libro
void editarContenidoLibro(Libro libros[], int cantidad) {
    int id;
    printf("Ingrese ID del libro para editar/leer su contenido: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < cantidad; i++) {
        if (libros[i].id == id) {
            int opcion, pagina;
            do {
                printf(COLOR_CYAN "\nContenido del libro \"%s\"\n" COLOR_RESET, libros[i].titulo);
                printf("1. Escribir página\n");
                printf("2. Leer página\n");
                printf("3. Volver\n");
                printf("Seleccione una opción: ");
                scanf("%d", &opcion);
                getchar();

                if (opcion == 1) {
                    printf("Número de página (1 a %d): ", MAX_PAGINAS);
                    scanf("%d", &pagina);
                    getchar();

                    if (pagina < 1 || pagina > MAX_PAGINAS) {
                        printf(COLOR_ROJO "Página inválida.\n" COLOR_RESET);
                        pausa();
                        continue;
                    }

                    printf("Escribe el texto de la página %d (máx %d caracteres):\n", pagina, MAX_TEXTO - 1);
                    fgets(libros[i].paginas[pagina - 1], MAX_TEXTO, stdin);
                    libros[i].paginas[pagina - 1][strcspn(libros[i].paginas[pagina - 1], "\n")] = '\0';

                    if (pagina > libros[i].cantidadPaginas)
                        libros[i].cantidadPaginas = pagina;

                    printf(COLOR_VERDE "Página %d guardada.\n" COLOR_RESET, pagina);
                    pausa();
                }

                else if (opcion == 2) {
                    printf("Número de página a leer: ");
                    scanf("%d", &pagina);
                    getchar();

                    if (pagina < 1 || pagina > libros[i].cantidadPaginas) {
                        printf(COLOR_ROJO "Página no existe.\n" COLOR_RESET);
                    } else {
                        printf(COLOR_AMARILLO "\nPágina %d:\n" COLOR_RESET, pagina);
                        printf("%s\n", libros[i].paginas[pagina - 1]);
                    }
                    pausa();
                }

            } while (opcion != 3);

            return;
        }
    }

    printf(COLOR_ROJO "Libro no encontrado.\n" COLOR_RESET);
    pausa();
}
