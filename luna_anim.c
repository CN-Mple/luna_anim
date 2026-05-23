/* luna_anim.c */
#include "luna_anim.h"

static struct core_timer **head;

void luna_anim_timer_register(struct core_timer **timer)
{
        head = timer;
}

static void anim_timer_callback(struct animation *anim);

static void _core_callback(struct core_timer *timer)
{
        struct animation *anim = (struct animation *)timer;
        anim_timer_callback(anim);
}

static void anim_timer_callback(struct animation *anim)
{
        if (!anim->onqueue) {
		return;
	}
        anim->onqueue = 0;

        LUNA_TICK_TYPE now     = LUNA_GET_TICK();
        LUNA_TICK_TYPE elapsed = now - anim->start_tick;

        // -----*********
        //     ^
        if (elapsed < anim->delay) {
                anim->timer.when = LUNA_GET_TICK() + ANIMATION_TICK_MS;
                anim->timer.callback = _core_callback;
                luna_timer_append(head, &(anim->timer));
                anim->onqueue        = true;
                return;
        }
        elapsed -= anim->delay;
        float progress = (float)((float)elapsed / (float)(anim->duration));
        float curved;
        if (anim->path) {
                curved = anim->path(progress);
        } else {
                curved = anim_path_linear(progress);
        }

        if (anim->reverse) {
                curved = 1 - curved;
        }
        int16_t value = anim->from + curved * (anim->to - anim->from);
        if(anim->callback) {
                anim->callback(anim->var, value);
        }

        if (elapsed >= anim->duration) {
                if(anim->teardown) {
                        anim->teardown(anim->var, value);
                }
                free(anim);
                return;
        }

        anim->timer.when     = LUNA_GET_TICK() + ANIMATION_TICK_MS;
        anim->timer.callback = _core_callback;
        luna_timer_append(head, &(anim->timer));
        anim->onqueue        = true;
}

void luna_anim_ctor(struct animation *anim)
{
        anim->reverse = false;
        anim->onqueue = false;
}

void luna_anim_dtor(struct animation *anim)
{
        luna_anim_stop(anim);
}

struct animation *luna_anim_create(void)
{
        struct animation *anim = malloc(sizeof(struct animation));
        memset(anim, 0, sizeof(struct animation));
        if (!anim) {
                return NULL;
        }
        luna_anim_ctor(anim);
        return anim;
}

void luna_anim_destroy(struct animation *anim)
{
        luna_anim_dtor(anim);
        free(anim);
}

void luna_anim_start(struct animation *anim)
{
        if (!anim || anim->onqueue) {
                return;
        }
        anim->start_tick     = LUNA_GET_TICK();
        anim->timer.when     = LUNA_GET_TICK() + anim->delay;
        anim->timer.callback = _core_callback;
        luna_timer_append(head, &(anim->timer));
        anim->onqueue        = true;
}

void luna_anim_stop(struct animation *anim)
{
        if (!anim || !(anim->onqueue)) {
                return;
        }
        luna_timer_remove(head, &(anim->timer));
        anim->onqueue = false;
}

void luna_anim_set_var(struct animation *anim, void *var)
{
        anim->var = var;
}

void luna_anim_set_values(struct animation *anim, ValueType from, ValueType to)
{
        anim->from = from;
        anim->to   = to;
}

void luna_anim_set_duration(struct animation *anim, int32_t duration)
{
        anim->duration = duration;
}

void luna_anim_set_delay(struct animation *anim, int32_t delay)
{
        anim->delay = delay;
}

void luna_anim_set_reverse(struct animation *anim, bool reverse)
{
        anim->reverse = reverse;
}

void luna_anim_set_path (struct animation *anim, luna_path_cb_t   path)
{
        anim->path = path;
}

void luna_anim_set_callback(struct animation *anim, luna_anim_cb_t   callback)
{
        anim->callback = callback;
}

void luna_anim_set_teardown(struct animation *anim, luna_finish_cb_t teardown)
{
        anim->teardown = teardown;
}