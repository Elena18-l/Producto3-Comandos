//Elena López Estremera
//Producto 3. Automatización básica de la administracion de un sistema operativo
//libreria de dns.c
#pragma once
#ifndef DNS_UTILS_H
#define DNS_UTILS_H

#define MAX_IP_LENGTH 16
#define MAX_DNS 100

typedef struct {
    char ip[MAX_IP_LENGTH];
    double tiempo_respuesta;
    int saltos;
} DnsServer;

int medir_latencia_tracert(const char* dns);
int verificar_dns(const char dns_ips[][20], int count, char dns_validos[][20]);

#endif // DNS_UTILS_H
