#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#ifndef CHACHA_H
#define CHACHA_H

uint32_t rotl(uint32_t* byte, uint8_t amt);
uint32_t conv(uint32_t word);
void qr(uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d);
void print_matrix(uint32_t* a);
void print_bytestream(uint32_t* keystream);
void print_charstream(unsigned char* st, size_t sz);
void load_constant(uint32_t* matrix);
void load_key();
void load_nonce();
void chacha(uint32_t* key, uint32_t* nonce, unsigned char* string, uint32_t ctr);
void chacha_stream(uint32_t* key, uint32_t* nonce, unsigned char* string, int blocks);

#endif	// CHACHA_H
