#pragma once
#ifndef DNS_UTILS_H
#define DNS_UTILS_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definiciones generales
#define _CRT_SECURE_NO_WARNINGS
#define MAX_LINE_LENGHT 100
#define MAX_IP_LENGTH 16
#define TEMP_FILE "dns_temporal.txt"

// Estructura para almacenar información de los servidores DNS
typedef struct {
    char ip[MAX_IP_LENGTH];  // Dirección IP del servidor DNS
    double tiempo_respuesta;  // Tiempo de respuesta del DNS (en segundos)
    int saltos;               // Número de saltos para la conexión
} DnsServer;

void obtener_ruta_documentos(char* ruta_documentos, size_t tamano, const char* nombre_archivo) {
    char* userprofile = getenv("USERPROFILE");
    if (userprofile) {
        snprintf(ruta_documentos, tamano, "%s\\Documents\\%s", userprofile, nombre_archivo);
    }
    else {
        snprintf(ruta_documentos, tamano, "%s", nombre_archivo); // Ruta relativa por defecto si no se encuentra USERPROFILE
    }
}
void limpiar_string(char* str) {
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        len--;
    }
}
void hacerPing(const char* direccionIP, const char* archivoSalida) {
    char command[255] = { 0 };
    char buffer[1024] = { 0 };
    FILE* pipe;

    // Crear el comando de ping
    sprintf(command, "ping -n 1 %s", direccionIP);

    // Ejecutar el comando y leer la salida
    pipe = _popen(command, "r");
    if (!pipe) {
        perror("No se pudo ejecutar el comando ping");
        return;
    }

    // Leer la salida del comando
    int pingExitoso = 0;
    while (fgets(buffer, sizeof(buffer), pipe)) {
        if (strstr(buffer, "TTL=")) {
            pingExitoso = 1;
        }
    }
    _pclose(pipe);

    // Abrir el archivo de salida para insertar los resultados
    FILE* fp = fopen(archivoSalida, "a");
    if (fp == NULL) {
        perror("No se pudo abrir el archivo de salida");
        return;
    }

    // Escribir el resultado en el archivo
    if (pingExitoso) {
        fprintf(fp, "Ping exitoso a %s\n", direccionIP);
    }
    else {
        fprintf(fp, "Ping fallido a %s\n", direccionIP);
    }

    fclose(fp);
}
void verificarIPs() {
    char archivoSalida[MAX_LINE_LENGHT];

    // Solicitar el archivo de salida al usuario
    printf("Ingrese el nombre del archivo para guardar los resultados (se guardará en Documentos): ");
    scanf("%s", archivoSalida);

    char rutaArchivoSalida[260];
    obtener_ruta_documentos(rutaArchivoSalida, sizeof(rutaArchivoSalida), archivoSalida);

    // Ruta dinámica para el archivo de entrada en Documentos
    char archivoEntrada[260];
    obtener_ruta_documentos(archivoEntrada, sizeof(archivoEntrada), "archivoips.txt");

    // Abrir el archivo de entrada
    FILE* fp = fopen(archivoEntrada, "r");
    if (fp == NULL) {
        perror("No se pudo abrir el archivo de entrada");
        exit(1);
    }

    // Leer las IPs línea por línea y hacer ping
    char direccionIP[MAX_LINE_LENGHT];
    while (fgets(direccionIP, MAX_LINE_LENGHT, fp)) {
        // Elimina el salto de línea al final de cada IP
        direccionIP[strcspn(direccionIP, "\n")] = '\0';
        hacerPing(direccionIP, rutaArchivoSalida);
    }

    fclose(fp);
}
int listar_adaptadores(const char* ruta_archivo, char adaptadores[][256], size_t max_adaptadores) {
    FILE* fp = fopen(ruta_archivo, "r");
    if (!fp) {
        perror("No se pudo abrir el archivo temporal");
        return 0;
    }

    char linea[256];
    size_t count = 0;

    while (fgets(linea, sizeof(linea), fp)) {
        if (strstr(linea, "Adaptador") || strstr(linea, "adapter")) {
            if (count < max_adaptadores) {
                limpiar_string(linea);  // Usamos limpiar_string para eliminar saltos de línea
                strcpy(adaptadores[count], linea);
                printf("[%zu] %s\n", count + 1, adaptadores[count]);
                count++;
            }
        }
    }

    fclose(fp);
    return count;  // Número de adaptadores encontrados
}
void mostrar_adaptadores() {
    char ruta_temp[MAX_PATH];
    obtener_ruta_documentos(ruta_temp, MAX_PATH, "temp_ipconfig.txt");  // Usa la función de obtener la ruta
    printf("Ruta temporal utilizada: %s\n", ruta_temp);

    // Ejecutar ipconfig y guardar en el archivo temporal
    ejecutar_comando_y_guardar("ipconfig", ruta_temp);

    // Listar adaptadores
    char adaptadores[10][256];
    int num_adaptadores = listar_adaptadores(ruta_temp, adaptadores, 10);

    if (num_adaptadores == 0) {
        printf("No se detectaron adaptadores de red.\n");
    }
    else {
        printf("Se encontraron %d adaptadores de red.\n", num_adaptadores);
    }

    // Eliminar el archivo temporal
    remove(ruta_temp);
}
void ejecutar_comando_y_guardar(const char* comando, const char* ruta_salida) {
    char comando_final[512];
    snprintf(comando_final, sizeof(comando_final), "%s > \"%s\"", comando, ruta_salida);
    int resultado = system(comando_final);
    if (resultado != 0) {
        fprintf(stderr, "Error al ejecutar el comando: %s\n", comando);
    }
}
int leer_dns_de_archivo(const char* ruta_archivo, char dns_ips[][20], int max_dns) {
    FILE* file = fopen(ruta_archivo, "r");
    if (!file) {
        perror("No se pudo abrir el archivo DNS");
        return -1;
    }

    char linea[256];
    int count = 0;
    while (fgets(linea, sizeof(linea), file) && count < max_dns) {
        limpiar_string(linea);  // Elimina saltos de línea
        strncpy(dns_ips[count], linea, 19);
        dns_ips[count][19] = '\0';
        count++;
    }
    fclose(file);
    return count;
}
int verificar_dns(const char dns_ips[][20], int count, char dns_validos[][20]) {
    int valid_count = 0;
    for (int i = 0; i < count; i++) {
        char comando[100];
        snprintf(comando, sizeof(comando), "ping -n 1 %s > nul", dns_ips[i]);

        if (system(comando) == 0) {  // Éxito si el comando devuelve 0
            strncpy(dns_validos[valid_count], dns_ips[i], 19);
            dns_validos[valid_count][19] = '\0';
            valid_count++;
        }
    }
    return valid_count;
}
void cambiar_dns(const char* adaptador, const char* dns) {
    char comando[256];
    snprintf(comando, sizeof(comando),
        "netsh interface ip set dns name=\"%s\" static %s",
        adaptador, dns);

    if (system(comando) == 0) {
        printf("DNS cambiado correctamente a %s para el adaptador %s.\n", dns, adaptador);
    }
    else {
        printf("Error al cambiar el DNS para el adaptador %s.\n", adaptador);
    }
}
int medir_latencia_tracert(const char* dns) {
    char comando[256];
    snprintf(comando, sizeof(comando), "tracert -d -h 1 %s", dns);

    FILE* pipe = _popen(comando, "r");
    if (!pipe) {
        perror("Error al ejecutar tracert");
        return -1;
    }

    char linea[256];
    int tiempo = -1;
    while (fgets(linea, sizeof(linea), pipe)) {
        if (strstr(linea, "ms")) {
            sscanf(linea, "%*s %*s %*s %dms", &tiempo);  // Extrae tiempo en ms
            break;
        }
    }

    _pclose(pipe);
    return tiempo;
}



#endif // DNS_UTILS_H