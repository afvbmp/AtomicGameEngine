
#include <Atomic/Resource/ResourceCache.h>
#include <Atomic/Resource/Image.h>

#include "Asset.h"
#include "AssetDatabase.h"
#include "TextureImporter.h"

namespace ToolCore
{

TextureImporter::TextureImporter(Context* context, Asset *asset) : AssetImporter(context, asset)
{

}

TextureImporter::~TextureImporter()
{

}

void TextureImporter::SetDefaults()
{
    AssetImporter::SetDefaults();
}

bool TextureImporter::Import()
{
    AssetDatabase* db = GetSubsystem<AssetDatabase>();

    ResourceCache* cache = GetSubsystem<ResourceCache>();
    SharedPtr<Image> image = cache->GetTempResource<Image>(asset_->GetPath());

    if (image.Null())
        return false;

    // todo, proper proportions
    image->Resize(64, 64);

    String cachePath = db->GetCachePath();

    // not sure entirely what we want to do here, though if the cache file doesn't exist
    // will reimport
    image->SavePNG(cachePath + asset_->GetGUID());

    // save thumbnail
    image->SavePNG(cachePath + asset_->GetGUID() + "_thumbnail.png");

    return true;
}

bool TextureImporter::LoadSettingsInternal()
{
    if (!AssetImporter::LoadSettingsInternal())
        return false;

    JSONValue import = jsonRoot_.GetChild("TextureImporter", JSON_OBJECT);

    return true;
}

bool TextureImporter::SaveSettingsInternal()
{
    if (!AssetImporter::SaveSettingsInternal())
        return false;

    JSONValue import = jsonRoot_.CreateChild("TextureImporter");

    return true;
}

Resource* TextureImporter::GetResource(const String& typeName)
{
    if (!typeName.Length())
        return 0;

    ResourceCache* cache = GetSubsystem<ResourceCache>();
    return cache->GetResource(typeName, asset_->GetPath());

}


}