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

    SharedTalkboxState shared; // again, ported from battle, probably not correct
} // namespace

void* ztalkbox::permit(U32 r3, U32 r4)
{
    return 0;
}

ztalkbox* ztalkbox::get_active()
{
    return shared.active;
}

void ztalkbox::hide()
{
    flag.visible = 0;
    if (prompt_box)
    {
        prompt_box->deactivate();
    }
    if (quit_box)
    {
        quit_box->deactivate();
    }
}
