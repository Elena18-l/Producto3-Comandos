//Elena López Estremera
//Producto 3. Automatización básica de la administracion de un sistema operativo
//libreria de red.c
#pragma once
#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

void cambiar_dns(const char* adaptador, const char* dns);
void mostrar_adaptadores();
void mostrar_dns_actual(const char* adaptador);

#endif // NETWORK_UTILS_H
