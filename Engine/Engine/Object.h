/**
* @class Object
*
* The Object class is the base class for all objects in the engine. 
* It provides a common interface for all objects to be loaded, updated, and destroyed.
*/
#pragma once

#ifndef _OBJECT_H_
#define _OBJECT_H_

class Object
{
    DECLARE_ABSTRACT_BASE_CLASS(Object)

protected:
    bool isActive = true;

    std::string name;
    std::string guid;
    STRCODE uid = -1;

protected:
    /**
     * @brief Default constructor for the Object class.
     */
    Object();

    /**
     * @brief Constructor for the Object class.
     *
     * @param guid The GUID of the object.
     */
    explicit Object(std::string guid);

    /**
     * @brief Constructor for the Object class.
     *
     * @param guid The GUID of the object.
     * @param name The name of the object.
     */
    Object(std::string guid, std::string name);

    /**
     * @brief Destructor for the Object class.
     */
    virtual ~Object() = default;

    /**
     * @brief Initializes the object.
     * This is a pure virtual function and must be implemented by derived classes.
     */
    virtual void Initialize() = 0;

    /**
     * @brief Destroys the object.
     * This function can be overridden by derived classes 
     * to perform additional cleanup before the object is destroyed.
     */
    virtual void Destroy() {}

public:

    /**
     * @brief Loads object data from JSON.
     *
     * @param The JSON node containing the object data.
     */
    virtual void Load(json::JSON&);

    /**
     * @brief Checks if the object is active.
     *
     * @return true if the object is active, false if not.
     */
    bool IsActive() const { return isActive; }

    /**
     * @brief Gets the name of the object.
     *
     * @return A reference to the name of the object.
     */
    std::string& GetName() { return name; }

    /**
     * @brief Gets the GUID of the object.
     *
     * @return A reference to the GUID of the object.
     */
    std::string& GetGuid() { return guid; }

    /**
     * @brief Gets the hashed representation of the GUID.
     *
     * @return The hashed representation of the GUID.
     */
    STRCODE GetUid() const { return uid; }
};

#endif // !_OBJECT_H_
