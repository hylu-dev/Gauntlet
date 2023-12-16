#include "EngineCore.h"
#include "AssetManager.h"

#include <filesystem>

using std::filesystem::directory_iterator;
using std::filesystem::recursive_directory_iterator;

void AssetManager::HandleAssetEntry(const std::filesystem::directory_entry& entry) {
	// Skip directories
	if (entry.is_directory()) {
		return;
	}

	// Check if the file is a .asset type
	if (entry.path().extension().compare(".asset") != 0) {
		return;
	}

	std::cout << "Found Asset: " << entry.path() << std::endl;

	std::ifstream inputStream(entry.path());
	std::string str((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
	json::JSON node = json::JSON::Load(str);

	if (!node.hasKey("AssetType")) {
		LOG("File " << entry.path() << " does not have a 'AssetType' field. Skipping.");
		return;
	}

	const auto assetType = node.at("AssetType").ToString();
	auto asset = (Asset*)CreateObject(assetType.c_str());
	asset->filepath = entry.path().relative_path().replace_extension("").generic_string();
	asset->Load(node);

	AddAsset(asset);
}

void AssetManager::Initialize() {
	if (recursiveSearch) {
		for (const auto& entry : recursive_directory_iterator(assetDirectory)) {
			HandleAssetEntry(entry);
		}
	} else {
		for (const auto& entry : directory_iterator(assetDirectory)) {
			HandleAssetEntry(entry);
		}
	}
}

void AssetManager::AddAsset(Asset* asset) {
	AssetMapEntry entry;
	entry.asset = asset;
	entry.ref_count = 0;
	assets.insert_or_assign(asset->GetUid(), entry);
}

void AssetManager::LoadSceneAsset(std::string guid) {
	STRCODE id = GetHashCode(guid.c_str());
	return LoadSceneAsset(id);
}

void AssetManager::LoadSceneAsset(unsigned id) {
	if (assets.find(id) == assets.end()) {
		LOG("Could not find Asset with id: " << id);
		return;
	}
	auto& [asset, ref_count] = assets.at(id);
	if (ref_count == 0)
	{
		asset->Initialize();
	}
	ref_count++;
}

void AssetManager::UnloadSceneAsset(std::string guid) {
	STRCODE id = GetHashCode(guid.c_str());
	return UnloadSceneAsset(id);
}

void AssetManager::UnloadSceneAsset(STRCODE id) {
	if (assets.find(id) == assets.end()) {
		LOG("Could not find Asset with id: " << id);
		return;
	}

	assets.at(id).ref_count--;
	if (assets.at(id).ref_count == 0) {
		RemoveAsset(id);
	}
}

Asset* AssetManager::GetAsset(std::string guid) {
	STRCODE id = GetHashCode(guid.c_str());
	return GetAsset(id);
}

Asset* AssetManager::GetAsset(STRCODE id) {
	if (assets.find(id) != assets.end()) {
		return assets.at(id).asset;
	}

	LOG("Could not find Asset with id: " << id);
	return nullptr;
}

void AssetManager::RemoveAsset(std::string guid) {
	STRCODE id = GetHashCode(guid.c_str());
	RemoveAsset(id);
}

void AssetManager::RemoveAsset(STRCODE id) {
	if (assets.find(id) != assets.end()) {
		assets.at(id).asset->Destroy();
	}
	assets.erase(id);
}

void AssetManager::Load(const std::string& config_file) {
	std::ifstream inputStream(config_file.c_str());
	std::string str((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
	json::JSON node = json::JSON::Load(str);

	if (node.hasKey("AssetDirectory")) {
		assetDirectory = node.at("AssetDirectory").ToString();
	}

	if (node.hasKey("RecursiveSearch")) {
		recursiveSearch = node.at("RecursiveSearch").ToBool();
	}
}

void AssetManager::Destroy() {
	for (auto& [_, asset] : assets) {
		delete asset.asset;
		asset.ref_count = 0;
	}
	assets.clear();
}
