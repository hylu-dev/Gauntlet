#include "EngineCore.h"
#include "Object.h"

IMPLEMENT_ABSTRACT_CLASS(Object)

Object::Object()
{
    UUID uuid;
    CreateUUID(&uuid);

    uid = GUIDToSTRCODE(uuid);
    guid = GUIDTostring(uuid);
}

Object::Object(std::string guid) : guid(std::move(guid))
{
    uid = GetHashCode(this->guid.c_str());
}

Object::Object(std::string guid, std::string name) : name(std::move(name)), guid(std::move(guid))
{
    uid = GetHashCode(this->guid.c_str());
}

void Object::Load(json::JSON& node)
{
    if (node.hasKey("Name"))
    {
        name = node["Name"].ToString();
    }

    if (node.hasKey("GUID"))
    {
        guid = node["GUID"].ToString();
        uid = GetHashCode(guid.c_str());
    }
}
