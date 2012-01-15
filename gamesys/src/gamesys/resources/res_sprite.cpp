#include "res_sprite.h"

#include <string.h>

#include <dlib/log.h>

#include "sprite_ddf.h"

namespace dmGameSystem
{
    dmResource::Result AcquireResources(dmResource::HFactory factory,
        const void* buffer, uint32_t buffer_size,
        SpriteResource* resource, const char* filename)
    {
        dmDDF::Result e  = dmDDF::LoadMessage(buffer, buffer_size, &resource->m_DDF);
        if ( e != dmDDF::RESULT_OK )
        {
            return dmResource::RESULT_FORMAT_ERROR;
        }
        if (resource->m_DDF->m_TileCount == 0 || resource->m_DDF->m_TilesPerRow == 0)
        {
            dmLogError("%s", "tile_count and tiles_per_row must both be over 0");
            return dmResource::RESULT_FORMAT_ERROR;
        }
        dmResource::Result fr = dmResource::Get(factory, resource->m_DDF->m_Texture, (void**)&resource->m_Texture);
        if (fr != dmResource::RESULT_OK)
        {
            return fr;
        }
        return dmResource::RESULT_OK;
    }

    void ReleaseResources(dmResource::HFactory factory, SpriteResource* resource)
    {
        if (resource->m_DDF != 0x0)
            dmDDF::FreeMessage(resource->m_DDF);
        if (resource->m_Texture != 0x0)
            dmResource::Release(factory, resource->m_Texture);
    }

    dmResource::Result ResSpriteCreate(dmResource::HFactory factory,
                                            void* context,
                                            const void* buffer, uint32_t buffer_size,
                                            dmResource::SResourceDescriptor* resource,
                                            const char* filename)
    {
        SpriteResource* sprite_resource = new SpriteResource();
        memset(sprite_resource, 0, sizeof(SpriteResource));
        dmResource::Result r = AcquireResources(factory, buffer, buffer_size, sprite_resource, filename);
        if (r == dmResource::RESULT_OK)
        {
            resource->m_Resource = (void*) sprite_resource;
        }
        else
        {
            ReleaseResources(factory, sprite_resource);
            delete sprite_resource;
        }
        return r;
    }

    dmResource::Result ResSpriteDestroy(dmResource::HFactory factory,
                                             void* context,
                                             dmResource::SResourceDescriptor* resource)
    {
        SpriteResource* sprite_resource = (SpriteResource*) resource->m_Resource;
        ReleaseResources(factory, sprite_resource);
        delete sprite_resource;
        return dmResource::RESULT_OK;
    }

    dmResource::Result ResSpriteRecreate(dmResource::HFactory factory,
                                              void* context,
                                              const void* buffer, uint32_t buffer_size,
                                              dmResource::SResourceDescriptor* resource,
                                              const char* filename)
    {
        SpriteResource tmp_sprite_resource;
        memset(&tmp_sprite_resource, 0, sizeof(SpriteResource));
        dmResource::Result r = AcquireResources(factory, buffer, buffer_size, &tmp_sprite_resource, filename);
        if (r == dmResource::RESULT_OK)
        {
            SpriteResource* sprite_resource = (SpriteResource*)resource->m_Resource;
            ReleaseResources(factory, sprite_resource);
            *sprite_resource = tmp_sprite_resource;
        }
        else
        {
            ReleaseResources(factory, &tmp_sprite_resource);
        }
        return r;
    }
}
