#include "xNME.h"
#include "xPlayer.h"

enum xCamCoordType
{
    XCAM_COORD_CART,
    XCAM_COORD_CYLINDER,
    XCAM_COORD_SPHERE,
    XCAM_COORD_MAX,
    XCAM_COORD_INVALID = 0xffffffff,
};

enum xCamOrientType
{
    XCAM_ORIENT_QUAT,
    XCAM_ORIENT_EULER,
    XCAM_ORIENT_MAX,
    XCAM_ORIENT_INVALID = 0xffffffff,
};

struct xCamBlend;

struct analog_data
{
    xVec2 offset;
    xVec2 dir;
    F32 mag;
    F32 ang;
};

struct xCamCoordCylinder
{
    xVec3 origin;
    F32 dist;
    F32 height;
    F32 theta;
};

struct xCamCoordSphere
{
    xVec3 origin;
    F32 dist;
    xQuat dir;
};

struct _class_11
{
    union
    {
        xVec3 cart;
        xCamCoordCylinder cylinder;
        xCamCoordSphere sphere;
    };
};

struct xCamOrientEuler
{
    F32 yaw;
    F32 pitch;
    F32 roll;
};

struct _class_16
{
    union
    {
        xQuat quat;
        xCamOrientEuler euler;
    };
};

struct xCamConfigCommon
{
    U8 priority;
    U8 pad1;
    U8 pad2;
    U8 pad3;
    F32 blend_time;
};

struct zone_data_1
{
    xVec3 offset;
    xVec3 face;
};

struct xCamConfigFollow
{
    zone_data_1 zone_rest;
    zone_data_1 zone_above;
    zone_data_1 zone_below;
    F32 speed_zone_offset;
    F32 speed_zone_face;
    F32 speed_move_orbit;
};

struct xCamGroup;

struct xCam
{
    xMat4x3 mat;
    F32 fov;
    S32 flags;
    U32 owner;
    xCamGroup* group;
    analog_data analog;
    F32 motion_factor;
    xCamCoordType coord_type;
    xCamOrientType orient_type;
    _class_11 coord;
    _class_16 orient;
    xCamConfigCommon cfg_common;
    S32 group_index;
    S32 group_flags;
    xCamBlend* blender;

    void destroy();
    xCam* get_next();
    xCamConfigFollow* config_follow();
    void scene_exit();
    void scene_enter();
};

struct xCamGroup
{
    xMat4x3 mat;
    xVec3 vel;
    F32 fov;
    F32 fov_default;
    S32 flags;
    xCam* primary;
    analog_data analog;
    xCam* owned[32];
    S32 size;
    S32 primary_index;
    S32 child_flags;
    S32 child_flags_mask;
    xCamBlend* blend_cam[4];
};

struct xCamBlend : xCam
{
    xCam* src;
    xCam* dst;
    F32 time;

    void destroy();
};

struct zone_data_0
{
    F32 distance;
    F32 height;
    F32 height_focus;
};

struct config_2
{
    zone_data_0 zone_rest;
    zone_data_0 zone_above;
    zone_data_0 zone_below;
    F32 move_speed;
    F32 turn_speed;
    F32 stick_speed;
    F32 stick_yaw_vel;
    F32 max_yaw_vel;
    F32 margin_angle;
};

struct zBossCam_Binary : xCam
{
    zPlayer* owner;
    xVec3 cam_loc;
    xQuat cam_dir;
    void (*fun_moveover)(zBossCam_Binary*, F32, void*);
    void* ctxt_moveover;
    config_2 cfg;
    S32 camera;
    xVec3* s1;
    xVec3* s2;
    F32 s2_radius;
    xVec2 stick_offset;
};
