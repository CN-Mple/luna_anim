/* luna_anim.h */
#ifndef LUNA_ANIM_H
#define LUNA_ANIM_H

#include "luna_timer.h"
#include "luna_anim_path.h"
#include "luna_anim_config.h"

#include <stdbool.h>
#include <stdlib.h>

#define ANIMATION_FPS                   (60)
#define ANIMATION_TICK_MS               (1000 / ANIMATION_FPS)

struct animation {
        struct core_timer timer;
        bool              onqueue;

        int32_t           start_tick;
        int32_t           duration;
        int32_t           delay;
        bool              reverse;

        ValueType         from;
        ValueType         to;
        void             *var;

        luna_path_cb_t    path;
        luna_anim_cb_t    callback;
        luna_finish_cb_t  teardown;
};


void luna_anim_timer_register(struct core_timer **timer);

void luna_anim_ctor(struct animation *anim);
void luna_anim_dtor(struct animation *anim);

struct animation *luna_anim_create(void);
void luna_anim_destroy(struct animation *anim);

void luna_anim_start(struct animation *anim);
void luna_anim_stop (struct animation *anim);

void luna_anim_set_var (struct animation *anim, void *var);
void luna_anim_set_values (struct animation *anim, ValueType from, ValueType to);
void luna_anim_set_duration(struct animation *anim, int32_t duration);
void luna_anim_set_reverse(struct animation *anim, bool reverse);
void luna_anim_set_delay(struct animation *anim, int32_t delay);


void luna_anim_set_path (struct animation *anim, luna_path_cb_t path);
void luna_anim_set_callback(struct animation *anim, luna_anim_cb_t callback);
void luna_anim_set_teardown(struct animation *anim, luna_finish_cb_t teardown);

#endif