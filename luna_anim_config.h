/* luna_anim_config.h */
#ifndef LUNA_ANIM_CONFIG_H
#define LUNA_ANIM_CONFIG_H

#include "layer.h"

typedef int16_t ValueType;

typedef float(*luna_path_cb_t)(float p);
typedef void (*luna_anim_cb_t)(struct layer *layer, ValueType val);
typedef void (*luna_finish_cb_t)(struct layer *layer, ValueType val);

#endif
