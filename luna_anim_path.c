/* luna_anim_path.c */
#include "luna_anim_path.h"

float anim_path_linear(float p)
{
        return p;
}

float anim_path_ease_in(float p)
{
        float v = p * p;
        return v;
}

float anim_path_ease_out(float p)
{
        float x = p - 1;
        float v = x * x;
        return 1 - v;
}

float anim_path_ease_in_out(float p)
{
        if (p < 0.5) {
                float v = 2 * p * p;
                return v;
        } else {
                float x = 2 * p - 2;
                float v = x * x;
                return 1 - (v / 2);
        }
}
