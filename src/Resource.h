#ifndef Magnum_Resource_h
#define Magnum_Resource_h
/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013 Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

/** @file /Resource.h
 * @brief Class Magnum::ResourceKey, Magnum::Resource, enum Magnum::ResourceState
 */

#include <Utility/Assert.h>
#include <Utility/MurmurHash2.h>

#include "Magnum.h"

#include "magnumVisibility.h"

namespace Magnum {

/** @relates Resource
 * @brief %Resource state
 *
 * @see Resource::state(), ResourceManager::state()
 */
enum class ResourceState: UnsignedByte {
    /** The resource is not yet loaded (and no fallback is available). */
    NotLoaded,

    /** The resource is not yet loaded and fallback resource is used instead. */
    NotLoadedFallback,

    /** The resource is currently loading (and no fallback is available). */
    Loading,

    /** The resource is currently loading and fallback resource is used instead. */
    LoadingFallback,

    /** The resource was not found (and no fallback is available). */
    NotFound,

    /** The resource was not found and fallback resource is used instead. */
    NotFoundFallback,

    /** The resource is loaded, but can be changed by the manager at any time. */
    Mutable,

    /** The resource is loaded and won't be changed by the manager anymore. */
    Final
};

/** @debugoperator{Magnum::Resource} */
Debug MAGNUM_EXPORT operator<<(Debug debug, ResourceState value);

/**
@brief Key for accessing resource

See ResourceManager for more information.
@see ResourceManager::referenceCount(), ResourceManager::state(),
    ResourceManager::get(), ResourceManager::set(), Resource::key()
*/
class ResourceKey: public Utility::MurmurHash2::Digest {
    public:
        /**
         * @brief Default constructor
         *
         * Creates zero key. Note that it is not the same as calling other
         * constructors with empty string.
         */
        constexpr ResourceKey() {}

        /** @brief Constructor */
        ResourceKey(const std::string& key): Utility::MurmurHash2::Digest(Utility::MurmurHash2()(key)) {}

        /**
         * @brief Constructor
         * @todo constexpr
         */
        template<std::size_t size> constexpr ResourceKey(const char(&key)[size]): Utility::MurmurHash2::Digest(Utility::MurmurHash2()(key)) {}
};

/** @debugoperator{Magnum::ResourceKey} */
inline Debug operator<<(Debug debug, const ResourceKey& value) {
    return debug << static_cast<const Utility::HashDigest<sizeof(std::size_t)>&>(value);
}

namespace Implementation {
    template<class> class ResourceManagerData;
}

/**
@brief %Resource reference

See ResourceManager for more information.
*/
#ifdef DOXYGEN_GENERATING_OUTPUT
template<class T, class U = T>
#else
template<class T, class U>
#endif
class Resource {
    friend class Implementation::ResourceManagerData<T>;

    public:
        /**
         * @brief Default constructor
         *
         * Creates empty resource. Resources are acquired from the manager by
         * calling ResourceManager::get().
         */
        explicit Resource(): manager(nullptr), lastCheck(0), _state(ResourceState::Final), data(nullptr) {}

        /** @brief Copy constructor */
        Resource(const Resource<T, U>& other): manager(other.manager), _key(other._key), lastCheck(other.lastCheck), _state(other._state), data(other.data) {
            if(manager) manager->incrementReferenceCount(_key);
        }

        /** @brief Move constructor */
        Resource(Resource<T, U>&& other): manager(other.manager), _key(other._key), lastCheck(other.lastCheck), _state(other._state), data(other.data) {
            /** @brief Make other's state well-defined */
            other.manager = nullptr;
        }

        /** @brief Destructor */
        ~Resource() {
            if(manager) manager->decrementReferenceCount(_key);
        }

        /** @brief Copy assignment */
        Resource<T, U>& operator=(const Resource<T, U>& other);

        /** @brief Move assignment */
        Resource<T, U>& operator=(Resource<T, U>&& other);

        /** @brief Resource key */
        ResourceKey key() const { return _key; }

        /**
         * @brief %Resource state
         *
         * @see operator bool(), ResourceManager::state()
         */
        ResourceState state() {
            acquire();
            return _state;
        }

        /**
         * @brief Whether the resource is available
         *
         * Returns `false` when resource is not loaded and no fallback is
         * available (i.e. state() is either @ref ResourceState "ResourceState::NotLoaded",
         * @ref ResourceState "ResourceState::Loading" or
         * @ref ResourceState "ResourceState::NotFound"), true otherwise.
         */
        operator bool() {
            acquire();
            return data;
        }

        /**
         * @brief %Resource data
         *
         * The resource must be loaded before accessing it. Use boolean
         * conversion operator or state() for testing whether it is loaded.
         */
        operator U*() {
            acquire();
            CORRADE_ASSERT(data, "Resource: accessing not loaded data with key" << key(), nullptr);
            return static_cast<U*>(data);
        }

        /** @overload */
        U* operator->() {
            acquire();
            CORRADE_ASSERT(data, "Resource: accessing not loaded data with key" << key(), nullptr);
            return static_cast<U*>(data);
        }

        /** @overload */
        U& operator*() {
            acquire();
            CORRADE_ASSERT(data, "Resource: accessing not loaded data with key" << key(), *static_cast<U*>(data));
            return *static_cast<U*>(data);
        }

    private:
        Resource(Implementation::ResourceManagerData<T>* manager, ResourceKey key): manager(manager), _key(key), lastCheck(0), _state(ResourceState::NotLoaded), data(nullptr) {
            manager->incrementReferenceCount(key);
        }

        void acquire();

        Implementation::ResourceManagerData<T>* manager;
        ResourceKey _key;
        std::size_t lastCheck;
        ResourceState _state;
        T* data;
};

template<class T, class U> Resource<T, U>& Resource<T, U>::operator=(const Resource<T, U>& other) {
    if(manager) manager->decrementReferenceCount(_key);

    manager = other.manager;
    _key = other._key;
    lastCheck = other.lastCheck;
    _state = other._state;
    data = other.data;

    if(manager) manager->incrementReferenceCount(_key);
    return *this;
}

template<class T, class U> Resource<T, U>& Resource<T, U>::operator=(Resource<T, U>&& other) {
    /** @todo Just swap the values */
    if(manager) manager->decrementReferenceCount(_key);

    manager = other.manager;
    _key = other._key;
    lastCheck = other.lastCheck;
    _state = other._state;
    data = other.data;

    other.manager = nullptr;
    return *this;
}

template<class T, class U> void Resource<T, U>::acquire() {
    /* The data are already final, nothing to do */
    if(_state == ResourceState::Final) return;

    /* Nothing changed since last check */
    if(manager->lastChange() < lastCheck) return;

    /* Acquire new data and save last check time */
    const typename Implementation::ResourceManagerData<T>::Data& d = manager->data(_key);
    lastCheck = manager->lastChange();

    /* Try to get the data */
    data = d.data;
    _state = static_cast<ResourceState>(d.state);

    /* Data are not available */
    if(!data) {
        /* Fallback found, add *Fallback to state */
        if((data = manager->fallback())) {
            if(_state == ResourceState::Loading)
                _state = ResourceState::LoadingFallback;
            else if(_state == ResourceState::NotFound)
                _state = ResourceState::NotFoundFallback;
            else _state = ResourceState::NotLoadedFallback;

        /* Fallback not found and loading didn't start yet */
        } else if(_state != ResourceState::Loading && _state != ResourceState::NotFound)
            _state = ResourceState::NotLoaded;
    }
}

}

/* Make the definition complete */
#include "ResourceManager.h"

#endif
