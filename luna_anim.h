/* luna_anim.h */
#ifndef LUNA_ANIMATION_H
#define LUNA_ANIMATION_H

#include "luna_timer.h"
#include "luna_anim_path.h"
#include <stdbool.h>
#include <stdlib.h>

#define ANIMATION_FPS                   (60)
#define ANIMATION_TICK_MS               (1000 / ANIMATION_FPS)

typedef enum {
        ANIM_CURVE_LINEAR,
        ANIM_CURVE_EASE_IN,
        ANIM_CURVE_EASE_OUT,
        ANIM_CURVE_EASE_IN_OUT,
} AnimCurveType;

struct animation {
        struct core_timer timer;
        bool              onqueue;

        int32_t           start_tick;
        int32_t           duration;
        int32_t           delay;
        bool              reverse;

        AnimCurveType     curve;
        float             curved;
        int32_t           from;
        int32_t           to;
        void             *var;
        float (*animpath)(float p);
        void  (*callback)(struct animation *anim);
};


void luna_anim_timer_register(struct core_timer **timer);

void luna_anim_ctor(struct animation *anim);
void luna_anim_dtor(struct animation *anim);

struct animation *luna_anim_create(void);
void luna_anim_destroy(struct animation *anim);

void luna_anim_start(struct animation *anim);
void luna_anim_stop(struct animation *anim);


void luna_anim_set_var(struct animation *anim, void *var);
void luna_anim_set_values(struct animation *anim, int32_t from, int32_t to);
void luna_anim_set_duration(struct animation *anim, int32_t duration);
void luna_anim_set_path(struct animation *anim, float (*animpath)(float p));
void luna_anim_set_exec_cb(struct animation *anim, void  (*callback)(struct animation *anim));

#endif