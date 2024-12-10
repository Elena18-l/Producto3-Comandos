//Elena López Estremera
//Producto 3. Automatización básica de la administracion de un sistema operativo
#define _CRT_SECURE_NO_WARNINGS
#include "red.h"
#include <stdio.h>
#include <stdlib.h>

//cambiar DNS con netsh
void cambiar_dns(const char* adaptador, const char* dns) {
    char comando[256];
    snprintf(comando, sizeof(comando),
        "netsh interface ip set dns name=\"%s\" static %s", adaptador, dns);

    if (system(comando) == 0) {
        printf("DNS cambiado correctamente a %s para el adaptador %s.\n", dns, adaptador);
    }
    else {
        printf("Error al cambiar el DNS para el adaptador %s.\n", adaptador);
    }
}
//mostrar adaptadores red
void mostrar_adaptadores() {
    system("netsh interface show interface");
}
//mostrar dns actual
void mostrar_dns_actual(const char* adaptador) {
    char comando[256];
    snprintf(comando, sizeof(comando), "netsh interface ip show dnsservers \"%s\"", adaptador);
    system(comando);
}
