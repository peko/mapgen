#pragma once

int  engine_init();
void engine_start();
void engine_deinit();

void (*engine_update)(float dt);
void (*engine_render)(float dt);
void (*engine_key)(int key, int action);
