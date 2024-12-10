//Elena López Estremera
//Producto 3. Automatización básica de la administracion de un sistema operativo
#define _CRT_SECURE_NO_WARNINGS
#include "dns.h"
#include "archivo.h"
#include "red.h"
#include <stdio.h>
#include <stdlib.h>


#define MAX_ADAPTADORES 10
#define MAX_LINE_LENGTH 256
#define MAX_DNS 100 
int main() {
    char archivo_dns[MAX_LINE_LENGTH];
    char ruta_archivo[MAX_LINE_LENGTH];
    char archivo_temporal[] = "dns_temporal.txt";
    char adaptadores[MAX_ADAPTADORES][256];
    char dns_ips[MAX_DNS][20];
    char dns_validos[MAX_DNS][20];

    //  Solicitar archivo DNS
    printf("Ingrese el nombre del archivo con las IPs de DNS (por ejemplo DNSips.txt): ");
    scanf("%s", archivo_dns);

    printf("Ingrese la ruta completa del archivo( por ejemplo la ruta a tu escritorio ): ");
    scanf("%s", ruta_archivo);

    // Mostrar contenido del archivo DNS
    printf("\nContenido de %s:\n", archivo_dns);
    char comando_mostrar[512];
    snprintf(comando_mostrar, sizeof(comando_mostrar), "type \"%s\\%s\"", ruta_archivo, archivo_dns);
    system(comando_mostrar);

    //  Listado de adaptadores de red
    printf("\nAdaptadores de red disponibles:\n");
    mostrar_adaptadores();

    // Solicitar adaptador
    char adaptador[256];
    printf("Ingrese el nombre del adaptador de red a modificar: ");
    scanf(" %[^\n]", adaptador);

    // Mostrar los DNS configurados en el adaptador
    printf("\nServidores DNS actuales en el adaptador %s:\n", adaptador);
    mostrar_dns_actual(adaptador);

    // Verificar DNS del archivo y guardar los accesibles
    char ruta_completa_dns[MAX_LINE_LENGTH];
    snprintf(ruta_completa_dns, sizeof(ruta_completa_dns), "%s\\%s", ruta_archivo, archivo_dns);

    printf("\nVerificando DNS accesibles...\n");
    verificar_y_guardar_dns(ruta_completa_dns, archivo_temporal);

    // Leer DNS accesibles desde archivo temporal
    int num_validos = leer_dns_de_archivo(archivo_temporal, dns_validos, MAX_DNS);

    if (num_validos <= 0) {
        printf("No se encontraron DNS accesibles.\n");
        return 1;
    }

    printf("\nDNS accesibles:\n");
    for (int i = 0; i < num_validos; i++) {
        printf("%s\n", dns_validos[i]);
    }

    //  Determinar el DNS más rápido
    int mejor_dns_index = 0;
    int menor_latencia = medir_latencia_tracert(dns_validos[0]);
    int menor_saltos = contar_saltos(dns_validos[0]);
    int count = 0;

    for (int i = 1; i < count; i++) {
        int latencia = medir_latencia_tracert(dns_validos[i]);
        if (latencia < menor_latencia || (latencia == menor_latencia && contar_saltos(dns_validos[i]) < menor_saltos)) {
            mejor_dns_index = i;
            menor_latencia = latencia;
            menor_saltos = contar_saltos(dns_validos[i]);
        }
    }

    printf("El DNS más rápido es: %s con %d ms y %d saltos.\n", dns_validos[mejor_dns_index], menor_latencia, menor_saltos);
    

    //  Cambiar DNS si no está configurado
    cambiar_dns(adaptador, dns_validos[mejor_dns_index]);

    printf("Proceso completado.\n");
    return 0;
}
