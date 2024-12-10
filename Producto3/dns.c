//Elena López Estremera
//Producto 3. Automatización básica de la administracion de un sistema operativo
#define _CRT_SECURE_NO_WARNINGS
#include "dns.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//medir la velocidad del dns con tracert
#include <stdio.h>
#include <stdlib.h>

int medir_latencia_tracert(const char* dns) {
    char comando[256];
    snprintf(comando, sizeof(comando), "ping -n 1 %s", dns); // Usar ping para medir latencia

    FILE* resultado = _popen(comando, "r");
    if (!resultado) {
        perror("No se pudo ejecutar el comando ping");
        return -1; // Error en la ejecución del comando
    }

    char linea[256];
    int latencia = -1;
    while (fgets(linea, sizeof(linea), resultado)) {
        // Buscar la línea que contiene el tiempo de respuesta
        if (strstr(linea, "tiempo=")) {
            sscanf(linea, "%*[^=]=%dms", &latencia);
            break;
        }
    }
    _pclose(resultado);

    // Si no se pudo encontrar una línea válida con "tiempo=", devolver -1
    if (latencia == -1) {
        printf("No se pudo medir la latencia para %s.\n", dns);
    }

    return latencia;
}

//contar los saltos con tracert
int contar_saltos(const char* dns) {
    char comando[256];
    snprintf(comando, sizeof(comando), "tracert -d -h 30 %s", dns); // Hasta 30 saltos

    FILE* pipe = _popen(comando, "r");
    if (!pipe) {
        perror("Error al ejecutar tracert");
        return -1; // Indicar error
    }

    char linea[256];
    int saltos = 0;

    while (fgets(linea, sizeof(linea), pipe)) {
        if (strstr(linea, "ms")) {
            saltos++;
        }
    }

    _pclose(pipe);

    if (saltos == 0) {
        printf("No se pudo determinar el número de saltos para %s.\n", dns);
        return -1;
    }

    return saltos;
}

//ping a dns para verificar si funciona
int verificar_dns(const char dns_ips[][20], int count, char dns_validos[][20]) {
    int valid_count = 0;
    for (int i = 0; i < count; i++) {
        char comando[100];
        snprintf(comando, sizeof(comando), "ping -n 1 %s > nul", dns_ips[i]);

        if (system(comando) == 0) {
            strncpy(dns_validos[valid_count], dns_ips[i], 19);
            dns_validos[valid_count][19] = '\0';
            valid_count++;
        }
    }
    return valid_count;
}
