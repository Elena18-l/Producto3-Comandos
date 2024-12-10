//Elena López Estremera
//Producto 3. Automatización básica de la administracion de un sistema operativo
//libreria de archivo.c
#pragma once
#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>

int leer_dns_de_archivo(const char* ruta_archivo, char dns_ips[][20], int max_dns);
void limpiar_string(char* str);
void verificar_y_guardar_dns(const char* archivoEntrada, const char* archivoTemporal);

#endif // FILE_UTILS_H
