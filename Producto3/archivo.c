//Elena López Estremera
//Producto 3. Automatización básica de la administracion de un sistema operativo
#define _CRT_SECURE_NO_WARNINGS
#include "archivo.h"
#include <string.h>
#define MAX_DNS 100 
// Limpiar la cadena (eliminar saltos de línea y espacios extras), reutilización de la función del prod anterior
void limpiar_string(char* str) {
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        len--;
    }
}
// lectura del archivo DNSips.txt
int leer_dns_de_archivo(const char* ruta_archivo, char dns_ips[][20], int max_dns) {
    FILE* file = fopen(ruta_archivo, "r");
    if (!file) {
        perror("No se pudo abrir el archivo DNS");
        return -1;
    }

    char linea[256];
    int count = 0;
    while (fgets(linea, sizeof(linea), file) && count < max_dns) {
        limpiar_string(linea);
        strncpy(dns_ips[count], linea, 19);
        dns_ips[count][19] = '\0';
        count++;
    }
    fclose(file);
    return count;
}
//guardado de DNS que realiza bien el ping
void verificar_y_guardar_dns(const char* archivoEntrada, const char* archivoTemporal) {
    char dns_ips[MAX_DNS][20];
    char dns_validos[MAX_DNS][20];
    int total_dns = leer_dns_de_archivo(archivoEntrada, dns_ips, MAX_DNS);

    if (total_dns > 0) {
        int validos = verificar_dns(dns_ips, total_dns, dns_validos);
        FILE* temp = fopen(archivoTemporal, "w");
        for (int i = 0; i < validos; i++) {
            fprintf(temp, "%s\n", dns_validos[i]);
        }
        fclose(temp);
    }
}
