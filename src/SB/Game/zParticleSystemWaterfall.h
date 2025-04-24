// #ifndef ZPARTICLESYSTEMWATERFALL_H
// #define ZPARTICLESYSTEMWATERFALL_H

// #include "xNME.h"

// struct _class_0
// {
//     F32 yaw;
//     F32 pitch;
//     F32 roll;
// };

// struct attach_fixed_data
// {
// };

// struct attach_entity_data
// {
//     U32 entity;
//     U8 bone;
//     U8 pad1;
//     U8 pad2;
//     U8 pad3;
// };

// struct attach_entity_tag_data
// {
//     U32 entity;
//     xVec3 tag;
// };

// struct attach_data
// {
//     union
//     {
//         attach_fixed_data fixed;
//         attach_entity_data entity;
//         attach_entity_tag_data entity_tag;
//     };
// };

// struct motion_none_data
// {
// };

// struct motion_spiral_data
// {
//     U8 flags;
//     U8 points;
//     U8 pad1;
//     U8 pad2;
//     F32 radius_inner;
//     F32 radius_outer;
//     F32 duration;
//     F32 frequency;
// };

// struct motion_data
// {
//     union
//     {
//         motion_none_data none;
//         motion_spiral_data spiral;
//     };
// };

// struct volume_point_data
// {
// };

// struct volume_sphere_data
// {
//     F32 radius;
// };

// struct volume_circle_data
// {
//     F32 radius;
//     F32 arc_length;
// };

// struct volume_line_data
// {
//     U8 flags;
//     U8 pad1;
//     U8 pad2;
//     U8 pad3;
//     F32 radius;
//     F32 length;
// };

// struct volume_model_data
// {
//     U8 flags;
//     U8 exclude;
//     U8 pad1;
//     U8 pad2;
//     F32 expand;
// };

// struct volume_data
// {
//     union
//     {
//         volume_point_data point;
//         volume_sphere_data sphere;
//         volume_circle_data circle;
//         volume_line_data line;
//         volume_model_data model;
//     };
// };

// struct zParticleAsset : xDynAsset
// {
//     U8 flags;
//     U8 attach_flags;
//     U8 motion_flags;
//     U8 volume_flags;
//     F32 rate;
//     U32 texture;
//     U8 attach_type;
//     U8 motion_type;
//     U8 volume_type;
//     U8 system_type;
//     xVec3 location;
//     _class_0 axis;
//     attach_data attach;
//     motion_data motion;
//     volume_data volume;
// };

// struct zParticleGenerator : xBase
// {
//     zParticleAsset* asset;
//     S32 flags;

//     void activate();
//     void deactivate();
//     void reset();
//     void perturb_dir(xVec3& dir, F32 angle);
//     xEnt* get_attach_entity(zParticleAsset& asset);
//     void event_handler(xBase* to, U32 event);
// };

// struct relative_ordering
// {
//     S32 other;
//     U8 before_other;
// };

// struct zParticleSystem
// {
//     S32 type;
//     S32 need;
//     zParticleGenerator** generators;
//     S32 generators_size;
//     S32 generators_active;

//     void destroy_generator();
//     void scene_exit();
//     void setup();
//     void reset();
//     void pre_render();
//     void render();
//     relative_ordering* get_ordering(S32& size);
//     void scene_enter();
//     void update();
// };

// struct xParticleBatchGroup
// {
//     U8* elements;
//     S32 elements_size;

//     void create();
// };

// struct zParticleSystemWaterfall : zParticleSystem
// {
//     S32 unk;
//     xParticleBatchGroup batch_group;

//     S32 get_asset_size() const;
//     void update(F32 dt);
//     void scene_enter();
//     zParticleGenerator* create_generator(zParticleAsset& asset);
// };

// struct activity_data
// {
// };

// struct zParticleLocator
// {
//     activity_data* activity;

//     void restart();
//     void scene_enter();
//     U8 activate(zParticleAsset& asset, xEnt* attach_to);
//     void deactivate();
// };

// struct zParticleGeneratorWaterfall : zParticleGenerator
// {
//     S32 unk;
//     zParticleSystemWaterfall* system;
//     xEnt* attach_to;
//     F32 emitted; //0x24
//     zParticleLocator locator;
//     S32 batch_id;
//     xVec3 vel_dir;

//     void deactivate();
//     void activate();
//     void reset();
//     void create(zParticleSystemWaterfall& system, zParticleAsset& asset);
//     void update(F32 dt);
// };

// struct zParticleSystemWaterfallMist : zParticleSystem
// {
//     S32 unk;
//     xParticleBatchGroup batch_group;

//     S32 get_asset_size() const;
//     void update(F32 dt);
//     void scene_enter();
//     zParticleGenerator* create_generator(zParticleAsset& asset);
// };

// struct zParticleGeneratorWaterfallMist : zParticleGenerator
// {
//     S32 unk;
//     zParticleSystemWaterfallMist* system;
//     xEnt* attach_to;
//     F32 emitted;
//     zParticleLocator locator;
//     S32 batch_id;

//     void deactivate();
//     void activate();
//     void reset();
//     void create(zParticleSystemWaterfallMist& system, zParticleAsset& asset);
//     void update(F32 dt);
// };

// struct zParticleSystemWaterfallSplash : zParticleSystem
// {
//     S32 unk;
//     xParticleBatchGroup batch_group;

//     S32 get_asset_size() const;
//     void update(F32 dt);
//     void scene_enter();
//     zParticleGenerator* create_generator(zParticleAsset& asset);
// };

// struct zParticleGeneratorWaterfallSplash : zParticleGenerator
// {
//     S32 unk;
//     zParticleSystemWaterfallSplash* system;
//     xEnt* attach_to;
//     F32 emitted;
//     zParticleLocator locator;
//     S32 batch_id;

//     void deactivate();
//     void activate();
//     void reset();
//     void create(zParticleSystemWaterfallSplash& system, zParticleAsset& asset);
//     void update(F32 dt);
// };

// #endif
