///////////////////////////////////
//
//
//
///////////////////////////////////

#pragma once

#include "Ogitors.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
   #ifdef PLUGIN_EXPORT
     #define PluginExport __declspec (dllexport)
   #else
     #define PluginExport __declspec (dllimport)
   #endif
#else
   #define PluginExport
#endif

using namespace Ogitors;

class PluginExport FaSoundEditor: public CBaseEditor
{
    friend class FaSoundEditorFactory;
public:

    virtual void            showBoundingBox(bool bShow);

    /** @copydoc CBaseEditor::load(bool) */
    virtual bool            load(bool async = true);
    /** @copydoc CBaseEditor::unLoad() */
    virtual bool            unLoad();
    /// Processes a NameValuePairList and Sets Object's Properties according to it
    virtual void            createProperties(OgitorsPropertyValueMap &params);
    /// Called by Serializer to write custom files during an export
    virtual void            onSave(bool forced = false){};
    /// Gets the Handle to encapsulated object
    inline virtual void    *getHandle() {return static_cast<void*>(mHandle);};
    
    virtual TiXmlElement*    exportDotScene(TiXmlElement *pParent);
    virtual void setParentImpl(CBaseEditor *oldparent, CBaseEditor *newparent);
    virtual void setSelectedImpl(bool bSelected);
    virtual bool setHighlightedImpl(bool bSelected);
    virtual void setDerivedPosition(Ogre::Vector3 val);
    virtual Ogre::Vector3 getDerivedPosition();
    virtual Ogre::AxisAlignedBox getAABB();
    virtual Ogre::SceneNode     *getNode(){if(mHandle) return mHandle; else return 0;};


    bool _setPosition(OgitorsPropertyBase* property, const Ogre::Vector3& position);
    bool _setSoundFile(OgitorsPropertyBase* property, const Ogre::String& soundfile);
    bool _setVolume(OgitorsPropertyBase* property, const Ogre::Real& vol);
    bool _setEnabled(OgitorsPropertyBase* property, const bool& is_enabled);
    bool _setLoop(OgitorsPropertyBase* property, const bool& is_loop);

protected:
    FaSoundEditor(CBaseEditorFactory *factory);
    virtual ~FaSoundEditor();

    Ogre::SceneNode *mHandle;
    Ogre::Entity *mEntity;
    OgitorsProperty<Ogre::Vector3>     *mPosition;
    OgitorsProperty<Ogre::String>      *mSoundFile;
    OgitorsProperty<Ogre::Real>        *mVol;
    OgitorsProperty<bool>              *mIsEnabled;
    OgitorsProperty<bool>              *mIsLoop;
};


class PluginExport FaSoundEditorFactory: public CBaseEditorFactory
{
public:
    FaSoundEditorFactory(OgitorsView *view = 0);
    virtual ~FaSoundEditorFactory() {};
    virtual CBaseEditorFactory* duplicate(OgitorsView *view);
    virtual CBaseEditor *CreateObject(CBaseEditor **parent, OgitorsPropertyValueMap &params);
    virtual Ogre::String GetPlaceHolderName();
};



extern "C" bool PluginExport dllStartPlugin(void *identifier, Ogre::String& name);

extern "C" bool PluginExport dllStopPlugin(void);
