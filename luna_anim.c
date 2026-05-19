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
        anim->onqueue = 0;
        anim_timer_callback(anim);
}

static void anim_timer_callback(struct animation *anim)
{
        if (!anim->onqueue) {
		return;
	}

        LUNA_TICK_TYPE now     = LUNA_GET_TICK();
        LUNA_TICK_TYPE elapsed = now - anim->start_tick;

        // -----*********
        //     ^
        if (elapsed < anim->delay) {
                anim->timer.when = LUNA_GET_TICK() + ANIMATION_TICK_MS;
                anim->timer.callback = _core_callback;
                luna_timer_append(head, &(anim->timer));
                return;
        }
        elapsed -= anim->delay;
        float progress = (float)((float)elapsed / (float)(anim->duration));
        float curved;
        if (anim->animpath) {
                curved = anim->animpath(progress);
        } else {
                curved = anim_path_linear(progress);
        }

        if (anim->reverse) {
                curved = 1 - curved;
        }

        anim->curved = curved;
        if(anim->callback) {
                anim->callback(anim);
        }

        if (elapsed >= anim->duration) {
                return;
        }

        anim->timer.when     = LUNA_GET_TICK() + ANIMATION_TICK_MS;
        anim->timer.callback = _core_callback;
        luna_timer_append(head, &(anim->timer));
}

void luna_anim_ctor(struct animation *anim)
{
        anim->curve   = ANIM_CURVE_LINEAR;
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

void luna_anim_set_values(struct animation *anim, int32_t from, int32_t to)
{
        anim->from = from;
        anim->to   = to;
}

void luna_anim_set_duration(struct animation *anim, int32_t duration)
{
        anim->duration = duration;
}

void luna_anim_set_path(struct animation *anim, float (*animpath)(float p))
{
        anim->animpath = animpath;
}

void luna_anim_set_exec_cb(struct animation *anim, void  (*callback)(struct animation *anim))
{
        anim->callback = callback;
}