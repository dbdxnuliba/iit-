/*
 * Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
 * Copyright (C) 2006-2010 RobotCub Consortium
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef YARP_OS_RESOURCEFINDER_H
#define YARP_OS_RESOURCEFINDER_H

#include <yarp/os/ConstString.h>
#include <yarp/os/Searchable.h>
#include <yarp/os/Value.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Property.h>
#include <yarp/os/ResourceFinderOptions.h>

namespace yarp {
    namespace os {
        class ResourceFinder;
    }
}

/**
 *
 * Helper class for finding config files and other external resources.
 *
 * More details on this class behaviour can be found in
 * \ref yarp_resource_finder_tutorials.
 *
 */
class YARP_OS_API yarp::os::ResourceFinder : public Searchable {
public:
    ResourceFinder();

    ResourceFinder(const ResourceFinder& alt);

    virtual ~ResourceFinder();

    const ResourceFinder& operator= (const ResourceFinder& alt);


    /**
     *
     * Request that information be printed to the console on how
     * resources are being found.  This is especially useful to
     * understand why resources are *not* found or the wrong resource
     * is picked up.
     *
     * @param verbose set/suppress printing of information
     *
     * @return true iff information will be printed
     *
     */
    bool setVerbose(bool verbose = true);

    /**
     *
     * Request that information be suppressed from the console.  By
     * default ResourceFinder will print messages if it fails to find
     * files, for example.
     *
     * @param quiet suppress printing of information
     *
     * @return true iff information will be suppressed
     *
     */
    bool setQuiet(bool quiet = true);

    /**
     * Sets up the ResourceFinder.
     *
     * @return true if configuration succeeded. Configuration fails if the user
     * requests a configuration file to be read (via --from for example) and
     * that file cannot be found. If a default configuration file has been set
     * with ResourceFinder::setDefaultConfigFile, the presence or absence of
     * that file doesn't by itself contribute to  success/failure (since it is
     * perfectly valid for it to be
     * absent).
     */
    bool configure(int argc, char *argv[], bool skipFirstArgument = true);

    /**
     * Sets the context for the current ResourceFinder object.
     *
     * A context is a folder collecting configuration files and data that may be
     * used to configure modules at runtime.
     * When the resource finder is configured with a specific contextName,
     * contexts/<context-name> is added to the search path in which the
     * initial configuration file and any additional files are sought.
     *
     * @param contextName The name of the context
     * @return true on success, false otherwise
     */
    bool setDefaultContext(const char *contextName) {
        clearContext();
        return addContext(contextName);
    }

    /**
     * Sets the context for the current ResourceFinder object.
     *
     * @param contextName The name of the context
     * @return true on success, false otherwise
     *
     * @see setDefaultContext(const char *contextName)
     */
    bool setDefaultContext(const yarp::os::ConstString& contextName) {
        return setDefaultContext(contextName.c_str());
    }

#ifndef YARP_NO_DEPRECATED // since YARP 2.3.70
    /**
     *
     * Deprecated name for setDefaultContext
     *
     * @deprecated since YARP 2.3.70
     */
    YARP_DEPRECATED bool setContext(const char *contextName) {
        return setDefaultContext(contextName);
    }
#endif // YARP_NO_DEPRECATED

    /**
     *
     * Provide a default value for a given key; the provided key will be converted to a
     * yarp::os::Value, so also string representations for lists and numerical values are accepted
     *
     */
    bool setDefault(const char *key, const yarp::os::ConstString& val);

    /**
     *
     * Provide a default value for a given key
     *
     */
    bool setDefault(const char *key, const yarp::os::Value& val);

    /**
     *
     * Provide a default value for the configuration file
     * (can be overridden from command line with the --from argument)
     *
     */
    bool setDefaultConfigFile(const char *fname) {
        return setDefault("from", fname);
    }

    /**
     *
     * Find the full path to a file.  The file is specified by the
     * name of a key.  The value of the key should be set up either
     * on the command line, through a loaded config file, or
     * by a call to setDefault.
     *
     * If all else fails, findFile will try interpreting key as
     * a file name - this is for backwards compatibility
     * and is behavior that will probably go away - don't
     * depend on it!
     *
     * The file is searched in a hierarchy of paths as defined in
     * \ref yarp_resource_finder_tutorials.
     *
     */
    yarp::os::ConstString findFile(const ConstString& name);

    /**
     *
     * Find the full path to a file. The name of the file is provided explicitly.
     *
     * The file is searched in a hierarchy of paths as defined in
     * \ref yarp_resource_finder_tutorials.
     */
    yarp::os::ConstString findFileByName(const ConstString& name);

    /**
     *
     * Expand a partial path to a full path.  The path is specified by the
     * name of a key.  The value of the key should be set up either
     * on the command line, through a loaded config file, or
     * by a call to setDefault.
     *
     * If all else fails, findPath will try interpreting key as
     * a path - this is for backwards compatibility
     * and is behavior that will probably go away - don't
     * depend on it!
     *
     * The path is searched in a hierarchy of paths as defined in
     * \ref yarp_resource_finder_tutorials.
     */
    yarp::os::ConstString findPath(const ConstString& name);

    /**
     *
     * Expand a partial path to a list of paths.
     * Like findPath(key), but continues on to find all
     * instances of the path.
     *
     * so findPaths("app") would return ["/foo/app", "/bar/app", ...]
     * depending on the search path in effect.
     * The first path is the list comes from the highest-priority
     * location, and would be the path returned by findPath("app")
     *
     * The path is searched in a hierarchy of paths as defined in
     * \ref yarp_resource_finder_tutorials.
     */
    yarp::os::Bottle findPaths(const ConstString& name);

    /**
     *
     * Find the first existing directory in the search path.
     *
     */
    yarp::os::ConstString findPath();


    /**
     *
     * Return the default "context" or "application name" used in searching
     * for configuration files.  The context is a keyword that is converted
     * into a search path in a policy-specific way.
     *
     */
    yarp::os::ConstString getContext();

    /**
     *
     * Return the full stack of contexts used in searching
     * for configuration files.
     *
     */
    yarp::os::Bottle getContexts();

    // Searchable interface
    virtual bool check(const ConstString& key) const override;
    virtual Value& find(const ConstString& key) const override;
    virtual Bottle& findGroup(const ConstString& key) const override;
    virtual bool isNull() const override;
    virtual ConstString toString() const override;


    /**
     * Gets a section as a ResourceFinder object, retaining the
     * context and configuration of the current ResourceFinder.
     * This is a thin wrapper around the Searchable::findGroup method.
     * @param key The section to look for
     * @return A ResourceFinder corresponding to the named section
     *
     */
    virtual ResourceFinder findNestedResourceFinder(const char *key);


    bool isConfigured() const {
        return isConfiguredFlag;
    }

    /**
     *
     * Access a ResourceFinder singleton whose lifetime will match that of
     * the YARP library.
     *
     * @return the ResourceFinder singleton
     *
     */
    static ResourceFinder& getResourceFinderSingleton();

    using Searchable::check;
    using Searchable::findGroup;


    /* YARP 2.4 changes begin */

    /**
     *
     * Location where user data files are stored.
     * If $YARP_DATA_HOME is set, that is returned.  We do not check
     * to see if that directory exists.
     * Otherwise:
     *   (In all the following cases, we attempt to create the directory
     *   returned if it does not already exist).
     *   If $XDG_DATA_HOME is set, "yarp" is appended to it after the
     *   OS-appropriate directory separator, and the result returned.
     *   Otherwise:
     *     On Windows
     *       %APPDATA%\yarp is returned.
     *     On Linux and all others:
     *       $HOME/.local/share is returned.
     *     (an OSX-specific case remains to be defined)
     *
     */
    static ConstString getDataHome() {
        return getDataHomeWithPossibleCreation(true);
    }


    /**
     *
     * Variant of getDataHome that will never create the directory
     * returned.
     *
     */
    static ConstString getDataHomeNoCreate() {
        return getDataHomeWithPossibleCreation(false);
    }

    /**
     *
     * Location where user config files are stored.
     * If $YARP_CONFIG_HOME is set, that is returned.
     * Otherwise:
     *   If $XDG_CONFIG_HOME is set, "yarp" is appended to it after the
     *   OS-appropriate directory separator, and the result returned.
     *   Otherwise:
     *     On Windows
     *       %APPDATA%\yarp\config is returned.
     *     On Linux and all others:
     *       $HOME/.config/yarp is returned.
     *     (an OSX-specific case remains to be defined)
     *
     */
    static ConstString getConfigHome() {
        return getConfigHomeWithPossibleCreation(true);
    }

    /**
     *
     * Variant of getConfigHome that will never create the directory
     * returned.
     *
     */
    static ConstString getConfigHomeNoCreate() {
        return getConfigHomeWithPossibleCreation(false);
    }

    /**
     *
     * Return the path to the "user" context directory for the current context
     *
     */
    yarp::os::ConstString getHomeContextPath();

     /**
     *
     * Return the path to the "user" robot directory
     *
     */
    yarp::os::ConstString getHomeRobotPath();

    /**
     *
     * Locations where packaged data and config files are stored.
     * If $YARP_DATA_DIRS is set, that is returned.
     * Otherwise:
     *   If $XDG_DATA_DIRS is set, "/yarp" or "\yarp" as appropriate
     *   is appended to each path and the result returned.
     *   Otherwise:
     *     On Windows
     *       %YARP_DIR%\share\yarp
     *     On Linux and all others:
     *       /usr/local/share/yarp:/usr/share/yarp is returned.
     *     (an OSX-specific case remains to be defined)
     *
     */
    static Bottle getDataDirs();

    /**
     *
     * Locations where system administrator data and config files are stored.
     * If $YARP_CONFIG_DIRS is set, that is returned.
     * Otherwise:
     *   If $XDG_CONFIG_DIRS is set, "/yarp" or "\yarp" as appropriate
     *   is appended to each path and the result returned.
     *   Otherwise:
     *     On Windows
     *       %ALLUSERSPROFILE%\yarp
     *     On Linux and all others:
     *       /etc/yarp is returned.
     *     (an OSX-specific case remains to be defined)
     *
     */
    static Bottle getConfigDirs();

    yarp::os::Bottle findPaths(const ConstString& name,
                               const ResourceFinderOptions& options);

    yarp::os::ConstString findPath(const ConstString& name,
                                   const ResourceFinderOptions& options);

    yarp::os::ConstString findFile(const ConstString& name,
                                   const ResourceFinderOptions& options);

    yarp::os::ConstString findFileByName(const ConstString& name,
                                   const ResourceFinderOptions& options);

    bool readConfig(Property& config,
                    const ConstString& key,
                    const ResourceFinderOptions& options);

    /* YARP 2.4 changes end */

private:

    // this might be useful, but is not in spec
    bool addContext(const char *appName);

    // this might be useful, but is not in spec
    bool clearContext();

    void *implementation;
    bool owned;
    bool nullConfig;
    bool isConfiguredFlag;
    yarp::os::Property config;

    ResourceFinder(Searchable& data, void *implementation);

    static ConstString getDataHomeWithPossibleCreation(bool mayCreate);
    static ConstString getConfigHomeWithPossibleCreation(bool mayCreate);
    static ConstString createIfAbsent(bool mayCreate, const ConstString& path);
};


#endif // YARP_OS_RESOURCEFINDER_H
