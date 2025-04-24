#include "zTalkBox.h"

#include <types.h>

// DIRECTLY PORTED FROM BFBB

namespace
{
    struct SharedTalkboxState
    {
        void* padding[2]; // FIXME: variables not verified
        ztalkbox* active;
    };

    SharedTalkboxState ztalkbox_shared;
} // namespace

ztalkbox* ztalkbox::get_active()
{
    return ztalkbox_shared.active;
}
