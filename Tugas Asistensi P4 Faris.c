#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Struct Hitbox: posisi (x,y) di pusat, lebar dan tinggi */
typedef struct Hitbox {
    float x;      /* center x */
    float y;      /* center y */
    float w;      /* width */
    float h;      /* height */
    bool active;  /* apakah hitbox aktif */
} Hitbox;

/* Create a new hitbox (dynamic allocation) */
Hitbox *create_hitbox(float x, float y, float w, float h) {
    Hitbox *hb = (Hitbox *)malloc(sizeof(Hitbox));
    if (!hb) {
        fprintf(stderr, "Error: malloc failed\n");
        return NULL;
    }
    hb->x = x;
    hb->y = y;
    hb->w = w;
    hb->h = h;
    hb->active = true;
    return hb;
}

/* Destroy (free) a hitbox */
void destroy_hitbox(Hitbox *hb) {
    if (!hb) return;
    free(hb);
}

/* Move a hitbox by dx, dy (pointer usage) */
void move_hitbox(Hitbox *hb, float dx, float dy) {
    if (!hb || !hb->active) return;
    hb->x += dx;
    hb->y += dy;
}

/* Check AABB collision between two hitboxes (both pointers) */
bool hitbox_collide(const Hitbox *a, const Hitbox *b) {
    if (!a || !b) return false;
    if (!a->active || !b->active) return false;

    /* Convert center w/h to min/max */
    float a_min_x = a->x - a->w * 0.5f;
    float a_max_x = a->x + a->w * 0.5f;
    float a_min_y = a->y - a->h * 0.5f;
    float a_max_y = a->y + a->h * 0.5f;

    float b_min_x = b->x - b->w * 0.5f;
    float b_max_x = b->x + b->w * 0.5f;
    float b_min_y = b->y - b->h * 0.5f;
    float b_max_y = b->y + b->h * 0.5f;

    bool overlapX = (a_min_x <= b_max_x) && (a_max_x >= b_min_x);
    bool overlapY = (a_min_y <= b_max_y) && (a_max_y >= b_min_y);

    return overlapX && overlapY;
}

/* Simple utility to print hitbox info */
void print_hitbox(const Hitbox *hb, const char *name) {
    if (!hb) return;
    printf("%s: pos=(%.2f, %.2f) size=(%.2f x %.2f) active=%s\n",
           name, hb->x, hb->y, hb->w, hb->h, hb->active ? "true" : "false");
}

/* Example: array of pointers to hitboxes, moving and checking collisions */
int main(void) {
    /* Buat dua hitbox secara dinamis */
    Hitbox *player = create_hitbox(0.0f, 0.0f, 1.0f, 2.0f);   /* pusat di (0,0) */
    Hitbox *enemy  = create_hitbox(5.0f, 0.0f, 2.0f, 2.0f);   /* pusat di (5,0) */

    if (!player || !enemy) {
        destroy_hitbox(player);
        destroy_hitbox(enemy);
        return 1;
    }

    /* Array pointer hitbox (menunjukkan penggunaan pointer dan struktur koleksi) */
    Hitbox *list[2];
    list[0] = player;
    list[1] = enemy;

    printf("Initial state:\n");
    print_hitbox(list[0], "Player");
    print_hitbox(list[1], "Enemy");

    /* Simulasi sederhana: gerakkan player ke kanan mendekati enemy */
    printf("\nSimulasi: gerakkan player ke kanan (dx = 1.0 per step)\n");
    for (int step = 0; step <= 6; ++step) {
        printf("Step %d: ", step);
        print_hitbox(player, "Player");

        if (hitbox_collide(player, enemy)) {
            printf("  => Collision detected pada step %d!\n", step);
            break;
        } else {
            printf("  => No collision\n");
        }

        /* gerakkan player 1 unit ke kanan setiap iterasi */
        move_hitbox(player, 1.0f, 0.0f);
    }

    /* Contoh menonaktifkan enemy dan pengecekan lagi */
    enemy->active = false;
    printf("\nSet enemy inactive lalu cek collision lagi: %s\n",
           hitbox_collide(player, enemy) ? "Collision (unexpected)" : "No collision (expected)");

    /* Bersihkan memori */
    destroy_hitbox(player);
    destroy_hitbox(enemy);

    return 0;
}
