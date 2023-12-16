#pragma once
#ifndef _ASSET_MANAGER_H_
#define _ASSET_MANAGER_H_

#include <list>
#include <filesystem>
#include "Asset.h"

struct AssetMapEntry
{
	Asset* asset;
	unsigned int ref_count;
};

class AssetManager {
	DECLARE_SINGLETON(AssetManager)

	std::string assetDirectory = "../Assets";
	bool recursiveSearch = true;

	std::map<STRCODE, AssetMapEntry> assets;
public:

	void Destroy();

	void HandleAssetEntry(const std::filesystem::directory_entry& entry);
	void Initialize();

	void AddAsset(Asset* asset);

	void LoadSceneAsset(std::string guid);
	void LoadSceneAsset(STRCODE id);
	void UnloadSceneAsset(std::string guid);
	void UnloadSceneAsset(STRCODE id);

	Asset* GetAsset(std::string guid);
	Asset* GetAsset(STRCODE id);
	
	void RemoveAsset(std::string guid);
	void RemoveAsset(STRCODE id);

	void Load(const std::string& config_file);

};

#endif // !_ASSET_MANAGER_H_